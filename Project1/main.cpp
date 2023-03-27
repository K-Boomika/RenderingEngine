
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Objects.h"
#include "Matrix.h"
#include "Vector.h"
#include <cstdlib>
#include <limits>
#include <string>
#include <sstream>


using namespace std;

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

//Object object;  // defining the object
vector<Object> objects;
int nObjects = 0;
Vector Pref;    // defining the Pref
Vector Ccord;   // defining the camera
Vector Vprime;        // defining the world coordinate system
Matrix4d Mmodel(1, 1, 1, 1); // Model matrix
Matrix4d Mview;  // View matrix
Matrix4d Mpers;  // Perspective Matrix
Matrix4d Mtransform;    // Tranformation matrix

std::string removeLeadingSpacesAndTabs(const std::string& input) {
    std::string result = input;
    size_t firstNonSpace = result.find_first_not_of(" \t");

    if (firstNonSpace != std::string::npos) {
        result.erase(0, firstNonSpace);
    }
    else {
        // The entire string consists of spaces and/or tabs, so clear the string
        result.clear();
    }

    return result;
}

// Get the object data from .d file and return it as Object
Object getObject(const char* filename) {
    int nPoints = -1;
    int nBlocks = -1;
    Vector* pts;
    vector <vector <int>> blocks;

    std::string str;
    int flag = 0;
    int i, j, k;

    std::ifstream house_file(filename);

    if (house_file.is_open()) { // always check whether the file is open
        getline(house_file, str);
        str = removeLeadingSpacesAndTabs(str);
        string noOfPoints, noOfBlocks;

        // Collect number of vertexes and polygons
        for (i = 0; i < str.size(); i++) {
            if ((str[i] == ' ' || str[i] == '\t') && noOfPoints.size() > 0 && flag == 0) {
                flag++;
            }
            else if (isdigit(str[i])) {
                if (flag == 0) {
                    noOfPoints += str[i];
                }
                else {
                    noOfBlocks += str[i];
                }
            }
        }
        nPoints = atoi(noOfPoints.c_str());
        nBlocks = atoi(noOfBlocks.c_str());
        pts = new Vector[nPoints];

        // Collect x,y,z coordinates
        for (j = 0; j < nPoints; j++) {
            getline(house_file, str);
            str = removeLeadingSpacesAndTabs(str);
            std::istringstream iss(str);
            iss >> pts[j].x >> pts[j].y >> pts[j].z;
        }

        // Collect the vertexes data for each polygon
        for (j = 0; j < nBlocks; j++) {
            getline(house_file, str);
            str = removeLeadingSpacesAndTabs(str);
            std::istringstream iss(str);
            int nVertices;
            iss >> nVertices;

            vector<int> block(nVertices);
            for (i = 0; i < nVertices; i++) {
                iss >> block[i];
            }
            blocks.push_back(block);
        }

        // Convert all of the previously obtained data into an Object
        Object object(nBlocks);
        for (i = 0; i < nBlocks; i++) {
            int nVertices = blocks[i].size();
            Polygon polygon(nVertices);
            for (j = 0; j < nVertices; j++) {
                polygon.vertices.push_back(pts[blocks[i][j] - 1]);
            }
            object.poly.push_back(polygon);
        }

        return object;
    }
}

// Draw a line using OpenGl using 2 vertex points
void drawLine(Vector v1, Vector v2) {
    glBegin(GL_LINES);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glEnd();
}

// Dram the object by applying transformation matrix multiplication to all vertexes in all polygons of the object
void drawObject(vector<Object> objects) {

    for (int k = 0; k < nObjects; k++)
    {
        // Apply transformation matrix
        for (int i = 0; i < objects[k].nPoly; i++) {
            for (int j = 0; j < objects[k].poly[i].nPts; j++) {
                Vector v(objects[k].poly[i].vertices[j].x, objects[k].poly[i].vertices[j].y, objects[k].poly[i].vertices[j].z);
                v = Mtransform * v;
                objects[k].poly[i].vertices[j].x = v.x;
                objects[k].poly[i].vertices[j].y = v.y;
                objects[k].poly[i].vertices[j].z = v.z;
            }
        }
        objects[k].calculateNormals();
    }
    
    // Set up lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);


    for (int k = 0; k < nObjects; k++)
    {
        // Iterate over polygons and draw them
        for (int i = 0; i < objects[k].nPoly; i++)
        {
            // Backface and hidden surface conditions
            if (objects[k].poly[i].normal.z > 0) {
                continue;
            }
            for (int j = 0; j < objects[k].poly[i].nPts; j++) {
                Vector v1(objects[k].poly[i].vertices[j].x, objects[k].poly[i].vertices[j].y, objects[k].poly[i].vertices[j].z);
                Vector v2(objects[k].poly[i].vertices[(j + 1) % objects[k].poly[i].nPts].x, objects[k].poly[i].vertices[(j + 1) % objects[k].poly[i].nPts].y, objects[k].poly[i].vertices[(j + 1) % objects[k].poly[i].nPts].z);
                //v1.print();
                drawLine(v1, v2);
            }
        }
    }

    // Disable lighting and depth test
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
}

// Keyboard control to move the camera to different directions: up, down, left, right, front and back
void handleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        Vector up(0, 1, 0);
        Ccord = Ccord + up;
        Mview = Matrix4d::getViewMatrix(Ccord, Pref, Vprime);
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        Vector down(0, 1, 0);
        Ccord = Ccord - down;
        Mview = Matrix4d::getViewMatrix(Ccord, Pref, Vprime);
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        Vector right(1,0,0);
        Ccord = Ccord - right;
        Mview = Matrix4d::getViewMatrix(Ccord, Pref, Vprime);
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        Vector left(1, 0, 0);
        Ccord = Ccord + left;
        Mview = Matrix4d::getViewMatrix(Ccord, Pref, Vprime);
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        Vector toward(0, 0, 1);
        Ccord = Ccord + toward;
        Mview = Matrix4d::getViewMatrix(Ccord, Pref, Vprime);
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        Vector backward(0, 0, 1);
        Ccord = Ccord - backward;
        Mview = Matrix4d::getViewMatrix(Ccord, Pref, Vprime);
    }
}

int main(void) {

    // Definr Pref, Camera coordinate and World coordinate
    Pref = Vector(0, 0, 0);
    Ccord = Vector(0,0,10);
    Vprime = Vector(0, 1, 0);
    
    Object object = getObject("./D files/donut.d.txt");
    objects.push_back(object);
    nObjects++;
    object = getObject("./D files/bench.d.txt");
    objects.push_back(object);
    nObjects++;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Computer Graphics II : Project 1 - Boomika Karuppaiah", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Set the key callback function
    glfwSetKeyCallback(window, handleKeyboardInput);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Apply matrix transformation
        Mview = Matrix4d::getViewMatrix(Ccord, Pref, Vprime);
        Mpers = Matrix4d::getPerspectiveMatrix();
        Mtransform = Mpers * Mview * Mmodel;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPushMatrix();
        drawObject(objects);
        glPopMatrix();

        /*glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(1.0f);
        glBegin(GL_POINTS);
        glVertex2f(-0.9f, 4.59102e-09f);
        glEnd();

        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(1.0f);
        glBegin(GL_POINTS);
        glVertex2f(-0.901f, 4.59102e-09f);
        glEnd();*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

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
#include <thread>
#include <chrono>

using namespace std;

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

vector<Object> objects;
int nObjects = 0;
Vector Pref;    // defining the Pref
Vector Ccord;   // defining the camera
Vector Vprime;        // defining the world coordinate system
Matrix4d Mmodel(1, 1, 1, 1); // Model matrix
Matrix4d Mview;  // View matrix
Matrix4d Mpers;  // Perspective Matrix
Matrix4d Mtransform;    // Tranformation matrix
vector<float> depth(2001, std::numeric_limits<float>::max());
vector<Vector> color(2001, Vector{ 0,0,0 });
vector<vector<float>> Z_depth(2001, depth);
vector<vector<Vector>> Z_frame(2001, color);
bool needsRedraw = true;

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

// Display objects obtained by Z-buffer algorithm
void display_zbuffer() {

    glBegin(GL_POINTS);
    for (int i = 0; i <= 2000; i++)
    {
        for (int j = 0; j <= 2000; j++) {
            glColor3f(Z_frame[i][j].x, Z_frame[i][j].y, Z_frame[i][j].z);
            glVertex2f(static_cast<float>(i) / 1000.0f - 1.0, static_cast<float>(j) / 1000.0f - 1.0);  
        }
    }
    glEnd();
    glFinish();
    glFlush();
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
    for (int k = 0; k < nObjects; k++)
    {
        for (int i = 0; i < objects[k].nPoly; i++) {
            objects[k].poly[i].findScanLineLimit();
        }
        objects[k].createEdgeTable();
    }
    for (int k = 0; k < nObjects; k++)
    {
        objects[k].scanConvert(Z_depth, Z_frame);
    }
    display_zbuffer();
}

int main(void) {

    // Define Pref, Camera coordinate and VPrime coordinate
    Pref = Vector(0, 0, 0);
    Ccord = Vector(0, 15, 14);
    Vprime = Vector(0, 1, 0);

    // Add objects
    Object object = getObject("./D files/cow.d.txt");
    objects.push_back(object);
    objects[nObjects].randomColor();
    nObjects++;
    object = getObject("./D files/knight.d.txt");
    objects.push_back(object);
    objects[nObjects].randomColor();
    nObjects++;

    // Apply matrix transformation
    Mview = Matrix4d::getViewMatrix(Ccord, Pref, Vprime);
    Mpers = Matrix4d::getPerspectiveMatrix();
    Mtransform = Mpers * Mview * Mmodel;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Computer Graphics II : Project 2 - Boomika Karuppaiah", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE); // Enable double buffering
    glfwSwapInterval(1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the clear color to black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glFlush();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPushMatrix();
        drawObject(objects);
        glPopMatrix();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
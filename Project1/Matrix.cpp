#include "Matrix.h"
#include "Vector.h"
#include "World.h"
#include <iostream>
#include <fstream>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Matrix4d::Matrix4d() {
    // set all values to 0
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            values[i][j] = 0;
        }
    }
}

Matrix4d::Matrix4d(double m00, double m01, double m02, double m03,
    double m10, double m11, double m12, double m13,
    double m20, double m21, double m22, double m23,
    double m30, double m31, double m32, double m33) {
    // set values based on input
    values[0][0] = m00;
    values[0][1] = m01;
    values[0][2] = m02;
    values[0][3] = m03;
    values[1][0] = m10;
    values[1][1] = m11;
    values[1][2] = m12;
    values[1][3] = m13;
    values[2][0] = m20;
    values[2][1] = m21;
    values[2][2] = m22;
    values[2][3] = m23;
    values[3][0] = m30;
    values[3][1] = m31;
    values[3][2] = m32;
    values[3][3] = m33;
}

Matrix4d::Matrix4d(double m00, double m11, double m22, double m33) {
    // set diagonal matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                if (i == 0) {
                    values[i][j] = m00;
                }
                else if (i == 1) {
                    values[i][j] = m11;
                }
                else if (i == 2) {
                    values[i][j] = m22;
                }
                else {
                    values[i][j] = m33;
                }
            }
            else{
                values[i][j] = 0;
            }
        }
    }
}

Matrix4d Matrix4d::operator*(const Matrix4d& other) const {
    Matrix4d result;
    // multiply matrices
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.values[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result.values[i][j] += values[i][k] * other.values[k][j];
            }
        }
    }
    return result;
}

Vector Matrix4d::operator*(const Vector& v) const {
    double x = values[0][0] * v.x + values[0][1] * v.y + values[0][2] * v.z + values[0][3];
    double y = values[1][0] * v.x + values[1][1] * v.y + values[1][2] * v.z + values[1][3];
    double z = values[2][0] * v.x + values[2][1] * v.y + values[2][2] * v.z + values[2][3];
    double w = values[3][0] * v.x + values[3][1] * v.y + values[3][2] * v.z + values[3][3];
    return Vector(x / w, y / w, z / w);
}

// Return View Matrix
Matrix4d Matrix4d::getViewMatrix(Vector Ccord, Vector Pref, World w) {
    Vector Nvec = Pref - Ccord;
    Nvec.unitVector();
    Vector N = Nvec;
    Vector Uvec = N * w.Y;
    Uvec.unitVector();
    Vector U = Uvec;
    Vector V = U * N;
    Matrix4d Mview = Matrix4d(
        U.x, U.y, U.z, -Ccord.dot(U),
        V.x, V.y, V.z, -Ccord.dot(V),
        N.x, N.y, N.z, -Ccord.dot(N),
        0, 0, 0, 1
    );
    return Mview;
}

// Return Perspective Matrix
Matrix4d Matrix4d::getPerspectiveMatrix() {
    double fov = 45.0f;           // field of view in degrees
    double near_plane = 0.1f;     // distance to the near plane
    double far_plane = 500.0f;    // distance to the far plane
    double h = tanf(fov * 0.5f) * near_plane;  // height of the viewport

    double d = near_plane;
    double f = far_plane;
    Matrix4d Mpers = Matrix4d(
        d / h, 0, 0, 0, 
        0, d / h, 0, 0, 
        0, 0, f / (f - d), -d * f / (f - d), 
        0, 0, 1, 0);
    return Mpers;
}

void Matrix4d::print() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << values[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

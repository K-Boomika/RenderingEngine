#ifndef MATRIX_H
#define MATRIX_H
#include "Vector.h"
#include "World.h"
#include <cmath>

class Matrix4d {
public:
    double values[4][4];

    Matrix4d(); // default constructor
    Matrix4d(double m00, double m01, double m02, double m03,
        double m10, double m11, double m12, double m13,
        double m20, double m21, double m22, double m23,
        double m30, double m31, double m32, double m33); // parameterized constructor
    Matrix4d(double m00, double m11, double m22, double m33); // parameterized constructor

    Matrix4d operator*(const Matrix4d& other) const; // matrix multiplication
    Vector operator*(const Vector& v) const; // matrix multiplication with Vector

    static Matrix4d getViewMatrix(Vector Ccord, Vector Pref, World w); // Get View matrix
    static Matrix4d getPerspectiveMatrix(); // Get perspective matrix

    void print();
};

#endif

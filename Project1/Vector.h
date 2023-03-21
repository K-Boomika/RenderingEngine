#ifndef VECTOR_H
#define VECTOR_H

#include <vector>

class Vector {
public:
    double x, y, z;
    Vector();
    Vector(double x, double y, double z);

    void unitVector();

    Vector operator-(const Vector& v);  // Vector - Vector function
    Vector operator+(const Vector& v);  // Vector + Vector function
    Vector operator*(const Vector& v);  // Vector * Vector function

    double dot(Vector v); // Vector dot Vector function

    void print();
};

#endif

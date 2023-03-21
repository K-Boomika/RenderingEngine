#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include "Vector.h"


class Polygon {
public:
    std::vector<Vector> vertices;
    int nPts;
    Vector normal;
    Polygon(int nPts);
};

class Object {
public:
    std::vector<Polygon> poly;
    int nPoly;
    Object(int nPoly);
    Object();
    void calculateNormals();
};

#endif

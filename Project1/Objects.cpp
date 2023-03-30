#include "Objects.h"
#include <vector>

Polygon::Polygon(int nPts) {
    this->nPts = nPts;
    this->backface = false;
}

Object::Object(int nPoly) {
    this->nPoly = nPoly;
}

Object::Object() {
    this->nPoly = 0;
}

void Object::calculateNormals() {
    for (int i = 0; i < nPoly; i++) {
        Polygon& poly = this->poly[i];
        Vector normal(0, 0, 0);
        for (int j = 2; j < poly.nPts; j++) {
            Vector v1(poly.vertices[0].x, poly.vertices[0].y, poly.vertices[0].z);
            Vector v2(poly.vertices[j - 1].x, poly.vertices[j - 1].y, poly.vertices[j - 1].z);
            Vector v3(poly.vertices[j].x, poly.vertices[j].y, poly.vertices[j].z);
            normal = normal + ((v2 - v1) * (v3 - v2));
        }
        normal.unitVector();
        this->poly[i].normal = normal;
        if (this->poly[i].normal.z>0) {
            this->poly[i].backface = true;
        }
        else {
            this->poly[i].backface = false;
        }
    }
}



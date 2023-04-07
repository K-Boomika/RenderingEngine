#ifndef OBJECTS_H
#define OBJECTS_H
#include <vector>
#include "Vector.h"
#include "Edge.h"

class Polygon {
public:
    std::vector<Vector> vertices;
    int nPts;
    Vector normal;
    bool backface;
    float colorR, colorG, colorB;
    int ymin, ymax;
    Polygon(int nPts);
    void findScanLineScope();
};

class Object {
public:
    std::vector<Polygon> poly;
    int nPoly;
    Object(int nPoly);
    Object();
    void calculateNormals();
    void randomColor();
    void creatEdgeTable();
    void ScanConvertion(std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame);
    std::vector<Edge>* edgetables;
};
inline bool Ymincompare(const Edge& e1, const Edge& e2) {
    return e1.Ymin < e2.Ymin;
}

inline bool Xcompare(const Edge& e1, const Edge& e2) {
    return e1.Xmin < e2.Xmin;
}

#endif

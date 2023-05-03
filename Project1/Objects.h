#ifndef OBJECTS_H
#define OBJECTS_H
#include <vector>
#include "Vector.h"
#include "Edge.h"
#include "Horizon.h"
#include "Draw_Line.h"
#include <map>
#include <set>
#include "Common.h"

struct VectorCompare {
    bool operator()(const Vector& v1, const Vector& v2) const {
        if (v1.x != v2.x) {
            return v1.x < v2.x;
        }
        else if (v1.y != v2.y) {
            return v1.y < v2.y;
        }
        else {
            return v1.z < v2.z;
        }
    }
};

class Polygon {
public:
    std::vector<Vector> vertices;
    int nPts;
    Vector normal;
    bool backface;
    float colorR, colorG, colorB;
    int ymin, ymax;
    Polygon(int nPts);
    void findScanLineLimit();
};

class Object {
public:
    std::vector<Polygon> poly;
    std::map<Vector, std::vector<int>, VectorCompare> vertexAdjPoly;
    std::map<Vector, Vector, VectorCompare> vertexNormal;
    std::map<Vector, Vector, VectorCompare> vertexColor;
    int nPoly;
    Object(int nPoly);
    Object();
    void calculateNormals();
    void randomColor();
    void prandomColor();
    void createEdgeTable(int shadingModel, Vector camera);
    Vector objectColor;
    Vector ka;
    Vector kd;
    Vector ks;
    int focus;
    void calculate_vertex_normal();
    void scanConvert(int shadingModel, std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame, Vector camera, std::vector<DrawLine> DrawLines);
    std::vector<Edge>* edgetables;
    std::vector<Horizon>* Horizons;
};
inline bool Ymincompare(const Edge& e1, const Edge& e2) {
    return e1.Ymin < e2.Ymin;
}

inline bool Xcompare(const Edge& e1, const Edge& e2) {
    return e1.Xmin < e2.Xmin;
}

#endif

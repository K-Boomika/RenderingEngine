#ifndef __EDGE__
#define __EDGE__

class Edge {
public:
	int Ymin, Ymax;
	float Xmin, x_over_y, Zmin, z_over_y;
	float rmin, gmin, bmin;
	float dr, dg, db;
	float nxmin, nymin, nzmin;
	float dnx, dny, dnz;
public:
	Edge();
	~Edge();
	Edge& operator= (const Edge& edge);
};

#endif
#ifndef __EDGE__
#define __EDGE__

class Edge {
public:
	int Ymin, Ymax;
	float Xmin, x_over_y, Zmin, z_over_y;
public:
	Edge();
	~Edge();
	Edge& operator= (const Edge& edge);
};

#endif //__EDGE__
#include "Edge.h"

Edge::Edge()
{
}

Edge::~Edge()
{
}

Edge& Edge::operator=(const Edge& edge)
{
	Ymin = edge.Ymin;
	Ymax = edge.Ymax;
	Xmin = edge.Xmin;
	Zmin = edge.Zmin;
	x_over_y = edge.x_over_y;
	z_over_y = edge.z_over_y;

	return *this;
}

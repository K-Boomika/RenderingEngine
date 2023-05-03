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

	rmin = edge.rmin;
	gmin = edge.gmin;
	bmin = edge.bmin;


	dr = edge.dr;
	dg = edge.dg;
	db = edge.db;


	nxmin = edge.nxmin;
	nymin = edge.nymin;
	nzmin = edge.nzmin;

	dnx = edge.dnx;
	dny = edge.dny;
	dnz = edge.dnz;

	return *this;
}

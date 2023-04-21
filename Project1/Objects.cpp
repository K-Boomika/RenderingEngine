#include "Edge.h"
#include "Objects.h"
#include <vector>
#include <algorithm>
#include<iostream>

Polygon::Polygon(int nPts) {
	this->nPts = nPts;
	this->backface = false;
	this->colorR = 0.0;
	this->colorG = 0.0;
	this->colorB = 0.0;
}

void Polygon::findScanLineLimit()
{
    int tempmin = INFINITY;
    int tempmax = -INFINITY;

    for (int i = 0; i < this->nPts; i++)
    {
        this->vertices[i] = (this->vertices[i] + 1.0) * 1000;

        if (this->vertices[i].y < tempmin) {
            tempmin = this->vertices[i].y;
        }
        if (this->vertices[i].y > tempmax) {
            tempmax = this->vertices[i].y;
        }
    }
    this->ymin = (int)tempmin;
    this->ymax = (int)tempmax + 1;
}

Object::Object() {
	this->nPoly = 0;
}

Object::Object(int nPoly) {
    this->nPoly = nPoly;
	this->edgetables = new std::vector<Edge>[nPoly];
}

// Calculate normals for each polygon
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

// Select random color for each polygon
void Object::randomColor()
{
	for (auto& polygon : poly) {
		polygon.colorR = (float)rand() / RAND_MAX;
		polygon.colorG = (float)rand() / RAND_MAX;
		polygon.colorB = (float)rand() / RAND_MAX;
	}
}

void Object::createEdgeTable()
{
	//create the edge table for each polygon
	for (int i = 0; i < nPoly; i++)
	{
		Polygon polygon = this->poly[i];
		if (!polygon.backface) {

			int nPts = polygon.nPts;
			Vector* vertices = new Vector[nPts];
			for (int j = 0; j < nPts; j++) {
				vertices[j] = polygon.vertices[j];
			}

			std::vector<Edge> temptable = edgetables[i];

			for (int j = 0; j < nPts; j++)
			{
				Vector v1, v2;
				float ymin, ymax, xmin, dx, zmin, dz;
				Edge tempedge;

				v1 = vertices[j];
				v2 = vertices[(j + 1 ) % nPts];

				dx = (v1.x - v2.x) / (v1.y - v2.y);//x over y
				dz = (v1.z - v2.z) / (v1.y - v2.y);//z over y

				if ((int)v1.y == (int)v2.y) {
					continue;
				}

				if (v1.y < v2.y) {
					ymin = (int)v1.y;
					ymax = (int)v2.y - 1;
					xmin = v1.x;
					zmin = v1.z;

				}
				else if (v1.y > v2.y) {
					ymin = (int)v2.y;
					ymax = (int)v1.y - 1;
					xmin = v2.x;
					zmin = v2.z;

				}

				tempedge.Ymin = ymin;
				tempedge.Ymax = ymax;
				tempedge.Xmin = xmin;
				tempedge.x_over_y = dx;
				tempedge.Zmin = zmin;
				tempedge.z_over_y = dz;
				temptable.push_back(tempedge);
			}

			//sort based on Y
			sort(temptable.begin(), temptable.end(), Ymincompare);
			edgetables[i] = temptable;
		}
	}
}

void Object::scanConvert(std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame) {
	//use AET to fulfill the polygons
	for (int i = 0; i < nPoly; i++)
	{
		Polygon polygon = this->poly[i];

		if (!polygon.backface) {

			std::vector<Edge> AET;//active edge table
			std::vector<Edge> edgetable = this->edgetables[i];//edge table
			Vector color = { polygon.colorR,polygon.colorG,polygon.colorB };
			int nPts = polygon.nPts;

			for (int j = polygon.ymin - 1; j <= polygon.ymax + 1; j++) {
				for (int k = 0; k < edgetable.size(); k++)
				{
					//add edges whose yMin equal to AET
					if (edgetable[k].Ymin == j) {
						AET.push_back(edgetable[k]);
					}
					else if (edgetable[k].Ymin > j) {
						break;
					}
				}

				//sort based on X
				sort(AET.begin(), AET.end(), Xcompare);

				for (int k = 0; k < AET.size() / 2; k = k + 2) {

					Vector start, end;
					start = { AET[k].Xmin,j * 1.0f,AET[k].Zmin };
					end = { AET[k + 1].Xmin,j * 1.0f,AET[k + 1].Zmin };

					int y = j;
					if (y > 2000 || y < 0) {

					}
					else {
						Vector left, right;

						left = start;
						right = end;

						int x1 = (int)left.x;
						int x2 = (int)right.x;
						float z1 = left.z;
						float z2 = right.z;
						float zoverx = (z2 - z1) / (right.x - left.x);

						float zp = z1;

						for (int x = x1; x <= x2; x++)
						{
							if (x >= 0 && x <= 2000 && zp <= Z_depth[x][y]) {
								Z_depth[x][y] = zp;
								Z_frame[x][y] = color;
							}
							zp += zoverx;
						}
					}
				}
				for (int k = 0; k < AET.size(); k++) {
					//Remove from AET entries for which y = ymax (leave edges) 
					if (AET[k].Ymax == j) {
						AET.erase(AET.begin() + k);
						k--;
					}
				}
				for (int k = 0; k < AET.size(); k++) {
					AET[k].Xmin += AET[k].x_over_y;
					AET[k].Zmin += AET[k].z_over_y;
					//For each entry remaining in AET, replace x by x + increment
				}

			sort(AET.begin(), AET.end(), Xcompare);
			}
		}
	}
}

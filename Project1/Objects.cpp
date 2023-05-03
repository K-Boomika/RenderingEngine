#include "Edge.h"
#include "Objects.h"
#include "Draw_Line.h"
#include <vector>
#include <algorithm>
#include<iostream>
#include "Common.h"

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
	this->Horizons = new std::vector<Horizon>[nPoly];
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
	objectColor.x = (float)rand() / RAND_MAX;
	objectColor.y = (float)rand() / RAND_MAX;
	objectColor.z = (float)rand() / RAND_MAX;
}

void Object::prandomColor()
{
	for (auto& polygon : poly) {
		polygon.colorR = (float)rand() / RAND_MAX;
		polygon.colorG = (float)rand() / RAND_MAX;
		polygon.colorB = (float)rand() / RAND_MAX;
	}
}

void Object::calculate_vertex_normal() {
	for (auto it = vertexAdjPoly.begin(); it != vertexAdjPoly.end(); ++it) {
		Vector temp;
		std::vector<int> adj_polygons = it->second;
		for (int k = 0; k < adj_polygons.size(); k++)
		{
			temp = temp + poly[adj_polygons[k]].normal;
		}
		temp.unitVector();
		vertexNormal[it->first] = temp;
		//vertexNormal[it->first].print(); std::cout << std::endl;

	}
}

void Object::createEdgeTable(int shadingModel, Vector camera)
{
	if (shadingModel == 2) {
		for (auto it = vertexNormal.begin(); it != vertexNormal.end(); ++it) {	//calculate the color of each vertex
			std::cout << "hi"; it->first.print(); it->second.print();
				vertexColor[it->first] = illuminationModel(it->second, objectColor,
					ambientLightIntensity, LightIntensity, LightColor, LightDirection,
					ka, kd, ks, fatt, camera, focus);
		}
	}
	std::cout << std::endl;
	//create the edge table for each polygon
	for (int i = 0; i < nPoly; i++)
	{
		Polygon polygon = this->poly[i];
		if (!polygon.backface) {

			int nPts = polygon.nPts;
			Vector* vertices = new Vector[nPts];
			Vector* verticesColor = new Vector[nPts];
			Vector* verticesNormal = new Vector[nPts];
			for (int j = 0; j < nPts; j++) {
				vertices[j] = polygon.vertices[j];
				if (shadingModel == 2) {
					//for (auto it = vertexColor.begin(); it != vertexColor.end(); ++it) {
					//	std::cout <<"new"; it->first.print(); it->second.print();
					//}
					vertices[j].print();
					Vector(static_cast<float>(vertices[j].x) / 1000.0f - 1.0, static_cast<float>(vertices[j].y) / 1000.0f - 1.0, static_cast<float>(vertices[j].z) / 1000.0f - 1.0).print();
					verticesColor[j] = vertexColor[Vector(static_cast<float>(vertices[j].x) / 1000.0f - 1.0, static_cast<float>(vertices[j].y) / 1000.0f - 1.0, static_cast<float>(vertices[j].z) / 1000.0f - 1.0)];
					std::cout << std::endl;
					verticesColor[j].print();
					vertexColor[vertices[j]].print();
				}
				else if (shadingModel == 3) {
					verticesNormal[j] = vertexNormal[vertices[j]];
				}
			}

			std::vector<Edge> temptable = edgetables[i];

			for (int j = 0; j < nPts; j++)
			{
				Vector v0, v1, v2, v3, v1Color, v2Color, v1Normal, v2Normal;
				float ymin, ymax, xmin, dx, zmin, dz;
				Vector minColor;
				Vector minNormal;
				float dr, dg, db;   //rgb
				float dnx, dny, dnz; //normal
				Edge tempedge;

				v0 = vertices[(j - 1 + nPts) % nPts];
				v3 = vertices[(j + 2 + nPts) % nPts];

				v1 = vertices[j];
				if (shadingModel == 2)v1Color = verticesColor[j];
				if (shadingModel == 3)v1Normal = verticesNormal[j];

				v2 = vertices[(j + 1 + nPts) % nPts];
				if (shadingModel == 2)v2Color = verticesColor[(j + 1 + nPts) % nPts];
				if (shadingModel == 3)v2Normal = verticesNormal[(j + 1 + nPts) % nPts];

				dx = ((v1.x - v2.x) / (v1.y - v2.y));//x over y
				dz = ((v1.z - v2.z)) / (v1.y - v2.y);//z over y

				if (shadingModel == 2){
				dr = (v1Color.x - v2Color.x) / (v1.y - v2.y); //red
				dg = (v1Color.y - v2Color.y) / (v1.y - v2.y); //green
				db = (v1Color.z - v2Color.z) / (v1.y - v2.y); //blue
				}

				if ((int)v1.y == (int)v2.y) {
					if (shadingModel == 1) {
						Horizons[i].push_back(Horizon(v1, v2));
					}
					else if (shadingModel == 2) {
						Horizons[i].push_back(Horizon(v1, v2, v1Color, v2Color));
					}
					else if (shadingModel == 3) {
						Horizons[i].push_back(Horizon(v1, v2, v1Normal, v2Normal));
					}
					continue;
				}

				if (shadingModel == 3) {
					dnx = (v1Normal.x - v2Normal.x) / (v1.y - v2.y); //normal x
					dny = (v1Normal.y - v2Normal.y) / (v1.y - v2.y); //normal y
					dnz = (v1Normal.z - v2Normal.z) / (v1.y - v2.y); //normal z
				}

				if (v1.y < v2.y) {
					ymin = (int)v1.y;
					ymax = (int)v2.y - 1;
					xmin = v1.x;
					zmin = v1.z;

					if (shadingModel == 2) minColor = v1Color;
					if (shadingModel == 3) minNormal = v1Normal;

				}
				else if (v1.y > v2.y) {
					ymin = (int)v2.y;
					ymax = (int)v1.y - 1;
					xmin = v2.x;
					zmin = v2.z;

					if (shadingModel == 2) minColor = v2Color;
					if (shadingModel == 3) minNormal = v2Normal;

				}

				tempedge.Ymin = ymin;
				tempedge.Ymax = ymax;
				tempedge.Xmin = xmin;
				tempedge.x_over_y = dx;
				tempedge.Zmin = zmin;
				tempedge.z_over_y = dz;

				if (shadingModel == 2) {
					tempedge.rmin = minColor.x;
					tempedge.gmin = minColor.y;
					tempedge.bmin = minColor.z;
					tempedge.dr = dr;
					tempedge.dg = dg;
					tempedge.db = db;
				}
				if (shadingModel == 3) {
					tempedge.nxmin = minNormal.x;
					tempedge.nymin = minNormal.y;
					tempedge.nzmin = minNormal.z;
					tempedge.dnx = dnx;
					tempedge.dny = dny;
					tempedge.dnz = dnz;
				}

				temptable.push_back(tempedge);
			}

			//sort based on Y
			sort(temptable.begin(), temptable.end(), Ymincompare);
			edgetables[i] = temptable;
		}
	}
}

void Object::scanConvert(int shadingModel, std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame, Vector camera, std::vector<DrawLine> DrawLines) {
	//use AET to fulfill the polygons
	for (int i = 0; i < nPoly; i++)
	{
		Polygon polygon = this->poly[i];

		if (!polygon.backface) {

			std::vector<Edge> AET;//active edge table
			std::vector<Edge> edgetable = this->edgetables[i];//edge table
			//Vector color = { polygon.colorR,polygon.colorG,polygon.colorB };
			Vector color = objectColor;
			int nPts = polygon.nPts;
			Vector constantColor;

			if (shadingModel == 1) {
				//constant shading, each polygon one color
				constantColor = illuminationModel(polygon.normal, objectColor,
					ambientLightIntensity, LightIntensity, LightColor, LightDirection,
					ka, kd, ks, fatt, camera, focus);
				//constantColor.print();

				color = constantColor;
				
				for (int h = 0; h < Horizons[i].size(); h++) {
					Horizons[i][h].Constant_DrawHorizon(color, Z_depth, Z_frame);
				}
			}
			if (shadingModel == 2) {
				//std::cout << Horizons[i].size();
				for (int h = 0; h < Horizons[i].size(); h++) {
					Horizons[i][h].Gouraud_DrawHorizon(color, camera, ka, kd, ks, focus, Z_depth, Z_frame);
				}
			}
			if (shadingModel == 3) {
				for (int h = 0; h < Horizons[i].size(); h++) {
					Horizons[i][h].Phong_DrawHorizon(color, camera, ka, kd, ks, focus, Z_depth, Z_frame);
				}
			}

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
					Vector startColor, endColor;
					Vector startNormal, endNormal;
					start = { AET[k].Xmin,j * 1.0f,AET[k].Zmin };
					end = { AET[k + 1].Xmin,j * 1.0f,AET[k + 1].Zmin };
					int y;

					if (shadingModel == 1) {
						DrawLine temp = DrawLine(start, end, color);
						DrawLines.push_back(temp);

						y = j;
					}
					if (shadingModel == 2) {
						startColor = { AET[k].rmin,AET[k].gmin,AET[k].bmin };
						endColor = { AET[k + 1].rmin,AET[k + 1].gmin,AET[k + 1].bmin };

						y = (int)start.y;
					}
					if (shadingModel == 3) {
						startNormal = { AET[k].nxmin,AET[k].nymin,AET[k].nzmin };
						endNormal = { AET[k + 1].nxmin,AET[k + 1].nymin,AET[k + 1].nzmin };

						y = (int)start.y;
					}

					if (y > 2000 || y < 0) {

					}
					else {
						Vector left, right;
						Vector leftColor, rightColor;
						Vector leftNormal, rightNormal;

						left = start;
						right = end;
						leftColor = startColor;
						rightColor = endColor;
						leftNormal = startNormal;
						rightNormal = endNormal;

						int x1 = (int)left.x;
						int x2 = (int)right.x;
						float z1 = left.z;
						float z2 = right.z;
						float zoverx = (z2 - z1) / (right.x - left.x);

						float rp, gp, bp, boverx, goverx, roverx;
						float nx1, nx2, ny1, ny2, nz1, nz2, nxoverx, nyoverx, nzoverx, nxp, nyp, nzp;

						if (shadingModel == 2) {
							float r1 = leftColor.x;
							float r2 = rightColor.x;
							roverx = (r2 - r1) / (right.x - left.x);
							float g1 = leftColor.y;
							float g2 = rightColor.y;
							goverx = (g2 - g1) / (right.x - left.x);
							float b1 = leftColor.z;
							float b2 = rightColor.z;
							boverx = (b2 - b1) / (right.x - left.x);
							rp = r1;
							gp = g1;
							bp = b1;
						}
						if (shadingModel == 3) {
							nx1 = leftNormal.x;
							nx2 = rightNormal.x;
							ny1 = leftNormal.y;
							ny2 = rightNormal.y;
							nz1 = leftNormal.z;
							nz2 = rightNormal.z;
							nxoverx = (nx2 - nx1) / (right.x - left.x);
							nyoverx = (ny2 - ny1) / (right.x - left.x);
							nzoverx = (nz2 - nz1) / (right.x - left.x);
							nxp = nx1;
							nyp = ny1;
							nzp = nz1;
						}

						float zp = z1;

						for (int x = x1; x <= x2; x++)
						{
							if (x >= 0 && x <= 2000 && zp <= Z_depth[x][y]) {
								Z_depth[x][y] = zp;
								if(shadingModel==1) Z_frame[x][y] = color;
								if(shadingModel==2) Z_frame[x][y] = { rp, gp, bp };
								if (shadingModel == 3) {
									Vector pixelColor = illuminationModel(Vector(nxp, nyp, nzp), objectColor,
										ambientLightIntensity, LightIntensity, LightColor, LightDirection,
										ka, kd, ks, fatt, camera, focus);
									Z_frame[x][y] = pixelColor;
								}
							}
							zp += zoverx;
							if (shadingModel == 2) {
								rp += roverx;
								gp += goverx;
								bp += boverx;

							}
							if (shadingModel == 3) {
								nxp += nxoverx;
								nyp += nyoverx;
								nzp += nzoverx;
							}
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
					if (shadingModel == 2) {
						AET[k].rmin += AET[k].dr;
						AET[k].gmin += AET[k].dg;
						AET[k].bmin += AET[k].db;
					}
					if (shadingModel == 3) {
						AET[k].nxmin += AET[k].dnx;
						AET[k].nymin += AET[k].dny;
						AET[k].nzmin += AET[k].dnz;
					}
				}

			sort(AET.begin(), AET.end(), Xcompare);
			}
		}
	}
}

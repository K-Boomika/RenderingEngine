#include "Horizon.h"
#include <vector>
#include "Vector.h"
#include "Illumination.h"


Horizon::Horizon()
{
}

Horizon::Horizon(Vector start, Vector end)
{
	this->start = start;
	this->end = end;
}

Horizon::Horizon(Vector start, Vector end, Vector startV, Vector endV)
{
	this->start = start;
	this->end = end;
	this->startV = startV;
	this->endV = endV;
}

Horizon::~Horizon()
{
}

void Horizon::Constant_DrawHorizon(Vector color, std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame)
{
	int y = (int)(this->start.y);
	if (y > 2000 || y < 0) {

	}
	else {
		float fx1, fx2;
		int x1, x2;
		float z1, z2;
		if (this->start.x < this->end.x)
		{
			x1 = (int)(this->start.x);
			fx1 = this->start.x;
			x2 = (int)(this->end.x);
			fx2 = this->end.x;
			z1 = this->start.z;
			z2 = this->end.z;
		}
		else {
			x1 = (int)(this->end.x);
			fx1 = this->end.x;
			x2 = (int)(this->start.x);
			fx2 = this->start.x;
			z1 = this->end.z;
			z2 = this->start.z;
		}

		float zoverx = (z2 - z1) / (fx2 - fx1);
		float zp = z1;
		if (x1 != x2) {
			for (int x = x1; x <= x2; x++)
			{
				if (x > 2000 || x < 0) {
					continue;
				}

				if (zp < Z_depth[x][y]) {
					Z_depth[x][y] = zp;
					Z_frame[x][y] = color;
				}
				zp += zoverx;
			}
		}
		else {
			if (x1 > 2000 || x1 < 0) {

			}
			else {
				float zp;
				if (z1 <= z2) { zp = z1; }
				else { zp = z2; }
				if (zp < Z_depth[x1][y]) {
					Z_depth[x1][y] = zp;
					Z_frame[x1][y] = color;
				}
			}
		}
	}
}

void Horizon::Gouraud_DrawHorizon(Vector objectColor, Vector camera, Vector ka, Vector kd, Vector ks, int focus, std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame)
{
	Vector v1 = this->start;
	Vector v2 = this->end;
	Vector v1Color = this->startV;
	Vector v2Color = this->endV;

	int y = (int)(v1.y);
	if (y > 2000 || y < 0) {

	}
	else {
		Vector left, right, leftColor, rightColor;
		if (v1.x < v2.x) {
			left = v1;
			leftColor = v1Color;
			right = v2;
			rightColor = v2Color;
		}
		else {
			left = v2;
			leftColor = v2Color;
			right = v1;
			rightColor = v1Color;
		}

		int x1 = (int)left.x;
		int x2 = (int)right.x;
		float z1 = left.z; 
		float z2 = right.z; 

		float zoverx = (z2 - z1) / (right.x - left.x);

		float r1 = leftColor.x;
		float r2 = rightColor.x;
		float roverx = (r2 - r1) / (right.x - left.x);

		float g1 = leftColor.y;
		float g2 = rightColor.y;
		float goverx = (g2 - g1) / (right.x - left.x);

		float b1 = leftColor.z;
		float b2 = rightColor.z;
		float boverx = (b2 - b1) / (right.x - left.x);

		float zp = z1;
		float rp = r1;
		float gp = g1;
		float bp = b1;

		if (x1 != x2) {
			for (int x = x1; x <= x2; x++)
			{
				if (x > 2000 || x < 0) {
					continue;
				}

				if (zp < Z_depth[x][y]) {
					Z_depth[x][y] = zp;
					Vector{ rp, gp, bp }.print();
					Vector pixelColor = illuminationModel(Vector{ rp, gp, bp }, objectColor,
						ambientLightIntensity, LightIntensity, LightColor, LightDirection,
						ka, kd, ks, fatt, camera, focus);
					Z_frame[x][y] = pixelColor;
					pixelColor.print();
				}
				zp += zoverx;
				rp += roverx;
				gp += goverx;
				bp += boverx;
			}
		}
		else {
			if (x1 > 2000 || x1 < 0) {

			}
			else {
				float zp;
				Vector colorP;
				if (z1 < z2) {
					zp = z1;
					colorP = leftColor;

				}
				else {
					zp = z2;
					colorP = rightColor;
				}
				if (zp < Z_depth[x1][y]) {
					Z_depth[x1][y] = zp;
					Z_frame[x1][y] = colorP;
				}
			}
		}
	}
}

void Horizon::Phong_DrawHorizon(Vector objectColor, Vector camera, Vector ka, Vector kd, Vector ks, int focus, std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame)
{
	Vector v1 = this->start;
	Vector v2 = this->end;
	Vector v1Normal = this->startV;
	Vector v2Normal = this->endV;
	int y = (int)(v1.y);
	if (y > 2000 || y < 0) {
	}
	else {
		Vector left, right, leftNormal, rightNormal;
		if (v1.x < v2.x) {
			left = v1;
			leftNormal = v1Normal;
			right = v2;
			rightNormal = v2Normal;
		}
		else {
			left = v2;
			leftNormal = v2Normal;
			right = v1;
			rightNormal = v1Normal;
		}


		int x1 = (int)left.x;
		int x2 = (int)right.x;
		float z1 = left.z;
		float z2 = right.z;

		float zoverx = (z2 - z1) / (right.x - left.x);

		float nx1 = leftNormal.x;
		float nx2 = rightNormal.x;
		float nxoverx = (nx2 - nx1) / (right.x - left.x);

		float ny1 = leftNormal.y;
		float ny2 = rightNormal.y;
		float nyoverx = (ny2 - ny1) / (right.x - left.x);
		float nz1 = leftNormal.z;
		float nz2 = rightNormal.z;
		float nzoverx = (nz2 - nz1) / (right.x - left.x);

		float zp = z1;
		float nxp = nx1;
		float nyp = ny1;
		float nzp = nz1;
		if (x1 != x2) {
			for (int x = x1; x <= x2; x++)
			{
				if (x > 2000 || x < 0) {
					continue;
				}
				if (zp < Z_depth[x][y]) {
					Z_depth[x][y] = zp;

					Vector pixelColor = illuminationModel(Vector(nxp, nyp, nzp), objectColor,
						ambientLightIntensity, LightIntensity, LightColor, LightDirection,
						ka, kd, ks, fatt, camera, focus);

					Z_frame[x][y] = pixelColor;
				}
				zp += zoverx;
				nxp += nxoverx;
				nyp += nyoverx;
				nzp += nzoverx;
			}
		}
		else {
			if (x1 > 2000 || x1 < 0) {

			}
			else {
				float zp;
				Vector normalP;
				if (z1 < z2) {
					zp = z1;
					normalP = leftNormal;

				}
				else {
					zp = z2;
					normalP = rightNormal;
				}
				if (zp < Z_depth[x1][y]) {
					Z_depth[x1][y] = zp;
					Vector pixelColor = illuminationModel(normalP, objectColor,
						ambientLightIntensity, LightIntensity, LightColor, LightDirection,
						ka, kd, ks, fatt, camera, focus);
					Z_frame[x1][y] = pixelColor;
				}
			}
		}
	}
}
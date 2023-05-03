#ifndef __Horizon__
#define __Horizon__
#include <vector>
#include "Vector.h"
#include "Illumination.h"
#include "Common.h"
class Horizon
{
public:
	Vector start, end;
	Vector startV, endV;
public:
	Horizon();
	Horizon(Vector start, Vector end);
	Horizon(Vector start, Vector end, Vector startV, Vector endV);
	~Horizon();
	void Constant_DrawHorizon(Vector color, std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame);
	void Gouraud_DrawHorizon(Vector objectColor, Vector camera, Vector ka, Vector kd, Vector ks, int focus, std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame);
	void Phong_DrawHorizon(Vector objectColor, Vector camera, Vector ka, Vector kd, Vector ks, int focus, std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame);

private:

};
#endif

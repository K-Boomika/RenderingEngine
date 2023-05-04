#ifndef __Horizon__
#define __Horizon__
#include <vector>
#include "Vector.h"
#include "Illumination.h"
#include "Common.h"
class ShadingModel
{
public:
	Vector start, end;
	Vector startV, endV;
public:
	ShadingModel();
	ShadingModel(Vector start, Vector end);
	ShadingModel(Vector start, Vector end, Vector startV, Vector endV);
	~ShadingModel();
	void ConstantDraw(Vector color, std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame);
	void GouraudDraw(std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame);
	void PhongDraw(Vector objectColor, Vector camera, Vector ka, Vector kd, Vector ks, int focus, std::vector<std::vector<float>>& Z_depth, std::vector<std::vector<Vector>>& Z_frame);

private:

};
#endif

#include "Illumination.h"
#include "Vector.h"

Vector ambientColor(Vector ObjColor, Vector ka, Vector ambientLightIntensity) {
	float x = ambientLightIntensity.x * ka.x * ObjColor.x;
	float y = ambientLightIntensity.y * ka.y * ObjColor.y;
	float z = ambientLightIntensity.z * ka.z * ObjColor.z;
	return Vector(x, y, z);
}
Vector diffuseColor(Vector kd, Vector ObjColor, Vector Normal, Vector LightDirection) {
	Vector L = LightDirection * -1;

	float temp = Normal.dot(L);

	float x = kd.x * ObjColor.x * temp;
	float y = kd.y * ObjColor.y * temp;
	float z = kd.z * ObjColor.z * temp;
	return Vector(x, y, z);
}
Vector specularColor(Vector pointLightColor, Vector ks, Vector Normal, Vector LightDirection, Vector Camera, int focus) {
	Vector L = LightDirection;
	Vector V = Camera;

	Vector LV = (L + V);
	LV.unitVector();
	Vector H = LV;

	float temp = pow(Normal.dot(H), focus);

	float x = pointLightColor.x * temp * ks.x;
	float y = pointLightColor.y * temp * ks.y;
	float z = pointLightColor.z * temp * ks.z;

	return Vector(x, y, z);
}

Vector illuminationModel(Vector normal, Vector ObjColor, Vector ambientLightIntensity, Vector LightIntensity, Vector LightColor,
	Vector LightDirection, Vector ka, Vector kd, Vector ks, float fatt, Vector Camera, int focus)
{
	LightDirection.unitVector();
	Camera.unitVector();
	normal.unitVector();
	Vector L = LightDirection;
	Vector camera = Camera;
	Vector N = normal;
	//LightDirection.print();

	Vector AmbientColor = ambientColor(ObjColor, ka, ambientLightIntensity);
	Vector DiffuseColor = diffuseColor(kd, ObjColor, N, L);
	Vector SpecularColor = specularColor(LightColor, ks, N, L, camera, focus);

	float x = AmbientColor.x + fatt * LightIntensity.x * (DiffuseColor.x + SpecularColor.x);
	float y = AmbientColor.y + fatt * LightIntensity.y * (DiffuseColor.y + SpecularColor.y);
	float z = AmbientColor.z + fatt * LightIntensity.z * (DiffuseColor.z + SpecularColor.z);
	//Vector(x, y, z).print();
	return Vector(x, y, z);
}

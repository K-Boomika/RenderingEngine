#ifndef __ILLUMINATION__
#define __ILLUMINATION__
#include "Vector.h"
#include "Common.h"

Vector ambientColor(Vector ObjColor, Vector ka, Vector ambientLightColor);
Vector diffuseColor(Vector kd, Vector ObjColor, Vector Normal, Vector Light);
Vector specularColor(Vector pointLightColor, Vector ks, Vector Normal, Vector LightDirection, Vector Camera, int focus);
Vector illuminationModel(Vector normal, Vector ObjColor, Vector ambientLightIntensity, Vector LightIntensity, Vector LightColor,
Vector LightDirection, Vector ka, Vector kd, Vector ks, float fatt, Vector Camera, int focus);

#endif
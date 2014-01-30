#ifndef MYMATH_H
#define MYMATH_H

#include "Types.h"

#include <vector>

using namespace std;


double DegToRad(double angle);

double RadToDeg(double angle);

Vector3f PolarToCartecian(Polar& vision);
Vector3f PolarToCartecian(Vector3f& vision);

Polar CartecianToPolar(Vector3f);

Vector3f rotateAroundLine(Vector3f,Vector3f,double,Vector3f=Vector3f(0,0,0));


#endif

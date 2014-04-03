#ifndef MYMATH_H
#define MYMATH_H

#include "Types.h"


#include <vector>
#include<eigen3/Eigen/Core>
using namespace std;


double DegToRad(double angle);

double RadToDeg(double angle);

Eigen::Vector3f PolarToCartecian(Polar& vision);
Eigen::Vector3f PolarToCartecian(Eigen::Vector3f& vision);

Polar CartecianToPolar(Eigen::Vector3f);

Eigen::Vector3f rotateAroundLine(Eigen::Vector3f,Eigen::Vector3f,double,Eigen::Vector3f=Eigen::Vector3f(0,0,0));


#endif

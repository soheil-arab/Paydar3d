#include "Math.h"
#include <fstream>

#define PI 3.141592653589793238512808959406186204433

using namespace std;

Vector3f PolarToCartecian(Vector3f& Vision)
{
    Polar vSense;
    vSense.dist=Vision.x();
    vSense.theta=Vision.y();
    vSense.phi=Vision.z();
    return PolarToCartecian(vSense);
}

Vector3f PolarToCartecian(Polar& vision)
{
    double x=vision.dist * cos(gDegToRad(vision.phi)) * cos(gDegToRad(vision.theta));
    double y=vision.dist * cos(gDegToRad(vision.phi)) * sin(gDegToRad(vision.theta));
    double z=vision.dist * sin(gDegToRad(vision.phi));
    return Vector3f(x,y,z);
}

Polar CartecianToPolar(Vector3f relPos)
{
    double x=relPos.x();
    double y=relPos.y();
    double z=relPos.z();
    double S=sqrt(x*x+y*y);

    Polar vSense;

    vSense.dist=sqrt(x*x+y*y+z*z);
    vSense.phi=gRadToDeg(asin(z/vSense.dist));
    vSense.theta=gRadToDeg(atan2(y,x));
    return vSense;
}

Vector3f rotateAroundLine(Vector3f point,Vector3f vec,double theta,Vector3f origin)
{
    double x=point.x();
    double y=point.y();
    double z=point.z();

    double a=origin.x();
    double b=origin.y();
    double c=origin.z();

    double u=vec.x();
    double v=vec.y();
    double w=vec.z();

    double newX=(a*(v*v+w*w)+u*(-b*v-c*w+u*x+v*y+w*z)+((x-a)*(v*v+w*w)+u*(b*v+c*w-v*y-w*z))*gCos(gDegToRad(theta))+sqrt(u*u+v*v+w*w)*(b*w-c*v-w*y+v*z)*gSin(gDegToRad(theta)))/(u*u+v*v+w*w);
    double newY=(b*(u*u+w*w)+v*(-a*u-c*w+u*x+v*y+w*z)+((y-b)*(u*u+w*w)+v*(a*u+c*w-u*x-w*z))*gCos(gDegToRad(theta))+sqrt(u*u+v*v+w*w)*(-a*w+c*u+w*x-u*z)*gSin(gDegToRad(theta)))/(u*u+v*v+w*w);
    double newZ=(c*(u*u+v*v)+w*(-a*u-b*v+u*x+v*y+w*z)+((z-c)*(u*u+v*v)+w*(a*u+b*v-u*x-v*y))*gCos(gDegToRad(theta))+sqrt(u*u+v*v+w*w)*(a*v-b*u-v*x+u*y)*gSin(gDegToRad(theta)))/(u*u+v*v+w*w);

    return Vector3f(newX,newY,newZ);
}

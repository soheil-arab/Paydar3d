#ifndef LINE_H
#define LINE_H

#include "Geom.h"
#include "rvdraw.h"
#include "cmath"
#include "Math.h"
#include "math.h"

using namespace std;
class line{
    public:
        line(){};
        line(Vector3f,Vector3f,double);
        line(Vector3f p1,Vector3f p2);
        Vector3f getBegin();
        Vector3f getEnd();
        string toString();
        line line_PolarToCartesian();
        double getLength();




        Vector3f begin;
        Vector3f end;
        double time_we_saw_it;
};

#endif

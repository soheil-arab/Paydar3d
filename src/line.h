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
        line(Vector3f,Vector3f,double);
        Vector3f getBegin();
        Vector3f getEnd();
        string toString();



        Vector3f begin;
        Vector3f end;
        double time_we_saw_it;
};

#endif

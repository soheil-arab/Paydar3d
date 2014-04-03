#ifndef LINE_H
#define LINE_H

#include "Geom.h"
#include "rvdraw.h"
#include "cmath"
#include "Math.h"
#include "math.h"
#include <eigen3/Eigen/Core>
using namespace std;
class line{
    public:
        line(){};
        line(Eigen::Vector3f,Eigen::Vector3f,double);
        line(Eigen::Vector3f p1,Eigen::Vector3f p2);
        Eigen::Vector3f getBegin();
        Eigen::Vector3f getEnd();
        string toString();
        line line_PolarToCartesian();
        double getLength();




        Eigen::Vector3f begin;
        Eigen::Vector3f end;
        double time_we_saw_it;
};

#endif

#include "line.h"
#include "Geom.h"
#include "rvdraw.h"
#include <string>
#include <eigen3/Eigen/Core>
using namespace std;

//
line::line(Eigen::Vector3f p1,Eigen::Vector3f p2,double time)
{
//    begin=PolarToCartecian(p1);
//    end=PolarToCartecian(p2);
    time_we_saw_it=time;
    begin =p1;
    end=p2;

}

line::line(Eigen::Vector3f p1,Eigen::Vector3f p2)
{
//    begin=PolarToCartecian(p1);
//    end=PolarToCartecian(p2);
    begin =p1;
    end=p2;
}



line line::line_PolarToCartesian()
{
    line l_cartesian;
    l_cartesian=line(PolarToCartecian(begin) , PolarToCartecian(end));
    return l_cartesian;
}





double line::getLength()
{
    return (end - begin).norm();
}



Eigen::Vector3f line::getBegin()
{
    return begin;
}

Eigen::Vector3f line::getEnd()
{
    return end;
}

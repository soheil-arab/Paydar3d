#include "line.h"
#include "Geom.h"
#include "rvdraw.h"
#include <string>

using namespace std;

//
line::line(Vector3f p1,Vector3f p2,double time)
{
//    begin=PolarToCartecian(p1);
//    end=PolarToCartecian(p2);
    time_we_saw_it=time;
    begin =p1;
    end=p2;

}

line::line(Vector3f p1,Vector3f p2)
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
    return (end - begin).Length();
}



Vector3f line::getBegin()
{
    return begin;
}

Vector3f line::getEnd()
{
    return end;
}

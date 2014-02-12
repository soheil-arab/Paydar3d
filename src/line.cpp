#include "line.h"
#include "Geom.h"
#include "rvdraw.h"
#include <string>

using namespace std;

//
line::line(Vector3f p1,Vector3f p2,double time)
{
    begin=p1;
    end=p2;
    double time_we_saw_it=time;

}










Vector3f line::getBegin()
{
    return begin;
}

Vector3f line::getEnd()
{
    return end;
}



#include "Localize.h"
#include "WorldModel.h"
#include "line.h"



using namespace std;

Localize::Localize(WorldModel *wm)
{
    WM=wm;
}

void Localize::test()
{
    if(WM->getLastSeenLines().size()>0 && WM->getFlag().size()>0){
    line l=WM->getLastSeenLines().at(0);
//    sideTurnRcout<<(l.begin)<<"end: "<<(l.end)<<" the point "<<Distance_Point_Line(Vector3f(0,0,0),l)<<endl;
    }

}

/*
 *calculate the distance of a point and a line
 */
double Localize::Distance_Point_Line(Vector3f point_polar,line l)
{
    Vector3f begin_polar=l.begin;
    Vector3f end_polar=l.end;
    Vector3f begin_cartesian=PolarToCartecian(begin_polar);
    Vector3f end_cartesian=PolarToCartecian(end_polar);
    Vector3f point_cartesian=PolarToCartecian(point_polar);

    Vector3f begin_to_point=point_cartesian-begin_cartesian;
    Vector3f begin_to_end=end_cartesian-begin_cartesian;

    double angle_rad=acos((begin_to_end.Dot(begin_to_point))/(begin_to_end.Length()*begin_to_point.Length()));
    return begin_to_point.Length()*sin(angle_rad);

}

Vector3f Localize::calcPlaneNormal(vector<Vector3f> planePoints)
{


}



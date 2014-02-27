

#include "Localize.h"
#include "WorldModel.h"
#include "line.h"



using namespace std;

Localize::Localize(WorldModel *wm)
{
    WM=wm;



    /*
     *             ---------------------------FR--------------------------
     *             |               |                    |                |
     *             |              PRN                  PRS               |
     *             |               |                    |                |
     *             |               |                    |                |
     *             |               ---------PRL----------                |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             FN                                                    FS
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             ------------------------------------------------------
     */

    //defining the global position of the lines
    line_global["FR"] = line(Vector3f(15,-10,0),Vector3f(15,10,0));
    line_global["FL"]=line(Vector3f(-15,-10,0),Vector3f(-15,10,0));
    line_global["FN"]=line(Vector3f(15,-10,0),Vector3f(-15,-10,0));
    line_global["FS"]=line(Vector3f(15,10,0),Vector3f(-15,10,0));
    line_global["PRN"]=line(Vector3f(15,-1.95,0),Vector3f(13.2,-1.95,0));
    line_global["PRL"]=line(Vector3f(13.2,-1.95,0),Vector3f(13.2,1.95,0));
    line_global["PRS"]=line(Vector3f(13.2,1.95,0),Vector3f(15,1.95,0));

}

void Localize::test()
{

//    cout<<WM->getFlagGlobal()["F1R"]<<"soheile kalle kiri"<< endl;
//    cout<<WM->getFlagGlobal()["G2L"]<<"soheili kalle tokhmi"<<endl;
//    cout<<Distance_Cartesian_Point_Line(WM->getFlagGlobal()["F1R"],line_global["PRL"])<<endl;
}

/*
 *calculate the distance of a point and a line
 */
double Localize::Distance_Polar_Point_Line(Vector3f point_polar,line l)
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

double Localize::Distance_Cartesian_Point_Line(Vector3f point_polar,line l)
{
    Vector3f begin_cartesian=l.begin;
    Vector3f end_cartesian=l.end;
    Vector3f point_cartesian=point_polar;

    Vector3f begin_to_point=point_cartesian-begin_cartesian;
    Vector3f begin_to_end=end_cartesian-begin_cartesian;

    double angle_rad=acos((begin_to_end.Dot(begin_to_point))/(begin_to_end.Length()*begin_to_point.Length()));
    return begin_to_point.Length()*sin(angle_rad);

}









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
    if(WM->getLastSeenLines().size()>0 && WM->getFlag().size()>0){
        line l=WM->getLastSeenLines().at(0);
        //    sideTurnRcout<<(l.begin)<<"end: "<<(l.end)<<" the point "<<Distance_Point_Line(Vector3f(0,0,0),l)<<endl;
    }

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


Vector3f Localize::calcPlaneNormal(vector<Vector3f> planePoints)
{
    if (planePoints.size() < 3) {
        return Vector3f(0,0,0) ;
    }

    double** vec = new double*[planePoints.size()];
    double x = 0, y = 0, z = 0;
    for(int i=0;i<planePoints.size();i++){
        vec[i] = new double[3];
        x += planePoints.at(i).x();
        y += planePoints.at(i).y();
        z += planePoints.at(i).z();
    }

    Vector3f cntPoint(x / planePoints.size(), y / planePoints.size(), z / planePoints.size());

    for(int i=0;i<planePoints.size();i++){
        vec[i][0] = planePoints.at(i).x() - cntPoint.x();
        vec[i][1] = planePoints.at(i).y() - cntPoint.y();
        vec[i][2] = planePoints.at(i).z() - cntPoint.z();
    }

    // Compute SVD & Plane Normal
    SVD svd(vec,planePoints.size(), 3);
    double** V = svd.getV();
    Vector3f res(V[0][2], V[1][2], V[2][2]);

    cntPoint.Normalize();

    if (cntPoint.Dot(res) > 0) {
        res*=-1;
    }

    return res;

}

Matrix Localize::calcGlobalCoordinate(Vector3f norm)
{
    for (map<string, Polar>::iterator i = WM->flagPolar.begin(); i != WM->flagPolar.end(); i++)
    {
        if (WM->flagLastSeen [ i->first ] != WM->getServerTime())
        {
            continue;
        }
        for (map<string, Polar>::iterator j = WM->flagPolar.begin() ; j != WM->flagPolar.end(); j++)
        {
            if (WM->flagLastSeen [ j->first ] != WM->getServerTime())
            {
                continue;
            }
            if (  i->first ==  j->first )
            {
                continue ;
            }
            if ( i->first[2] != j->first[2] )
            {
                continue ;
            }


        }
    }
}







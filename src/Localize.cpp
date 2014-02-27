

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
     *             |               ---------PR----------                 |
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
     *             -------------------------M-----------------------------
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |               ---------PL----------                 |
     *             |               |                    |                |
     *             |               PLN                  PLS              |
     *             |               |                    |                |
     *             |               |                    |                |
     *              -----------------------FL-----------------------------
     */

    //defining the global position of the lines
    line_global["FR"] = line(Vector3f(15,-10,0),Vector3f(15,10,0));
    line_global["FL"]=line(Vector3f(-15,-10,0),Vector3f(-15,10,0));
    line_global["FN"]=line(Vector3f(15,-10,0),Vector3f(-15,-10,0));
    line_global["FS"]=line(Vector3f(15,10,0),Vector3f(-15,10,0));
    line_global["PRN"]=line(Vector3f(15,-1.95,0),Vector3f(13.2,-1.95,0));
    line_global["PR"]=line(Vector3f(13.2,-1.95,0),Vector3f(13.2,1.95,0));
    line_global["PRS"]=line(Vector3f(13.2,1.95,0),Vector3f(15,1.95,0));
    line_global["PLN"]=line(Vector3f(-15,-1.95,0),Vector3f(-13.2,-1.95,0));
    line_global["PL"]=line(Vector3f(-13.2,-1.95,0),Vector3f(-13.2,1.95,0));
    line_global["PLS"]=line(Vector3f(-15,1.95,0),Vector3f(-13.2,1.95,0));

    flag_names.push_back("G1L");
    flag_names.push_back("G2L");
    flag_names.push_back("G1R");
    flag_names.push_back("G2R");
    flag_names.push_back("F1L");
    flag_names.push_back("F2L");
    flag_names.push_back("F1R");
    flag_names.push_back("F2R");

      line_names.push_back("FL");
    line_names.push_back("FR");

    line_names.push_back("FN");
    line_names.push_back("FS");
    line_names.push_back("PRN");
    line_names.push_back("PR");
    line_names.push_back("PRS");
    line_names.push_back("PLN");
    line_names.push_back("PL");
    line_names.push_back("PLS");





}



void Localize::test()
{
    if(WM->getLastSeenLines().size()>0)
    {

        for(vector<line>::iterator l=WM->getLastSeenLines().begin();l!=WM->getLastSeenLines().end();l++)
        {
            cout<<"num of lines : "<<WM->getLastSeenLines().size()<<endl;
            Vector3f beg = l->getBegin();
            Vector3f end = l->getEnd();
            beg = PolarToCartecian(beg);
            end = PolarToCartecian(end);
            line lcart=line(beg,end);
            cout<<"beg : " << beg << "end : " <<end <<"---> "<<line_recognitation(lcart)<<endl;
        }
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




//Vector3f Localize::calcPlaneNormal(vector<Vector3f> planePoints)
//{
//    if (planePoints.size() < 3) {
//        return ;
//    }

//    double** vec = new double*[planePoints.size()];
//    for(int i=0;i<planePoints.size();i++){
//        vec[i] = new double[3];
//        x += planePoints.at(i).x();
//        y += planePoints.at(i).y();
//        z += planePoints.at(i).z();
//    }
//    double x = 0, y = 0, z = 0;

//    Vector3f cntPoint(x / points.size(), y / points.size(), z / points.size());

//    for(int i=0;i<planePoints.size();i++){
//        vec[i][0] = planePoints.at(i).x() - cntPoint.x();
//        vec[i][1] = planePoints.at(i).y() - cntPoint.y();
//        vec[i][2] = planePoints.at(i).z() - cntPoint.z();
//    }

//    // Compute SVD & Plane Normal
//    SVD svd(vec,planePoints.size(), 3);
//    double** V = svd.getV();
//    Vector3f res = new Vector3f(V[0][2], V[1][2], V[2][2]);

//    cntPoint.Normalize();

//    if (centroid3d.dot(res) > 0) {
//        res*=-1;
//    }

//    return res;

//}




string Localize::line_recognitation(line line_to_detect)
{
    double min=1000;
    string result="";


    for(vector<string>::iterator i=flag_names.begin();i!=flag_names.end();i++){

        if(WM->getFlagLastSeen(*i)==WM->serverTime)
        {

            cout<<"flag seen"<<endl;
            for(vector<string> ::iterator j=line_names.begin();j!=line_names.end();j++)
            {



                if(fabs(Distance_Cartesian_Point_Line(WM->flag[*i],line_to_detect) - Distance_Cartesian_Point_Line(WM->flagGlobal[*i],line_global[*j]))<0.5)
                {

                    cout<<"flag name is:"<<*i<<endl;
                    cout<<"line name is:"<<*j<<endl;
                    cout<<Distance_Cartesian_Point_Line(WM->flag[*i],line_to_detect)<<"local distance"<<endl;

                    cout<<Distance_Cartesian_Point_Line(WM->flagGlobal[*i],line_global[*j])<<"global distance"<<endl;
                    cout<<"fabse kiri"<<fabs(Distance_Cartesian_Point_Line(WM->flag[*i],line_to_detect) - Distance_Cartesian_Point_Line(WM->flagGlobal[*i],line_global[*j]))<<endl;

                    result = *j;
                    return result;
//                    cout<<"flag name:"<<*i<<"coordinate: "WM->flag[*i]<<"line name:"<<*j<<endl;
//                    cout<<"min: "<<min<<endl;
                }
            }
        }
    }

    return result;
}






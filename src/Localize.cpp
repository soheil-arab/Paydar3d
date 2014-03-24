

#include "Localize.h"
#include "WorldModel.h"
#include "line.h"
#include "string.h"
#include <eigen3/Eigen/Core>


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
    line_global["PRN"]=line(Vector3f(15,3,0),Vector3f(13.2,3,0));
    line_global["PR"]=line(Vector3f(13.2,-3,0),Vector3f(13.2,3,0));
    line_global["PRS"]=line(Vector3f(13.2,-3,0),Vector3f(15,-3,0));
    line_global["PLN"]=line(Vector3f(-15,3,0),Vector3f(-13.2,3,0));
    line_global["PL"]=line(Vector3f(-13.2,-3,0),Vector3f(-13.2,3,0));
    line_global["PLS"]=line(Vector3f(-15,-3,0),Vector3f(-13.2,-3,0));

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
    vector<line> lastSeenLines =WM->getLastSeenLines() ;
    int nrOFLastSeen = lastSeenLines.size() ;
    for(int counter=0;counter<nrOFLastSeen;counter++)
    {
        line l=lastSeenLines.at(counter);
        Vector3f beg=PolarToCartecian(l.begin);
        Vector3f end=PolarToCartecian(l.end);
        line lcart(beg,end);
        string l_name=line_recognitation(lcart);
    }

    WM->resetLastSeenLines();
}

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

double Localize::Distance_Cartesian_Point_Line(Vector3f point_cartesian,line l)
{
    Vector3f begin_cartesian=l.begin;
    Vector3f end_cartesian=l.end;

    Vector3f begin_to_point=point_cartesian-begin_cartesian;
    Vector3f begin_to_end=end_cartesian-begin_cartesian;

    double begin_to_end_length = begin_to_end.Length();
    double begin_to_point_length = begin_to_point.Length();

    if( begin_to_end_length <0.1 || begin_to_point_length <0.1)
        return 0;


    double angle_rad=acos((begin_to_end.Dot(begin_to_point))/(begin_to_end_length*begin_to_point_length));
    return begin_to_point_length*sin(angle_rad);

}


Eigen::MatrixXi Localize::overdetermined(Eigen::MatrixXi A,Eigen::VectorXf b)
{
    return (A);
}



Vector3f Localize::line_intersection(line l1, line l2)
{
    Vector3f result=Vector3f(0,0,0);
    Vector3f normal_l1=(l1.end-l1.begin)/((l1.end-l1.begin).Length());
    Vector3f normal_l2=(l2.end-l2.begin)/((l2.end-l2.begin).Length());

    if((normal_l1+normal_l2).Length() < 0.1 || (normal_l1-normal_l2).Length() < 0.1 || (normal_l2-normal_l1).Length() < 0.1)
    {
        cout<<"parallel"<<endl;
        return result;
    }
}


string Localize::line_recognitation(line line_to_detect)
{
    string result="";

    bool breakup=false;
    Vector3f distance;

    for (unordered_map<string,line>::iterator l_iterator=line_global.begin() ; l_iterator != line_global.end() ; l_iterator++)
    {
        for (unordered_map<string, Vector3f>::iterator f_iterator=WM->flagGlobal.begin() ; f_iterator != WM->flagGlobal.end() ; f_iterator++)
        {
            distance = f_iterator->second - l_iterator->second.begin;
            if(WM->getFlagLastSeen(f_iterator->first) ==  WM->serverTime
                    && (  fabs(Distance_Cartesian_Point_Line(WM->flag[f_iterator->first],line_to_detect)
                    -  Distance_Cartesian_Point_Line(f_iterator->second,l_iterator->second)) > 0.5
                                                                        || distance.Length() > 20 ) )
            {
                breakup=true;
                break;
            }

        }
        if(breakup)
        {
            breakup=false;
            continue;
        }
        result=l_iterator->first;
        return result;
    }
}

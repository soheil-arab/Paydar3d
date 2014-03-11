

#include "Localize.h"
#include "WorldModel.h"
#include "line.h"
#include "string.h"



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
    if(WM->getLastSeenLines().size()>1)
    {
        for(int counter=0;counter<WM->getLastSeenLines().size();counter++)
        {
            line l=WM->getLastSeenLines()[counter];
            Vector3f beg=PolarToCartecian(l.begin);
            Vector3f end=PolarToCartecian(l.end);
            line lcart=line(beg,end);
            string l_name=line_recognitation(lcart);

            //            if(l_name=="PRS")
            //                RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate.Rotate(lcart.begin),WM->sensedPos+WM->headRotate.Rotate(lcart.end),RED,1);
            if(l_name=="PR")
                RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate.Rotate(lcart.begin),WM->sensedPos+WM->headRotate.Rotate(lcart.end),GREEN,2);
            if(l_name=="PRN")
                RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate.Rotate(lcart.begin),WM->sensedPos+WM->headRotate.Rotate(lcart.end),BLACK,3);
            if(l_name=="PRS")
                RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate.Rotate(lcart.begin),WM->sensedPos+WM->headRotate.Rotate(lcart.end),YELLO,7);
            if(l_name=="FR")
                RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate.Rotate(lcart.begin),WM->sensedPos+WM->headRotate.Rotate(lcart.end),RED,4);
            if(l_name=="FS")
                RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate.Rotate(lcart.begin),WM->sensedPos+WM->headRotate.Rotate(lcart.end),BLUE,5);
            if(l_name=="FN")
                RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate.Rotate(lcart.begin),WM->sensedPos+WM->headRotate.Rotate(lcart.end),GREEN,6);
        }
    }
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

double Localize::Distance_Cartesian_Point_Line(Vector3f point_cartesian,line l)
{
    Vector3f begin_cartesian=l.begin;
    Vector3f end_cartesian=l.end;

    Vector3f begin_to_point=point_cartesian-begin_cartesian;
    Vector3f begin_to_end=end_cartesian-begin_cartesian;


    if(begin_to_end.Length()<0.1 || begin_to_end.Length()<0.1)
        return 0;


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

    bool breakup=false;
    Vector3f distance;


    Vector3f beg;
    Vector3f end;
    Vector3f the_other_vector;

    Vector3f beg_to_flag;
    Vector3f end_to_flag;






    // cout <<"avalesh"<<endl;

    for(vector<string>::iterator l_iterator=line_names.begin() ; l_iterator != line_names.end(); l_iterator++)
    {

//        if(*l_iterator == "FR" || *l_iterator=="FN" || *l_iterator=="FS")
//        {
//            if(WM->getFlagLastSeen("F1R") == WM->serverTime)
//            {
//                if(Distance_Cartesian_Point_Line(WM->flag["F1R"],line_to_detect) < 0.1)
//                {

//                    beg=PolarToCartecian(line_to_detect.begin);
//                    end=PolarToCartecian(line_to_detect.end);
//                    the_other_vector=end;

//                    beg_to_flag=WM->flag["F1R"]-beg;
//                    end_to_flag=WM->flag["F1R"]-end;

//                    if(end_to_flag.Length()<0.1 && end_to_flag.Length()<beg_to_flag.Length())
//                    {
//                        the_other_vector=beg;
//                    }

//                    if( CartecianToPolar(WM->flag["F1R"]).theta > CartecianToPolar(the_other_vector).theta)
//                    {
//                        return "FR";
//                    }

//                    else{
//                        return "FN";
//                    }

//                }

//            }



//            if(WM->getFlagLastSeen("F2R") == WM->serverTime)
//            {
//                if(Distance_Cartesian_Point_Line(WM->flag["F2R"],line_to_detect) < 0.1)
//                {

//                    beg=PolarToCartecian(line_to_detect.begin);
//                    end=PolarToCartecian(line_to_detect.end);
//                    the_other_vector=end;

//                    beg_to_flag=WM->flag["F2R"]-beg;
//                    end_to_flag=WM->flag["F2R"]-end;

//                    if(end_to_flag.Length()<0.1 && end_to_flag.Length()<beg_to_flag.Length())
//                    {
//                        the_other_vector=beg;
//                    }



//                    if( CartecianToPolar(WM->flag["F2R"]).theta > CartecianToPolar(the_other_vector).theta)
//                    {
//                        return "FS";
//                    }

//                    else{
//                        return "FR";
//                    }

//                }

//            }
//        }



//        // cout <<"vasate"<<endl;

        //the other situations
        for(vector<string>::iterator f_iterator=flag_names.begin();f_iterator!=flag_names.end();f_iterator++)
        {

            // cout <<"avale for"<<endl;

            distance=WM->flagGlobal[*f_iterator]-line_global[*l_iterator].begin ;

            // cout <<"dovome for"<<endl;


            if(WM->getFlagLastSeen(*f_iterator) ==  WM->serverTime &&(  fabs(Distance_Cartesian_Point_Line(WM->flag[*f_iterator],line_to_detect)
                                                                             -  Distance_Cartesian_Point_Line(WM->flagGlobal[*f_iterator],line_global[*l_iterator])) > 0.5
                                                                        || distance.Length() > 20 )
                    )
            {
                // cout <<"omad vase break"<<endl;
                breakup=true;
                break;
            }


        }

        // cout <<"for tamom shod"<<endl;

        if(breakup)
        {
            breakup=false;
            continue;
        }
        result=*l_iterator;

        // cout <<"result :"<<result<<endl;

        return result;
    }








    //                          // cout <<CartecianToPolar(flag).theta<<"and the other is: "<<CartecianToPolar(the_other_vector).theta<<endl;

    //                            for(vector<string>::iterator i=flag_names.begin();i!=flag_names.end();i++){

    //                                if(WM->getFlagLastSeen(*i)==WM->serverTime)
    //                                {


    //                                    for(vector<string> ::iterator j=line_names.begin();j!=line_names.end();j++)
    //                                    {

    //                                        if(!((*j=="FR" && *i=="F1R") || (*j=="FR" && *i=="F2R") || (*j=="FS" && *i=="F2R") || (*j=="FN" && *i=="F1R")))
    //                                        {

    //                                            if(fabs(Distance_Cartesian_Point_Line(WM->flag[*i],line_to_detect) - Distance_Cartesian_Point_Line(WM->flagGlobal[*i],line_global[*j]))<min)
    //                                            {
    //                                                min=fabs(Distance_Cartesian_Point_Line(WM->flag[*i],line_to_detect) - Distance_Cartesian_Point_Line(WM->flagGlobal[*i],line_global[*j]));
    //                                                result=*j;

    //                                            }
    //                                        }

    //                                        else
    //                                        {

    //                                            if(fabs(Distance_Cartesian_Point_Line(WM->flag[*i],line_to_detect))<0.1)
    //                                            {

    //                                                // cout <<"omad to"<<endl;

    //                                                Vector3f flag=WM->flag[*i];
    //                                                Vector3f beg=PolarToCartecian(line_to_detect.begin);
    //                                                Vector3f end=PolarToCartecian(line_to_detect.end);
    //                                                Vector3f the_other_vector=end;

    //                                                Vector3f beg_to_flag=flag-beg;
    //                                                Vector3f end_to_flag=flag-end;

    //                                                if(end_to_flag.Length()<0.1 && end_to_flag.Length()<beg_to_flag.Length())
    //                                                {
    //                                                    the_other_vector=beg;
    //                                                }


    //                                                if(*i=="F1R")
    //                                                {


    //                                                    // cout <<"F1R"<<endl;
    //                            if(CartecianToPolar(flag).theta > CartecianToPolar(the_other_vector).theta)
    //                            {
    //                                return "FR";

    //                            }
    //                            else
    //                            {
    //                                // cout <<"FN"<<endl;
    //                                return "FN";
    //                            }
    //                        }


    //                        if(*i=="F2R")
    //                        {
    //                            // cout <<"F2R"<<endl;
    //                            // cout <<CartecianToPolar(flag).theta<<"and the other is: "<<CartecianToPolar(the_other_vector).theta<<endl;

    //                            if(CartecianToPolar(flag).theta > CartecianToPolar(the_other_vector).theta)
    //                            {
    //                                // cout <<"FS"<<endl;
    //                                return "FS";

    //                            }
    //                            else
    //                            {
    //                                return "FR";
    //                            }
    //                        }
    //                    }
    //                }

    //            }

    //        }
    //    }

    //    if(min<0.1)
    //    {
    //        //        // cout <<result<<endl;
    //        return result;
    //    }
    //    else
    //    {
    //        //        // cout <<"the others"<<endl;
    //        return "the others";
    //    }
}






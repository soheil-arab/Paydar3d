
/*
   RRRR    OOOOO   BBBBB    OOOOO  TTTTTTTT  OOOOO   OOOOO   SSSSS
   R  R    O   O   B    B   O   O     T      O   O   O   O  S
   RRRR    O   O   BBBBB    O   O     T      O   O   O   O   SSSS
   R  R    O   O   B    B   O   O     T      O   O   O   O       S
   R   R   OOOOO   BBBBB    OOOOO     T      OOOOO   OOOOO  SSSSS
*/
#include "WorldModel.h"
#include <cstdlib>
#include <algorithm>
#include "Geom.h"
#include "rvdraw.h"

WorldModel::WorldModel()
{
    ballLastSeen = -1;
    ballPrevLastSeen = -1;
    timeLastHear=-1;
    Localed=false;
    Sense = false;
    myZ = 0.51 ;
    myOrien = 0;

    bodyRotate.Identity();
    speed=Vector3f(0,0,0);

    \
    headR.Identity();
    /// set Joints Name
    names[0] = "he1";
    names[1] = "he2";
    names[2] = "lle1";
    names[3] = "rle1";
    names[4] = "lle2";
    names[5] = "rle2";
    names[6] = "lle3";
    names[7] = "rle3";
    names[8] = "lle4";
    names[9] = "rle4";
    names[10] = "lle5";
    names[11] = "rle5";
    names[12] = "lle6";
    names[13] = "rle6";
    names[14] = "lae1";
    names[15] = "rae1";
    names[16] = "lae2";
    names[17] = "rae2";
    names[18] = "lae3";
    names[19] = "rae3";
    names[20] = "lae4";
    names[21] = "rae4";
}

void WorldModel::clearBodyRotate(){
    bodyRotate.Identity();
}

WorldModel::~WorldModel()
{

}

map<string,Vector3f> WorldModel::getFlagGlobal()
{
    return flagGlobal;
}


///localize with lines and flags
Vector3f WorldModel::localize_with_lines(map<line, double> lines_we_see,map<string, Polar> flagPolar)
{

}


vector<line> WorldModel::getLastSeenLines()
{
    vector<line> lastSeenLines;
    for(int i=0;i<lines_we_see.size();i++)
    {
        if(lines_we_see[i].time_we_saw_it==serverTime)
            lastSeenLines.insert(lastSeenLines.end(),lines_we_see[i]);
    }

    return lastSeenLines;
}


///set seen lines,the lines are in head rotation coordination and should be convert to global if needed
void WorldModel::setSeenLines(line l)
{
    lines_we_see.insert(lines_we_see.end(),l);
    //    cout<<l.begin<<" and "<<l.end<<endl;
    //    cout<<"begin "<<sensedPos+bodyRotate.Rotate(l.begin)<<endl;
    //    RVDraw::instance()->drawVector3f(sensedPos+headRotate.Rotate(PolarToCartecian(l.begin)),RED,10);
    //    RVDraw::instance()->drawVector3f(sensedPos+headRotate.Rotate(PolarToCartecian(l.end)),BLACK,10);
}




//vector<line> WorldModel::getLines_WeSawLastCycle()
//{

//}


///
map<string , Vector3f> WorldModel::getFlag()
{
    return flag;
}



/// set Seen Flags Polar Pos
void WorldModel::setFlagPos(string name, Polar pos)
{
    flagPolar[name] = pos;
    flag[name] = PolarToCartecian(pos);
    flagLastSeen[name] = serverTime;
}
double WorldModel::getFlagLastSeen(string name)
{
    return flagLastSeen[name];
}
double WorldModel::getBallLastSeen()
{
    return ballLastSeen;
}

//checks that if the agent was beamed by server after goal or a foul
bool WorldModel::isServerBeamed()
{
    static Vector3f lastPos=getMyPos();

    for (map<string, Polar>::iterator i = flagPolar.begin(); i != flagPolar.end(); i++)
    {
        if (flagLastSeen [ i->first ] != serverTime)
        {
            continue;
        }
        if(fabs((i->second.dist)-(lastPos-flagGlobal[i->first]).Length())>1){
            lastPos=getMyPos();
            return true;
        }
    }
    lastPos=getMyPos();
    return false;

}

///~ Our Team Localization


void WorldModel::RotateHead1(double val){
    headR.RotateZ(-Deg2Rad(val));

}

void WorldModel::RotateHead2(double val){
    headR.RotateY(Deg2Rad(val));
}




//this function will rotate an orirentation around an axis with specified angle, the angle should be in radian

Vector3f WorldModel::general_rotation(Vector3f initial,Vector3f axis,double angle)
{
    //    cout<<"angle: "<<angle<<endl;
    //    cout<<"axis: "<<axis<<endl;
    Vector3f normaled_axis(axis.x()/axis.Length(),axis.y()/axis.Length(),axis.z()/axis.Length());
    double ux=normaled_axis.x();
    double uy=normaled_axis.y();
    double uz=normaled_axis.z();
    Matrix Rotation(cos(angle)+ux*ux*(1-cos(angle)), ux*uy*(1-cos(angle))-uz*sin(angle), ux*uz*(1-cos(angle))+uy*sin(angle), 0,
                    ux*uy*(1-cos(angle))+uz*sin(angle), cos(angle)+uy*uy*(1-cos(angle)), uy*uz*(1-cos(angle))-ux*sin(angle), 0,
                    ux*uz*(1-cos(angle))-uy*sin(angle), uy*uz*(1-cos(angle))+ux*sin(angle), cos(angle)+uz*uz*(1-cos(angle)), 0,
                    0, 0, 0, 1);
    Vector3f rotated=Rotation.Transform(initial);
    //    cout<<"rotated: "<<rotated<<endl;
    return rotated;


}



void WorldModel::brinBeMA(){
    isServerBeamed();
    initDimentions();
    initFlags();
    kalman_filter();


    //    cout<<(int)(getACC().x()*10000)/(float)10000<<" "<<(int)(getACC().y()*10000)/(float)10000<<" "<<(int)(getACC().z()*10000)/(float)10000<<endl;

    //    RotateHead1(getLastJointAngle("he1")-getJointAngle("he1"));
    //    RotateHead2(getLastJointAngle("he2")-getJointAngle("he2"));

    //    headR.RotationZ(Deg2Rad(getJointAngle("he1")));
    //    headR.RotateY(-Deg2Rad(getJointAngle("he2")));

    //        cout<<"he1: "<<getJointAngle("he1")<<endl;
    //        cout<<"he2: "<<getJointAngle("he2")<<endl;

    //    cout << (getJointAngle("he1")) << endl;
    //    cout << (getJointAngle("he2")) << endl;
    //    cout << "===" << endl;

    Vector3f rightGyro (gyro.y(),-gyro.x(),gyro.z());
    Vector3f newGyro = bodyRotate.Rotate(rightGyro);

    double theta = Deg2Rad( newGyro.Length() * 0.02);

    if ( newGyro.Length() > 0.2 )
        newGyro = newGyro / newGyro.Length();


    Vector3f x = bodyRotate.Transform(Vector3f(1,0,0));
    Vector3f y = bodyRotate.Transform(Vector3f(0,1,0));
    Vector3f z = bodyRotate.Transform(Vector3f(0,0,1));


    Vector3f newx = x*cos(theta) + (newGyro.Cross(x))*sin(theta) + newGyro * (newGyro.Dot(x))*(1-cos(theta));
    Vector3f newy = y*cos(theta) + (newGyro.Cross(y))*sin(theta) + newGyro * (newGyro.Dot(y))*(1-cos(theta));
    Vector3f newz = newx.Cross(newy);


    bodyRotate.Set( newx.x() , newy.x() , newz.x() , 0 ,
                    newx.y() , newy.y() , newz.y() , 0 ,
                    newx.z() , newy.z() , newz.z() , 0 ,
                    0 , 0 , 0 , 1);

    setMyAngle(atan2Deg(newx.y(),newx.x()));
    //    cout << atan2Deg(newx.y(),newx.x()) << endl;


    //        RVDraw::instance()->drawLine(sensedPos,sensedPos+newx,GREEN,24);
    //        RVDraw::instance()->drawLine(sensedPos,sensedPos+newy,GREEN,25);
    //        RVDraw::instance()->drawLine(sensedPos,sensedPos+newz,GREEN,26);


    Vector3f x_after_he1_rotation=general_rotation(newx,newz,Deg2Rad(getJointAngle("he1")));
    Vector3f y_after_he1_rotation=general_rotation(newy,newz,Deg2Rad(getJointAngle("he1")));
    Vector3f z_after_he1_rotation=general_rotation(newz,newz,Deg2Rad(getJointAngle("he1")));

    Vector3f x_after_he2_rotation=general_rotation(x_after_he1_rotation,y_after_he1_rotation,-Deg2Rad(getJointAngle("he2")));
    Vector3f y_after_he2_rotation=general_rotation(y_after_he1_rotation,y_after_he1_rotation,-Deg2Rad(getJointAngle("he2")));
    Vector3f z_after_he2_rotation=general_rotation(z_after_he1_rotation,y_after_he1_rotation,-Deg2Rad(getJointAngle("he2")));

    headRotate.Set(x_after_he2_rotation.x(),y_after_he2_rotation.x(),z_after_he2_rotation.x(),0,
                   x_after_he2_rotation.y(),y_after_he2_rotation.y(),z_after_he2_rotation.y(),0,
                   x_after_he2_rotation.z(),y_after_he2_rotation.z(),z_after_he2_rotation.z(),0,
                   0, 0, 0, 1);


    //    cout<<"he1: "<<(getJointAngle("he1"))<<endl;
    //    cout<<"we : "<<Rad2Deg(acos(newx.Dot(x_after_he1_rotation)))<<endl;


    //    cout<<"he2: "<<(getJointAngle("he2"))<<endl;
    //    cout<<"we : "<<Rad2Deg(acos(x_after_he2_rotation.Dot(x_after_he1_rotation)))<<endl;


    //           RVDraw::instance()->drawLine(sensedPos,sensedPos+x_after_he2_rotation*10,RED,21);
    //           RVDraw::instance()->drawLine(sensedPos,sensedPos+y_after_he2_rotation*10,BLACK,22);
    //           RVDraw::instance()->drawLine(sensedPos,sensedPos+z_after_he2_rotation*10,GREEN,23);


    int numberOfFlags = 0 ;

    Vector3f myPos (0,0,0);
    for (map<string, Vector3f>::iterator i = flag.begin(); i != flag.end(); i++)
    {
        if (flagLastSeen [ i->first ] != serverTime)
        {
            continue;
        }

        Vector3f poss = flagGlobal[i->first] - headRotate.Rotate(i->second);
//        RVDraw::instance()->drawLine(sensedPos,sensedPos+headRotate.Rotate(i->second),GREEN,numberOfFlags);

        myPos += poss;
        numberOfFlags++;
    }

    /**
      for testing purpose
    */
    //    static int posN = 100 ;
    //    posN++;


    if ( numberOfFlags ){
        Localed= true;
        myPos/=numberOfFlags;
        //        if(myPos.z()<=0.04)
        //        {
        //            cout<<"kamtar shod "<<endl;
        //            myPos.z()=0.042;
        //        }
        //        cout<<"mypos: "<<myPos<<endl;
        //
        mylastPos=getMyPos();
        //        myPos=((mylastPos+getSpeed()*0.02)*12+myPos*8)/20;
        myPos=((mylastPos)*8+myPos*12)/20;

        setSpeed((myPos-mylastPos)*50);

        setMyPos(myPos);
//        setMyAngle(Rad2Deg(gArcTan(newx.y()/newx.x())));
        //        RVDraw::instance()->drawVector3f(myPos,GREEN,posN);
        if ( ballLastSeen == serverTime )          /// if See Ball In This Cycle Set Its Pos !
        {
            Vector3f ballPos = myPos + headRotate.Rotate(ball);
            //            RVDraw::instance()->drawVector3f(ballPos,RED,++posN);
            setBallVel(ballPos-getBallPos());
            setBallPos(ballPos);
            if(ballPos.z()<=0)
            {
                //                cout<<"goh shod "<<serverTime<<endl;
                ballPos.Set(Vector3f(ballPos.x(),ballPos.y(),0.042));
            }
            //            else
            //                cout<<"oooo "<<endl;
        }

//        RVDraw::instance()->drawVector3f(getMyPos(),GREEN,5);
//        RVDraw::instance()->drawVector3f(getBallPos(),GREEN,7);
        for ( map<int,RelPlayerInfo>::iterator i = ourRel.begin() ; i != ourRel.end() ; i++ )
        {
            if ( i->first != getMyNum() )
            {
                our [i->first].head = myPos + headRotate.Rotate ( PolarToCartecian(i->second.head) ) ;
            }
            our [i->first].lfoot = myPos + headRotate.Rotate ( PolarToCartecian(i->second.lfoot) ) ;
            our [i->first].rfoot = myPos +headRotate.Rotate ( PolarToCartecian(i->second.rfoot) ) ;
            our [i->first].rlowerarm = myPos +headRotate.Rotate ( PolarToCartecian(i->second.rlowerarm) ) ;
            our [i->first].llowerarm = myPos +headRotate.Rotate ( PolarToCartecian(i->second.llowerarm) ) ;
        }

        for ( map<int,RelPlayerInfo>::iterator i = theirRel.begin() ; i != theirRel.end() ; i++ )
        {
            their [i->first].head = myPos +headRotate.Rotate ( PolarToCartecian(i->second.head) ) ;
            their [i->first].lfoot = myPos +headRotate.Rotate ( PolarToCartecian(i->second.lfoot) ) ;
            their [i->first].rfoot = myPos +headRotate.Rotate ( PolarToCartecian(i->second.rfoot) ) ;
            their [i->first].rlowerarm = myPos +headRotate.Rotate ( PolarToCartecian(i->second.rlowerarm) ) ;
            their [i->first].llowerarm = myPos +headRotate.Rotate ( PolarToCartecian(i->second.llowerarm) ) ;
        }
    }




    if(getLastSeenLines().size()>0  && Time>20)
//    if(lines_we_see.size()>0 && Time>20)
    {
        int size=getLastSeenLines().size();

        for(int i=0;i<size;i++)
        {
            line l=getLastSeenLines()[size-i-1];
            if(l.time_we_saw_it==serverTime ){
//                RVDraw::instance()->drawLine(sensedPos+headRotate.Rotate(PolarToCartecian(l.begin)),sensedPos+headRotate.Rotate(PolarToCartecian(l.end)),RED,2*size+i);

            }
        }
    }
}


//kalman filter
void WorldModel::kalman_filter()
{
    static Vector3f pos  (-0.19,0,0);
    static Vector3f speed(0,0,0);

    if(getACC().Length()>9.809)
    {
        static Matrix state(getMyPos().x(),getMyPos().y(),getMyPos().z(),0,
                            0, 0, 0, 0,
                            0, 0, 0, 0,
                            0, 0, 0, 0);

        Vector3f Real_Acc(0,0,-9.81);
        Vector3f rightACC(getACC().y(),getACC().x(),-getACC().z());
        Vector3f shetab=bodyRotate.Rotate(getACC());
        Vector3f mofid=shetab-Real_Acc;
        //        cout << getACC()<< " server" << endl;
        //        cout<<"niro "<<getFootPress("lf").f<<"and "<<getFootPress("rf").f<<endl;
        //                cout<<"vazn "<<shetab.Length()*4.6071<<endl;

        //4.6071:mass

        pos=pos+0.0002*mofid+0.02*speed;
        speed=speed+0.02*mofid;

        Matrix F(1, 0.02, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 0, 0,
                 0, 0, 0, 0);

        Matrix priori_state_estimate(0,0,0,0,
                                     0,0,0,0,
                                     0,0,0,0,
                                     0,0,0,0);

        Matrix G(0.0002, 0, 0, 0,
                 0.02, 0, 0, 0,
                 0, 0, 0, 0,
                 0, 0, 0, 0);

        Matrix ACC((int)((mofid.z())*10000)/(float)10000,(int)(mofid.y()*10000)/(float)10000,(int)(-mofid.x()*10000)/(float)10000,0,
                   0, 0, 0, 0,
                   0, 0, 0, 0,
                   0, 0, 0, 0);

        static Matrix cov(0,0,0,0,
                          0,0,0,0,
                          0,0,0,0,
                          0,0,0,0);

        Matrix predicted_cov(0,0,0,0,
                             0,0,0,0,
                             0,0,0,0,
                             0,0,0,0);

        Matrix H(1, 0, 0, 0,
                 0, 0, 0, 0,
                 0, 0, 0, 0,
                 0, 0, 0, 0);


        //i'm not sure about this covariance!!
        Matrix cov_of_acc(0.0004, 0, 0, 0,
                          0, 0.0004, 0, 0,
                          0, 0, 0, 0.0004,
                          0, 0, 0, 0);

        Matrix position_by_censors(getMyPos().x(),getMyPos().y(),getMyPos().z(),0,
                                   0, 0, 0, 0,
                                   0, 0, 0, 0,
                                   0, 0, 0, 0);

        //i'm not sure about this covriance!
        Matrix cov_of_position_by_censors(0.01, 0, 0, 0,
                                          0, 0.01, 0, 0,
                                          0, 0, 0.01, 0,
                                          0, 0, 0, 0);

        Matrix I;
        I.Identity();

        Matrix domain_of_error(0,0,0,0,
                               0,0,0,0,
                               0,0,0,0,
                               0,0,0,0);

        domain_of_error=position_by_censors - H * priori_state_estimate;



        Matrix S(0,0,0,0,
                 0,0,0,0,
                 0,0,0,0,
                 0,0,0,0);

        Matrix K(0,0,0,0,
                 0,0,0,0,
                 0,0,0,0,
                 0,0,0,0);

        Matrix H_C=H;
        H_C.InvertRotationMatrix();
        Matrix S_C=S;
        S_C.InvertRotationMatrix();
        Matrix F_C=F;
        F_C.InvertRotationMatrix();
        Matrix G_C=F;
        G_C.InvertRotationMatrix();


        priori_state_estimate = F*state + G*ACC;


        predicted_cov = F * cov * (F_C) + G * G_C * cov_of_acc;

        S= H * predicted_cov * H_C + cov_of_position_by_censors;

        K =predicted_cov * H * S_C;

        state = priori_state_estimate + K * H;

        cov= (I - K*H)*predicted_cov;



        //        RVDraw::instance()->drawVector3f(Vector3f(state.Transform(Vector3f(1,0,0)).x(),state.Transform(Vector3f(0,1,0)).x(),state.Transform(Vector3f(0,0,1)).x()),RED,10);

        //        RVDraw::instance()->drawVector3f(pos,RED,10);

    }



}


//
//void WorldModel::kiiri()
//{
//    static Vector3f posss=myPos;
//    static Vector3f speed(0,0,0);
//    posss=posss+0.0002*(Vector3f(ACC.x()*bodyRotate.Transform(Vector3f(1,0,0))))+
//            0.02*(Vector3f(speed.x()*bodyRotate.Transform(Vector3f(1,0,0))));
//    speed=0.02*(Vector3f(ACC.x()*bodyRotate.Transform(Vector3f(1,0,0))));
//     RVDraw::instance()->drawVector3f(posss,RED,10);
//}



void WorldModel::Localize()
{
    brinBeMA();
    //    getPosbyTwoFlag();
}

void WorldModel::getPosbyTwoFlag()
{

    initDimentions();
    initFlags();
    Localed=false;
    Vector3f pos ;
    int co = 0 ;
    Vector3f sum (0,0,0);

    for (map<string, Polar>::iterator i = flagPolar.begin(); i != flagPolar.end(); i++)
    {
        if (flagLastSeen [ i->first ] != serverTime)
        {
            continue;
        }
        for (map<string, Polar>::iterator j = flagPolar.begin() ; j != flagPolar.end(); j++)
        {
            if (flagLastSeen [ j->first ] != serverTime)
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

            if (   i->first[0]=='G' && j->first[0] == 'F' && i->first[1] == j->first[1]  )
            {
                co ++;
                Localed = true ;
                double fz=ZFromLeft();

                double h1 = 0;
                double h2 = 0.8;
                double a = i->second.dist;
                double b = j->second.dist;
                double d = fabs(flagGlobal[i->first].y() - flagGlobal[j->first].y()) ;


                double fy = (d*d+b*b-a*a+(h2-h1)*(h1+h2-2*fz))/(2*d);
                double fx = sqrt ( b*b-(h1-fz)*(h1-fz) - fy*fy ) ;


                if ( j->first[2] == 'L' && getTeamSide() == Left)
                {
                    fx *= -1 ;
                }
                if ( j->first[2] == 'R' && getTeamSide() == Right)
                {
                    fx *= -1 ;
                }
                if ( getTeamSide() == Right )
                {
                    fy *= -1 ;
                }
                if ( j->first[1] == '2' )
                {
                    fy *= -1 ;
                }


                pos = flagGlobal [j->first] - Vector3f ( fx, fy , -fz );

                sum += pos;

                double theta = VecPosition::normalizeAngle ( CartecianToPolar ( flagGlobal[j->first] - pos ) .theta - j->second.theta  ) ;
                double phi   = VecPosition::normalizeAngle ( CartecianToPolar ( flagGlobal[j->first] - pos ) .phi - j->second.phi ) ;

                setMyAngle ( theta - getJointAngle ("he1") ) ;
                setMyPhi   ( phi   - getJointAngle ("he2") ) ;

                Matrix x ;

                Vector3f I , J , K ;

                x.RotationY (  Deg2Rad ( phi )  ) ;
                x.RotateZ ( Deg2Rad ( -theta ) ) ;

                I = x.Rotate ( Vector3f ( 1 , 0 , 0 ) ) ;
                J = x.Rotate ( Vector3f ( 0 , 1 , 0 ) ) ;
                K = x.Rotate ( Vector3f ( 0 , 0 , 1 ) ) ;

                R.Set ( I.x() , I.y() , I.z() , 0 ,
                        J.x() , J.y() , J.z() , 0 ,
                        K.x() , K.y() , K.z() , 0 ,
                        0     , 0     , 0     , 1 ) ;
            }
            if (  i->first[0]=='G' && j->first[0] == 'G' && i->first[1] == '1' && j->first[1] == '2'   )
            {
                co ++;
                Localed = true ;
                double fz=ZFromLeft();

                double h1 = 0.8;
                double h2 = 0.8;
                double a = i->second.dist;
                double b = j->second.dist;
                double d = fabs(flagGlobal[i->first].y() - flagGlobal[j->first].y()) ;


                double fy = (d*d+b*b-a*a+(h2-h1)*(h1+h2-2*fz))/(2*d);
                double fx = sqrt ( b*b-(h1-fz)*(h1-fz) - fy*fy ) ;

                if ( i->first[2] == 'L' && getTeamSide() == Left)
                {
                    fx *= -1 ;
                }

                if ( i->first[2] == 'R' && getTeamSide() == Right)
                {
                    fx *= -1 ;
                }
                if ( getTeamSide() == Right )
                {
                    fy *= -1 ;
                }
                if ( j->first[1] == '2' )
                {
                    fy *= -1 ;
                }


                pos = flagGlobal [j->first] - Vector3f ( fx, fy , 0.8 - fz );

                sum+=pos;

                double theta = VecPosition::normalizeAngle(  CartecianToPolar ( flagGlobal[i->first] - pos ) .theta - i->second.theta  ) ;
                double phi   = VecPosition::normalizeAngle(  CartecianToPolar ( flagGlobal[i->first] - pos ) .phi - i->second.phi ) ;

                setMyAngle ( theta - getJointAngle ("he1") ) ;
                setMyPhi   ( phi   - getJointAngle ("he2") ) ;

                Matrix x ;

                Vector3f I , J , K ;

                x.RotationY (  Deg2Rad ( phi )  ) ;
                x.RotateZ ( Deg2Rad ( -theta ) ) ;

                I = x.Rotate ( Vector3f ( 1 , 0 , 0 ) ) ;
                J = x.Rotate ( Vector3f ( 0 , 1 , 0 ) ) ;
                K = x.Rotate ( Vector3f ( 0 , 0 , 1 ) ) ;

                R.Set ( I.x() , I.y() , I.z() , 0 ,
                        J.x() , J.y() , J.z() , 0 ,
                        K.x() , K.y() , K.z() , 0 ,
                        0     , 0     , 0     , 1 ) ;
            }
            if ( i->first[0]=='F' && j->first[0] == 'F' && i->first[1] == '1' && j->first[1] == '2'   )
            {
                co ++;
                Localed = true ;
                double fz=ZFromLeft();

                double h1 = 0;
                double h2 = 0;
                double a = i->second.dist;
                double b = j->second.dist;
                double d = fabs(flagGlobal[i->first].y() - flagGlobal[j->first].y()) ;


                double fy = (d*d+b*b-a*a+(h2-h1)*(h1+h2-2*fz))/(2*d);
                double fx = sqrt ( b*b-(h1-fz)*(h1-fz) - fy*fy ) ;

                if ( i->first[2] == 'L' && getTeamSide() == Left)
                {
                    fx *= -1 ;
                }

                if ( i->first[2] == 'R' && getTeamSide() == Right)
                {
                    fx *= -1 ;
                }
                if ( getTeamSide() == Right )
                {
                    fy *= -1 ;
                }
                if ( j->first[1] == '2' )
                {
                    fy *= -1 ;
                }


                pos = flagGlobal [j->first] - Vector3f ( fx, fy , - fz );

                sum+=pos;

                double theta = VecPosition::normalizeAngle(  CartecianToPolar ( flagGlobal[i->first] - pos ) .theta - i->second.theta  ) ;
                double phi   = VecPosition::normalizeAngle(  CartecianToPolar ( flagGlobal[i->first] - pos ) .phi - i->second.phi ) ;

                setMyAngle ( theta - getJointAngle ("he1") ) ;
                setMyPhi   ( phi   - getJointAngle ("he2") ) ;

                Matrix x ;

                Vector3f I , J , K ;

                x.RotationY (  Deg2Rad ( phi )  ) ;
                x.RotateZ ( Deg2Rad ( -theta ) ) ;

                I = x.Rotate ( Vector3f ( 1 , 0 , 0 ) ) ;
                J = x.Rotate ( Vector3f ( 0 , 1 , 0 ) ) ;
                K = x.Rotate ( Vector3f ( 0 , 0 , 1 ) ) ;

                R.Set ( I.x() , I.y() , I.z() , 0 ,
                        J.x() , J.y() , J.z() , 0 ,
                        K.x() , K.y() , K.z() , 0 ,
                        0     , 0     , 0     , 1 ) ;
            }

        }
    }
    if ( Localed )                                /// if At Least Seen 3 Flags Or More
    {
        pos = sum/co;
        int pppp=14;
        //        RVDraw::instance()->drawLine(pos,pos2,BLACK,pppp++);
        RVDraw::instance()->drawLine(Vector3f(0,0,0),pos,BLACK,pppp++);
        setMyPos( pos );        	          /// set My Pos

        if ( ballLastSeen == serverTime )          /// if See Ball In This Cycle Set Its Pos !
        {
            Vector3f temp = translate ( ball );
            setBallVel ( temp - getBallPos() ) ;  /// set Velocity of Ball
            setBallPos ( temp );                  /// set Ball Pos
            RVDraw::instance()->drawVector3f(temp,BLUE,pppp++);
            setBallAng(gRadToDeg(atan2(getBallVel().y(), getBallVel().x())));
        }

        ///~  Calculate Players Diffrent Body Pos

        for ( map<int,RelPlayerInfo>::iterator i = ourRel.begin() ; i != ourRel.end() ; i++ )
        {
            if ( i->first != getMyNum () )
            {
                our [i->first].head = translate ( PolarToCartecian(i->second.head) ) ;
            }
            our [i->first].lfoot = translate ( PolarToCartecian(i->second.lfoot) ) ;
            our [i->first].rfoot = translate ( PolarToCartecian(i->second.rfoot) ) ;
            our [i->first].rlowerarm = translate ( PolarToCartecian(i->second.rlowerarm) ) ;
            our [i->first].llowerarm = translate ( PolarToCartecian(i->second.llowerarm) ) ;
        }

        for ( map<int,RelPlayerInfo>::iterator i = theirRel.begin() ; i != theirRel.end() ; i++ )
        {
            their [i->first].head = translate ( PolarToCartecian(i->second.head) ) ;
            their [i->first].lfoot = translate ( PolarToCartecian(i->second.lfoot) ) ;
            their [i->first].rfoot = translate ( PolarToCartecian(i->second.rfoot) ) ;
            their [i->first].rlowerarm = translate ( PolarToCartecian(i->second.rlowerarm) ) ;
            their [i->first].llowerarm = translate ( PolarToCartecian(i->second.llowerarm) ) ;
        }

    }
}

//this function will set the speed which is an approximation of the exact speed
void WorldModel::setSpeed(Vector3f this_cycle_speed)
{
    speed=(this_cycle_speed*3+getSpeed())/4;
}


Vector3f WorldModel::getSpeed()
{
    return speed;
}

//void WorldModel::getPosbyTwoFlag()
//{

//    initDimentions();
//    initFlags();
//    Localed=false;
//    Vector3f pos ;
//    int co = 0 ;
//    Vector3f sum (0,0,0);

//    for (map<string, Polar>::iterator i = flagPolar.begin(); i != flagPolar.end(); i++)
//    {
//        if (flagLastSeen [ i->first ] != serverTime)
//        {
//            continue;
//        }
//        for (map<string, Polar>::iterator j = flagPolar.begin() ; j != flagPolar.end(); j++)
//        {
//            if (flagLastSeen [ j->first ] != serverTime)
//            {
//                continue;
//            }
//            if (  i->first ==  j->first )
//            {
//                continue ;
//            }
//            if ( i->first[2] != j->first[2] )
//            {
//                continue ;
//            }

//            if (   i->first[0]=='G' && j->first[0] == 'F' && i->first[1] == j->first[1]  )
//            {
//                co ++;
//                Localed = true ;
//                double fz=ZFromLeft();

//                double h1 = 0;
//                double h2 = 0.8;
//                double a = i->second.dist;
//                double b = j->second.dist;
//                double d = fabs(flagGlobal[i->first].y() - flagGlobal[j->first].y()) ;


//                double fy = (d*d+b*b-a*a+(h2-h1)*(h1+h2-2*fz))/(2*d);
//                double fx = sqrt ( b*b-(h1-fz)*(h1-fz) - fy*fy ) ;


//                if ( j->first[2] == 'L' && getTeamSide() == Left)
//                {
//                    fx *= -1 ;
//                }
//                if ( j->first[2] == 'R' && getTeamSide() == Right)
//                {
//                    fx *= -1 ;
//                }
//                if ( getTeamSide() == Right )
//                {
//                    fy *= -1 ;
//                }
//                if ( j->first[1] == '2' )
//                {
//                    fy *= -1 ;
//                }


//                pos = flagGlobal [j->first] - Vector3f ( fx, fy , -fz );

//                sum += pos;

//                double theta = VecPosition::normalizeAngle ( CartecianToPolar ( flagGlobal[j->first] - pos ) .theta - j->second.theta  ) ;
//                double phi   = VecPosition::normalizeAngle ( CartecianToPolar ( flagGlobal[j->first] - pos ) .phi - j->second.phi ) ;

//                setMyAngle ( theta - getJointAngle ("he1") ) ;
//                setMyPhi   ( phi   - getJointAngle ("he2") ) ;

//                Matrix x ;

//                Vector3f I , J , K ;

//                x.RotationY (  Deg2Rad ( phi )  ) ;
//                x.RotateZ ( Deg2Rad ( -theta ) ) ;

//                I = x.Rotate ( Vector3f ( 1 , 0 , 0 ) ) ;
//                J = x.Rotate ( Vector3f ( 0 , 1 , 0 ) ) ;
//                K = x.Rotate ( Vector3f ( 0 , 0 , 1 ) ) ;

//                R.Set ( I.x() , I.y() , I.z() , 0 ,
//                        J.x() , J.y() , J.z() , 0 ,
//                        K.x() , K.y() , K.z() , 0 ,
//                        0     , 0     , 0     , 1 ) ;
//            }
//            if (  i->first[0]=='G' && j->first[0] == 'G' && i->first[1] == '1' && j->first[1] == '2'   )
//            {
//                co ++;
//                Localed = true ;
//                double fz=ZFromLeft();

//                double h1 = 0.8;
//                double h2 = 0.8;
//                double a = i->second.dist;
//                double b = j->second.dist;
//                double d = fabs(flagGlobal[i->first].y() - flagGlobal[j->first].y()) ;


//                double fy = (d*d+b*b-a*a+(h2-h1)*(h1+h2-2*fz))/(2*d);
//                double fx = sqrt ( b*b-(h1-fz)*(h1-fz) - fy*fy ) ;

//                if ( i->first[2] == 'L' && getTeamSide() == Left)
//                {
//                    fx *= -1 ;
//                }

//                if ( i->first[2] == 'R' && getTeamSide() == Right)
//                {
//                    fx *= -1 ;
//                }
//                if ( getTeamSide() == Right )
//                {
//                    fy *= -1 ;
//                }
//                if ( j->first[1] == '2' )
//                {
//                    fy *= -1 ;
//                }


//                pos = flagGlobal [j->first] - Vector3f ( fx, fy , 0.8 - fz );

//                sum+=pos;

//                double theta = VecPosition::normalizeAngle(  CartecianToPolar ( flagGlobal[i->first] - pos ) .theta - i->second.theta  ) ;
//                double phi   = VecPosition::normalizeAngle(  CartecianToPolar ( flagGlobal[i->first] - pos ) .phi - i->second.phi ) ;

//                setMyAngle ( theta - getJointAngle ("he1") ) ;
//                setMyPhi   ( phi   - getJointAngle ("he2") ) ;

//                Matrix x ;

//                Vector3f I , J , K ;

//                x.RotationY (  Deg2Rad ( phi )  ) ;
//                x.RotateZ ( Deg2Rad ( -theta ) ) ;

//                I = x.Rotate ( Vector3f ( 1 , 0 , 0 ) ) ;
//                J = x.Rotate ( Vector3f ( 0 , 1 , 0 ) ) ;
//                K = x.Rotate ( Vector3f ( 0 , 0 , 1 ) ) ;

//                R.Set ( I.x() , I.y() , I.z() , 0 ,
//                        J.x() , J.y() , J.z() , 0 ,
//                        K.x() , K.y() , K.z() , 0 ,
//                        0     , 0     , 0     , 1 ) ;
//            }
//            if ( i->first[0]=='F' && j->first[0] == 'F' && i->first[1] == '1' && j->first[1] == '2'   )
//            {
//                co ++;
//                Localed = true ;
//                double fz=ZFromLeft();

//                double h1 = 0;
//                double h2 = 0;
//                double a = i->second.dist;
//                double b = j->second.dist;
//                double d = fabs(flagGlobal[i->first].y() - flagGlobal[j->first].y()) ;


//                double fy = (d*d+b*b-a*a+(h2-h1)*(h1+h2-2*fz))/(2*d);
//                double fx = sqrt ( b*b-(h1-fz)*(h1-fz) - fy*fy ) ;

//                if ( i->first[2] == 'L' && getTeamSide() == Left)
//                {
//                    fx *= -1 ;
//                }

//                if ( i->first[2] == 'R' && getTeamSide() == Right)
//                {
//                    fx *= -1 ;
//                }
//                if ( getTeamSide() == Right )
//                {
//                    fy *= -1 ;
//                }
//                if ( j->first[1] == '2' )
//                {
//                    fy *= -1 ;
//                }


//                pos = flagGlobal [j->first] - Vector3f ( fx, fy , - fz );

//                sum+=pos;

//                double theta = VecPosition::normalizeAngle(  CartecianToPolar ( flagGlobal[i->first] - pos ) .theta - i->second.theta  ) ;
//                double phi   = VecPosition::normalizeAngle(  CartecianToPolar ( flagGlobal[i->first] - pos ) .phi - i->second.phi ) ;

//                setMyAngle ( theta - getJointAngle ("he1") ) ;
//                setMyPhi   ( phi   - getJointAngle ("he2") ) ;

//                Matrix x ;

//                Vector3f I , J , K ;

//                x.RotationY (  Deg2Rad ( phi )  ) ;
//                x.RotateZ ( Deg2Rad ( -theta ) ) ;

//                I = x.Rotate ( Vector3f ( 1 , 0 , 0 ) ) ;
//                J = x.Rotate ( Vector3f ( 0 , 1 , 0 ) ) ;
//                K = x.Rotate ( Vector3f ( 0 , 0 , 1 ) ) ;

//                R.Set ( I.x() , I.y() , I.z() , 0 ,
//                        J.x() , J.y() , J.z() , 0 ,
//                        K.x() , K.y() , K.z() , 0 ,
//                        0     , 0     , 0     , 1 ) ;
//            }

//        }
//    }
//    if ( Localed )                                /// if At Least Seen 3 Flags Or More
//    {
//        pos = sum/co;
//        int pppp=14;
//        //        RVDraw::instance()->drawLine(pos,pos2,BLACK,pppp++);
//        RVDraw::instance()->drawLine(Vector3f(0,0,0),pos,BLACK,pppp++);
//        setMyPos( pos );        	          /// set My Pos

//        if ( ballLastSeen == serverTime )          /// if See Ball In This Cycle Set Its Pos !
//        {
//            Vector3f temp = translate ( ball );
//            setBallVel ( temp - getBallPos() ) ;  /// set Velocity of Ball
//            setBallPos ( temp );                  /// set Ball Pos
//            RVDraw::instance()->drawVector3f(temp,BLUE,pppp++);
//            setBallAng(gRadToDeg(atan2(getBallVel().y(), getBallVel().x())));
//        }

//        ///~  Calculate Players Diffrent Body Pos

//        for ( map<int,RelPlayerInfo>::iterator i = ourRel.begin() ; i != ourRel.end() ; i++ )
//        {
//            if ( i->first != getMyNum () )
//            {
//                our [i->first].head = translate ( PolarToCartecian(i->second.head) ) ;
//            }
//            our [i->first].lfoot = translate ( PolarToCartecian(i->second.lfoot) ) ;
//            our [i->first].rfoot = translate ( PolarToCartecian(i->second.rfoot) ) ;
//            our [i->first].rlowerarm = translate ( PolarToCartecian(i->second.rlowerarm) ) ;
//            our [i->first].llowerarm = translate ( PolarToCartecian(i->second.llowerarm) ) ;
//        }

//        for ( map<int,RelPlayerInfo>::iterator i = theirRel.begin() ; i != theirRel.end() ; i++ )
//        {
//            their [i->first].head = translate ( PolarToCartecian(i->second.head) ) ;
//            their [i->first].lfoot = translate ( PolarToCartecian(i->second.lfoot) ) ;
//            their [i->first].rfoot = translate ( PolarToCartecian(i->second.rfoot) ) ;
//            their [i->first].rlowerarm = translate ( PolarToCartecian(i->second.rlowerarm) ) ;
//            their [i->first].llowerarm = translate ( PolarToCartecian(i->second.llowerarm) ) ;
//        }

//    }
//}

// set All Flags Real Position
void WorldModel::initFlags()
{
    // if Team Side is Right Every Thing Will be Chappe
    int q = (getTeamSide() == Right) ? -1 : 1;

    //Corner flags real position
    flagGlobal["F1L"].x() = -q * length / 2;
    flagGlobal["F1L"].y() = q * width / 2;
    flagGlobal["F1L"].z() = 0.0;

    flagGlobal["F2L"].x() = -q * length / 2;
    flagGlobal["F2L"].y() = -q * width / 2;
    flagGlobal["F2L"].z() = 0.0;

    flagGlobal["F1R"].x() = q * length / 2;
    flagGlobal["F1R"].y() = q * width / 2;
    flagGlobal["F1R"].z() = 0.0;

    flagGlobal["F2R"].x() = q * length / 2;
    flagGlobal["F2R"].y() = -q * width / 2;
    flagGlobal["F2R"].z() = 0.0;

    const double H = 2.0;    // height
    const double D = 0.0;    // Depth
    //goal flags real position
    flagGlobal["G1L"].x() = -q * (length / 2.0 + D);
    flagGlobal["G1L"].y() = q * goalWidth / 2;
    flagGlobal["G1L"].z() = 0.4 * H;

    flagGlobal["G2L"].x() = -q * (length / 2.0 + D);
    flagGlobal["G2L"].y() = -q * goalWidth / 2;
    flagGlobal["G2L"].z() = 0.4 * H;

    flagGlobal["G1R"].x() = q * (length / 2.0 + D);
    flagGlobal["G1R"].y() = q * goalWidth / 2;
    flagGlobal["G1R"].z() = 0.4 * H;

    flagGlobal["G2R"].x() = q * (length / 2.0 + D);
    flagGlobal["G2R"].y() = -q * goalWidth / 2;
    flagGlobal["G2R"].z() = 0.4 * H;
}

void WorldModel::initDimentions()
{
    width = 20.0;
    length = 30.0;
    goalWidth = 2.1;
}

void WorldModel::setMyNum(int number)
{
    num = number;
}
void WorldModel::setACC( Vector3f pos )
{
    if ( gIsNan ( pos.x() ) )
    {
        pos.x() = 0;
    }
    if ( gIsNan ( pos.y() ) )
    {
        pos.y() = 0;
    }
    if ( gIsNan ( pos.z() ) )
    {
        pos.z() = 0;
    }
    ACC = 0.9*ACC+0.1*pos;
}

void WorldModel::setBallPolarPos(Polar pos)
{
    /// ball Prev Last Seen For Calculating Ball Vel in Restricted Vision Server
    ballPrevLastSeen = ballLastSeen;
    ballLastSeen = serverTime;
    ball = PolarToCartecian(pos);  /// convert Polar To Cartesian
    ballPolar = pos;
}

void WorldModel::setBallPos(Vector3f pos)
{
    ballPos = pos;
}

void WorldModel::setTime(double t)
{
    Time = t;
}

void WorldModel::setServerTime(double t)
{
    serverTime = t;
}

void WorldModel::setGyro(Vector3f g)
{
    gyro = g;
}

void WorldModel::setBallAng(double ang)
{
    ballAng=ang;
}
void WorldModel::setJointAngle(string name, double ang)
{
    lastJoint[str2str(name)] = joint[str2str(name)] ;
    joint[str2str(name)] = ang;
}

/// ~ if rsg/agent/nao/naoneckhead.rsg --> senseMyPos true --> then this will be true
void WorldModel::setSense(bool b)
{
    Sense = b ;
}

void WorldModel::setOurPlayerPos(int num, Polar head)
{
    ourRel[num].head = head;
    ourRel[num].lastSeen = getServerTime();
}

void WorldModel::setOppPlayerPos(int num, Polar head)
{
    theirRel[num].head = head;
    theirRel[num].lastSeen = getServerTime();
}

void WorldModel::setOurPlayerPartPos( int num , string part , Polar pos )
{
    if ( part == "lfoot" )
    {
        ourRel[num].lfoot = pos;
    }
    if ( part == "rfoot" )
    {
        ourRel[num].rfoot = pos;
    }
    if ( part == "llowerarm" )
    {
        ourRel[num].llowerarm = pos;
    }
    if ( part == "rlowerarm" )
    {
        ourRel[num].rlowerarm = pos;
    }
}

void WorldModel::setOppPlayerPartPos( int num , string part , Polar pos )
{
    if ( part == "lfoot" )
    {
        theirRel[num].lfoot = pos;
    }
    if ( part == "rfoot" )
    {
        theirRel[num].rfoot = pos;
    }
    if ( part == "llowerarm" )
    {
        theirRel[num].llowerarm = pos;
    }
    if ( part == "rlowerarm" )
    {
        theirRel[num].rlowerarm = pos;
    }
}

void WorldModel::setOurPos(int num, Vector3f pos)
{
    our[num].head = pos;
}

void WorldModel::setOppPos(int num, Vector3f pos)
{
    our[num].head = pos;
}

void WorldModel::setLastMsg(string hear,double time)
{
    timeLastHear = time;

    lastHear = hear;
}

void WorldModel::setOurName(string name)
{
    ourName = name;
}

void WorldModel::setOppName(string name)
{
    oppName = name;
}

void WorldModel::setTeamSide(string tSide)
{
    side = tSide;
}

void WorldModel::setPlayMode(PlayMode mode)
{
    playMode = mode;
}

void WorldModel::setFootPress(string side, FootRes fp)
{
    if ( side == "rf" )
    {
        rfTime = serverTime ;
    }
    else
    {
        lfTime = serverTime ;
    }
    footPress[side] = fp;
}

void WorldModel::setMyPos(Vector3f pos)
{
    our[getMyNum()].head = pos ;
}

void WorldModel::setMyAngle(double angle)
{
    myAngle = angle;
}

void WorldModel::setMyPhi(double angle)
{
    myPhi = angle;
}

void WorldModel::setBallVel(Vector3f vel)
{
    ballVel = vel;
}

int WorldModel::getNrOfUs()
{
    return ourRel.size();
}

int WorldModel::getNrOfThem()
{
    return theirRel.size();
}

Vector3f WorldModel::getBallVel()
{
    return ballVel;
}

Polar WorldModel::getBallPolarPos()
{
    return ballPolar;
}

double WorldModel::getMyAngle()
{
    return myAngle;
}

double WorldModel::getMyPhi()
{
    return myPhi;
}

double WorldModel::getTime()
{
    return Time;
}

double WorldModel::getServerTime()
{
    return serverTime;
}
/// Return The Gyroscope Status
Vector3f WorldModel::getGyro()
{
    return gyro;
}
/// return Current Cycle Joint Angle
double WorldModel::getJointAngle(string name)
{
    return joint[name];
}
/// return Last Cycle Joint Angle
double WorldModel::getLastJointAngle(string name)
{
    return lastJoint[name];
}

int WorldModel::getMyNum()
{
    return num;
}

bool WorldModel::getSense()
{
    return Sense;
}

string WorldModel::getOurName()
{
    return ourName;
}

string WorldModel::getOppName()
{
    return oppName;
}

SideT WorldModel::getTeamSide()
{
    if ( side == "left" )
    {
        return Left;
    }
    else
    {
        return Right;
    }
}

PlayMode WorldModel::getPlayMode()
{
    return playMode;
}
/// return a FootRes Which is the Pressure of each foot
FootRes WorldModel::getFootPress(string side)
{
    return footPress[side];
}

string WorldModel::getLastMsg()
{
    return lastHear;
}
////
Vector3f WorldModel::getMyPos()
{
    return our[getMyNum()].head;
}

Vector3f WorldModel::getMyPos(string part)
{
    if ( part == "head" )
    {
        return our[getMyNum()].head;
    }
    if ( part == "lfoot" )
    {
        return our[getMyNum()].lfoot;
    }
    if ( part == "rfoot" )
    {
        return our[getMyNum()].rfoot;
    }
    if ( part == "rlowerarm" )
    {
        return our[getMyNum()].rlowerarm;
    }
    if ( part == "llowerarm" )
    {
        return our[getMyNum()].llowerarm;
    }
    return Vector3f(0,0,0);
}

Vector3f WorldModel::getOurPos(int i)
{
    return our[i].head;
}

Vector3f WorldModel::getOurPos(int i , string part)
{
    if ( part == "head" )
    {
        return our[i].head;
    }
    if ( part == "lfoot" )
    {
        return our[i].lfoot;
    }
    if ( part == "rfoot" )
    {
        return our[i].rfoot;
    }
    if ( part == "rlowerarm" )
    {
        return our[i].rlowerarm;
    }
    if ( part == "llowerarm" )
    {
        return our[i].llowerarm;
    }
    return Vector3f(0,0,0);
}

Vector3f WorldModel::getOppPos(int i)
{
    return their[i].head;
}

Vector3f WorldModel::getOppPos(int i , string part)
{
    if ( part == "head" )
    {
        return their[i].head;
    }
    if ( part == "lfoot" )
    {
        return their[i].lfoot;
    }
    if ( part == "rfoot" )
    {
        return their[i].rfoot;
    }
    if ( part == "rlowerarm" )
    {
        return their[i].rlowerarm;
    }
    if ( part == "llowerarm" )
    {
        return their[i].llowerarm;
    }
    return Vector3f(0,0,0);
}
////
Vector3f WorldModel::getBallPos()
{
    return ballPos;
}
/// return agent Angle To Ball
double WorldModel::getMyAngleToBall()
{
    return getMyAngleTo(getBallPos());
}
/// return Agent Angle To Goal Center
double WorldModel::getMyAngleToGoal()
{
    return getMyAngleTo(Vector3f(length/2.0,0,0));
}
/// return Agent Angle To Pos
double WorldModel::getMyAngleTo(Vector3f p)
{
    Vector3f pos = p - getMyPos();
    VecPosition diff(pos.x(), pos.y());
    return VecPosition::normalizeAngle(diff.getDirection() - getMyAngle());
}

double WorldModel::getBallAng()
{
    return VecPosition::normalizeAngle(ballAng);
}

/// Convert a Str to Its Number (Used In ACT && fix && ...)
string WorldModel::num2Str(int i)
{
    return names[i];
}

/// Convert a Number to its Str
int WorldModel::str2num(string str)
{
    for( int i = 0 ; i < 22 ; i ++ )
    {
        if ( str == num2Str(i) )
        {
            return i ;
        }
    }
    return 0 ;
}

/// Convert Perceptor To Effector
string WorldModel::str2str(string s)
{
    string str = s;
    if (str[0] != 'h')
    {
        str[2] = 'e';
    }
    else
    {
        str[1] = 'e';
    }
    return str;
}

/// Convert a Rel Position To Global By Matrix R (Rotation Matrix)
Vector3f WorldModel::translate(Vector3f Pos)
{

    return (getMyPos() + R.Rotate(Pos));
}

Vector3f WorldModel::translate(Vector3f Pos,Matrix T)
{
    return (getMyPos() - T.Rotate(Pos));
}

Vector3f WorldModel::translate(Vector3f Pos,Vector3f parent)
{
    return ( parent - R.Rotate(Pos) );
}

///~Acceleration Sensor
Vector3f WorldModel::   getACC()
{
    return ACC;
}

/// Return BodyPart Pos Using Forward Kinematics
/// This Method Return whether the agent is fallen or not
bool WorldModel::isFeltDown()
{
    Vector3f acc = getACC();
    if ( acc.x() < -9 )
    {
        return true;
    }
    if ( acc.x() > 9 )
    {
        return true;
    }
    if ( acc.y() < -9 )
    {
        return true;
    }
    if ( acc.y() > 9 )
    {
        return true;
    }

    return false;
}
/// This Method Return The side which agent Felt if Not Felt Returns "None"
SideT WorldModel::feltType()
{
    /// calculating Pos Of Right , Front , back , Left By R Rotation Matrix
    Vector3f acc = getACC() ;
    if ( acc.x() < -9 )
    {
        return Right;
    }
    if ( acc.x() > 9 )
    {
        return Left;
    }
    if ( acc.y() < -9 )
    {
        return Front;
    }
    if ( acc.y() > 9 )
    {
        return Back;
    }
    return Unknown;
}

/// Return Number Of Closesr Opp To Ball
int WorldModel::getClosestsOppToBall()
{
    double min = 999999;
    int num=-1;
    for ( map<int,GloPlayerInfo>::iterator i = their.begin(); i != their.end() ; i++)
    {
        double dist = ( i->second.head - getBallPos() ).Length() ;
        if ( dist  < min  )
        {
            min = dist ;
            num = i->first;
        }
    }
    return num;
}

int WorldModel::getClosestOurToBall()
{
    double score = 0 ;
    double max = -99999 ;
    int num = 0 ;
    VecPosition ball(getBallPos().x(),getBallPos().y());
    for ( map<int,GloPlayerInfo>::iterator i = our.begin() ; i != our.end() ; i++ )
    {
        if(i->first == 10 || i->first == 11){
            continue;
        }
        VecPosition pl (i->second.head.x(),i->second.head.y());
        double dist = pl.getDistanceTo(ball);
        double headZ = i->second.head.z() ;
        score = -dist ;

        if ( headZ < 0.3 )
        {
            score -= 1.5 ;
            score -= fabs( pl.getY() - ball.getY() );
        }
        if ( pl.getX() > ( ball.getX()  ) )
        {
            score -= 1.5 ;
        }
        if ( score > max )
        {
            max = score ;
            num = i->first ;
        }
    }
    return num ;
}


double WorldModel::getOurMinDist()
{
    double min = 999999 ;

    for ( map<int,GloPlayerInfo>::iterator i = our.begin(); i != our.end() ; i++)
    {
        double dist = ( i->second.head - getBallPos() ).Length() ;
        if ( dist  < min  )
        {
            min = dist ;
        }
    }
    return min;
}

bool   WorldModel::isBallUs ()
{
    if ( getOurMinDist() < getOppMinDist () )
    {
        return false;
    }
    return true;
}

double WorldModel::getOppMinDist()
{
    double min = 999999;
    for ( map<int,GloPlayerInfo>::iterator i = their.begin(); i != their.end() ; i++)
    {
        double dist = ( i->second.head - getBallPos() ).Length() ;
        if ( dist  < min  )
        {
            min = dist ;
        }
    }
    return min;
}
/// Return Whether The Agent is Closest To Ball
bool WorldModel::amIClosestToBall()
{
    for ( map<int,GloPlayerInfo>::iterator i = our.begin(); i != our.end() ; i++)
    {
        if ( i->first !=1 || i->first!=getMyNum() )
            if ( ( getMyPos() - getBallPos() ).Length() > ( i->second.head - getBallPos() ).Length() )
            {
                return false;
            }
    }
    return true;
}

bool WorldModel::shouldDive(SideT &side)
{
    Vector3f ball  = getBallPos()  ;
    Vector3f vel =   getBallVel() ;
    if ( vel.Length() > 0.02 )
    {
        ball+= ( vel*50 );
        vel*=0.96;
    }
    if ( ball.y() > 0 )
    {
        side =  Left;
    }
    else
    {
        side = Right ;
    }
    if (ball.x() < -9 && fabs ( ball.y() ) < 1.05  )
    {
        return true;
    }
    return false;
}

bool WorldModel::isLocaled()
{
    return Localed;
}

string WorldModel::uniquee ( string x )
{
    string send = x;
    for (int i = 0; i < 22 ; i++)
    {
        int count = 0;
        string del ("("+num2Str(i)) ;
        int pos =  send.find ( del );
        while ( pos != string::npos )
        {
            count++;
            pos =  send.find (  del , pos + 1 );
        }
        for (int j = 0; j < count-1; j++)
        {
            int pos =  send.find ( del );
            int opos = send.find ( ")" , pos );
            int diff = ( opos - pos ) +1 ;
            if ( pos != string::npos && opos != string::npos )
            {
                send.replace ( pos , diff , "" );
            }
        }
    }
    return send;
}
int WorldModel::getNrOfFlag()
{
    int x=0;
    for (map<string, Vector3f>::iterator it = flag.begin(); it != flag.end(); it++)
        if (fabs( flagLastSeen [ it->first ] - serverTime ) <= 0.06)
        {
            x++;
        }
    return x;
}

bool WorldModel::seeBall ()
{
    if (  fabs( getServerTime() - getBallLastSeen() )  <= 0.06 )
    {
        return true;
    }
    return false;
}

bool WorldModel::seeEnoughFlag()
{
    if ( getNrOfFlag() > 0 )
        return true;
    return false;
}
double WorldModel::ZFromLeft()
{
    return myZ;
}

///////////


double WorldModel::ZFromRight()
{
    return myZ;
}

////////////

void WorldModel::setZ(double z)
{
    myZ = z ;
}

////////////
int WorldModel::isOppInCircle ( Circle c ,VecPosition &p1,VecPosition &p2)
{
    int t = 0 ;
    for ( map<int,GloPlayerInfo>::iterator i = their.begin(); i != their.end() ; i++)
    {
        Vector3f p = i->second.head  ;
        VecPosition pos ( p.x() , p.y () );
        if ( c.isInside ( pos ) )
        {
            t++ ;
            if(t==1)
                p1=pos;
            if(t==2)
                p2=pos;
        }
    }
    return t ;
}

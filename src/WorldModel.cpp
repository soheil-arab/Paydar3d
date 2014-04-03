
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
#include <Eigen/Geometry>
#include "rvdraw.h"


WorldModel::WorldModel()
    : joint(50)
    , lastJoint(50)
{

    ballLastSeen = -1;
    ballPrevLastSeen = -1;
    timeLastHear = -1;
    Localed = false;
    Sense = false;
    myZ = 0.51;
    myOrien = 0;

    bodyRotate.Identity();
    speed = Eigen::Vector3f(0, 0, 0);

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

    for (int i = 0; i < 22; i++) {
        joint[names[i]] = 0;
        lastJoint[names[i]] = 0;
    }
}

void WorldModel::clearBodyRotate()
{
    bodyRotate.Identity();
}

WorldModel::~WorldModel()
{
}

unordered_map<string, Eigen::Vector3f> WorldModel::getFlagGlobal()
{
    return flagGlobal;
}

void WorldModel::resetLastSeenLines()
{
    lines_we_see.clear();
}

void WorldModel::setGyroPos(Eigen::Vector3f pos)
{
    gyroPos = pos;
}

vector<line> WorldModel::getLastSeenLines()
{
    vector<line> lastSeenLines;
    for(int i=0;i<lines_we_see.size();i++)
    {
        if(lines_we_see[i].time_we_saw_it==serverTime)
            lastSeenLines.push_back(lines_we_see[i]);
    }

    return lastSeenLines;
}

///set seen lines,the lines are in head rotation coordination and should be convert to global if needed

void WorldModel::setSeenLines(line l)
{
    if (l.getLength() > 1)
        lines_we_see.push_back(l);
}

///

unordered_map<string, Eigen::Vector3f> WorldModel::getFlag()
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
    static Eigen::Vector3f lastPos=getMyPos();
    static Eigen::Vector3f lastGyroPos=gyroPos;

    for (unordered_map<string, Polar>::iterator i = flagPolar.begin(); i != flagPolar.end(); i++)
    {
        if (flagLastSeen [ i->first ] != serverTime)
        {
            continue;
        }
        if(fabs((i->second.dist)-(lastPos-flagGlobal[i->first]).norm())>1){
            lastPos=getMyPos();
            return true;
        }
        if(fabs((i->second.dist)-(lastGyroPos-flagGlobal[i->first]).norm())>1){
            lastGyroPos=gyroPos;
            return true;
        }

    }
    lastPos=getMyPos();
    lastGyroPos=gyroPos;
    return false;

}

///~ Our Team Localization

//this function will rotate an orirentation around an axis with specified angle, the angle should be in radian

Eigen::Vector3f WorldModel::general_rotation(Eigen::Vector3f initial, Eigen::Vector3f axis, double angle)
{
    //    cout<<"angle: "<<angle<<endl;
    //    cout<<"axis: "<<axis<<endl;
    Eigen::Vector3f normaled_axis = axis.normalized();
    double ux = normaled_axis.x();
    double uy = normaled_axis.y();
    double uz = normaled_axis.z();
    Eigen::Matrix3f Rotation;
    Rotation << cos(angle) + ux * ux * (1 - cos(angle)), ux * uy * (1 - cos(angle)) - uz * sin(angle), ux * uz * (1 - cos(angle)) + uy * sin(angle),
                 ux * uy * (1 - cos(angle)) + uz * sin(angle), cos(angle) + uy * uy * (1 - cos(angle)), uy * uz * (1 - cos(angle)) - ux * sin(angle),
                 ux * uz * (1 - cos(angle)) - uy * sin(angle), uy * uz * (1 - cos(angle)) + ux * sin(angle), cos(angle) + uz * uz * (1 - cos(angle));

    Eigen::Vector3f rotated = Rotation*initial;
//    Eigen::Vector3f rotated;
    return rotated;
}

void WorldModel::brinBeMA()
{
    //    isServerBeamed();
    //    kalman_filter();

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

    Eigen::Vector3f rightGyro(gyro.y(), -gyro.x(), gyro.z());
    Eigen::Vector3f newGyro = bodyRotate*rightGyro;



    double theta = Deg2Rad(newGyro.norm() * 0.02);

    if (newGyro.norm() > 0.2)
        newGyro = newGyro / newGyro.norm();

    Eigen::Vector3f x = bodyRotate.col(0);
    Eigen::Vector3f y = bodyRotate.col(1);
    Eigen::Vector3f z = bodyRotate.col(2);

    Eigen::Vector3f newx = x * cos(theta) + (newGyro.cross(x)) * sin(theta) + newGyro * (newGyro.dot(x)) * (1 - cos(theta));
    Eigen::Vector3f newy = y * cos(theta) + (newGyro.cross(y)) * sin(theta) + newGyro * (newGyro.dot(y)) * (1 - cos(theta));
    Eigen::Vector3f newz = newx.cross(newy);

    bodyRotate << newx.x(), newy.x(), newz.x(),
                   newx.y(), newy.y(), newz.y(),
                   newx.z(), newy.z(), newz.z();


    setMyAngle(atan2Deg(newx.y(), newx.x()));
    //    cout << atan2Deg(newx.y(),newx.x()) << endl;

    //        RVDraw::instance()->drawLine(sensedPos,sensedPos+newx,GREEN,24);
    //        RVDraw::instance()->drawLine(sensedPos,sensedPos+newy,GREEN,25);
    //        RVDraw::instance()->drawLine(sensedPos,sensedPos+newz,GREEN,26);

    Eigen::Vector3f x_after_he1_rotation = general_rotation(newx, newz, Deg2Rad(getJointAngle("he1")));
    Eigen::Vector3f y_after_he1_rotation = general_rotation(newy, newz, Deg2Rad(getJointAngle("he1")));
    Eigen::Vector3f z_after_he1_rotation = general_rotation(newz, newz, Deg2Rad(getJointAngle("he1")));

    Eigen::Vector3f x_after_he2_rotation = general_rotation(x_after_he1_rotation, y_after_he1_rotation, -Deg2Rad(getJointAngle("he2")));
    Eigen::Vector3f y_after_he2_rotation = general_rotation(y_after_he1_rotation, y_after_he1_rotation, -Deg2Rad(getJointAngle("he2")));
    Eigen::Vector3f z_after_he2_rotation = general_rotation(z_after_he1_rotation, y_after_he1_rotation, -Deg2Rad(getJointAngle("he2")));

    headRotate << x_after_he2_rotation.x(), y_after_he2_rotation.x(), z_after_he2_rotation.x(),
                   x_after_he2_rotation.y(), y_after_he2_rotation.y(), z_after_he2_rotation.y(),
                   x_after_he2_rotation.z(), y_after_he2_rotation.z(), z_after_he2_rotation.z();

    //    cout<<"he1: "<<(getJointAngle("he1"))<<endl;
    //    cout<<"we : "<<Rad2Deg(acos(newx.Dot(x_after_he1_rotation)))<<endl;

    //    cout<<"he2: "<<(getJointAngle("he2"))<<endl;
    //    cout<<"we : "<<Rad2Deg(acos(x_after_he2_rotation.Dot(x_after_he1_rotation)))<<endl;

    //           RVDraw::instance()->drawLine(sensedPos,sensedPos+x_after_he2_rotation*10,RED,21);
    //           RVDraw::instance()->drawLine(sensedPos,sensedPos+y_after_he2_rotation*10,BLACK,22);
    //           RVDraw::instance()->drawLine(sensedPos,sensedPos+z_after_he2_rotation*10,GREEN,23);

    int numberOfFlags = 0;

    Eigen::Vector3f myPos(0, 0, 0);

    for (unordered_map<string, Eigen::Vector3f>::iterator i = flag.begin(); i != flag.end(); i++) {
//        cout << serverTime << "  " << flagLastSeen[i->first] << endl;
        if (flagLastSeen[i->first] != serverTime) {
            continue;
        }

        Eigen::Vector3f poss = flagGlobal[i->first] - headRotate*i->second;
        //        RVDraw::instance()->drawLine(sensedPos,sensedPos+headRotate.Rotate(i->second),GREEN,numberOfFlags);

        myPos += poss;
        numberOfFlags++;
//        cout << "asdasd" << endl;

    }

    /**
      for testing purpose
     */
    //    static int posN = 100 ;
    //    posN++;

    if (numberOfFlags) {

        Localed = true;
        myPos /= numberOfFlags;
        //        if(myPos.z()<=0.04)
        //        {
        //            cout<<"kamtar shod "<<endl;
        //            myPos.z()=0.042;
        //        }
        //        cout<<"mypos: "<<myPos<<endl;
        //
        mylastPos = getMyPos();
        //        myPos=((mylastPos+getSpeed()*0.02)*12+myPos*8)/20;
        myPos = ((mylastPos) * 8 + myPos * 12) / 20;

        setSpeed((myPos - mylastPos) * 50);

        setMyPos(myPos);
        //        setMyAngle(Rad2Deg(gArcTan(newx.y()/newx.x())));
        //        RVDraw::instance()->drawEigen::Vector3f(myPos,GREEN,posN);
        if (ballLastSeen == serverTime) /// if See Ball In This Cycle Set Its Pos !
        {
            Eigen::Vector3f ballPos = myPos + headRotate*ball;
            //            RVDraw::instance()->drawEigen::Vector3f(ballPos,RED,++posN);
            setBallVel(ballPos - getBallPos());
            setBallPos(ballPos);
            if (ballPos.z() <= 0) {
                //                cout<<"goh shod "<<serverTime<<endl;
                ballPos << (Eigen::Vector3f(ballPos.x(), ballPos.y(), 0.042));
            }
            //            else
            //                cout<<"oooo "<<endl;
        }

        //        RVDraw::instance()->drawEigen::Vector3f(getMyPos(),GREEN,5);
        //        RVDraw::instance()->drawEigen::Vector3f(getBallPos(),GREEN,7);
        for (unordered_map<int, RelPlayerInfo>::iterator i = ourRel.begin(); i != ourRel.end(); i++) {
            if (i->first != getMyNum()) {
                our[i->first].head = myPos + headRotate*PolarToCartecian(i->second.head);
            }
            our[i->first].lfoot = myPos + headRotate*PolarToCartecian(i->second.lfoot);
            our[i->first].rfoot = myPos + headRotate*PolarToCartecian(i->second.rfoot);
            our[i->first].rlowerarm = myPos + headRotate*PolarToCartecian(i->second.rlowerarm);
            our[i->first].llowerarm = myPos + headRotate*PolarToCartecian(i->second.llowerarm);
        }

        for (unordered_map<int, RelPlayerInfo>::iterator i = theirRel.begin(); i != theirRel.end(); i++) {
            their[i->first].head = myPos + headRotate*PolarToCartecian(i->second.head);
            their[i->first].lfoot = myPos + headRotate*PolarToCartecian(i->second.lfoot);
            their[i->first].rfoot = myPos + headRotate*PolarToCartecian(i->second.rfoot);
            their[i->first].rlowerarm = myPos + headRotate*PolarToCartecian(i->second.rlowerarm);
            their[i->first].llowerarm = myPos + headRotate*PolarToCartecian(i->second.llowerarm);
        }
    }

//    if (getLastSeenLines().size() > 0 && Time > 20)
        //    if(lines_we_see.size()>0 && Time>20)
//    {
//        int size = getLastSeenLines().size();

//        for (int i = 0; i < size; i++) {
//            line l = getLastSeenLines()[size - i - 1];
//            if (l.time_we_saw_it == serverTime) {
                //                RVDraw::instance()->drawLine(sensedPos+headRotate.Rotate(PolarToCartecian(l.begin)),sensedPos+headRotate.Rotate(PolarToCartecian(l.end)),RED,2*size+i);
//            }
//        }
//    }
}

//kalman filter

void WorldModel::kalman_filter()
{
//    static Eigen::Vector3f pos(-0.19, 0, 0);
//    static Eigen::Vector3f speed(0, 0, 0);

//    if (getACC().norm() > 9.809) {
//        static Eigen::Matrix3f state(getMyPos().x(), getMyPos().y(), getMyPos().z(), 0,
//                            0, 0, 0, 0,
//                            0, 0, 0, 0,
//                            0, 0, 0, 0);

//        Eigen::Vector3f Real_Acc(0, 0, -9.81);
//        Eigen::Vector3f rightACC(getACC().y(), getACC().x(), -getACC().z());
//        Eigen::Vector3f shetab = getACC()*bodyRotate;
//        Eigen::Vector3f mofid = shetab - Real_Acc;
//        //        cout << getACC()<< " server" << endl;
//        //        cout<<"niro "<<getFootPress("lf").f<<"and "<<getFootPress("rf").f<<endl;
//        //                cout<<"vazn "<<shetab.norm()*4.6071<<endl;

//        //4.6071:mass

//        pos = pos + 0.0002 * mofid + 0.02 * speed;
//        speed = speed + 0.02 * mofid;

//        Eigen::Matrix3f F(1, 0.02, 0, 0,
//                 0, 1, 0, 0,
//                 0, 0, 0, 0,
//                 0, 0, 0, 0);

//        Eigen::Matrix3f priori_state_estimate(0, 0, 0, 0,
//                                     0, 0, 0, 0,
//                                     0, 0, 0, 0,
//                                     0, 0, 0, 0);

//        Eigen::Matrix3f G(0.0002, 0, 0, 0,
//                 0.02, 0, 0, 0,
//                 0, 0, 0, 0,
//                 0, 0, 0, 0);

//        Eigen::Matrix3f ACC((int)((mofid.z()) * 10000) / (float)10000, (int)(mofid.y() * 10000) / (float)10000, (int)(-mofid.x() * 10000) / (float)10000, 0,
//                   0, 0, 0, 0,
//                   0, 0, 0, 0,
//                   0, 0, 0, 0);

//        static Eigen::Matrix3f cov(0, 0, 0, 0,
//                          0, 0, 0, 0,
//                          0, 0, 0, 0,
//                          0, 0, 0, 0);

//        Eigen::Matrix3f predicted_cov(0, 0, 0, 0,
//                             0, 0, 0, 0,
//                             0, 0, 0, 0,
//                             0, 0, 0, 0);

//        Eigen::Matrix3f H(1, 0, 0, 0,
//                 0, 0, 0, 0,
//                 0, 0, 0, 0,
//                 0, 0, 0, 0);

//        //i'm not sure about this covariance!!
//        Eigen::Matrix3f cov_of_acc(0.0004, 0, 0, 0,
//                          0, 0.0004, 0, 0,
//                          0, 0, 0, 0.0004,
//                          0, 0, 0, 0);

//        Eigen::Matrix3f position_by_censors(getMyPos().x(), getMyPos().y(), getMyPos().z(), 0,
//                                   0, 0, 0, 0,
//                                   0, 0, 0, 0,
//                                   0, 0, 0, 0);

//        //i'm not sure about this covriance!
//        Eigen::Matrix3f cov_of_position_by_censors(0.01, 0, 0, 0,
//                                          0, 0.01, 0, 0,
//                                          0, 0, 0.01, 0,
//                                          0, 0, 0, 0);

//        Eigen::Matrix3f I;
//        I.Identity();

//        Eigen::Matrix3f domain_of_error(0, 0, 0, 0,
//                               0, 0, 0, 0,
//                               0, 0, 0, 0,
//                               0, 0, 0, 0);

//        domain_of_error = position_by_censors - H * priori_state_estimate;

//        Eigen::Matrix3f S(0, 0, 0, 0,
//                 0, 0, 0, 0,
//                 0, 0, 0, 0,
//                 0, 0, 0, 0);

//        Eigen::Matrix3f K(0, 0, 0, 0,
//                 0, 0, 0, 0,
//                 0, 0, 0, 0,
//                 0, 0, 0, 0);

//        Eigen::Matrix3f H_C = H;
//        H_C.InvertRotationEigen::Matrix3f();
//        Eigen::Matrix3f S_C = S;
//        S_C.InvertRotationEigen::Matrix3f();
//        Eigen::Matrix3f F_C = F;
//        F_C.InvertRotationEigen::Matrix3f();
//        Eigen::Matrix3f G_C = F;
//        G_C.InvertRotationEigen::Matrix3f();

//        priori_state_estimate = F * state + G * ACC;

//        predicted_cov = F * cov * (F_C) + G * G_C * cov_of_acc;

//        S = H * predicted_cov * H_C + cov_of_position_by_censors;

//        K = predicted_cov * H * S_C;

//        state = priori_state_estimate + K * H;

//        cov = (I - K * H) * predicted_cov;

        //        RVDraw::instance()->drawEigen::Vector3f(Eigen::Vector3f(state.Transform(Eigen::Vector3f(1,0,0)).x(),state.Transform(Eigen::Vector3f(0,1,0)).x(),state.Transform(Eigen::Vector3f(0,0,1)).x()),RED,10);

        //        RVDraw::instance()->drawEigen::Vector3f(pos,RED,10);
//    }
}

//
//void WorldModel::kiiri()
//{
//    static Eigen::Vector3f posss=myPos;
//    static Eigen::Vector3f speed(0,0,0);
//    posss=posss+0.0002*(Eigen::Vector3f(ACC.x()*bodyRotate.Transform(Eigen::Vector3f(1,0,0))))+
//            0.02*(Eigen::Vector3f(speed.x()*bodyRotate.Transform(Eigen::Vector3f(1,0,0))));
//    speed=0.02*(Eigen::Vector3f(ACC.x()*bodyRotate.Transform(Eigen::Vector3f(1,0,0))));
//     RVDraw::instance()->drawEigen::Vector3f(posss,RED,10);
//}

void WorldModel::Localize()
{
    //    getPosbyTwoFlag();
    brinBeMA();
}

void WorldModel::getPosbyTwoFlag()
{

//    initDimentions();
//    initFlags();
//    Localed = false;
//    Eigen::Vector3f pos;
//    int co = 0;
//    Eigen::Vector3f sum(0, 0, 0);

//    for (unordered_map<string, Polar>::iterator i = flagPolar.begin(); i != flagPolar.end(); i++) {
//        if (flagLastSeen[i->first] != serverTime) {
//            continue;
//        }
//        for (unordered_map<string, Polar>::iterator j = flagPolar.begin(); j != flagPolar.end(); j++) {
//            if (flagLastSeen[j->first] != serverTime) {
//                continue;
//            }
//            if (i->first == j->first) {
//                continue;
//            }
//            if (i->first[2] != j->first[2]) {
//                continue;
//            }

//            if (i->first[0] == 'G' && j->first[0] == 'F' && i->first[1] == j->first[1]) {
//                co++;
//                Localed = true;
//                double fz = ZFromLeft();

//                double h1 = 0;
//                double h2 = 0.8;
//                double a = i->second.dist;
//                double b = j->second.dist;
//                double d = fabs(flagGlobal[i->first].y() - flagGlobal[j->first].y());

//                double fy = (d * d + b * b - a * a + (h2 - h1) * (h1 + h2 - 2 * fz)) / (2 * d);
//                double fx = sqrt(b * b - (h1 - fz) * (h1 - fz) - fy * fy);

//                if (j->first[2] == 'L' && getTeamSide() == Left) {
//                    fx *= -1;
//                }
//                if (j->first[2] == 'R' && getTeamSide() == Right) {
//                    fx *= -1;
//                }
//                if (getTeamSide() == Right) {
//                    fy *= -1;
//                }
//                if (j->first[1] == '2') {
//                    fy *= -1;
//                }

//                pos = flagGlobal[j->first] - Eigen::Vector3f(fx, fy, -fz);

//                sum += pos;

//                double theta = VecPosition::normalizeAngle(CartecianToPolar(flagGlobal[j->first] - pos).theta - j->second.theta);
//                double phi = VecPosition::normalizeAngle(CartecianToPolar(flagGlobal[j->first] - pos).phi - j->second.phi);

//                setMyAngle(theta - getJointAngle("he1"));
//                setMyPhi(phi - getJointAngle("he2"));

//                Eigen::Matrix3f x;

//                Eigen::Vector3f I, J, K;

//                x.RotationY(Deg2Rad(phi));
//                x.RotateZ(Deg2Rad(-theta));

//                I = x.Rotate(Eigen::Vector3f(1, 0, 0));
//                J = x.Rotate(Eigen::Vector3f(0, 1, 0));
//                K = x.Rotate(Eigen::Vector3f(0, 0, 1));

//                R.Set(I.x(), I.y(), I.z(), 0,
//                      J.x(), J.y(), J.z(), 0,
//                      K.x(), K.y(), K.z(), 0,
//                      0, 0, 0, 1);
//            }
//            if (i->first[0] == 'G' && j->first[0] == 'G' && i->first[1] == '1' && j->first[1] == '2') {
//                co++;
//                Localed = true;
//                double fz = ZFromLeft();

//                double h1 = 0.8;
//                double h2 = 0.8;
//                double a = i->second.dist;
//                double b = j->second.dist;
//                double d = fabs(flagGlobal[i->first].y() - flagGlobal[j->first].y());

//                double fy = (d * d + b * b - a * a + (h2 - h1) * (h1 + h2 - 2 * fz)) / (2 * d);
//                double fx = sqrt(b * b - (h1 - fz) * (h1 - fz) - fy * fy);

//                if (i->first[2] == 'L' && getTeamSide() == Left) {
//                    fx *= -1;
//                }

//                if (i->first[2] == 'R' && getTeamSide() == Right) {
//                    fx *= -1;
//                }
//                if (getTeamSide() == Right) {
//                    fy *= -1;
//                }
//                if (j->first[1] == '2') {
//                    fy *= -1;
//                }

//                pos = flagGlobal[j->first] - Eigen::Vector3f(fx, fy, 0.8 - fz);

//                sum += pos;

//                double theta = VecPosition::normalizeAngle(CartecianToPolar(flagGlobal[i->first] - pos).theta - i->second.theta);
//                double phi = VecPosition::normalizeAngle(CartecianToPolar(flagGlobal[i->first] - pos).phi - i->second.phi);

//                setMyAngle(theta - getJointAngle("he1"));
//                setMyPhi(phi - getJointAngle("he2"));

//                Eigen::Matrix3f x;

//                Eigen::Vector3f I, J, K;

//                x.RotationY(Deg2Rad(phi));
//                x.RotateZ(Deg2Rad(-theta));

//                I = x.Rotate(Eigen::Vector3f(1, 0, 0));
//                J = x.Rotate(Eigen::Vector3f(0, 1, 0));
//                K = x.Rotate(Eigen::Vector3f(0, 0, 1));

//                R.Set(I.x(), I.y(), I.z(), 0,
//                      J.x(), J.y(), J.z(), 0,
//                      K.x(), K.y(), K.z(), 0,
//                      0, 0, 0, 1);
//            }
//            if (i->first[0] == 'F' && j->first[0] == 'F' && i->first[1] == '1' && j->first[1] == '2') {
//                co++;
//                Localed = true;
//                double fz = ZFromLeft();

//                double h1 = 0;
//                double h2 = 0;
//                double a = i->second.dist;
//                double b = j->second.dist;
//                double d = fabs(flagGlobal[i->first].y() - flagGlobal[j->first].y());

//                double fy = (d * d + b * b - a * a + (h2 - h1) * (h1 + h2 - 2 * fz)) / (2 * d);
//                double fx = sqrt(b * b - (h1 - fz) * (h1 - fz) - fy * fy);

//                if (i->first[2] == 'L' && getTeamSide() == Left) {
//                    fx *= -1;
//                }

//                if (i->first[2] == 'R' && getTeamSide() == Right) {
//                    fx *= -1;
//                }
//                if (getTeamSide() == Right) {
//                    fy *= -1;
//                }
//                if (j->first[1] == '2') {
//                    fy *= -1;
//                }

//                pos = flagGlobal[j->first] - Eigen::Vector3f(fx, fy, -fz);

//                sum += pos;

//                double theta = VecPosition::normalizeAngle(CartecianToPolar(flagGlobal[i->first] - pos).theta - i->second.theta);
//                double phi = VecPosition::normalizeAngle(CartecianToPolar(flagGlobal[i->first] - pos).phi - i->second.phi);

//                setMyAngle(theta - getJointAngle("he1"));
//                setMyPhi(phi - getJointAngle("he2"));

//                Eigen::Matrix3f x;

//                Eigen::Vector3f I, J, K;

//                x.RotationY(Deg2Rad(phi));
//                x.RotateZ(Deg2Rad(-theta));

//                I = x.Rotate(Eigen::Vector3f(1, 0, 0));
//                J = x.Rotate(Eigen::Vector3f(0, 1, 0));
//                K = x.Rotate(Eigen::Vector3f(0, 0, 1));

//                R.Set(I.x(), I.y(), I.z(), 0,
//                      J.x(), J.y(), J.z(), 0,
//                      K.x(), K.y(), K.z(), 0,
//                      0, 0, 0, 1);
//            }
//        }
//    }
//    if (Localed) /// if At Least Seen 3 Flags Or More
//    {
//        pos = sum / co;
//        int pppp = 14;
//        //        RVDraw::instance()->drawLine(pos,pos2,BLACK,pppp++);
//        //        RVDraw::instance()->drawLine(Eigen::Vector3f(0,0,0),pos,BLACK,pppp++);
//        setMyPos(pos); /// set My Pos

//        if (ballLastSeen == serverTime) /// if See Ball In This Cycle Set Its Pos !
//        {
//            Eigen::Vector3f temp = translate(ball);
//            setBallVel(temp - getBallPos()); /// set Velocity of Ball
//            setBallPos(temp); /// set Ball Pos
//            //            RVDraw::instance()->drawEigen::Vector3f(temp,BLUE,pppp++);
//            setBallAng(gRadToDeg(atan2(getBallVel().y(), getBallVel().x())));
//        }

//        ///~  Calculate Players Diffrent Body Pos

//        for (unordered_map<int, RelPlayerInfo>::iterator i = ourRel.begin(); i != ourRel.end(); i++) {
//            if (i->first != getMyNum()) {
//                our[i->first].head = translate(PolarToCartecian(i->second.head));
//            }
//            our[i->first].lfoot = translate(PolarToCartecian(i->second.lfoot));
//            our[i->first].rfoot = translate(PolarToCartecian(i->second.rfoot));
//            our[i->first].rlowerarm = translate(PolarToCartecian(i->second.rlowerarm));
//            our[i->first].llowerarm = translate(PolarToCartecian(i->second.llowerarm));
//        }

//        for (unordered_map<int, RelPlayerInfo>::iterator i = theirRel.begin(); i != theirRel.end(); i++) {
//            their[i->first].head = translate(PolarToCartecian(i->second.head));
//            their[i->first].lfoot = translate(PolarToCartecian(i->second.lfoot));
//            their[i->first].rfoot = translate(PolarToCartecian(i->second.rfoot));
//            their[i->first].rlowerarm = translate(PolarToCartecian(i->second.rlowerarm));
//            their[i->first].llowerarm = translate(PolarToCartecian(i->second.llowerarm));
//        }
//    }

//    static int ppp = 0;
//    RVDraw::instance()->drawEigen::Vector3f(pos, RED, ppp++);
}

//this function will set the speed which is an approximation of the exact speed

void WorldModel::setSpeed(Eigen::Vector3f this_cycle_speed)
{
    speed = (this_cycle_speed * 3 + getSpeed()) / 4;
}

Eigen::Vector3f WorldModel::getSpeed()
{
    return speed;
}

//void WorldModel::getPosbyTwoFlag()
//{

//    initDimentions();
//    initFlags();
//    Localed=false;
//    Eigen::Vector3f pos ;
//    int co = 0 ;
//    Eigen::Vector3f sum (0,0,0);

//    for (unordered_map<string, Polar>::iterator i = flagPolar.begin(); i != flagPolar.end(); i++)
//    {
//        if (flagLastSeen [ i->first ] != serverTime)
//        {
//            continue;
//        }
//        for (unordered_map<string, Polar>::iterator j = flagPolar.begin() ; j != flagPolar.end(); j++)
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

//                pos = flagGlobal [j->first] - Eigen::Vector3f ( fx, fy , -fz );

//                sum += pos;

//                double theta = VecPosition::normalizeAngle ( CartecianToPolar ( flagGlobal[j->first] - pos ) .theta - j->second.theta  ) ;
//                double phi   = VecPosition::normalizeAngle ( CartecianToPolar ( flagGlobal[j->first] - pos ) .phi - j->second.phi ) ;

//                setMyAngle ( theta - getJointAngle ("he1") ) ;
//                setMyPhi   ( phi   - getJointAngle ("he2") ) ;

//                Eigen::Matrix3f x ;

//                Eigen::Vector3f I , J , K ;

//                x.RotationY (  Deg2Rad ( phi )  ) ;
//                x.RotateZ ( Deg2Rad ( -theta ) ) ;

//                I = x.Rotate ( Eigen::Vector3f ( 1 , 0 , 0 ) ) ;
//                J = x.Rotate ( Eigen::Vector3f ( 0 , 1 , 0 ) ) ;
//                K = x.Rotate ( Eigen::Vector3f ( 0 , 0 , 1 ) ) ;

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

//                pos = flagGlobal [j->first] - Eigen::Vector3f ( fx, fy , 0.8 - fz );

//                sum+=pos;

//                double theta = VecPosition::normalizeAngle(  CartecianToPolar ( flagGlobal[i->first] - pos ) .theta - i->second.theta  ) ;
//                double phi   = VecPosition::normalizeAngle(  CartecianToPolar ( flagGlobal[i->first] - pos ) .phi - i->second.phi ) ;

//                setMyAngle ( theta - getJointAngle ("he1") ) ;
//                setMyPhi   ( phi   - getJointAngle ("he2") ) ;

//                Eigen::Matrix3f x ;

//                Eigen::Vector3f I , J , K ;

//                x.RotationY (  Deg2Rad ( phi )  ) ;
//                x.RotateZ ( Deg2Rad ( -theta ) ) ;

//                I = x.Rotate ( Eigen::Vector3f ( 1 , 0 , 0 ) ) ;
//                J = x.Rotate ( Eigen::Vector3f ( 0 , 1 , 0 ) ) ;
//                K = x.Rotate ( Eigen::Vector3f ( 0 , 0 , 1 ) ) ;

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

//                pos = flagGlobal [j->first] - Eigen::Vector3f ( fx, fy , - fz );

//                sum+=pos;

//                double theta = VecPosition::normalizeAngle(  CartecianToPolar ( flagGlobal[i->first] - pos ) .theta - i->second.theta  ) ;
//                double phi   = VecPosition::normalizeAngle(  CartecianToPolar ( flagGlobal[i->first] - pos ) .phi - i->second.phi ) ;

//                setMyAngle ( theta - getJointAngle ("he1") ) ;
//                setMyPhi   ( phi   - getJointAngle ("he2") ) ;

//                Eigen::Matrix3f x ;

//                Eigen::Vector3f I , J , K ;

//                x.RotationY (  Deg2Rad ( phi )  ) ;
//                x.RotateZ ( Deg2Rad ( -theta ) ) ;

//                I = x.Rotate ( Eigen::Vector3f ( 1 , 0 , 0 ) ) ;
//                J = x.Rotate ( Eigen::Vector3f ( 0 , 1 , 0 ) ) ;
//                K = x.Rotate ( Eigen::Vector3f ( 0 , 0 , 1 ) ) ;

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
//        RVDraw::instance()->drawLine(Eigen::Vector3f(0,0,0),pos,BLACK,pppp++);
//        setMyPos( pos );        	          /// set My Pos

//        if ( ballLastSeen == serverTime )          /// if See Ball In This Cycle Set Its Pos !
//        {
//            Eigen::Vector3f temp = translate ( ball );
//            setBallVel ( temp - getBallPos() ) ;  /// set Velocity of Ball
//            setBallPos ( temp );                  /// set Ball Pos
//            RVDraw::instance()->drawEigen::Vector3f(temp,BLUE,pppp++);
//            setBallAng(gRadToDeg(atan2(getBallVel().y(), getBallVel().x())));
//        }

//        ///~  Calculate Players Diffrent Body Pos

//        for ( unordered_map<int,RelPlayerInfo>::iterator i = ourRel.begin() ; i != ourRel.end() ; i++ )
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

//        for ( unordered_map<int,RelPlayerInfo>::iterator i = theirRel.begin() ; i != theirRel.end() ; i++ )
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

    const double H = 2.0; // height
    const double D = 0.0; // Depth
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

void WorldModel::setACC(Eigen::Vector3f pos)
{
    if (gIsNan(pos.x())) {
        pos.x() = 0;
    }
    if (gIsNan(pos.y())) {
        pos.y() = 0;
    }
    if (gIsNan(pos.z())) {
        pos.z() = 0;
    }
    ACC = 0.9 * ACC + 0.1 * pos;
}

void WorldModel::setBallPolarPos(Polar pos)
{
    /// ball Prev Last Seen For Calculating Ball Vel in Restricted Vision Server
    ballPrevLastSeen = ballLastSeen;
    ballLastSeen = serverTime;
    ball = PolarToCartecian(pos); /// convert Polar To Cartesian
    ballPolar = pos;
}

void WorldModel::setBallPos(Eigen::Vector3f pos)
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

void WorldModel::setGyro(Eigen::Vector3f g)
{
    gyro = g;
}

void WorldModel::setBallAng(double ang)
{
    ballAng = ang;
}

void WorldModel::setJointAngle(string name, double ang)
{
    string effector = str2str(name);
    lastJoint[effector] = joint[effector];
    joint[effector] = ang;
}

/// ~ if rsg/agent/nao/naoneckhead.rsg --> senseMyPos true --> then this will be true

void WorldModel::setSense(bool b)
{
    Sense = b;
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

void WorldModel::setOurPlayerPartPos(int num, string part, Polar pos)
{
    if (part == "lfoot") {
        ourRel[num].lfoot = pos;
    }
    if (part == "rfoot") {
        ourRel[num].rfoot = pos;
    }
    if (part == "llowerarm") {
        ourRel[num].llowerarm = pos;
    }
    if (part == "rlowerarm") {
        ourRel[num].rlowerarm = pos;
    }
}

void WorldModel::setOppPlayerPartPos(int num, string part, Polar pos)
{
    if (part == "lfoot") {
        theirRel[num].lfoot = pos;
    }
    if (part == "rfoot") {
        theirRel[num].rfoot = pos;
    }
    if (part == "llowerarm") {
        theirRel[num].llowerarm = pos;
    }
    if (part == "rlowerarm") {
        theirRel[num].rlowerarm = pos;
    }
}

void WorldModel::setOurPos(int num, Eigen::Vector3f pos)
{
    our[num].head = pos;
}

void WorldModel::setOppPos(int num, Eigen::Vector3f pos)
{
    our[num].head = pos;
}

void WorldModel::setLastMsg(string hear, double time)
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

void WorldModel::setTeamSide(SideT tSide)
{
    side = tSide;
}

void WorldModel::setPlayMode(PlayMode mode)
{
    playMode = mode;
}

void WorldModel::setFootPress(string side, FootRes fp)
{
    if (side == "lf")
        lf = fp;
    else
        rf = fp;
}

void WorldModel::setMyPos(Eigen::Vector3f pos)
{
    our[getMyNum()].head = pos;
}

void WorldModel::setMyAngle(double angle)
{
    myAngle = angle;
}

void WorldModel::setMyPhi(double angle)
{
    myPhi = angle;
}

void WorldModel::setBallVel(Eigen::Vector3f vel)
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

Eigen::Vector3f WorldModel::getBallVel()
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

Eigen::Vector3f WorldModel::getGyro()
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
    return side;
}

PlayMode WorldModel::getPlayMode()
{
    return playMode;
}
/// return a FootRes Which is the Pressure of each foot

FootRes WorldModel::getFootPress(string side)
{
    if (side == "lf")
        return lf;
    else // if ( side == rf )
        return rf;
}

string WorldModel::getLastMsg()
{
    return lastHear;
}
////

Eigen::Vector3f WorldModel::getMyPos()
{
    return our[getMyNum()].head;
}

Eigen::Vector3f WorldModel::getMyPos(string part)
{
    if (part == "head") {
        return our[getMyNum()].head;
    }
    if (part == "lfoot") {
        return our[getMyNum()].lfoot;
    }
    if (part == "rfoot") {
        return our[getMyNum()].rfoot;
    }
    if (part == "rlowerarm") {
        return our[getMyNum()].rlowerarm;
    }
    if (part == "llowerarm") {
        return our[getMyNum()].llowerarm;
    }
    return Eigen::Vector3f(0, 0, 0);
}

Eigen::Vector3f WorldModel::getOurPos(int i)
{
    return our[i].head;
}

Eigen::Vector3f WorldModel::getOurPos(int i, string part)
{
    if (part == "head") {
        return our[i].head;
    }
    if (part == "lfoot") {
        return our[i].lfoot;
    }
    if (part == "rfoot") {
        return our[i].rfoot;
    }
    if (part == "rlowerarm") {
        return our[i].rlowerarm;
    }
    if (part == "llowerarm") {
        return our[i].llowerarm;
    }
    return Eigen::Vector3f(0, 0, 0);
}

Eigen::Vector3f WorldModel::getOppPos(int i)
{
    return their[i].head;
}

Eigen::Vector3f WorldModel::getOppPos(int i, string part)
{
    if (part == "head") {
        return their[i].head;
    }
    if (part == "lfoot") {
        return their[i].lfoot;
    }
    if (part == "rfoot") {
        return their[i].rfoot;
    }
    if (part == "rlowerarm") {
        return their[i].rlowerarm;
    }
    if (part == "llowerarm") {
        return their[i].llowerarm;
    }
    return Eigen::Vector3f(0, 0, 0);
}
////

Eigen::Vector3f WorldModel::getBallPos()
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
    return getMyAngleTo(Eigen::Vector3f(length / 2.0, 0, 0));
}
/// return Agent Angle To Pos

double WorldModel::getMyAngleTo(Eigen::Vector3f p)
{
    Eigen::Vector3f pos = p - getMyPos();
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
    for (int i = 0; i < 22; i++) {
        if (str == num2Str(i)) {
            return i;
        }
    }
    return 0;
}

/// Convert Perceptor To Effector

string WorldModel::str2str(string s)
{
    if (s[0] != 'h') // head joint
    {
        s[2] = 'e';
    } else // others
    {
        s[1] = 'e';
    }
    return s;
}

/// Convert a Rel Position To Global By Eigen::Matrix3f R (Rotation Eigen::Matrix3f)

Eigen::Vector3f WorldModel::translate(Eigen::Vector3f Pos)
{
    return (getMyPos() + R*Pos);
}

Eigen::Vector3f WorldModel::translate(Eigen::Vector3f Pos, Eigen::Matrix3f T)
{
    return (getMyPos() - T*Pos);
}

Eigen::Vector3f WorldModel::translate(Eigen::Vector3f Pos, Eigen::Vector3f parent)
{
    return (parent - R*Pos);
}

///~Acceleration Sensor

Eigen::Vector3f WorldModel::getACC()
{
    return ACC;
}

/// Return BodyPart Pos Using Forward Kinematics
/// This Method Return whether the agent is fallen or not

bool WorldModel::isFeltDown()
{
    Eigen::Vector3f acc = getACC();
    if (acc.x() < -9) {
        return true;
    }
    if (acc.x() > 9) {
        return true;
    }
    if (acc.y() < -9) {
        return true;
    }
    if (acc.y() > 9) {
        return true;
    }

    return false;
}
/// This Method Return The side which agent Felt if Not Felt Returns "None"

SideT WorldModel::feltType()
{
    /// calculating Pos Of Right , Front , back , Left By R Rotation Eigen::Matrix3f
    Eigen::Vector3f acc = getACC();
    if (acc.x() < -9) {
        return Right;
    }
    if (acc.x() > 9) {
        return Left;
    }
    if (acc.y() < -9) {
        return Front;
    }
    if (acc.y() > 9) {
        return Back;
    }
    return Unknown;
}

/// Return Number Of Closesr Opp To Ball

int WorldModel::getClosestsOppToBall()
{
    double min = 999999;
    int num = -1;
    for (unordered_map<int, GloPlayerInfo>::iterator i = their.begin(); i != their.end(); i++) {
        double dist = (i->second.head - getBallPos()).norm();
        if (dist < min) {
            min = dist;
            num = i->first;
        }
    }
    return num;
}

int WorldModel::getClosestOurToBall()
{
    double score = 0;
    double max = -99999;
    int num = 0;
    VecPosition ball(getBallPos().x(), getBallPos().y());
    for (unordered_map<int, GloPlayerInfo>::iterator i = our.begin(); i != our.end(); i++) {
//        if (i->first == 10 || i->first == 11) {
//            continue;
//        }
        VecPosition pl(i->second.head.x(), i->second.head.y());
        double dist = pl.getDistanceTo(ball);
        double headZ = i->second.head.z();
        score = -dist;

        if (headZ < 0.3) {
            score -= 1.5;
            score -= fabs(pl.getY() - ball.getY());
        }
        if (pl.getX() > (ball.getX())) {
            score -= 1.5;
        }
        if (score > max) {
            max = score;
            num = i->first;
        }
    }
    return num;
}

double WorldModel::getOurMinDist()
{
    double min = 999999;

    for (unordered_map<int, GloPlayerInfo>::iterator i = our.begin(); i != our.end(); i++) {
        double dist = (i->second.head - getBallPos()).norm();
        if (dist < min) {
            min = dist;
        }
    }
    return min;
}

bool WorldModel::isBallUs()
{
    if (getOurMinDist() < getOppMinDist()) {
        return false;
    }
    return true;
}

double WorldModel::getOppMinDist()
{
    double min = 999999;
    for (unordered_map<int, GloPlayerInfo>::iterator i = their.begin(); i != their.end(); i++) {
        double dist = (i->second.head - getBallPos()).norm();
        if (dist < min) {
            min = dist;
        }
    }
    return min;
}
/// Return Whether The Agent is Closest To Ball

bool WorldModel::amIClosestToBall()
{
    for (unordered_map<int, GloPlayerInfo>::iterator i = our.begin(); i != our.end(); i++) {
        if (i->first != 1 || i->first != getMyNum())
            if ((getMyPos() - getBallPos()).norm() > (i->second.head - getBallPos()).norm()) {
                return false;
            }
    }
    return true;
}

bool WorldModel::shouldDive(SideT& side)
{
    Eigen::Vector3f ball = getBallPos();
    Eigen::Vector3f vel = getBallVel();
    if (vel.norm() > 0.02)  {
        ball += (vel * 25);
        vel *= 0.96;
    }
    if (ball.y() > 0) {
        side = Left;
    } else {
        side = Right;
    }
    if (ball.x() < -14 && fabs(ball.y()) < 1.1) {
        return true;
    }
    return false;
}

bool WorldModel::isLocaled()
{
    return Localed;
}

string WorldModel::uniquee(string x)
{
    string send = "";
    int opos = 0;
    int pos = 0;
    for (int i = 0; i < 22; i++) {
        string del("(" + num2Str(i));
        pos = x.rfind(del);

        if (pos != string::npos) {
            opos = x.find(")", pos + 1);
            send += x.substr(pos, opos - pos + 1);
        }
    }
    pos = x.rfind("(beam");
    if (pos != string::npos) {
        opos = x.find(")", pos);
        send += x.substr(pos, opos - pos + 1);
    }

    pos = x.rfind("(say");
    if (pos != string::npos) {
        opos = x.find(")", pos);
        send += x.substr(pos, opos - pos + 1);
    }

    return send;
}

int WorldModel::getNrOfFlag()
{
    int x = 0;
    for (unordered_map<string, Eigen::Vector3f>::iterator it = flag.begin(); it != flag.end(); it++)
        if (fabs(flagLastSeen[it->first] - serverTime) <= 0.06) {
            x++;
        }
    return x;
}

bool WorldModel::seeBall()
{
    if (fabs(getServerTime() - getBallLastSeen()) <= 0.06) {
        return true;
    }
    return false;
}

bool WorldModel::seeEnoughFlag()
{
    if (getNrOfFlag() > 0)
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
    myZ = z;
}

////////////

int WorldModel::isOppInCircle(Circle c, VecPosition& p1, VecPosition& p2)
{
    int t = 0;
    for (unordered_map<int, GloPlayerInfo>::iterator i = their.begin(); i != their.end(); i++) {
        Eigen::Vector3f p = i->second.head;
        VecPosition pos(p.x(), p.y());
        if (c.isInside(pos)) {
            t++;
            if (t == 1)
                p1 = pos;
            if (t == 2)
                p2 = pos;
        }
    }
    return t;
}

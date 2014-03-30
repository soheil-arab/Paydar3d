#include "Skill.h"

string Skill::GeneralWalk(int& t, double angToTurn, double angToGo, double A)
{

    unsigned int nj2 = lLeg.getNrOfSegments();

    KDL::JntArray jointpositionsl = KDL::JntArray(nj2);
    KDL::JntArray jointpositionsr = KDL::JntArray(nj2);

    jointpositionsl(0) = Deg2Rad(0);
    jointpositionsl(1) = Deg2Rad(0);
    jointpositionsl(2) = Deg2Rad(20);
    jointpositionsl(3) = Deg2Rad(-60);
    jointpositionsl(4) = Deg2Rad(40);
    jointpositionsl(5) = Deg2Rad(0);

    jointpositionsr(0) = Deg2Rad(0);
    jointpositionsr(1) = Deg2Rad(0);
    jointpositionsr(2) = Deg2Rad(20);
    jointpositionsr(3) = Deg2Rad(-60);
    jointpositionsr(4) = Deg2Rad(40);
    jointpositionsr(5) = Deg2Rad(0);

    static bool d2 = false;
    static double standingTime = 0;

    static bool onePeriodFinished = true;

    //  cout <<"Speed: " << speed << "     Acc: " << acc << endl;

    if (t == 0) {
        speed = 0;
    }

    if (d2 == true && t == 0 && WM->getJointAngle("lae3") > -5) { //Bug khoresh malase!
        d2 = false;
        standingTime = 0;
    }

    if (!d2) {
        return Standing(d2, standingTime);
    }

    t++;

    int AllTime = 5;
    int Period = 2 * AllTime;

    KDL::Frame rHipStanding;
    KDL::Frame lAnkleStanding;

    static KDL::JntArray JLLeg(nj2);
    static KDL::JntArray JLLegInit(nj2);

    static KDL::JntArray JRLeg(nj2);
    static KDL::JntArray JRLegInit(nj2);

    static double angleToGo = Deg2Rad(angToGo);
    static double angleToTurn = Deg2Rad(angToTurn);
    static bool walkMethodChange = false;
    static int phase = 0;
    static bool halfStepHappend = false;

//    cout << Rad2Deg(angleToTurn) << endl;
    if (t % AllTime == AllTime - 1) {
        double alpha = Rad2Deg(angleToTurn);

        if (t % Period == Period - 1) {
            //                cout << "Kire meymoon sefr shod" << endl;
            if (angToTurn < 0 == phase) {
//                cout << "need nim wlak" << endl;
                angleToTurn = 0;
                halfStepHappend = true;
            } else {
                onePeriodFinished = true;
            }
        } else if (fabs(angToTurn * alpha) < 0.5) {
            if (t % Period == AllTime - 1) {
                //                cout << "One Period Finished !!! And Roll back time ." << endl;
                onePeriodFinished = true;
                if (halfStepHappend) {
                    t += AllTime;
                    phase = (phase + 1) % 2;
                    halfStepHappend = false;
                }
            }
        } else {
            onePeriodFinished = false;
        }

    } else {
        onePeriodFinished = false;
    }

    //    cout <<  "And The angle of the turn :" << angleToTurn << " will embrace the world's in gray "<< endl;
    if (fabs(Rad2Deg(angleToGo) - angToGo) > 45 && !walkMethodChange) {
        walkMethodChange = true;
    }

    if (walkMethodChange) {
        if (onePeriodFinished && speed < 0.7) {

            angleToGo = Deg2Rad(angToGo);
            angleToTurn = Deg2Rad(angToTurn);
            walkMethodChange = false;
        } else {
            A = -0.01;
        }
    } else {
        if (onePeriodFinished) {
            //            cout << "Kire meymoon setttttttttt shod : " << angToTurn     << endl;
            angleToGo = Deg2Rad(angToGo);
            angleToTurn = Deg2Rad(angToTurn);
        }
    }

    newSpeedController(A);
    double cof = sqrt(1 - 0.36 * sin(angleToGo) * sin(angleToGo));
    double bb = speed * cof;

    //    cout<< bb << endl;

    double T = Period * 0.02;
    double w = (2 * M_PI) / T;
    double a = 0.05 * bb, b = -0.015 * bb;
    double a1 = a, b1 = b;
    double x1, x2, y1, y2, z1, z2;

    if (t == 1) {
        if (angleToTurn > -0.05)
            phase = 1;
        else
            phase = 0;
    }
    x1 = -a * cos(w * t * 0.02 + (phase + 1) * M_PI) * sin((angleToGo)),
    y1 = a * cos(w * t * 0.02 + (phase + 1) * M_PI) * cos((angleToGo)),
    z1 = b * sin(w * t * 0.02 + (phase + 1) * M_PI);
    x2 = -a1 * cos(w * t * 0.02 + (phase) * M_PI) * sin((angleToGo)),
    y2 = a1 * cos(w * t * 0.02 + (phase) * M_PI) * cos((angleToGo)),
    z2 = b1 * sin(w * t * 0.02 + (phase) * M_PI);

    //    if (t % AllTime == 0)
    //    cout << "S Moos: " << x1 << "  " << y1 << "  " << z1 << endl;

    KDL::Rotation lLegRot = KDL::Rotation::RotZ(0);
    KDL::Rotation rHipRot = KDL::Rotation::RotZ(0);

    if (angleToTurn > 0) {
        if (t % (2 * AllTime) < AllTime) {
            lLegRot = KDL::Rotation::RotZ((t % (2 * AllTime)) * fabs(angleToTurn) / (double)AllTime);
        } else {
            lLegRot = KDL::Rotation::RotZ(
                ((2 * AllTime - (t % (2 * AllTime))) * fabs(angleToTurn)) / (double)AllTime);
        }

    } else {
        if (t % (2 * AllTime) < AllTime) {
            rHipRot = KDL::Rotation::RotZ(-(t % (2 * AllTime)) * fabs(angleToTurn) / (double)AllTime);
        } else {
            rHipRot = KDL::Rotation::RotZ(
                -((2 * AllTime - (t % (2 * AllTime))) * fabs(angleToTurn)) / (double)AllTime);
        }
    }

    //    cout << lLegRot.GetRot() << endl;
    //    cout << rHipRot.GetRot() << endl;

    lLegFKSolver->JntToCart(jointpositionsl, lAnkleStanding);

    KDL::Frame lfrm(lLegRot, KDL::Vector(lAnkleStanding.p.x() + x1,
                                         lAnkleStanding.p.y() + y1,
                                         lAnkleStanding.p.z() + z1));

    rLegFKSolver->JntToCart(jointpositionsr, rHipStanding);
    KDL::Frame rfrm(rHipRot,
                    KDL::Vector(rHipStanding.p.x() + x2,
                                rHipStanding.p.y() + y2,
                                rHipStanding.p.z() + z2));

    int solvedl = lLegIksolverPosJ->CartToJnt(JLLegInit, lfrm, JLLeg);
    JLLegInit = JLLeg;

    int solvedr = rLegIksolverPosJ->CartToJnt(JRLegInit, rfrm, JRLeg);
    JRLegInit = JRLeg;

    //    cout << "--------------" << endl;
    if (solvedl != 0 || solvedr != 0)
        return "";
    return moveJoints(JLLeg, JRLeg, 0.2);
}

//////////////////////////////

string Skill::TurnLib(int& t, SideT side)
{
    t++;

    unsigned int nj2 = lLeg.getNrOfSegments();

    KDL::JntArray jointpositionsl = KDL::JntArray(nj2);
    KDL::JntArray jointpositionsr = KDL::JntArray(nj2);

    jointpositionsl(0) = Deg2Rad(0);
    jointpositionsl(1) = Deg2Rad(-0.5);
    jointpositionsl(2) = Deg2Rad(30);
    jointpositionsl(3) = Deg2Rad(-70);
    jointpositionsl(4) = Deg2Rad(50);
    jointpositionsl(5) = Deg2Rad(0.5);

    jointpositionsr(0) = Deg2Rad(0);
    jointpositionsr(1) = Deg2Rad(-0.5);
    jointpositionsr(2) = Deg2Rad(30);
    jointpositionsr(3) = Deg2Rad(-70);
    jointpositionsr(4) = Deg2Rad(50);
    jointpositionsr(5) = Deg2Rad(0.5);

    static bool d2 = false;
    static double standingTime = 0;

    static double bb = 0.1;

    //    newSpeedController(bb, t, TU);

    if (t == 1 && WM->getJointAngle("lle4") > -5) {
        d2 = false;
        standingTime = 0;
    }

    if (!d2) {
        return Standing(d2, standingTime);
    }

    KDL::Frame rHipStanding;
    KDL::Frame lAnkleStanding;

    static KDL::JntArray JLLeg(nj2);
    static KDL::JntArray JLLegInit(nj2);

    static KDL::JntArray JRLeg(nj2);
    static KDL::JntArray JRLegInit(nj2);

    double theta = Deg2Rad(30.0);

    int Period = 10;
    static double T = Period * 0.02;
    static double w = (2 * M_PI) / T;
    double b = -0.015 * bb;
    double z2 = b * sin(w * t * 0.02 + M_PI);
    double z1 = b * sin(w * t * 0.02);

    KDL::Rotation lLegRot = KDL::Rotation::RotZ(0);
    KDL::Rotation rHipRot = KDL::Rotation::RotZ(0);

    int AllTime = Period / 2;

    if (t % (2 * AllTime) < AllTime) {
        lLegRot = KDL::Rotation::RotZ((t % (2 * AllTime)) * theta / (double)AllTime);
    } else {
        lLegRot = KDL::Rotation::RotZ(
            ((2 * AllTime - (t % (2 * AllTime))) * theta) / (double)AllTime);
    }

    lLegFKSolver->JntToCart(jointpositionsl, lAnkleStanding);
    rLegFKSolver->JntToCart(jointpositionsr, rHipStanding);

    KDL::Frame lfrm(lLegRot,
                    KDL::Vector(lAnkleStanding.p.x(), lAnkleStanding.p.y(),
                                lAnkleStanding.p.z() + z1));
    KDL::Frame rfrm(rHipRot, KDL::Vector(rHipStanding.p.x(), rHipStanding.p.y(),
                                         rHipStanding.p.z() + z2));

    int solvedl = lLegIksolverPosJ->CartToJnt(JLLegInit, lfrm, JLLeg);
    int solvedr = rLegIksolverPosJ->CartToJnt(JRLegInit, rfrm, JRLeg);

    JLLegInit = JLLeg;
    JRLegInit = JRLeg;

    JLLeg(2) += Deg2Rad(10);
    JRLeg(2) += Deg2Rad(10);

    if (solvedl != 0 || solvedr != 0)
        return "";
    if (side == Right)
        return moveJoints(JLLeg, JRLeg, 0.2);
    else if (side == Left)
        return moveJoints(JRLeg, JLLeg, 0.2);
}

//////////////////////////////

string Skill::moveJoints(KDL::JntArray left, KDL::JntArray right, double p)
{
    KDL::JntArray curLeft(6), curRight(6);
    KDL::JntArray diffLeft(6), diffRight(6);

    string s1, s2;
    stringstream ss;

    string sc;

    for (int i = 0; i < 6; ++i) {
        string rightStr = WM->num2Str(2 + 2 * i + 1);
        string leftStr = WM->num2Str(2 + 2 * i);
        curRight(i) = WM->getJointAngle(rightStr);
        curLeft(i) = WM->getJointAngle(leftStr);

        diffRight(i) = (right(i)) - Deg2Rad(curRight(i));

        diffRight(i) = Rad2Deg(diffRight(i));
        diffRight(i) = fabs(diffRight(i)) < 0.1 ? 0 : diffRight(i) * p;
        diffLeft(i) = (left(i)) - Deg2Rad(curLeft(i));
        diffLeft(i) = Rad2Deg(diffLeft(i));
        diffLeft(i) = fabs(diffLeft(i)) < 0.1 ? 0 : diffLeft(i) * p;

        ss.str("");
        ss << diffRight(i);
        s1 = ss.str();
        ss.str("");
        ss << diffLeft(i);
        s2 = ss.str();
        sc += "(" + rightStr + " " + s1 + ")";
        sc += "(" + leftStr + " " + s2 + ")";
    }

    //  double A1 = 15, A2 = 30, fi = 0, B1 = -90 + A1, B2 = A2;
    //  static double TT, ww, ta = 0;
    //  static int Tint = 30;
    //  TT = Tint * 0.02;
    //  ww = (2 * M_PI) / TT;
    //  sc = sc + moveJoint("lae1",
    //                      B1 + A1 * sin(ww * ta + fi) - WM->getJointAngle("lae1")) +
    //       moveJoint("rae1",
    //                 B1 + -1 * A1 * sin(ww * ta + fi) - WM->getJointAngle("rae1")) +
    //       moveJoint("lae4", -1 * B2 + -1 * A2 * sin(ww * ta + fi) -
    //                             WM->getJointAngle("lae4")) +
    //       moveJoint("rae4",
    //                 B2 + -1 * A2 * sin(ww * ta + fi) - WM->getJointAngle("rae4"));
    //  ta += .02;

    return sc;
}

//////////////////////////////

void Skill::newSpeedController(double A)
{
    //    double maxAccDifferent = 0.00005;
    //    if (fabs(acc - A) > maxAccDifferent) {
    //        if (acc - A > 0) {
    //            acc += -maxAccDifferent;
    //        } else {
    //            acc += maxAccDifferent;
    //        }
    //    } else {
    acc = A;
    //    }
    speed += acc;
    speed = max(speed, 0.2);
    speed = min(speed, 1.1);
    //  VecPosition ball(WM->getBallPos().x(), WM->getBallPos().y());
    //  VecPosition me(WM->getMyPos().x(), WM->getMyPos().y());

    //  if (comm == W || comm == WA) {
    //    bb = 0.1 + (t - 1) * 0.015;
    //    bb = min(bb, 1.2);
    //    if (me.getDistanceTo(bralWall) < 1) {
    //      bb = min(bb, 0.8);
    //    }
    //  } else if (comm == TU) {
    //    bb = 0.4 + (t - 1) * 0.02;
    //    bb = min(bb, 1);
    //  } else if (comm == ST) {
    //    bb = 0.2 + (t - 1) * 0.04;
    //    bb = min(bb, 1);
    //  } else {
    //    bb = 0.01 + (t - 1) * 0.02;
    //    bb = min(bb, 1);
    //  }
}

//void Skill::newSpeedController(double& bb, int t, Command comm)
//{
//    VecPosition ball(WM->getBallPos().x(), WM->getBallPos().y());
//    VecPosition me(WM->getMyPos().x(), WM->getMyPos().y());

//    if (comm == W || comm == WA) {
//        bb = 0.1 + (t - 1) * 0.010;
//        bb = min(bb, 1);
//        if (me.getDistanceTo(ball) < 1) {
//            bb = min(bb, 0.8);
//        }
//    } else if (comm == TU) {
//        bb = 0.4 + (t - 1) * 0.02;
//        bb = min(bb, 1);
//    } else if (comm == ST) {
//        bb = 0.1 + (t - 1) * 0.01;
//        bb = min(bb, 0.5);
//    } else {
//        bb = 0.01 + (t - 1) * 0.02;
//        bb = min(bb, 1);
//    }
//}

///////////////////////////////////////

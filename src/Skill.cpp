#include "Skill.h"
#include "Skill.h"

Skill::Skill(WorldModel* wm)
{
    WM = wm;
    f.open("ACT/standupB.txt");
    if (!f) {
        cerr << "Could'nt Open File standUpB" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++) {
            f >> standupB[i][j];
        }
    f.close();
    fill1(standupF, "standupF");
    fill1(standupB, "standupB");

    f.open("ACT/standingK.txt");
    if (!f) {
        cerr << "Could'nt Open File divrR" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++) {
            f >> standingK[i][j];
        }
    f.close();

    f.open("ACT/hfr.txt");
    if (!f) {
        cerr << "Could'nt Open File hfr" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++) {
            f >> DPR[i][j];
        }
    f.close();

    f.open("ACT/hfl.txt");
    if (!f) {
        cerr << "Could'nt Open File hfl" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++) {
            f >> DPL[i][j];
        }
    f.close();

    f.open("ACT/zero.txt");
    if (!f) {
        cerr << "Could'nt Open File" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++) {
            f >> zero[i][j];
        }
    f.close();

    f.open("ACT/zero-eh.txt");
    if (!f) {
        cerr << "Could'nt Open File" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++) {
            f >> zeroe[i][j];
        }
    f.close();

    f.open("ACT/standing.txt");
    if (!f) {
        cerr << "Could'nt Open File" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++) {
            f >> standing[i][j];
        }
    f.close();

    f.open("ACT/standing2.txt");
    if (!f) {
        cerr << "Could'nt Open File" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++) {
            f >> standing[i][j];
        }
    f.close();

    f.open("ACT/standingBack.txt");
    if (!f) {
        cerr << "Could'nt Open File" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++) {
            f >> standingB[i][j];
        }
    f.close();

    f.open("ACT/diveL.txt");
    if (!f) {
        cerr << "Could'nt Open File" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++) {
            f >> diveL[i][j];
        }
    f.close();

    f.open("ACT/diveR.txt");
    if (!f) {
        cerr << "Could'nt Open File" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++) {
            f >> diveR[i][j];
        }
    f.close();

    rHipYawPitch = KDL::Joint(KDL::Vector(0, 0, 0), KDL::Vector(-0.707, 0.0, 0.707), KDL::Joint::RotAxis);
    rHipRoll = KDL::Joint(KDL::Vector(0, 0, 0), KDL::Vector(0, 1, 0), KDL::Joint::RotAxis);
    rHipPitch = KDL::Joint(KDL::Vector(0, -0.01, 0.04) + KDL::Vector(0, 0.01, -0.04), KDL::Vector(1, 0, 0), KDL::Joint::RotAxis);
    rKneePitch = KDL::Joint(KDL::Vector(0, -0.01, 0.045) + KDL::Vector(0, 0.005, -0.125), KDL::Vector(1, 0, 0), KDL::Joint::RotAxis);
    rAnkleRoll = KDL::Joint(KDL::Vector(0, -0.03, 0.035) + KDL::Vector(0, 0.03, -0.035), KDL::Vector(0, 1, 0), KDL::Joint::RotAxis);
    rAnklePitch = KDL::Joint(KDL::Vector(0.0, 0.0, 0.0) + KDL::Vector(0, -0.01, -0.055), KDL::Vector(1, 0, 0), KDL::Joint::RotAxis);

    rLeg.addSegment(KDL::Segment("rHipYawPitch", rHipYawPitch, KDL::Frame(KDL::Vector(0.0, 0.0, 0.0))));
    rLeg.addSegment(KDL::Segment("rHipRoll", rHipRoll, KDL::Frame(KDL::Vector(0, 0, 0))));
    rLeg.addSegment(KDL::Segment("rHipPitch", rHipPitch, KDL::Frame(KDL::Vector(0, 0.01, -0.04))));
    rLeg.addSegment(KDL::Segment("rKneePitch", rKneePitch, KDL::Frame(KDL::Vector(0, 0.005, -0.125))));
    rLeg.addSegment(KDL::Segment("rAnklePitch", rAnklePitch, KDL::Frame(KDL::Vector(0, -0.01, -0.055))));
    rLeg.addSegment(KDL::Segment("rAnkleRoll", rAnkleRoll, KDL::Frame(KDL::Vector(0, 0.0, 0.0))));

    lHipYawPitch = KDL::Joint(KDL::Vector(0, 0, 0), KDL::Vector(-0.707, 0.0, -0.707), KDL::Joint::RotAxis);
    lHipRoll = KDL::Joint(KDL::Vector(0, 0, 0), KDL::Vector(0, 1, 0), KDL::Joint::RotAxis);
    lHipPitch = KDL::Joint(KDL::Vector(0, -0.01, 0.04) + KDL::Vector(0, 0.01, -0.04), KDL::Vector(1, 0, 0), KDL::Joint::RotAxis);
    lKneePitch = KDL::Joint(KDL::Vector(0, -0.01, 0.045) + KDL::Vector(0, 0.005, -0.125), KDL::Vector(1, 0, 0), KDL::Joint::RotAxis);
    lAnkleRoll = KDL::Joint(KDL::Vector(0, -0.03, 0.035) + KDL::Vector(0, 0.03, -0.035), KDL::Vector(0, 1, 0), KDL::Joint::RotAxis);
    lAnklePitch = KDL::Joint(KDL::Vector(0.0, 0.0, 0.0) + KDL::Vector(0, -0.01, -0.055), KDL::Vector(1, 0, 0), KDL::Joint::RotAxis);

    lLeg.addSegment(KDL::Segment("lHipYawPitch", lHipYawPitch, KDL::Frame(KDL::Vector(0.0, 0.0, 0.0))));
    lLeg.addSegment(KDL::Segment("lHipRoll", lHipRoll, KDL::Frame(KDL::Vector(0, 0, 0))));
    lLeg.addSegment(KDL::Segment("lHipPitch", lHipPitch, KDL::Frame(KDL::Vector(0, 0.01, -0.04))));
    lLeg.addSegment(KDL::Segment("lKneePitch", lKneePitch, KDL::Frame(KDL::Vector(0, 0.005, -0.125))));
    lLeg.addSegment(KDL::Segment("lAnklePitch", lAnklePitch, KDL::Frame(KDL::Vector(0, -0.01, -0.055))));
    lLeg.addSegment(KDL::Segment("lAnkleRoll", lAnkleRoll, KDL::Frame(KDL::Vector(0, 0.0, 0.0))));

    unsigned int nj2 = lLeg.getNrOfSegments();

    KDL::JntArray lLegJMin = KDL::JntArray(nj2);
    lLegJMin(0) = Deg2Rad(-90);
    lLegJMin(1) = Deg2Rad(-25);
    lLegJMin(2) = Deg2Rad(-25);
    lLegJMin(3) = Deg2Rad(-130);
    lLegJMin(4) = Deg2Rad(-45);
    lLegJMin(5) = Deg2Rad(-45);

    KDL::JntArray lLegJMax = KDL::JntArray(nj2);
    lLegJMax(0) = Deg2Rad(1);
    lLegJMax(1) = Deg2Rad(45);
    lLegJMax(2) = Deg2Rad(100);
    lLegJMax(3) = Deg2Rad(1);
    lLegJMax(4) = Deg2Rad(75);
    lLegJMax(5) = Deg2Rad(25);

    KDL::JntArray rLegJMin = KDL::JntArray(nj2);

    rLegJMin(0) = Deg2Rad(-90);
    rLegJMin(1) = Deg2Rad(-45);
    rLegJMin(2) = Deg2Rad(-25);
    rLegJMin(3) = Deg2Rad(-130);
    rLegJMin(4) = Deg2Rad(-45);
    rLegJMin(5) = Deg2Rad(-25);

    KDL::JntArray rLegJMax = KDL::JntArray(nj2);

    rLegJMax(0) = Deg2Rad(1);
    rLegJMax(1) = Deg2Rad(25);
    rLegJMax(2) = Deg2Rad(100);
    rLegJMax(3) = Deg2Rad(1);
    rLegJMax(4) = Deg2Rad(75);
    rLegJMax(5) = Deg2Rad(45);

    lLegFKSolver = new KDL::ChainFkSolverPos_recursive(lLeg);
    lLegIkSolverVel = new KDL::ChainIkSolverVel_wdls(lLeg, 0.01, 25); //Inverse velocity solver, from Cartesian to joint space
    lLegIksolverPosJ = new KDL::ChainIkSolverPos_NR_JL(lLeg, lLegJMin, lLegJMax, *lLegFKSolver, *lLegIkSolverVel, 25, 0.01); //Maximum 100 iterations, stop at accuracy 1e-6

    rLegFKSolver = new KDL::ChainFkSolverPos_recursive(rLeg);
    rLegIkSolverVel = new KDL::ChainIkSolverVel_wdls(rLeg, 0.01, 25); //Inverse velocity solver, from Cartesian to joint space
    rLegIksolverPosJ = new KDL::ChainIkSolverPos_NR_JL(rLeg, rLegJMin, rLegJMax, *rLegFKSolver, *rLegIkSolverVel, 25, 0.01);
}

////////////////////////////////////////////////////////////////////

string Skill::moveJoint(string j, double val)
{
    stringstream message;
    message << "(" << j << " " << val * 0.872664626 << ")";
    return message.str();
}
///////////////////////////////////////////////////////////////////

string Skill::moveJointTo(string j, double val, double speed)
{
    speed = fabs(speed);
    if (fabs(WM->getJointAngle(j) - val) < 0.1) {
        return "";
    }
    if (val - WM->getJointAngle(j) <= 0) {
        speed *= -1;
    }
    if (fabs(val - WM->getJointAngle(j)) < fabs(speed)) {
        speed = val - WM->getJointAngle(j);
    }
    return moveJoint(j, speed);
}
/////////////////////////////////////////////////////////////////////

void Skill::fill1(double action[24][24], string fileName)
{

    string path = "./ACT/" + fileName + ".txt";
    ifstream fin;
    fin.open(path.c_str());
    fin >> action[0][0] >> action[1][0];
    double temp;
    for (int i = 0; i < action[0][0]; i++) {
        string joint;
        if (i > 1) {
            fin >> action[i][0];
        } else {
            fin >> temp;
        }
        for (int j = 1; j < action[1][0]; j++) {
            if (i == 0 && j == 1) {
                fin >> temp;
                continue;
            }
            fin >> action[i][j];
        }
    }

    fin.close();
}
/////////////////////////////////////////////////////////////////////

string Skill::act(double a[][24], double& time)
{
    int m = a[0][0];
    int n = a[1][0];

    string ss = "";
    for (int i = 1; i < n; i++) {
        if (a[0][i] - 0.01 <= time && time < a[1][i] - 0.01) {
            for (int j = 2; j < m; j++) {
                double speed = (a[j][i] - WM->getJointAngle(WM->num2Str(a[j][0] - 1))) / (a[1][i] - time) / 50.0;
                ss += moveJoint(WM->num2Str(a[j][0] - 1), speed);
            }
            break;
        } else {
            continue;
        }
    }
    time += 0.02;
    if (time > a[1][n - 1]) {
        ss = "";
        for (int i = 0; i < 22; i++) {
            ss += moveJoint(WM->num2Str(i), 0);
        }

        return ss;
    }
    return ss;
}
/////////////////////////////////////////////////////////////////////

string Skill::StandUpFront(bool& done)
{
    stringstream ss;
    done = false;
    bool did = false;
    static double tt = 0;
    ss << sefr(did, tt, false);
    if (did == false) {
        return ss.str();
    }
    static int jjj = 0;
    jjj++;
    static int co = -1;
    static double tmp = 0;
    co++;
    static bool islied = false;
    if (co == 0) {
        islied = false;
    }

    if (WM->getJointAngle("lae1") > -89 and islied == false) {
        stringstream ss;
        ss << fix() << moveJointTo("rae1", -90, -4) << moveJointTo("lae1", -90, -4);
        return ss.str();
    }

    islied = true;
    bool dd;
    if (tmp > 2.6) {
        jjj = 0;
        did = false;
        tt = 0;
        tmp = 0;
        co = -1;
        done = true;
        return fix();
    }
    return act(standupF, tmp);
}

/////////////////////////////////////////////////////////////////////

string Skill::act(double a[][24], int m, int n, double& time, bool& t)
{
    if (t) {
        return "";
    }
    string ss = "";
    for (int i = 1; i < n; i++) {
        if (a[0][i] < time && time <= a[1][i]) {
            for (int j = 2; j < m; j++) {
                double speed = (a[j][i] - WM->getJointAngle(WM->num2Str(a[j][0]))) / (a[1][i] - time) / 50.0;
                if (fabs(a[j][i] - WM->getJointAngle(WM->num2Str(a[j][0]))) > .4) {
                    ss += moveJoint(WM->num2Str(a[j][0]), speed);
                } else if ((WM->getLastJointAngle(WM->num2Str(a[j][0])) - WM->getJointAngle(WM->num2Str(a[j][0]))) != 0) {
                    ss += fix(WM->num2Str(a[j][0]));
                }
            }
            break;
        } else {
            continue;
        }
    }
    time += 0.02;
    if (time > a[1][n - 1]) {
        t = true;
        return "";
    }
    return ss;
}
//////////////////////////////////////////////////////////////////////

string Skill::fix()
{
    stringstream ss;
    for (int i = 0; i < 22; i++) {
        ss << fix(WM->num2Str(i));
    }
    return ss.str();
}
//////////////////////////////////////////////////////////////////////

string Skill::feh(bool a, double s)
{
    stringstream ss("");
    for (int i = 0; i < 22; i++)
        if (fabs(WM->getJointAngle(WM->num2Str(i))) > 0.2 && ((i != 14 && i != 15) || a)) {
            ss << moveJointTo(WM->num2Str(i), 0, s);
        }
    return ss.str();
}

//////////////////////////set////////////////////////////////////////

string Skill::set(string j, double ang, int cycle)
{
    stringstream ss;
    double speed = (ang) / cycle;
    if (speed > 7) {
        speed = 7;
    }
    ss << moveJoint(j, speed);
    return ss.str();
}
/////////////////////////////////////////////////////////////////////

string Skill::fix(string j)
{
    return moveJoint(j, 0);
}
/////////////////////////////////////turn//////////////////////////////////

string Skill::standUp(SideT s, bool& done, double& t)
{
    if (done) {
        t = 0;
        return "";
    }
    if (s == Front) {
        return act(standupF, standupF[0][0], standupF[1][0], t, done);
    } else {
        return StandUpBack(done);
    }
}

/////////////////////////////////////////////////////////////////////

string Skill::dive(SideT s, bool& done, double& t)
{
    if (done) {
        t = 0;
        return "";
    }
    if (s == Right) {
        return act(diveR, diveR[0][0], diveR[1][0], t, done);
    } else {
        return act(diveL, diveL[0][0], diveL[1][0], t, done);
    }
}
/////////////////////////////////////////////////////////////////////

string Skill::DP(SideT s, bool& done, double& t)
{
    if (done) {
        t = 0;
        return "";
    }
    if (s == Right) {
        return act(DPR, DPR[0][0], DPR[1][0], t, done);
    } else {
        return act(DPL, DPL[0][0], DPL[1][0], t, done);
    }
}
///////////////////////////////////////////////////////////////////////

string Skill::sefr(bool& done, double& t, bool b)
{
    if (done) {
        t = 0;
        return "";
    }
    if (b) {
        return act(zeroe, zeroe[0][0], zeroe[1][0], t, done);
    } else {
        return act(zero, zero[0][0], zero[1][0], t, done);
    }
}
////////////////////////////////////////////////////////////////////////

string Skill::Standing(bool& done, double& t)
{
    if (done) {
        t = 0;
        return "";
    }
    return act(standing, standing[0][0], standing[1][0], t, done);
}
//////////////////////////////////////////////////////////////////////

string Skill::Standing2(bool& done, double& t)
{
    if (done) {
        t = 0;
        return "";
    }
    return act(standing2, standing2[0][0], standing2[1][0], t, done);
}
///////////////////////////////////////////////////////////////////////

string Skill::StandingBack(bool& done, double& t)
{
    if (done) {
        t = 0;
        return "";
    }
    return act(standingB, standingB[0][0], standingB[1][0], t, done);
}
////////////////////////////////////////////////////////////////////////

string Skill::beam(double x, double y, double ang)
{

    stringstream message;
    message << "(beam " << x << " " << y << " " << ang << ")";
    return message.str();
}
////////////////////////////////////////////////////////////////////////

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///                             Say Ball Pos
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

string Skill::sayBallPos()
{
    int num = WM->getClosestOurToBall();
    Eigen::Vector3f m = WM->getOurPos(num) ;
    RVDraw::instance()->drawVector3f(salt::Vector3f(m.x(),m.y(),m.z()),BLUE,7878,3);
    stringstream ss;
    ss << "(say " << (char)(num + 60) << (char)(num + 60) << (char)(num + 60)  << ")";
    return ss.str();
}

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///                             Beam
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

string Skill::whereBeam(bool& stand, double& stan)
{

    stringstream ss("");
    if (WM->getMyNum() == 9) {
        if (WM->getTeamSide() == Left) {
            if (WM->getPlayMode() == PM_BeforeKickOff || WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right) {
                    ss << beam(-0.19, 0.0, 0);
            } else {
                ss << beam(-1.6, 0.0, 0);
            }
        } else {
            if (WM->getPlayMode() == PM_KickOff_Right || WM->getPlayMode() == PM_Goal_Left) {
                ss << beam(-0.19, 0, 0);
            } else {
                ss << beam(-1.6, 0.0, 0);
            }
        }
    } else if (WM->getMyNum() == 8) {
        if (WM->getTeamSide() == Left) {
            if (WM->getPlayMode() == PM_BeforeKickOff || WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right) {
                ss << beam(-0.18, -2.5, 0);
            } else {
                ss << beam(-2.6, 0.7, 0);
            }
        } else {
            if (WM->getPlayMode() == PM_KickOff_Right || WM->getPlayMode() == PM_Goal_Left) {
                ss << beam(-0.18, -2.5, 0);
            } else {
                ss << beam(-2.6, 0.7, 0);
            }
        }
    } else if (WM->getMyNum() == 3) {
        ss << beam(-13, -1.5, 0);
    } else if (WM->getMyNum() == 2) {
        ss << beam(-13, 0, 0);
    } else if (WM->getMyNum() == 4) {
        ss << beam(-13, 1.5, 0);
    } else if (WM->getMyNum() == 5) {
        ss << beam(-11, -1, 0);
    } else if (WM->getMyNum() == 6) {
        ss << beam(-11, 1, 0);
    } else if (WM->getMyNum() == 7) {
        ss << beam(-7, 0, 0);
    } else if (WM->getMyNum() == 10) {
        ss << beam(-4, 1.5, 0);
    } else if (WM->getMyNum() == 11) {
        ss << beam(-4, -1.5, 0);
    } else if (WM->getMyNum() == 1) {
        ss << beam(-14.5, 0, 0);
    }

    return ss.str();
}
//////////////////////////////////////////////////////////////////////

string Skill::updatem(bool checkBall)
{
    static int level = 0;
    if (level == 6 || (WM->seeEnoughFlag() && (WM->seeBall() || !checkBall))) {
        level = 0;
        return fix("he1") + fix("he2");
    }
    double theta = 2 * (level % 2 - 0.5) * 120.0;
    double phi = (level / 2 - 1) * 45;

    if (fabs(WM->getJointAngle("he1") - theta) < 5 && fabs(WM->getJointAngle("he2") - phi) < 5) {
        level++;
    }

    string s1 = moveJointTo("he1", theta, 6);
    string s2 = moveJointTo("he2", phi, 3);
    return s1 + s2;
}
//////////////////////////////////////////////////////////////////////

string Skill::StandUpBack(bool& done)
{
    stringstream ss;
    done = false;
    bool did = false;
    static double tt = 0;
    ss << sefr(did, tt, false);
    if (did == false) {
        return ss.str();
    }
    static int co = -1;
    static double tmp = 0;
    co++;
    if (tmp > 1.8) {
        did = false;
        tt = 0;
        done = true;
        tmp = 0;
        co = -1;
        return fix();
    }
    return act(standupB, tmp);
}

string Skill::ToRightSide(int i, bool& done)
{
    done = false;
    stringstream ss;
    static int cmd = 0;
    static bool doneS = false;
    static double tt = 0;
    cmd++;
    if (cmd < 3) {
        ss << fix() << moveJoint("lle2", 1.1) << moveJoint("rle2", 1.1) << moveJoint("lle6", -1.1) << moveJoint("rle6", -1.01);
    } else if (cmd < 19) {
        ss << fix() << moveJoint("lle2", i);
    } else if (doneS == false) {
        ss << sefr(doneS, tt, false);
    } else {
        done = true;
        cmd = 0;
        doneS = false;
        tt = 0;
    }
    return ss.str();
}

string Skill::ToLeftSide(int i, bool& done)
{
    done = false;
    stringstream ss;
    static int cmd = 0;
    static bool doneS = false;
    static double tt = 0;
    cmd++;
    if (cmd < 3) {
        ss << fix() << moveJoint("lle2", -1.1) << moveJoint("rle2", -1.1) << moveJoint("lle6", 1.1) << moveJoint("rle6", 1.01);
    } else if (cmd < 19) {
        ss << fix() << moveJoint("rle2", i * -1);
    } else if (doneS == false) {
        ss << sefr(doneS, tt, false);
    } else {
        done = true;
        cmd = 0;
        doneS = false;
        tt = 0;
    }
    return ss.str();
}
///////////////////////////////////////////////////////////////////

VecPosition Skill::FK_FOOT_HIP(double le4, double le5)
{
    double l3 = 0.12;
    double l4 = 0.10;
    double h, w;
    double a1 = 90 - le5;
    double a2 = a1 - le4;
    double s1 = sinDeg(a1);
    double c1 = cosDeg(a1);
    double s2 = sinDeg(a2);
    double c2 = cosDeg(a2);
    h = s1 * l4 + s2 * l3;
    w = c1 * l4 + c2 * l3;
    return VecPosition(w, h);
}
/////////////////////////////////////////////////////////////////////

bool Skill::IK_ALL(VecPosition Hip, VecPosition RAnkle, double& rle3, double& rle4, double& rle5, double& lle3, double& lle4, double& lle5, double P)
{
    double l3 = 0.12;
    double l4 = 0.10;
    double Hx = Hip.getX(), Hz = Hip.getY();
    double Ax = RAnkle.getX() - Hx, Az = RAnkle.getY() - Hz;
    //// LFoot
    /// First Answer
    double a51 = atan2Deg(((Hz * Hz * Hx * Hx) - Hx * sqrt(-(Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 + pow(l3, 4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 + pow(Hz, 4) + pow(Hx, 4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 + pow(l4, 4)))) + (Hz * Hz * l3 * l3) + pow(Hz, 4) - (Hz * Hz * l4 * l4)) / (Hz * Hz + Hx * Hx) / Hz / l3, ((l3 * l3 * Hx) + pow(Hx, 3) - (Hx * l4 * l4) + (Hz * Hz * Hx) + sqrt(-(Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 + pow(l3, 4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 + pow(Hz, 4) + pow(Hx, 4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 + pow(l4, 4))))) / (Hz * Hz + Hx * Hx) / l3);
    double a41 = atan2Deg(-(-(Hz * Hz * Hx * Hx) - Hx * sqrt(-(Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 + pow(l3, 4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 + pow(Hz, 4) + pow(Hx, 4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 + pow(l4, 4)))) + (Hz * Hz * l3 * l3) - pow(Hz, 4) - (Hz * Hz * l4 * l4)) / (Hz * Hz + Hx * Hx) / Hz / l4 / 0.2e1, -((l3 * l3 * Hx) - pow(Hx, 3) - (Hx * l4 * l4) - (Hz * Hz * Hx) + sqrt(-(Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 + pow(l3, 4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 + pow(Hz, 4) + pow(Hx, 4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 + pow(l4, 4))))) / (Hz * Hz + Hx * Hx) / l4 / 0.2e1);

    ///Second Answer
    double a52 = atan2Deg(((Hz * Hz * Hx * Hx) + Hx * sqrt(-(Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 + pow(l3, 4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 + pow(Hz, 4) + pow(Hx, 4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 + pow(l4, 4)))) + (Hz * Hz * l3 * l3) + pow(Hz, 4) - (Hz * Hz * l4 * l4)) / (Hz * Hz + Hx * Hx) / Hz / l3, ((l3 * l3 * Hx) + pow(Hx, 3) - (Hx * l4 * l4) + (Hz * Hz * Hx) - sqrt(-(Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 + pow(l3, 4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 + pow(Hz, 4) + pow(Hx, 4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 + pow(l4, 4))))) / (Hz * Hz + Hx * Hx) / l3);
    double a42 = atan2Deg(-(-(Hz * Hz * Hx * Hx) + Hx * sqrt(-(Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 + pow(l3, 4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 + pow(Hz, 4) + pow(Hx, 4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 + pow(l4, 4)))) + (Hz * Hz * l3 * l3) - pow(Hz, 4) - (Hz * Hz * l4 * l4)) / (Hz * Hz + Hx * Hx) / Hz / l4 / 0.2e1, -((l3 * l3 * Hx) - pow(Hx, 3) - (Hx * l4 * l4) - (Hz * Hz * Hx) - sqrt(-(Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 + pow(l3, 4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 + pow(Hz, 4) + pow(Hx, 4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 + pow(l4, 4))))) / (Hz * Hz + Hx * Hx) / l4 / 0.2e1);

    //// RFoot
    /// First Answer
    double b41 = atan2Deg(-((Az * Az * Ax * Ax) + Ax * sqrt(-(Az * Az * (-2 * Az * Az * l4 * l4 + pow(l4, 4) - 2 * l4 * l4 * Ax * Ax + pow(Ax, 4) + pow(Az, 4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 + pow(l3, 4)))) - (Az * Az * l4 * l4) + pow(Az, 4) + (Az * Az * l3 * l3)) / (Az * Az + Ax * Ax) / Az / l3 / 0.2e1, (-(l4 * l4 * Ax) + (Ax * l3 * l3) + pow(Ax, 3) + (Az * Az * Ax) - sqrt(-(Az * Az * (-2 * Az * Az * l4 * l4 + pow(l4, 4) - 2 * l4 * l4 * Ax * Ax + pow(Ax, 4) + pow(Az, 4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 + pow(l3, 4))))) / (Az * Az + Ax * Ax) / l3 / 0.2e1);
    double b31 = atan2Deg(-((Az * Az * Ax * Ax) - Ax * sqrt(-(Az * Az * (-2 * Az * Az * l4 * l4 + pow(l4, 4) - 2 * l4 * l4 * Ax * Ax + pow(Ax, 4) + pow(Az, 4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 + pow(l3, 4)))) + (Az * Az * l4 * l4) + pow(Az, 4) - (Az * Az * l3 * l3)) / (Az * Az + Ax * Ax) / Az / l4 / 0.2e1, ((l4 * l4 * Ax) - (Ax * l3 * l3) + pow(Ax, 3) + (Az * Az * Ax) + sqrt(-(Az * Az * (-2 * Az * Az * l4 * l4 + pow(l4, 4) - 2 * l4 * l4 * Ax * Ax + pow(Ax, 4) + pow(Az, 4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 + pow(l3, 4))))) / (Az * Az + Ax * Ax) / l4 / 0.2e1);

    ///Second Answer
    double b42 = atan2Deg(-((Az * Az * Ax * Ax) - Ax * sqrt(-(Az * Az * (-2 * Az * Az * l4 * l4 + pow(l4, 4) - 2 * l4 * l4 * Ax * Ax + pow(Ax, 4) + pow(Az, 4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 + pow(l3, 4)))) - (Az * Az * l4 * l4) + pow(Az, 4) + (Az * Az * l3 * l3)) / (Az * Az + Ax * Ax) / Az / l3 / 0.2e1, (-(l4 * l4 * Ax) + (Ax * l3 * l3) + pow(Ax, 3) + (Az * Az * Ax) + sqrt(-(Az * Az * (-2 * Az * Az * l4 * l4 + pow(l4, 4) - 2 * l4 * l4 * Ax * Ax + pow(Ax, 4) + pow(Az, 4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 + pow(l3, 4))))) / (Az * Az + Ax * Ax) / l3 / 0.2e1);
    double b32 = atan2Deg(-((Az * Az * Ax * Ax) + Ax * sqrt(-(Az * Az * (-2 * Az * Az * l4 * l4 + pow(l4, 4) - 2 * l4 * l4 * Ax * Ax + pow(Ax, 4) + pow(Az, 4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 + pow(l3, 4)))) + (Az * Az * l4 * l4) + pow(Az, 4) - (Az * Az * l3 * l3)) / (Az * Az + Ax * Ax) / Az / l4 / 0.2e1, ((l4 * l4 * Ax) - (Ax * l3 * l3) + pow(Ax, 3) + (Az * Az * Ax) - sqrt(-(Az * Az * (-2 * Az * Az * l4 * l4 + pow(l4, 4) - 2 * l4 * l4 * Ax * Ax + pow(Ax, 4) + pow(Az, 4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 + pow(l3, 4))))) / (Az * Az + Ax * Ax) / l4 / 0.2e1);

    double lle5_1 = 90 - a51;
    double lle5_2 = 90 - a52;
    double lle4_1 = a51 - a41;
    double lle4_2 = a52 - a42;

    double rle3_1 = 90 + P - b31;
    double rle3_2 = 90 + P - b32;
    double rle4_1 = b31 - b41;
    double rle4_2 = b32 - b42;

    if (isPossible(lle4_1)) {
        lle4 = lle4_1;
        lle5 = lle5_1;
    } else if (isPossible(lle4_2)) {
        lle4 = lle4_2;
        lle5 = lle5_2;
    } else {
        return false;
    }
    lle3 = P - lle5 - lle4;

    if (isPossible(rle4_1)) {
        rle4 = rle4_1;
        rle3 = rle3_1;
    } else if (isPossible(rle4_2)) {
        rle4 = rle4_2;
        rle3 = rle3_2;
    } else {
        return false;
    }
    rle5 = P - rle3 - rle4;
    return true;
}
///////////////////////////////////////////////////////////////////

bool Skill::isPossible(double l4)
{
    if (l4 < 0 && l4 > -130) {
        return true;
    }
    return false;
}
///////////////////////////////////////////////////////////////////

string Skill::shootR(bool& canshoot, bool& done, double& tFinal)
{
    //    return shootR2(done);
    WM->setZ(0.54);
    stringstream ss("");
    VecPosition me(WM->getMyPos().x(), WM->getMyPos().y());
    VecPosition ball(WM->getBallPos().x(), WM->getBallPos().y());
    static bool ne = true;
    static double t = 0;
    static bool did = false;
    static double tt = 0;
    static VecPosition H = FK_FOOT_HIP(-65, 45);
    VecPosition pos = VecPosition::givePosition(ball, VecPosition::normalizeAngle(WM->getMyAngle()), -0.01);
    double x1 = 0.002, y1 = 0.0;
    double x2 = H.getX(), y2 = H.getY();
    double lj3 = WM->getJointAngle("lle3");
    double lj4 = WM->getJointAngle("lle4");
    double lj5 = WM->getJointAngle("lle5");
    double rj3 = WM->getJointAngle("rle3");
    double rj4 = WM->getJointAngle("rle4");
    double rj5 = WM->getJointAngle("rle5");
    double r3, r4, r5, l3, l4, l5;
    double P = 0.2;
    static int id = 0;
    static bool sw = true;
    canshoot = IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5);

    if (t > 1.7 || me.getDistanceTo(ball) > 0.4) {
        id = 0;
    }

    if (  WM->isFeltDown() || t > 1.76  ) {
        t = 0;
        done = true;
        sw = true;
        did = false;
        tt = 0;
        return ss.str();
    }
    if (me.getDistanceTo(ball) < 0.17 && id < 18 && !(WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right)) {
        /// Shoot 2
        ne = true;
    } else if ((me.getDistanceTo(ball) > 0.17 && id < 18) || (WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right)) {
        /// Shoot 1
        ne = false;
    }
    //    if(fabs(WM->getMyAngleToGoal())>5 && sw && !(WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right) && me.getX()>-1 )
    //    {
    //        if(WM->getMyAngleToGoal()>0)
    //        {
    //            return finalAction("turnL",tFinal);
    //        }
    //        else
    //        {
    //            return finalAction("turnR",tFinal);
    //        }
    //    }
    id++;
    sw = false;

    if (!did && t < 0.1) {
        ss << sefr(did, tt, false);
        return ss.str();
    }

    t += 0.02;
    if (t < 0.3) {
        ss << moveJoint("lle2", -1) << moveJoint("rle2", -1) << moveJoint("lle6", 1) << moveJoint("rle6", 1);
        return ss.str();
    } else if (t < 0.45) {
        did = false;
        tt = 0;
        ss << moveJoint("lle2", 0) << moveJoint("rle3", 2) << moveJoint("rle4", -4) << moveJoint("rle5", 3.5);
        return ss.str();
    } else if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5) && t < 0.9) {
        ss << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
    } else if (t > 0.9 && t < 1.1 && !ne) {
        ss << moveJoint("rle6", -0.3) << moveJoint("rle2", 0.3);
        ss << moveJoint("rle5", 0) << moveJoint("rle4", -1.7);
        ss << moveJoint("rle3", -1.2) << moveJoint("lle3", 1.2);
    } else if (t > 1.2 && t < 1.3 && !ne) {
        ss << moveJoint("rle5", 0.6);
        return ss.str();
    } else if (t > 1.4 && t < 1.5 && !ne) {
        ss << moveJoint("rle3", 8);
        ss << moveJoint("lle3", -8) << moveJoint("rle5", -8);
        ss << moveJoint("rle4", 8);
    } else if (t > 1.5 && t < 1.7 && !ne) {
        ss << moveJoint("rle3", -5) << moveJoint("rle5", -5.5);
        ss << moveJoint("rle4", 8);
    } else if (t > 0.9 && t < 1.4 && ne) {
        ss << moveJoint("rle6", -0.2) << moveJoint("rle2", 0.2);
        ss << moveJoint("rle5", -2.65) << moveJoint("rle4", -3.04);
        ss << moveJoint("rle3", -0.8) << moveJoint("lle3", 0.8);
    } else if (t > 1.6 && t < 1.72 && ne) {
        ss << moveJoint("rle3", 8);
        ss << moveJoint("lle3", -8) << moveJoint("rle5", 8);
        ss << moveJoint("rle4", 8);
    } else if (t > 1.7 && t < 1.9 && ne) {
        ss << moveJoint("rle3", -5) << moveJoint("rle5", -5.5);
        ss << moveJoint("rle4", 8);
    }
    return ss.str();
}
///////////////////////////////////////////////////////////////////

string Skill::shootL(bool& canshoot, bool& done, double& tFinal)
{
    //    return shootL2(done);
    WM->setZ(0.54);
    stringstream ss;
    VecPosition me(WM->getMyPos().x(), WM->getMyPos().y());
    VecPosition ball(WM->getBallPos().x(), WM->getBallPos().y());
    static double t = 0;
    static bool did = false, ne = true;
    static double tt = 0;
    static int id = 0;
    static VecPosition H = FK_FOOT_HIP(-65, 45);
    VecPosition pos = VecPosition::givePosition(ball, VecPosition::normalizeAngle(WM->getMyAngle()), -0.01);
    double x1 = 0.002, y1 = 0.0;
    double x2 = H.getX(), y2 = H.getY();
    double lj3 = WM->getJointAngle("lle3");
    double lj4 = WM->getJointAngle("lle4");
    double lj5 = WM->getJointAngle("lle5");
    double rj3 = WM->getJointAngle("rle3");
    double rj4 = WM->getJointAngle("rle4");
    double rj5 = WM->getJointAngle("rle5");
    double r3, r4, r5, l3, l4, l5;
    double P = 0.2;
    static bool sw = true;
    canshoot = IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5);
    if (t > 1.7 || me.getDistanceTo(ball) > 0.4) {
        id = 0;
    }
    if (WM->isFeltDown() || t > 1.7 ) {
        t = 0;
        done = true;
        sw = true;
        did = false;
        tt = 0;
        return ss.str();
    }
    if (me.getDistanceTo(ball) < 0.17 && id < 18 && !(WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right)) {
        ne = true;
    } else if ((me.getDistanceTo(ball) > 0.17 && id < 18) || (WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right)) {
        ne = false;
    }
    //    if(fabs(WM->getMyAngleTo(Eigen::Vector3f(9,0,0)))>5 && sw && !(WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right) && me.getX()>-1)
    //    {
    //        if(WM->getMyAngleTo(Eigen::Vector3f(9,0,0))>0)
    //        {
    //            return finalAction("turnL",tFinal);
    //        }
    //        else
    //        {
    //            return finalAction("turnR",tFinal);
    //        }
    //    }
    sw = false;
    id++;

    if (!did && t < 0.1) {
        ss << sefr(did, tt, false);
        return ss.str();
    }

    t += 0.02;
    if (t < 0.3) {
        ss << moveJoint("lle2", 1) << moveJoint("rle2", 1) << moveJoint("lle6", -1) << moveJoint("rle6", -1);
        return ss.str();
    } else if (t < 0.45) {
        did = false;
        tt = 0;
        ss << moveJoint("rle2", 0) << moveJoint("lle3", 2) << moveJoint("lle4", -4) << moveJoint("lle5", 2);
        return ss.str();
    } else if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5) && t < 0.9) {
        ss << moveJoint("lle3", (r3 - lj3) * P) << moveJoint("lle4", (r4 - lj4) * P) << moveJoint("lle5", (r5 - lj5) * P);
    } else if (t > 0.9 && t < 1.1 && !ne) {
        ss << moveJoint("lle6", 0.3) << moveJoint("lle2", -0.3);
        ss << moveJoint("lle5", 0) << moveJoint("lle4", -1.7);
        ss << moveJoint("lle3", -1.2) << moveJoint("rle3", 1.2);
    } else if (t > 1.2 && t < 1.3 && !ne) {
        ss << moveJoint("lle5", 0.8);
        return ss.str();
    } else if (t > 1.4 && t < 1.5 && !ne) {
        ss << moveJoint("lle3", 8);
        ss << moveJoint("rle3", -8);
        ss << moveJoint("lle4", 8) << moveJoint("lle5", -8);
    } else if (t > 1.5 && t < 1.7 && !ne) {
        ss << moveJoint("lle3", -5) << moveJoint("lle5", -5);
        ss << moveJoint("lle4", 8);
    } else if (t > 0.9 && t < 1.4 && ne) {
        ss << moveJoint("lle6", 0.2) << moveJoint("lle2", -0.2);
        ss << moveJoint("lle5", -2.65) << moveJoint("lle4", -3.04);
        ss << moveJoint("lle3", -0.8) << moveJoint("rle3", 0.8);
    } else if (t > 1.5 && t < 1.62 && ne) {
        ss << moveJoint("lle3", 8);
        ss << moveJoint("rle3", -8);
        ss << moveJoint("lle4", 8) << moveJoint("lle5", 8);
    } else if (t > 1.6 && t < 1.8 && ne) {
        ss << moveJoint("lle3", -5) << moveJoint("lle5", -5);
        ss << moveJoint("lle4", 8);
    }
    return ss.str();
}
////////////////////////////////////////////////////////////////////

bool Skill::nazdik(string type)
{
    if (type == "turnL" || type == "turnR" || type == "sideWalkR" || type == "sideWalkL" || type == "sideTurnL" || type == "sideTurnR")
        return true;
    return false;
}

string Skill::finalAction(string type, double& t, double maxV)
{
    WM->setZ(0.51);
    VecPosition ball(WM->getBallPos().x(), WM->getBallPos().y());
    VecPosition me(WM->getMyPos().x(), WM->getMyPos().y());
    stringstream ss;
    static double df = 0;
    static double time = 0;
    static double dd = 0, bb = 0, sd = 2;
    static bool done2 = false;
    static bool d2 = false;

    static VecPosition H = FK_FOOT_HIP(-65, 45);
    static double T = 15 * 0.02;

    static double w = (2 * M_PI) / T;
    double a = 0.05 * dd, b = -0.017 * bb;
    if (nazdik(type))
        b = -0.014;

    double a1 = a / 2, b1 = b / 2;
    double x1 = a * cos(w * t), y1 = b * sin(w * t);
    double x2 = H.getX() + a1 * cos(w * t), y2 = H.getY() + b1 * sin(w * t);

    double lj1 = WM->getJointAngle("lle1");
    double rj1 = WM->getJointAngle("rle1");
    double lj2 = WM->getJointAngle("lle2");
    double rj2 = WM->getJointAngle("rle2");
    double lj3 = WM->getJointAngle("lle3");
    double rj3 = WM->getJointAngle("rle3");
    double lj4 = WM->getJointAngle("lle4");
    double rj4 = WM->getJointAngle("rle4");
    double lj5 = WM->getJointAngle("lle5");
    double rj5 = WM->getJointAngle("rle5");
    double lj6 = WM->getJointAngle("lle6");
    double rj6 = WM->getJointAngle("rle6");
    double r3, r4, r5, l3, l4, l5, l11, lf11, lf22, l22, l33, d = 0, f = 0;
    double P = 0.5;
    if (t == 0) {
        d2 = false;
        time = 0;
        dd = 0;
        sd = 2;
        bb = 1;
    }

    if (d2) {
        speedControler(type, t, dd, bb, sd, maxV);
    }
    t += 0.02;

    /////////
    if (!d2) {
        stringstream s;
        s << Standing(d2, time);
        dd = 0;
        return s.str();
    }
    ////////
    if (type == "walk") {
        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("rle1", (0 - rj1) * P) << moveJoint("lle1", (0 - lj1) * P) << moveJoint("rle2", (0 - rj2) * P) << moveJoint("lle2", (0 - lj2) * P) << moveJoint("rle6", (0 - rj6) * P) << moveJoint("lle6", (0 - lj6) * P);
        }
    } else if (type == "bwalk") {

        if (me.getDistanceTo(ball) > 0.4) {
            a = 0.06 * dd;
        }
        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("rle1", (0 - rj1) * P) << moveJoint("lle1", (0 - lj1) * P) << moveJoint("rle2", (0 - rj2) * P) << moveJoint("lle2", (0 - lj2) * P) << moveJoint("rle6", (0 - rj6) * P) << moveJoint("lle6", (0 - lj6) * P);
        }
    } else if (type == "walkAngleL") {

        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = 5;
            } else {
                d = -5;
            }

            l11 = d;
            l22 = (-27 / 45) * d;
            l33 = (-10 / 45) * d;
            ss << moveJoint("lle3", (l3 - lj3 + (l33)) * P) << moveJoint("lle4", (l4 - lj4 - (2 * l33)) * P) << moveJoint("lle5", (l5 - lj5 + (l33)) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("lle1", (l11 - lj1) * P) << moveJoint("rle1", (l11 - rj1) * P) << moveJoint("lle2", (l22 - lj2) * P);
            ss << moveJoint("rle2", (0 - rj2) * P) << moveJoint("rle6", (0 - rj6) * P) << moveJoint("lle6", (0 - lj6) * P);
        }
    } else if (type == "walkAngleR") {

        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = -5;
            } else {
                d = 5;
            }
            l11 = d;
            l22 = (-27 / 45) * d;
            l33 = (-10 / 45) * d;
            ss << moveJoint("lle3", (l3 - lj3 + l33) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5 + l33) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("lle1", (l11 - lj1) * P) << moveJoint("rle1", (l11 - rj1) * P) << moveJoint("rle2", (l22 - rj2) * P);
            ss << moveJoint("lle2", (0 - lj2) * P) << moveJoint("rle6", (0 - rj6) * P) << moveJoint("lle6", (0 - lj6) * P);
        }
    } else if (type == "bwalkAngleL") {
        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = 5;
            } else {
                d = -5;
            }

            l11 = d;
            l22 = (-27 / 45) * d;
            l33 = (-10 / 45) * d;
            ss << moveJoint("lle3", (l3 - lj3 + l33) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5 + l33) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("lle1", (l11 - lj1) * P) << moveJoint("rle1", (l11 - rj1) * P) << moveJoint("lle2", (l22 - lj2) * P);
            ss << moveJoint("rle2", (0 - rj2) * P) << moveJoint("rle6", (0 - rj6) * P) << moveJoint("lle6", (0 - lj6) * P);
        }
    } else if (type == "bwalkAngleR") {
        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = -5;
            } else {
                d = 5;
            }

            l11 = d;
            l22 = (-27 / 45) * d;
            l33 = (-10 / 45) * d;
            ss << moveJoint("lle3", (l3 - lj3 + l33) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5 + l33) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("lle1", (l11 - lj1) * P) << moveJoint("rle1", (l11 - rj1) * P) << moveJoint("rle2", (l22 - rj2) * P);
            ss << moveJoint("lle2", (0 - lj2) * P) << moveJoint("rle6", (0 - rj6) * P) << moveJoint("lle6", (0 - lj6) * P);
        }
    } else if (type == "turnR") {
        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = -5;
            } else {
                d = 5;
            }

            l11 = d;
            l22 = (-25 / 45) * d;
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("lle1", (l11 - lj1) * P) << moveJoint("rle1", (l11 - rj1) * P) << moveJoint("rle2", (l22 - rj2) * P);
            ss << moveJoint("lle2", (0 - lj2) * P) << moveJoint("rle6", (0 - rj6) * P) << moveJoint("lle6", (0 - lj6) * P);
        }
    } else if (type == "turnL") {
        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = 5;
            } else {
                d = -5;
            }

            l11 = d;
            l22 = (-25 / 45) * d;
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("lle1", (l11 - lj1) * P) << moveJoint("rle1", (l11 - rj1) * P) << moveJoint("lle2", (l22 - lj2) * P) << moveJoint("lle6", (l22 - lj6) * P);
            ss << moveJoint("rle2", (0 - rj2) * P) << moveJoint("rle6", (0 - rj6) * P) << moveJoint("lle6", (0 - lj6) * P);
        }
    } else if (type == "sideWithWalkR") {

        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = 2;
            } else {
                d = -2;
            }

            l11 = d / 2.5;
            l22 = -d;
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("lle2", (l22 - lj2) * P) << moveJoint("rle2", -(l22 + rj2) * P) << moveJoint("lle6", -(l22 + lj6) * P) << moveJoint("rle6", (l22 - rj6) * P) << moveJoint("lle1", (l11 - lj1) * P);
            ss << moveJoint("rle1", (0 - rj1) * P);
        }
    } else if (type == "sideWithWalkL") {

        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = 2;
            } else {
                d = -2;
            }

            l11 = -d / 2.5;
            l22 = -d;
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("rle2", (l22 - rj2) * P) << moveJoint("lle2", -(l22 + lj2) * P) << moveJoint("rle6", -(l22 + rj6) * P) << moveJoint("lle6", (l22 - lj6) * P) << moveJoint("rle1", (l11 - rj1) * P);
            ss << moveJoint("lle1", (0 - lj1) * P);
        }
    } else if (type == "sideWithbWalkR") {

        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = 3;
            } else {
                d = -3;
            }

            l11 = d / 2.5;
            l22 = -d;
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("lle2", (l22 - lj2) * P) << moveJoint("rle2", -(l22 + rj2) * P) << moveJoint("lle6", -(l22 + lj6) * P) << moveJoint("rle6", (l22 - rj6) * P) << moveJoint("lle1", (l11 - lj1) * P);
            ss << moveJoint("rle1", (0 - rj1) * P);
        }
    } else if (type == "sideWithbWalkL") {

        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = 3;
            } else {
                d = -3;
            }

            l11 = -d / 2.5;
            l22 = -d;
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("rle2", (l22 - rj2) * P) << moveJoint("lle2", -(l22 + lj2) * P) << moveJoint("rle6", -(l22 + rj6) * P) << moveJoint("lle6", (l22 - lj6) * P) << moveJoint("rle1", (l11 - rj1) * P);
            ss << moveJoint("lle1", (0 - lj1) * P);
        }
    } else if (type == "sideWalkR") {
        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = sd;
            } else {
                d = -sd;
            }

            l11 = d / 2.5;
            l22 = -d;
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("lle2", (l22 - lj2) * P) << moveJoint("rle2", -(l22 + rj2) * P) << moveJoint("lle6", -(l22 + lj6) * P) << moveJoint("rle6", (l22 - rj6) * P) << moveJoint("lle1", (l11 - lj1) * P);
            ss << moveJoint("rle1", (0 - rj1) * P);
        }
    } else if (type == "sideWalkL") {
        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = sd;
            } else {
                d = -sd;
            }

            l11 = -d / 2.5;
            l22 = -d;
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("rle2", (l22 - rj2) * P) << moveJoint("lle2", -(l22 + lj2) * P) << moveJoint("rle6", -(l22 + rj6) * P) << moveJoint("lle6", (l22 - lj6) * P) << moveJoint("rle1", (l11 - rj1) * P);
            ss << moveJoint("lle1", (0 - lj1) * P);
        }
    } else if (type == "sideTurnL") {
        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = 4;
                f = 4;
            } else {
                d = -4;
                f = -4;
            }

            lf11 = f;
            lf22 = (-25 / 45) * f;
            l11 = -d / 2.5;
            l22 = -d;
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("rle2", (l22 - rj2) * P) << moveJoint("lle2", (-(l22 + lj2) + (lf22 - lj2)) / 2 * P) << moveJoint("rle6", -(l22 + rj6) * P) << moveJoint("lle6", ((l22 - lj6) + (lf22 - lj6)) / 2 * P);
            ss << moveJoint("lle1", (lf11 - lj1) * P) << moveJoint("rle1", (lf11 - rj1) * P);
            //ss<<moveJoint ( "lle1" , (0-lj1)*P);
        }
    } else if (type == "sideTurnR") {
        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            if (((l4 - lj4) * P) < 0) {
                d = 4;
                f = -4;
            } else {
                d = -4;
                f = 4;
            }
            lf11 = f;
            lf22 = (-25 / 45) * f;
            l11 = d / 2.5;
            l22 = -d;
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("lle2", (l22 - lj2) * P) << moveJoint("rle2", (-(l22 + rj2) + (lf22 - rj2)) / 2 * P) << moveJoint("lle6", -(l22 + lj6) * P) << moveJoint("rle6", ((l22 - rj6) + (lf22 - rj6)) / 2 * P);
            ss << moveJoint("lle1", (lf11 - lj1) * P) << moveJoint("rle1", (lf11 - rj1) * P);
            //ss<<moveJoint ( "rle1" , (0-rj1)*P);
        }
    } else {

        if (IK_ALL(VecPosition(x2, y2), VecPosition(x1, y1), r3, r4, r5, l3, l4, l5)) {
            ss << moveJoint("lle3", (l3 - lj3) * P) << moveJoint("lle4", (l4 - lj4) * P) << moveJoint("lle5", (l5 - lj5) * P)
               << moveJoint("rle3", (r3 - rj3) * P) << moveJoint("rle4", (r4 - rj4) * P) << moveJoint("rle5", (r5 - rj5) * P);
            ss << moveJoint("rle1", (0 - rj1) * P) << moveJoint("lle1", (0 - lj1) * P) << moveJoint("rle2", (0 - rj2) * P) << moveJoint("lle2", (0 - lj2) * P) << moveJoint("rle6", (0 - rj6) * P) << moveJoint("lle6", (0 - lj6) * P);
        }
    }

//    double A1 = 15, A2 = 30, fi = 0, B1 = -90 + A1, B2 = A2;
//    static double TT, ww, ta = 0;
//    static int Tint = 55;
//    TT = Tint * 0.02;
//    ww = (2 * M_PI) / TT;
//    ss << moveJoint("lae1", B1 + A1 * sin(ww * ta + fi) - WM->getJointAngle("lae1"))
//       << moveJoint("rae1", B1 + -1 * A1 * sin(ww * ta + fi) - WM->getJointAngle("rae1"))
//       << moveJoint("lae4", -1 * B2 + -1 * A2 * sin(ww * ta + fi) - WM->getJointAngle("lae4"))
//       << moveJoint("rae4", B2 + -1 * A2 * sin(ww * ta + fi) - WM->getJointAngle("rae4"));
//    ta += .02;

    return ss.str();
}

void Skill::speedControler(string type, double t, double& dd, double& bb, double& sd, double maxV)
{
    VecPosition ball(WM->getBallPos().x(), WM->getBallPos().y());
    VecPosition me(WM->getMyPos().x(), WM->getMyPos().y());
    if (t == 0) {
        dd = 0;
    }
    if (type == "walk") {
        if (me.getDistanceTo(ball) > 0.8) {
            if (dd > 0.98 && dd < (maxV + 0.4)) {
                dd += 0.005;
            }
            if (dd < 1) {
                dd += 0.01;
            }
            if (dd > 1 && bb < 1.4) {
                bb += 0.005;
            }
        } else {
            if (bb < (0.75 + (maxV - 1))) {
                bb += 0.008;
            }
            if (bb > (0.75 + (maxV - 1))) {
                bb -= 0.005;
            }
            if (dd < (maxV - 0.3)) {
                dd += 0.03;
            }
            if (dd > (maxV - 0.3)) {
                dd -= 0.015;
            }
        }
    } else if (type == "walkAngleL" || type == "walkAngleR") {
        if (dd < 1.1) {
            dd += 0.01;
        }
        if (dd > 1.1) {
            dd -= 0.02;
        }
        if (bb > 1) {
            bb -= 0.012;
        }
    } else if (type == "bwalkAngleL" || type == "bwalkAngleR") {
        if (dd > -1) {
            dd -= 0.01;
        }
        if (bb > 1) {
            bb -= 0.005;
        }
    } else if (type == "bwalk" || type == "sideWithbWalkR" || type == "sideWithbWalkL") {
        if (dd > -1) {
            dd -= 0.02;
        } else if (dd > -1.5) {
            dd -= 0.005;
        }
        if (bb > 1) {
            bb -= 0.005;
        }
    } else if (type == "sideWithWalkR" || type == "sideWithWalkL") {
        if (dd < 0.9) {
            dd += 0.02;
        }
        if (bb > 1) {
            bb -= 0.005;
        }
    } else {
        if (fabs(dd) < 0.1) {
            dd = 0;
        } else if (dd > 0) {
            dd -= 0.05;
        } else if (dd < 0) {
            dd += 0.025;
        }
        if (bb > 0.75) {
            bb -= 0.008;
        }
    }
    if (type == "sideWalkR" || type == "sideWalkL" || type == "sideWithWalkR" || type == "sideWithWalkL" || type == "sideWithbWalkR" || type == "sideWithbWalkL") {
        if (sd < 6)
            sd += 0.08;
        else
            sd = 6;
    } else {
        sd = 2;
    }
}
////////////////////////////////

string Skill::shootL2(bool& done)
{
    static int t = 0;
    int t1 = 13, t2 = 28, t3 = 50, t4 = 70, t5 = 90;
    t++;

    unsigned int nj2 = lLeg.getNrOfSegments();

    KDL::JntArray jointpositionsl = KDL::JntArray(nj2);
    KDL::JntArray jointpositionsr = KDL::JntArray(nj2);

    KDL::JntArray lLegJMin = KDL::JntArray(nj2);
    lLegJMin(0) = Deg2Rad(-90);
    lLegJMin(1) = Deg2Rad(-25);
    lLegJMin(2) = Deg2Rad(-25);
    lLegJMin(3) = Deg2Rad(-130);
    lLegJMin(4) = Deg2Rad(-45);
    lLegJMin(5) = Deg2Rad(-45);

    KDL::JntArray lLegJMax = KDL::JntArray(nj2);
    lLegJMax(0) = Deg2Rad(1);
    lLegJMax(1) = Deg2Rad(45);
    lLegJMax(2) = Deg2Rad(100);
    lLegJMax(3) = Deg2Rad(1);
    lLegJMax(4) = Deg2Rad(75);
    lLegJMax(5) = Deg2Rad(25);

    KDL::JntArray rLegJMin = KDL::JntArray(nj2);

    rLegJMin(0) = Deg2Rad(-90);
    rLegJMin(1) = Deg2Rad(-45);
    rLegJMin(2) = Deg2Rad(-25);
    rLegJMin(3) = Deg2Rad(-130);
    rLegJMin(4) = Deg2Rad(-45);
    rLegJMin(5) = Deg2Rad(-25);

    KDL::JntArray rLegJMax = KDL::JntArray(nj2);

    rLegJMax(0) = Deg2Rad(1);
    rLegJMax(1) = Deg2Rad(25);
    rLegJMax(2) = Deg2Rad(100);
    rLegJMax(3) = Deg2Rad(1);
    rLegJMax(4) = Deg2Rad(75);
    rLegJMax(5) = Deg2Rad(45);

    KDL::ChainFkSolverPos_recursive lLegFKSolver = KDL::ChainFkSolverPos_recursive(lLeg);
    KDL::ChainIkSolverVel_wdls lLegIkSolverVel(lLeg, 0.01, 100); //Inverse velocity solver, from Cartesian to joint space
    KDL::ChainIkSolverPos_NR_JL lLegIksolverPosJ(lLeg, lLegJMin, lLegJMax, lLegFKSolver, lLegIkSolverVel, 100, 1e-3); //Maximum 100 iterations, stop at accuracy 1e-6

    KDL::ChainFkSolverPos_recursive rLegFKSolver = KDL::ChainFkSolverPos_recursive(lLeg);
    KDL::ChainIkSolverVel_wdls rLegIkSolverVel(lLeg, 0.01, 100); //Inverse velocity solver, from Cartesian to joint space
    KDL::ChainIkSolverPos_NR_JL rLegIksolverPosJ(lLeg, rLegJMin, rLegJMax, rLegFKSolver, rLegIkSolverVel, 100, 1e-3); //Maximum 100 iterations, stop at accuracy 1e-6

    //    int initialJ = 5;

    jointpositionsl(0) = Deg2Rad(0);
    jointpositionsl(1) = Deg2Rad(0);
    jointpositionsl(2) = Deg2Rad(0);
    jointpositionsl(3) = Deg2Rad(0);
    jointpositionsl(4) = Deg2Rad(0);
    jointpositionsl(5) = Deg2Rad(0);

    jointpositionsr(5) = Deg2Rad(0);
    jointpositionsr(4) = Deg2Rad(0);
    jointpositionsr(3) = Deg2Rad(0);
    jointpositionsr(2) = Deg2Rad(0);
    jointpositionsr(1) = Deg2Rad(0);
    jointpositionsr(0) = Deg2Rad(0);

    static KDL::Frame rHipStanding;
    static KDL::Frame lAnkleStanding;

    if (t == t4) {
        lLegFKSolver.JntToCart(jointpositionsl, lAnkleStanding);
        rLegFKSolver.JntToCart(jointpositionsr, rHipStanding);
    }

    Eigen::Vector3f ball = WM->getBallPos();
    //    ball.z() = 0.04*(1-0.707106781);

    //    cout << "Ball Pos: " << ball << endl;

    stringstream ss("");

    double rj3 = WM->getJointAngle("rle3");
    double lj2 = WM->getJointAngle("lle2");
    double lj3 = WM->getJointAngle("lle3");
    double lj4 = WM->getJointAngle("lle4");
    double lj5 = WM->getJointAngle("lle5");

    if (t > 0 && t <= t1) {
        return moveJoints(jointpositionsl, jointpositionsr, 0.2);
    } else if (t > t1 && t <= t2) {
        ss << moveJoint("lle2", 1.2) << moveJoint("rle2", 1.2) << moveJoint("lle6", -1.2) << moveJoint("rle6", -1.2);
        return ss.str();
    } else if (t > t2 && t <= t3) {
        ss << moveJoint("lle3", 2.5) << moveJoint("lle4", -5) << moveJoint("lle5", 4.25) << moveJoint("lle2", 0.4);
        return ss.str();
    } else if (t > t3 && t <= t4) {
        double l3 = (-25 - lj3) / double(t4 + 1 - t);
        double l4 = (-130 - lj4) / double(t4 + 1 - t);
        double r3 = (30 - rj3) / double(t4 + 1 - t);
        double l5 = (75 - lj5) / double(t4 + 1 - t);
        //        cout<<"Amin: t3->"<<r3<<" "<<r4<<" "<<r5<<" "<<l3<<endl;
        ss << moveJoint("lle3", l3) << moveJoint("lle4", l4) << moveJoint("lle5", l5) << moveJoint("rle3", r3);
        return ss.str();
    } else if (t > t4 && t <= t5) {

        cout << ball << endl;
        cout << rHipStanding.p << endl;
        static KDL::JntArray ballJoints;
        //        if (t <= t4 + (t5-t4)/2) {
        double l3 = (100 - lj3) / (t5 - t);
        double l4 = (0 - lj4) / (t5 - t);
        double r3 = 4 * (-25 - rj3) / (t5 - t);
        double l5 = 1.5 * (-45 - lj5) / (t5 - t);
        double l2 = (-40 - lj2) / (t5 - t);
        if (1 || t <= t4 + 2 * (t5 - t4) / 5) {
            ss << moveJoint("lle3", l3) << moveJoint("lle4", l4) << moveJoint("lle5", l5) << moveJoint("rle3", r3);

        } else {
            ss << moveJoint("lle3", l3) << moveJoint("lle4", l4) << moveJoint("lle5", l5) << moveJoint("rle3", r3) << moveJoint("lle2", l2);
        }
        return ss.str();
    } else if (t > t5) {
        done = true;
        t = 0;
    }
    return "";
}

string Skill::shootR2(bool& done)
{
    static int t = 0;
    int t1 = 13, t2 = 28, t3 = 50, t4 = 70, t5 = 90;
    t++;

    unsigned int nj2 = lLeg.getNrOfSegments();

    KDL::JntArray jointpositionsl = KDL::JntArray(nj2);
    KDL::JntArray jointpositionsr = KDL::JntArray(nj2);

    KDL::JntArray lLegJMin = KDL::JntArray(nj2);
    lLegJMin(0) = Deg2Rad(-90);
    lLegJMin(1) = Deg2Rad(-25);
    lLegJMin(2) = Deg2Rad(-25);
    lLegJMin(3) = Deg2Rad(-130);
    lLegJMin(4) = Deg2Rad(-45);
    lLegJMin(5) = Deg2Rad(-45);

    KDL::JntArray lLegJMax = KDL::JntArray(nj2);
    lLegJMax(0) = Deg2Rad(1);
    lLegJMax(1) = Deg2Rad(45);
    lLegJMax(2) = Deg2Rad(100);
    lLegJMax(3) = Deg2Rad(1);
    lLegJMax(4) = Deg2Rad(75);
    lLegJMax(5) = Deg2Rad(25);

    KDL::JntArray rLegJMin = KDL::JntArray(nj2);

    rLegJMin(0) = Deg2Rad(-90);
    rLegJMin(1) = Deg2Rad(-45);
    rLegJMin(2) = Deg2Rad(-25);
    rLegJMin(3) = Deg2Rad(-130);
    rLegJMin(4) = Deg2Rad(-45);
    rLegJMin(5) = Deg2Rad(-25);

    KDL::JntArray rLegJMax = KDL::JntArray(nj2);

    rLegJMax(0) = Deg2Rad(1);
    rLegJMax(1) = Deg2Rad(25);
    rLegJMax(2) = Deg2Rad(100);
    rLegJMax(3) = Deg2Rad(1);
    rLegJMax(4) = Deg2Rad(75);
    rLegJMax(5) = Deg2Rad(45);

    KDL::ChainFkSolverPos_recursive lLegFKSolver = KDL::ChainFkSolverPos_recursive(lLeg);
    KDL::ChainIkSolverVel_wdls lLegIkSolverVel(lLeg, 0.01, 100); //Inverse velocity solver, from Cartesian to joint space
    KDL::ChainIkSolverPos_NR_JL lLegIksolverPosJ(lLeg, lLegJMin, lLegJMax, lLegFKSolver, lLegIkSolverVel, 100, 1e-3); //Maximum 100 iterations, stop at accuracy 1e-6

    KDL::ChainFkSolverPos_recursive rLegFKSolver = KDL::ChainFkSolverPos_recursive(lLeg);
    KDL::ChainIkSolverVel_wdls rLegIkSolverVel(lLeg, 0.01, 100); //Inverse velocity solver, from Cartesian to joint space
    KDL::ChainIkSolverPos_NR_JL rLegIksolverPosJ(lLeg, rLegJMin, rLegJMax, rLegFKSolver, rLegIkSolverVel, 100, 1e-3); //Maximum 100 iterations, stop at accuracy 1e-6

    //    int initialJ = 5;

    jointpositionsl(0) = Deg2Rad(0);
    jointpositionsl(1) = Deg2Rad(0);
    jointpositionsl(2) = Deg2Rad(0);
    jointpositionsl(3) = Deg2Rad(0);
    jointpositionsl(4) = Deg2Rad(0);
    jointpositionsl(5) = Deg2Rad(0);

    jointpositionsr(5) = Deg2Rad(0);
    jointpositionsr(4) = Deg2Rad(0);
    jointpositionsr(3) = Deg2Rad(0);
    jointpositionsr(2) = Deg2Rad(0);
    jointpositionsr(1) = Deg2Rad(0);
    jointpositionsr(0) = Deg2Rad(0);

    static KDL::Frame rHipStanding;
    static KDL::Frame lAnkleStanding;

    if (t == t4) {
        lLegFKSolver.JntToCart(jointpositionsl, lAnkleStanding);
        rLegFKSolver.JntToCart(jointpositionsr, rHipStanding);
    }

    Eigen::Vector3f ball = WM->getBallPos();
    //    ball.z() = 0.04*(1-0.707106781);

    //    cout << "Ball Pos: " << ball << endl;

    stringstream ss("");

    double lj3 = WM->getJointAngle("lle3");
    double rj2 = WM->getJointAngle("rle2");
    double rj3 = WM->getJointAngle("rle3");
    double rj4 = WM->getJointAngle("rle4");
    double rj5 = WM->getJointAngle("rle5");

    if (t > 0 && t <= t1) {
        return moveJoints(jointpositionsl, jointpositionsr, 0.2);
    } else if (t > t1 && t <= t2) {
        ss << moveJoint("lle2", -1.2) << moveJoint("rle2", -1.2) << moveJoint("lle6", 1.2) << moveJoint("rle6", 1.2);
        return ss.str();
    } else if (t > t2 && t <= t3) {
        ss << moveJoint("rle3", 2.5) << moveJoint("rle4", -5) << moveJoint("rle5", 4.25) << moveJoint("rle2", -0.4);
        return ss.str();
    } else if (t > t3 && t <= t4) {
        double r3 = (-25 - rj3) / double(t4 + 1 - t);
        double r4 = (-130 - rj4) / double(t4 + 1 - t);
        double l3 = (30 - lj3) / double(t4 + 1 - t);
        double r5 = (75 - rj5) / double(t4 + 1 - t);
        //        cout<<"Amin: t3->"<<r3<<" "<<r4<<" "<<r5<<" "<<l3<<endl;
        ss << moveJoint("rle3", r3) << moveJoint("rle4", r4) << moveJoint("rle5", r5) << moveJoint("lle3", l3);
        return ss.str();
    } else if (t > t4 && t <= t5) {

        cout << ball << endl;
        cout << rHipStanding.p << endl;
        static KDL::JntArray ballJoints;
        //        if (t <= t4 + (t5-t4)/2) {
        double r3 = (100 - rj3) / (t5 - t);
        double r4 = (0 - rj4) / (t5 - t);
        double l3 = 4 * (-25 - lj3) / (t5 - t);
        double r5 = 1.5 * (-45 - rj5) / (t5 - t);
        double r2 = (35 - rj2) / (t5 - t);
        if (1 || t <= t4 + 2 * (t5 - t4) / 5) {
            ss << moveJoint("rle3", r3) << moveJoint("rle4", r4) << moveJoint("rle5", r5) << moveJoint("lle3", l3);

        } else {
            ss << moveJoint("rle3", r3) << moveJoint("rle4", r4) << moveJoint("rle5", r5) << moveJoint("lle3", l3) << moveJoint("rle2", r2);
        }
        return ss.str();
        //        } else if (t == t4 + (t5-t4)/2 + 1) {
        jointpositionsr(0) = Deg2Rad(WM->getJointAngle("rle1"));
        jointpositionsr(1) = Deg2Rad(WM->getJointAngle("rle2"));
        jointpositionsr(2) = Deg2Rad(WM->getJointAngle("rle3"));
        jointpositionsr(3) = Deg2Rad(WM->getJointAngle("rle4"));
        jointpositionsr(4) = Deg2Rad(WM->getJointAngle("rle5"));
        jointpositionsr(5) = Deg2Rad(WM->getJointAngle("rle6"));

        ballJoints = jointpositionsr;
        rLegFKSolver.JntToCart(ballJoints, rHipStanding);
        //        }

        //        jointpositionsl(0) = Deg2Rad(WM->getJointAngle("lle1")) ;
        //        jointpositionsl(1) = Deg2Rad(WM->getJointAngle("lle2")) ;
        //        jointpositionsl(2) = Deg2Rad(WM->getJointAngle("lle3")) ;
        //        jointpositionsl(3) = Deg2Rad(WM->getJointAngle("lle4")) ;
        //        jointpositionsl(4) = Deg2Rad(WM->getJointAngle("lle5")) ;
        //        jointpositionsl(5) = Deg2Rad(WM->getJointAngle("lle6")) ;

        //        jointpositionsr(0) = Deg2Rad(WM->getJointAngle("rle1")) ;
        //        jointpositionsr(1) = Deg2Rad(WM->getJointAngle("rle2")) ;
        //        jointpositionsr(2) = Deg2Rad(WM->getJointAngle("rle3")) ;
        //        jointpositionsr(3) = Deg2Rad(WM->getJointAngle("rle4")) ;
        //        jointpositionsr(4) = Deg2Rad(WM->getJointAngle("rle5")) ;
        //        jointpositionsr(5) = Deg2Rad(WM->getJointAngle("rle6")) ;

        //        lLegFKSolver.JntToCart( jointpositionsl ,lAnkleStanding );
        //        rLegFKSolver.JntToCart( jointpositionsr , rHipStanding );
        //        double ttt = double(t-1-t4)/double(t5-1-t4);
        //        cout << "pos.push_back(Eigen::Vector3f(" << rHipStanding.p.x() << "," << rHipStanding.p.y() << "," << rHipStanding.p.z() << "));" << endl;
        //        cout << lAnkleStanding.p.x() << "," << lAnkleStanding.p.y() << "," << lAnkleStanding.p.z() << ";" << endl;
        //        double roll, pitch, yaw;
        //        rHipStanding.M.GetRPY(roll,pitch,yaw);
        //        cout << "trn.push_back(Eigen::Vector3f("  << roll <<  "," << pitch << "," << yaw <<"));"<<  endl;

        return ss.str();

        static KDL::JntArray JLLeg(lLeg.getNrOfSegments());
        //        static KDL::JntArray JLLegInit(lLeg.getNrOfSegments());

        static KDL::JntArray JRLeg(rLeg.getNrOfSegments());
        static KDL::JntArray JRLegInit(rLeg.getNrOfSegments());

        //        if(t == t4+1) {
        //            jointpositionsr(0) = Deg2Rad(0.01) ;
        //            jointpositionsr(1) = Deg2Rad(-26.72) ;
        //            jointpositionsr(2) = Deg2Rad(-25) ;
        //            jointpositionsr(3) = Deg2Rad(-130) ;
        //            jointpositionsr(4) = Deg2Rad(75) ;
        //            jointpositionsr(5) = Deg2Rad(17.95) ;
        //            JRLegInit = jointpositionsr;
        //        }

        double x1, x2, y1, y2, z1, z21, z22, x21, x22;
        //        KDL::Rotation lLegRot ;
        KDL::Rotation rHipRot;

        double tt = double(t - 1 - t4) / double(t5 - 1 - t4);
        //        y2 = (-0.847293*tt*tt*tt*tt)+(0.538521*tt*tt*tt)+(0.864160*tt*tt)+(-0.262945*tt)+(-0.081479);

        //        y2 = max(y2, -0.095);
        //        y2 = min (y2, 0.218345) ;

        //        z21 =  0.13558795835531871672647968550575*y2 - 0.63880048599940974835093654539252*sqrt(- 1.762880489556*y2*y2 + 0.216396505228*y2 + 0.037540757073) - 0.03433105451906627810562438275903;
        //        z22 =  0.13558795835531871672647968550575*y2 + 0.63880048599940974835093654539252*sqrt(- 1.762880489556*y2*y2 + 0.216396505228*y2 + 0.037540757073) - 0.03433105451906627810562438275903;

        //        x21 = 0.51219222369278300669127921032252*sqrt(- 0.628524467312*y2*y2 + 0.077934318568*y2 + 0.013290952817) - 0.06868497719720220119730054210425*y2 + 0.017288024126302504824850747186016;
        //        x22 = 0.017288024126302504824850747186016 - 0.51219222369278300669127921032252*sqrt(- 0.628524467312*y2*y2 + 0.077934318568*y2 + 0.013290952817) - 0.06868497719720220119730054210425*y2;

        //        cout << rHipStanding.p << endl;
        //        Eigen::Vector3f start =  Eigen::Vector3f(-1.25496,-0.389516 ,-0.333869);
        //        Eigen::Vector3f end =  Eigen::Vector3f(0.864065,-0.273051,0.265589);
        //        Eigen::Vector3f tr = start*(1-tt) + end*tt;
        //        Eigen::Vector3f tr = Eigen::Vector3f((0.350110*tt*tt*tt)+(0.466737*tt*tt)+(1.510568*tt)+(-1.477457), (-0.898292*tt*tt*tt)+(0.714302*tt*tt)+(0.371692*tt)+(-0.455817), (-0.215220*tt*tt*tt)+(0.908404*tt*tt)+(-0.075519*tt)+(-0.343416));
        //        Eigen::Vector3f tr = trn[(int)(tt*30)];
        //        cout << tr << endl;
        //        cout << "RPY: "  << roll <<  " " << pitch << " " << yaw << endl;
        //        cout << "MIN: " << tr << endl;
        //        return ss.str();
        //        rHipRot =  rHipRot.RPY(tr.x(), tr.y(), tr.z());

        //        z2 = (rHipStanding.p.z())*(t-t2)/(double)(t5-t4) ;
        //        y2 = (rHipStanding.p.y())*(t-t2)/(double)(t5-t4) ;
        //        x2 = (rHipStanding.p.x())*(t-t2)/(double)(t5-t4) ;

        /*    if (t > t1 && t <= t2) {
        //        cout << t1 << endl;
                static double bb = 1;

                double theta = Deg2Rad(5);


            //    bb+=0.008;
            //    if ( bb > 1.8 )
            //        bb=1.2;

                int Period = 30 ;
                static double T = Period * 0.02;
                static double w = (2*M_PI)/T;
                double a = 0.05*bb  , b = -0.018*bb ;
                double a1 = a, b1 = b;

                x1 = -a*cos(w*(t-t1)*0.02)*sin(theta) ; y1= a*cos(w*(t-t1)*0.02)*cos(theta) ; z1 =b*sin(w*(t-t1)*0.02)  ;
                x2 = -a1*cos(w*(t-t1)*0.02)*sin(theta) ; y2=a1*cos(w*(t-t1)*0.02)*cos(theta) ; z2 = b1*sin(w*(t-t1)*0.02) ;


                lLegRot ;
                rHipRot = KDL::Rotation::RotZ(0);

                int AllTime = Period / 2;

                if ( (t-t1) % (2*AllTime) < AllTime ) {
                    lLegRot = KDL::Rotation::RotZ(((t-t1)%(2*AllTime))*theta/(double)AllTime);
                } else {
                    lLegRot = KDL::Rotation::RotZ(((2*AllTime-((t-t1)%(2*AllTime)))*theta)/(double)AllTime);
                }


                if (t == t2) {
                    jointpositionsl(0) = Deg2Rad(WM->getJointAngle("lle1")) ;
                    jointpositionsl(1) = Deg2Rad(WM->getJointAngle("lle2")) ;
                    jointpositionsl(2) = Deg2Rad(WM->getJointAngle("lle3")) ;
                    jointpositionsl(3) = Deg2Rad(WM->getJointAngle("lle4")) ;
                    jointpositionsl(4) = Deg2Rad(WM->getJointAngle("lle5")) ;
                    jointpositionsl(5) = Deg2Rad(WM->getJointAngle("lle6")) ;

                    jointpositionsr(5) = Deg2Rad(WM->getJointAngle("rle1")) ;
                    jointpositionsr(4) = Deg2Rad(WM->getJointAngle("rle2")) ;
                    jointpositionsr(3) = Deg2Rad(WM->getJointAngle("rle3")) ;
                    jointpositionsr(2) = Deg2Rad(WM->getJointAngle("rle4")) ;
                    jointpositionsr(1) = Deg2Rad(WM->getJointAngle("rle5")) ;
                    jointpositionsr(0) = Deg2Rad(WM->getJointAngle("rle6")) ;


                    lLegFKSolver.JntToCart( jointpositionsl ,lAnkleStanding );
                    rLegFKSolver.JntToCart( jointpositionsr , rHipStanding );

                    cout << rHipStanding.p << endl;
                }

            } else if (t > t2 && t <= t3) {
        //        stringstream ss("");
        //        double lj3 = WM->getJointAngle ( "lle3" ) ;
        //        double rj3 = WM->getJointAngle ( "rle3" ) ;
        //        double rj4 = WM->getJointAngle ( "rle4" ) ;
        //        double rj5 = WM->getJointAngle ( "rle5" ) ;

                double r3 = (-25-rj3)/double(t3-t);
                double r4 = (-130-rj4)/double(t3-t);
                double l3 = (25-lj3)/double(t3-t);
                double r5 = (-25-rj5)/double(t3-t);
        //                if (t < t2 + (t3-t2)/4)
        //                    ss<<moveJoint("rle2",-2.5)<<moveJoint("lle2",-2.5)<<moveJoint("rle6",2.5)<<moveJoint("lle6",2.5);
        //                else
                            ss<<moveJoint("rle3",r3)<<moveJoint("rle4",r4)<<moveJoint("rle5",r5)<<moveJoint("lle3" , l3);
                        return ss.str();
        //        static double bb = 1.6 ;

        //        double theta = Deg2Rad(5);

        //        int Period = 60 ;
        //        static double T = Period * 0.02;
        //        static double w = (2*M_PI)/T;
        //        double a = 0.05*bb  , b = -0.015*bb ;
        //        double a1 = a, b1 = b;

        //        double lanky = lAnkleStanding.p.y();
        //        double lankz = lAnkleStanding.p.z();

        //        double myTan = atan2(lankz,lanky);
        //        double rad = sqrt(lankz*lankz + lanky*lanky)/2;
        //        y2 = -0.11*double(t-t2)/(t3-t2) + 0.06*(1-double(t-t2)/(t3-t2))-rHipStanding.p.y();
        //        z2 = -10.4*y2*y2*y2 -7.518*y2*y2 + 0.3525*y2 + 0.1676-rHipStanding.p.z();
        //        x2 = 0;
        ////        x2 = (lAnkleStanding.p.x())*(t-t2)/(double)(t3-t2) ;
        ////        y2=rad*cos(M_PI*(1-double(t-t2)/(t3-t2))+myTan)+lanky/2  ; z2 =rad*sin(M_PI*(1-double(t-t2)/(t3-t2))+myTan)+lankz/2  ;
        //        x1 = -a*cos(w*t*0.02)*sin(theta) ; y1= a*cos(w*t*0.02)*cos(theta) ; z1 =b*sin(w*t*0.02)  ;

        //        cout << "x: "<< x2 <<  " y: " << y2 << " z: " << z2 << endl;
        ////        cout << x1 << " " << y1 << " " << z1 << endl;
        ////        lLegRot ;
        ////        rHipRot = KDL::Rotation::RotZ(0);
        ////        int AllTime = Period / 2;

        ////        if ( t % (2*AllTime) < AllTime )
        ////        {
        ////             lLegRot = KDL::Rotation::RotZ(
        ////                         (t%(2*AllTime))*theta/(double)AllTime
        ////                         );
        ////        } else
        ////        {
        ////            lLegRot = KDL::Rotation::RotZ(
        ////                        ((2*AllTime-(t%(2*AllTime)))*theta)/(double)AllTime
        ////                        );
        ////        }


            } else if (t > t3 && t <= t4) {
                return "";
            } else if (t > t4) {
                done = true;
                t = 0;
                return "";
            }*/

        //        cout << "Ans:  " <<rHipStanding.p<< endl;
        //        cout << "Ans1: " << x2 << " " << y2 << " " << z22<< endl;
        //        cout << "Ans2: " << x2 << " " << y2 << " " << z21 << endl;

        //        return ss.str();
        //        KDL::Frame lfrm (  lLegRot , KDL::Vector( lAnkleStanding.p.x() ,lAnkleStanding.p.y(),lAnkleStanding.p.z())  );

        //        cout << "[" << x21 << "," << x22 << "]" << "," << y2 << "[" <<  z21 << "," << z22 << "]" << endl;
        //        Eigen::Vector3f p = pos[(int)(tt*30)];
        //        cout << p << endl;
        //        KDL::Frame rfrm1( rHipRot, KDL::Vector( x21 ,y2,z21 ));
        //        KDL::Frame rfrm1( rHipRot, KDL::Vector(p.x(),p.y(),p.z()));
        //        int solvedr = rLegIksolverPosJ.CartToJnt(JRLegInit , rfrm1 , JRLeg ) ;

        //        if (solvedr != 0) {
        //            cout << "First eq not solved" << endl;
        //            KDL::Frame rfrm2 ( rHipRot, KDL::Vector( x2 ,y2,z21));
        //            int solvedr2 = rLegIksolverPosJ.CartToJnt(JRLegInit , rfrm2 , JRLeg ) ;
        //            if (solvedr2 != 0) {
        //                cout << "Second eq not solved" << endl;
        //                return "";
        //            }
        //            return "";
        //        }

        JRLegInit = JRLeg;

        KDL::JntArray serversendR(nj2);

        serversendR(5) = JRLeg(5);
        serversendR(4) = JRLeg(4);
        serversendR(3) = JRLeg(3);
        serversendR(2) = JRLeg(2) + Deg2Rad(5);
        serversendR(1) = JRLeg(1);
        serversendR(0) = JRLeg(0);

        //        JLLeg(2) = JLLeg(2) + Deg2Rad(5);

        return moveJoints(JLLeg, serversendR, 0.7);

    } else if (t > t5) {
        done = true;
        t = 0;
    }
    return "";
}

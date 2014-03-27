#include "Skill.h"

string Skill::SideWalkLib(int &t, SideT side) {
  t++;

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

  static double bb = 0.1;

  newSpeedController(bb, t, SW);

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

  static double T = 10 * 0.02;
  static double w = (2 * M_PI) / T;
  double a = 0.05 * bb, b = -0.015 * bb;
  double a1 = a, b1 = b;
  double x1 = a * cos(w * t * 0.02), y1 = b * sin(w * t * 0.02);
  double x2 = a1 * cos(w * t * 0.02 + M_PI), y2 = b1 * sin(w * t * 0.02 + M_PI);

  lLegFKSolver->JntToCart(jointpositionsl, lAnkleStanding);

  KDL::Frame lfrm(KDL::Vector(lAnkleStanding.p.x() + x1, lAnkleStanding.p.y(),
                              lAnkleStanding.p.z() + y1));

  rLegFKSolver->JntToCart(jointpositionsr, rHipStanding);
  KDL::Frame rfrm(KDL::Vector(rHipStanding.p.x() + x2, rHipStanding.p.y(),
                              rHipStanding.p.z() + y2));

  int solvedl = lLegIksolverPosJ->CartToJnt(JLLegInit, lfrm, JLLeg);
  int solvedr = rLegIksolverPosJ->CartToJnt(JRLegInit, rfrm, JRLeg);

  JLLegInit = JLLeg;
  JRLegInit = JRLeg;

  if (solvedl != 0 || solvedr != 0)
    return "";

  return moveJoints(JLLeg, JRLeg, 0.2);
  return "";
}

//////////////////

string Skill::WalkLib(int &t, SideT side) {
  t++;

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
  static double bb = 0.1;

  newSpeedController(bb, t, W);

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

  static double T = 10 * 0.02;
  static double w = (2 * M_PI) / T;
  double a = 0.05 * bb, b = -0.015 * bb;
  double a1 = a, b1 = b;
  double x1 = a * cos(w * t * 0.02), y1 = b * sin(w * t * 0.02);
  double x2 = a1 * cos(w * t * 0.02 + M_PI), y2 = b1 * sin(M_PI + w * t * 0.02);

  lLegFKSolver->JntToCart(jointpositionsl, lAnkleStanding);

  KDL::Frame lfrm(KDL::Vector(lAnkleStanding.p.x(), lAnkleStanding.p.y() + x1,
                              lAnkleStanding.p.z() + y1));

  rLegFKSolver->JntToCart(jointpositionsr, rHipStanding);
  KDL::Frame rfrm(KDL::Vector(rHipStanding.p.x(), rHipStanding.p.y() + x2,
                              rHipStanding.p.z() + y2));

  int solvedl = lLegIksolverPosJ->CartToJnt(JLLegInit, lfrm, JLLeg);
  JLLegInit = JLLeg;

  int solvedr = rLegIksolverPosJ->CartToJnt(JRLegInit, rfrm, JRLeg);
  JRLegInit = JRLeg;

  if (solvedl != 0 || solvedr != 0)
    return "";

  return moveJoints(JLLeg, JRLeg, 0.2);
}

//////////////////////////////

string Skill::SideTurn(int &t, SideT side) {
  t++;

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
  static double bb = 0.1;

  newSpeedController(bb, t, ST);

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

  double theta = Deg2Rad(10);

  int Period = 10;
  static double T = Period * 0.02;
  static double w = (2 * M_PI) / T;
  double a = 0.05 * bb, b = -0.015 * bb;
  double a1 = a, b1 = b;
  double x1 = a * cos(w * t * 0.02) * cos(theta),
         y1 = a * cos(w * t * 0.02) * sin(theta), z1 = b * sin(w * t * 0.02);
  double x2 = a1 * cos(w * t * 0.02 + M_PI) * cos(theta),
         y2 = a1 * cos(w * t * 0.02 + M_PI) * sin(theta),
         z2 = b1 * sin(w * t * 0.02 + M_PI);

  if (side == Left) {
    x1 *= -1;
    x2 *= -1;
  }

  KDL::Rotation lLegRot;
  KDL::Rotation rHipRot = KDL::Rotation::RotZ(0);

  int AllTime = Period / 2;

  if (side == Right) {
    if (t % (2 * AllTime) < AllTime) {
      lLegRot = KDL::Rotation::RotZ(
          ((2 * AllTime - (t % (2 * AllTime))) * theta) / (double)AllTime);
    } else {
      lLegRot =
          KDL::Rotation::RotZ((t % (2 * AllTime)) * theta / (double)AllTime);
    }
  } else {
    if (t % (2 * AllTime) < AllTime) {
      lLegRot =
          KDL::Rotation::RotZ((t % (2 * AllTime)) * theta / (double)AllTime);

    } else {

      lLegRot = KDL::Rotation::RotZ(
          ((2 * AllTime - (t % (2 * AllTime))) * theta) / (double)AllTime);
    }
  }

  lLegFKSolver->JntToCart(jointpositionsl, lAnkleStanding);

  KDL::Frame lfrm(lLegRot, KDL::Vector(lAnkleStanding.p.x() + x1,
                                       lAnkleStanding.p.y() + y1,
                                       lAnkleStanding.p.z() + z1));

  rLegFKSolver->JntToCart(jointpositionsr, rHipStanding);
  KDL::Frame rfrm(rHipRot,
                  KDL::Vector(rHipStanding.p.x() + x2, rHipStanding.p.y() + y2,
                              rHipStanding.p.z() + z2));

  int solvedl = lLegIksolverPosJ->CartToJnt(JLLegInit, lfrm, JLLeg);
  JLLegInit = JLLeg;

  int solvedr = rLegIksolverPosJ->CartToJnt(JRLegInit, rfrm, JRLeg);
  JRLegInit = JRLeg;

  if (solvedl != 0 || solvedr != 0)
    return "";
  return moveJoints(JLLeg, JRLeg, 0.2);
}

//////////////////////////////

string Skill::WalkAngleLib(int &t, SideT side) {

  t++;

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

  static double bb = 0.1;

  newSpeedController(bb, t, WA);

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

  double theta = Deg2Rad(30);

  int Period = 10;
  static double T = Period * 0.02;
  static double w = (2 * M_PI) / T;
  double a = 0.05 * bb, b = -0.015 * bb;
  double a1 = a, b1 = b;
  double x1 = -a * cos(w * t * 0.02) * sin(theta),
         y1 = a * cos(w * t * 0.02) * cos(theta), z1 = b * sin(w * t * 0.02);
  double x2 = -a1 * cos(w * t * 0.02 + M_PI) * sin(theta),
         y2 = a1 * cos(w * t * 0.02 + M_PI) * cos(theta),
         z2 = b1 * sin(w * t * 0.02 + M_PI);

  KDL::Rotation lLegRot;
  KDL::Rotation rHipRot = KDL::Rotation::RotZ(0);

  int AllTime = Period / 2;

  if (t % (2 * AllTime) < AllTime) {
    lLegRot =
        KDL::Rotation::RotZ((t % (2 * AllTime)) * theta / (double)AllTime);
  } else {
    lLegRot = KDL::Rotation::RotZ(
        ((2 * AllTime - (t % (2 * AllTime))) * theta) / (double)AllTime);
  }

  lLegFKSolver->JntToCart(jointpositionsl, lAnkleStanding);

  KDL::Frame lfrm(lLegRot, KDL::Vector(lAnkleStanding.p.x() + x1,
                                       lAnkleStanding.p.y() + y1,
                                       lAnkleStanding.p.z() + z1));

  rLegFKSolver->JntToCart(jointpositionsr, rHipStanding);
  KDL::Frame rfrm(rHipRot,
                  KDL::Vector(rHipStanding.p.x() + x2, rHipStanding.p.y() + y2,
                              rHipStanding.p.z() + z2));

  int solvedl = lLegIksolverPosJ->CartToJnt(JLLegInit, lfrm, JLLeg);
  JLLegInit = JLLeg;

  int solvedr = rLegIksolverPosJ->CartToJnt(JRLegInit, rfrm, JRLeg);
  JRLegInit = JRLeg;

  if (solvedl != 0 || solvedr != 0)
    return "";
  if (side == Right)
    return moveJoints(JLLeg, JRLeg, 0.2);
  else if (side == Left)
    return moveJoints(JRLeg, JLLeg, 0.2);
}
//////////////////////////////

string Skill::TurnLib(int &t, SideT side) {
  t++;

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

  static double bb = 0.1;

  newSpeedController(bb, t, TU);

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
    lLegRot =
        KDL::Rotation::RotZ((t % (2 * AllTime)) * theta / (double)AllTime);
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

  if (solvedl != 0 || solvedr != 0)
    return "";
  if (side == Right)
    return moveJoints(JLLeg, JRLeg, 0.2);
  else if (side == Left)
    return moveJoints(JRLeg, JLLeg, 0.2);
}

string Skill::GeneralWalk(int &t, double angle, bool &onePeriodFinished) {
  t++;

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

  static double bb = 0.1;

  newSpeedController(bb, t, WA);

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

  double theta = Deg2Rad(fabs(angle));

  int Period = 10;
  static double T = Period * 0.02;
  static double w = (2 * M_PI) / T;
  double a = 0.05 * bb, b = -0.015 * bb;
  double a1 = a, b1 = b;
  double x1 = -a * cos(w * t * 0.02) * sin(theta),
         y1 = a * cos(w * t * 0.02) * cos(theta), z1 = b * sin(w * t * 0.02);
  double x2 = -a1 * cos(w * t * 0.02 + M_PI) * sin(theta),
         y2 = a1 * cos(w * t * 0.02 + M_PI) * cos(theta),
         z2 = b1 * sin(w * t * 0.02 + M_PI);

  KDL::Rotation lLegRot;
  KDL::Rotation rHipRot = KDL::Rotation::RotZ(0);

  int AllTime = Period / 2;

  if (t % (2 * AllTime) < AllTime) {
    lLegRot =
        KDL::Rotation::RotZ((t % (2 * AllTime)) * theta / (double)AllTime);
  } else {
    lLegRot = KDL::Rotation::RotZ(
        ((2 * AllTime - (t % (2 * AllTime))) * theta) / (double)AllTime);
  }

  lLegFKSolver->JntToCart(jointpositionsl, lAnkleStanding);

  KDL::Frame lfrm(lLegRot, KDL::Vector(lAnkleStanding.p.x() + x1,
                                       lAnkleStanding.p.y() + y1,
                                       lAnkleStanding.p.z() + z1));

  rLegFKSolver->JntToCart(jointpositionsr, rHipStanding);
  KDL::Frame rfrm(rHipRot,
                  KDL::Vector(rHipStanding.p.x() + x2, rHipStanding.p.y() + y2,
                              rHipStanding.p.z() + z2));

  int solvedl = lLegIksolverPosJ->CartToJnt(JLLegInit, lfrm, JLLeg);
  JLLegInit = JLLeg;

  int solvedr = rLegIksolverPosJ->CartToJnt(JRLegInit, rfrm, JRLeg);
  JRLegInit = JRLeg;

  if ( t % Period == Period-1 )
  {
      onePeriodFinished = true;
  }
  else {
      onePeriodFinished = false ;
  }
  if (solvedl != 0 || solvedr != 0)
    return "";
  if (angle < 0 )
    return moveJoints(JLLeg, JRLeg, 0.2);
  else
    return moveJoints(JRLeg, JLLeg, 0.2);
}

//////////////////////////////

string Skill::moveJoints(KDL::JntArray left, KDL::JntArray right, double p) {
  KDL::JntArray curLeft(6), curRight(6);
  KDL::JntArray diffLeft(6), diffRight(6);

  string s1, s2;
  stringstream ss;

  string sc;

  for (int i = 0; i < 6; ++i) {
    string rightStr = WM->num2Str(2 + 2 * i + 1) ;
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

void Skill::newSpeedController(double &bb, int t, Command comm) {
  VecPosition ball(WM->getBallPos().x(), WM->getBallPos().y());
  VecPosition me(WM->getMyPos().x(), WM->getMyPos().y());

  if (comm == W || comm == WA) {
    bb = 0.1 + (t - 1) * 0.015;
    bb = min(bb, 1.2);
    if (me.getDistanceTo(ball) < 1) {
      bb = min(bb, 0.8);
    }
  } else if (comm == TU) {
    bb = 0.4 + (t - 1) * 0.02;
    bb = min(bb, 1);
  } else if (comm == ST) {
    bb = 0.2 + (t - 1) * 0.04;
    bb = min(bb, 1);
  } else {
    bb = 0.01 + (t - 1) * 0.02;
    bb = min(bb, 1);
  }
}

///////////////////////////////////////

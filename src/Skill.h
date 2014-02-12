#ifndef SKILL
#define SKILL
#include "WorldModel.h"
class Skill
{
private :
  WorldModel *WM;
  ifstream f;
  string jointName[22];
  int x,y;
  double standupB[24][24];
  double standupF[24][24];
  double diveR[24][24];
  double diveL[24][24];
  double diveC[24][24];
  double DPR[24][24];
  double DPL[24][24];
  double zero[24][24];
  double zeroe[22][24];
  double swL[6][24];
  double standing[24][24];
  double standingK[24][24];
  double standing2[24][24];
  double standingB[24][24];
  double Turn20[24][24];
  double Turn20R[24][24];
  double  Walker[35][12];
  double Turn30[24][24];
  double Turn30R[24][24];
  double Turn40[24][24];
  double Turn40R[24][24];
  double Turn50[24][24];
  double Turn50R[24][24];
  double Turn60[24][24];
  double Turn60R[24][24];
  double Turn10[12][24];
  double Turn10R[24][24];
public :
  Skill(WorldModel*);
  string beam ( double x , double y , double ang);
  string moveJoint(string,double);
  string moveJointTo(string,double,double);
  string act(double a[][24],int m,int n,double &time,bool &);
  string fix();
  string feh(bool =false,double =2.5);
  string set(string j,double ang,int cycle);
  string fix(string j);
  string turn(SideT ,bool &);
  string DP(SideT s,bool &done,double &t);
  string standUp(SideT ,bool &,double &);
  string dive(SideT ,bool &,double &);
  string walk(bool &,bool=false,double k=0.4444);
  string walk1(bool &,bool=false);
  string backWalk(bool &,bool=false);
  string sefr(bool &,double &,bool=false );
  string turning(bool &,double );
  string Standing(bool &done, double &t);
  string Standing2(bool &done, double &t);
  string StandingBack(bool &done, double &t);
  string sideSWalkR2(bool &done);
  string sideSWalkL2(bool &done);
  string whereBeam(bool &stand,double &stan);
  string sayBallPos ();
  string updatem ( bool checkBall=false ) ;
  string StandUpFront ( bool &done );
  string act(double a[][24],double &time);
  void   fill1(double action[24][24],string fileName);
  string StandUpBack ( bool &done );
  string ToRightSide(int i,bool &done);
  string ToLeftSide(int i,bool &done);

  /// Inverse Knimatics
  VecPosition FK_FOOT_HIP (double le4 , double le5);
  bool   IK_ALL ( VecPosition Hip , VecPosition RAnkle , double &rle3 , double &rle4 , double &rle5 , double &lle3 , double &lle4 , double &lle5 , double P=5);
  string shootR (bool&,bool&,double&);
  string shootL (bool&,bool&,double&);
  bool   isPossible ( double);
  string Walking ( bool set = false);
  string finalAction(string,double&,double maxV=1.2);
  void   speedControler(string,double,double&,double&,double&,double);
  bool   nazdik(string);
  void getAnklePos (   double P0[3]
                  , double A1 = 0 , double A2 = 0 , double A3 = 0 , double A4 = 0 );
};
#endif

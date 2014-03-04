#ifndef DECIDE
#define DECIDE
#include "WorldModel.h"
#include "Skill.h"
#include "Act.h"
class Decide
{
private:
  WorldModel *WM;
  Skill *SK;
  Vector3f myPos, ballPos;
  VecPosition center;
  Act *ACT;
  bool beamed;
  Logger Log ;
    ofstream fout;
    ofstream out;
public:
  Decide(WorldModel*);
  ~Decide();
  string decide();
  string goalie();
  string Attack();
  string Deffend();
  string Middle();
  bool canShoot(double &dx, double &dy,string &side);
  bool shouldClear(double&, double&,SideT&,Triangle t);
  bool shouldShoot(double&, double&,SideT&);
  bool canBackShoot(string &);
  bool shouldTurn90(string &);
  bool shouldTurn(double&);
  bool shouldTurnn(double&);
  bool shouldSideWalk(string &);
  bool shouldTurnAroundBall(string &);
  bool shouldDive(string &side);
  bool shouldPlay();
  bool shouldStandUp(Command&, SideT&);
  bool shouldBackWalk(VecPosition);
  bool shouldSideDeffend(string &s);
  bool canBeam();
  void setBeamed();
  bool isBeamed();
  bool shouldSSideWalk(SideT &side);
  bool isBackOfBall(VecPosition);
  bool isInCone(double dWidth, VecPosition start , VecPosition end, VecPosition pos);
  bool objectIsInCone(double dWidth, VecPosition start, VecPosition end,Vector3f &posobj, object ob=OBJECT_BALL);
  int minDistToMeInArray(int [],int,bool);
  string moveToPos(VecPosition,double,bool&);
  VecPosition defendpositioning();
  VecPosition attackpositioning();
  VecPosition middlepositioning();
  double      goaliepositioning();
  string actionOfPositioning(VecPosition pos);
  string goBackOfBall();
  int   parseBallPos ( );
  string moveToPosP(bool,VecPosition,double&);
  string moveToNearBall(double&);
  string moveToPosition(VecPosition,double&);
  bool shSide(string&);
  bool shSide2(string&);
  bool shback();
  bool canshoot2();
  bool isDribble(string&);
  bool shouldPlay2();
  string mvPos(VecPosition,bool,double&,double ang,bool);
  bool startShoot();
};
#endif

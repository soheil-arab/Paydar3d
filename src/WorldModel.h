
#ifndef WORLD_MODEL
#define WORLD_MODEL
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include "Math.h"
#include "Logger.h"
#include "line.h"
using namespace std;
class WorldModel
{
public:
  WorldModel();
  ~WorldModel();

  void kalman_filter();
  Vector3f general_rotation(Vector3f,Vector3f,double);
  void setSpeed(Vector3f);
  Vector3f getSpeed();


  void setFlagPos(string, Polar);
  void setBallPolarPos(Polar);
  void setBallAng(double);
  void setTime(double);
  void setServerTime(double);
  void setGyro(Vector3f);
  void setJointAngle(string, double);
  void setOurPlayerPos(int, Polar);
  void setOppPlayerPos(int, Polar);
  void setOurPos(int, Vector3f);
  void setOppPos(int, Vector3f);
  void setOurName(string);
  void setOppName(string);
  void setTeamSide(string);
  void setPlayMode(PlayMode);
  void setFootPress(string, FootRes);
  void setMyNum(int);
  void setLastMsg(string, double);

  void setMyPos(Vector3f);
  void setBallPos(Vector3f);
  void setMyAngle(double);
  void setMyPhi(double);
  void setBallVel(Vector3f vel);
  void initFlags();
  void initDimentions();
  void Localize();
  void setSense ( bool b );
  void setOurPlayerPartPos ( int num , string part , Polar pos );
  void setOppPlayerPartPos ( int num , string part , Polar pos );
  void setACC ( Vector3f pos );


  double   getMyAngle();
  double   getMyPhi();
  double   getBallAng();
  double   getMyAngleToGoal();
  double   getMyAngleToBall();
  double   getMyAngleTo(Vector3f p);
  double   getTime();
  double   getServerTime();
  double   getJointAngle(string);
  double   getLastJointAngle(string);
  int      getMyNum();
  Polar    getBallPolarPos();
  Vector3f getOurPos(int);
  Vector3f getOurPos(int,string part);
  Vector3f getOppPos(int);
  Vector3f getOppPos(int , string part);
  double   getFlagLastSeen(string name);
  double   getBallLastSeen();
  FootRes  getFootPress(string);
  Vector3f getACC ();
  Vector3f getGyro();
  Vector3f getBallPos();
  Vector3f getMyPos();
  Vector3f getMyPos(string part);
  Vector3f getBallVel();
  Vector3f translate(Vector3f);
  Vector3f translate(Vector3f,Matrix T);
  Vector3f translate(Vector3f,Vector3f);
  int      getNrOfUs();
  int      getNrOfThem();
  bool     getSense();
  bool     isFeltDown();
  bool     isServerBeamed();
  SideT    feltType();
  string   getOurName();
  string   getOppName();
  SideT    getTeamSide();
  PlayMode getPlayMode();
  string   getLastMsg();
  string   num2Str(int);
  int      str2num ( string );
  string   str2str(string);
  int      getClosestsOppToBall();
  int      getClosestOurToBall();
  double   getOurMinDist();
  double   getOppMinDist();
  bool     isLocaled();
  bool     amIClosestToBall();
  bool     shouldDive(SideT &side);
  string   uniquee(string x ) ;
  int      getNrOfFlag();
  bool     seeEnoughFlag();
  bool     seeBall () ;
  bool     isBallUs () ;
  double   ZFromLeft () ;
  double   ZFromRight () ;
  void      setZ(double z);
  int       isOppInCircle ( Circle c,VecPosition&,VecPosition&) ;
  Vector3f  sensedPos;
  double myOrien;
  int getOurPlayerCount() { return our.size()-2; }
  double getFieldLength(){ return length; }
  double getFieldWidth(){ return width; }
  void  brinBeMA();
  void setInitialGyro();
  void RotateHead1 (double val);
  void RotateHead2 (double val);
  void clearBodyRotate ();
  void setSeenLines(line);
  double    serverTime, Time;

private:
  Vector3f  ACC;
  bool      canSeeFlags;
  int       num;
  double    myZ;
  double    ballLastSeen, ballPrevLastSeen;
  double    timeLastHear;
  double    myAngle,myPhi;
  double    ballAng;
  bool      Sense;
  string    side, oppName, ourName;
  PlayMode  playMode ;
  string    lastHear;
  string    names[22];
  Polar     ballPolar;
  Vector3f  gyro;
  Vector3f  ballPos;
  Vector3f  ballVel;
  Vector3f  ball;
  map<string, Polar> flagPolar;
  map<string, Vector3f> flag;
  map<string, double> flagLastSeen;
  map<string, Vector3f> flagGlobal;
  map<string, double> joint;
  map<string, double> lastJoint;
  map<int, Polar> mates, opps;
  map<string, FootRes> footPress;
  Matrix    R;
  double    width, length, goalWidth;
  map<int, RelPlayerInfo> ourRel;
  map<int, RelPlayerInfo> theirRel;
  map<int, GloPlayerInfo> our;
  map<int, GloPlayerInfo> their;
  bool      Localed;
  double    rfTime  ;
  double    lfTime  ;
  Matrix bodyRotate;
  Matrix headRotate;
  Matrix headR ;
  Vector3f speed;

  vector<line> lines_we_see;
  Vector3f localize_with_lines(map<line,double> ,map<string,Polar> );





};
#endif

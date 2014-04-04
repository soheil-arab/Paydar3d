
#ifndef WORLD_MODEL
#define WORLD_MODEL
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cmath>
#include "Math.h"
#include "Logger.h"
#include "line.h"
#include <eigen3/Eigen/Core>

using namespace std;
class WorldModel
{
public:
  WorldModel() ;
  ~WorldModel();

  void kalman_filter();
  Eigen::Vector3f general_rotation(Eigen::Vector3f,Eigen::Vector3f,double);
  void setSpeed(Eigen::Vector3f);
  Eigen::Vector3f getSpeed();


  void setFlagPos(string, Polar);
  void setBallPolarPos(Polar);
  void setBallAng(double);
  void setTime(double);
  void setServerTime(double);
  void setGyro(Eigen::Vector3f);
  void setJointAngle(string, double);
  void setOurPlayerPos(int, Polar);
  void setOppPlayerPos(int, Polar);
  void setOurPos(int, Eigen::Vector3f);
  void setOppPos(int, Eigen::Vector3f);
  void setOurName(string);
  void setOppName(string);
  void setTeamSide(SideT);
  void setPlayMode(PlayMode);
  void setFootPress(string, FootRes);
  void setMyNum(int);
  void setLastMsg(string, double);
  unordered_map<string,Eigen::Vector3f> getLastFlagSeen();
  void setGyroPos(Eigen::Vector3f pos);

  void setMyPos(Eigen::Vector3f);
  void setBallPos(Eigen::Vector3f);
  void setMyAngle(double);
  void setMyPhi(double);
  void setBallVel(Eigen::Vector3f vel);
  void initFlags();
  void initDimentions();
  void Localize();
  void setSense ( bool b );
  void setOurPlayerPartPos ( int num , string part , Polar pos );
  void setOppPlayerPartPos ( int num , string part , Polar pos );
  void setACC ( Eigen::Vector3f pos );
  int getClosestTo(int i);
  vector<line> getLastSeenLines();
  unordered_map<string,Eigen::Vector3f> getFlagGlobal();





  double   getMyAngle();
  double   getMyPhi();
  double   getBallAng();
  double   getMyAngleToGoal();
  double   getMyAngleToBall();
  double   getMyAngleTo(Eigen::Vector3f p);
  double   getTime();
  double   getServerTime();
  double   getJointAngle(string);
  double   getLastJointAngle(string);
  int      getMyNum();
  Polar    getBallPolarPos();
  Eigen::Vector3f getOurPos(int);
  Eigen::Vector3f getOurPos(int,string part);
  Eigen::Vector3f getOppPos(int);
  Eigen::Vector3f getOppPos(int , string part);
  double   getFlagLastSeen(string name);
  double   getBallLastSeen();
  FootRes  getFootPress(string);
  Eigen::Vector3f getACC ();
  Eigen::Vector3f getGyro();
  Eigen::Vector3f getBallPos();
  Eigen::Vector3f getMyPos();
  Eigen::Vector3f getMyPos(string part);
  Eigen::Vector3f getBallVel();
  Eigen::Vector3f translate(Eigen::Vector3f);
  Eigen::Vector3f translate(Eigen::Vector3f,Eigen::Matrix3f T);
  Eigen::Vector3f translate(Eigen::Vector3f,Eigen::Vector3f);
  Eigen::Vector3f gyroPos;
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
  void getPosbyTwoFlag();

  vector<line> lines_we_see;

  Eigen::Vector3f  sensedPos;
  double myOrien;
  int getOurPlayerCount() { return our.size()-2; }
  double getFieldLength(){ return length; }
  double getFieldWidth(){ return width; }
  void  brinBeMA();
  void setInitialGyro();
  void clearBodyRotate ();
  void setSeenLines(line);
  double    serverTime, Time;
  unordered_map<string,Eigen::Vector3f> getFlag();
  void resetLastSeenLines();




  //duplicated
  Eigen::Vector3f  ACC;
  Eigen::Vector3f mylastPos;
  bool      canSeeFlags;
  int       num;
  double    myZ;
  double    ballLastSeen, ballPrevLastSeen;
  double    timeLastHear;
  double    myAngle,myPhi;
  double    ballAng;
  bool      Sense;
  SideT side;
  string    oppName, ourName;
  PlayMode  playMode ;
  string    lastHear;
  string    names[22];
  Polar     ballPolar;
  Eigen::Vector3f  gyro;
  Eigen::Vector3f  ballPos;
  Eigen::Vector3f  ballVel;
  Eigen::Vector3f  ball;
  FootRes lf,rf;
  unordered_map<string, Polar> flagPolar;
  unordered_map<string, Eigen::Vector3f> flag;
  unordered_map<string, double> flagLastSeen;
  unordered_map<string, Eigen::Vector3f> flagGlobal;
  unordered_map<string, double> joint;
  unordered_map<string, double> lastJoint;
  unordered_map<int, Polar> mates, opps;

  Eigen::Matrix3f     R;
  double    width, length, goalWidth;
  unordered_map<int, RelPlayerInfo> ourRel;
  unordered_map<int, RelPlayerInfo> theirRel;
  unordered_map<int, GloPlayerInfo> our;
  unordered_map<int, GloPlayerInfo> their;
  bool      Localed;
  Eigen::Matrix3f bodyRotate;
  Eigen::Matrix3f headRotate;
  Eigen::Matrix3f headR ;
  Eigen::Vector3f speed;

    //  Eigen::Vector3f localize_with_lines(unordered_map<line,double> ,unordered_map<string,Polar> );



};
#endif

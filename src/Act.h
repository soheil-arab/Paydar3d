#ifndef ACT_H
#define ACT_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include "Skill.h"

using namespace std;

class Act
{
private:
  WorldModel *WM;
  Skill *ACS;
  Command curr;
  bool done;
  double t;
  bool started;
  double ang;
  double d1;
  SideT s;
public:
  Act(WorldModel *,Skill *);
  void setCurrentAct(Command,SideT=Right,double=0,double =0);
  Command getCurrentAct();
  string doCurrentAct();
  bool isDone();
};
#endif

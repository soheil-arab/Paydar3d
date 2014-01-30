#ifndef Parser_H
#define Parser_H
#include "WorldModel.h"
using namespace std;
class Parser
{
private :
  WorldModel *WM;
  map <string ,PlayMode > pms;
public :
  Parser(WorldModel*);
  void Parse(string &);
};
#endif

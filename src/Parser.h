#ifndef Parser_H
#define Parser_H
#include "WorldModel.h"
#include <mutex>
using namespace std;
class Parser
{
private :
  WorldModel *WM;
  unordered_map <string ,PlayMode > pms;
  mutex *wm_lock;
public :
  Parser(WorldModel*);
  void parseHingeJoint(string &);
  void parseLines(string &);
  void parseFlags(string &);
  void parsePlayers(string &);
  void parseTime(string &);
  void parseGameState(string &);
  void parseGyroAndAccell(string &);
  void parseBall(string &);
  void parseSide(string &);
  void parseFrp(string &);
  void parseHearMessage(string &);
  void parseSense (string &);
  void Parse(string &);
  PlayMode getPlayModeByString(string &str);
};
#endif

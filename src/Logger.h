#ifndef LOGGER
#define LOGGER
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdarg>
#include "Types.h"
using namespace std;
class Logger
{
private :
  vector<int> levels;
  ofstream file;
  LogT type ;
  double cycle ;
public :
  Logger();
  void AddLevel (int level);
  void Log ( int level ,const char *format,...);
  void Info (double c );
  void File (string Path , int num);
  void LogType ( LogT logtype );
};
#endif

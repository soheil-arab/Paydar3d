#include "Logger.h"

Logger::Logger()
{
    type = LOG_STD ;
}

void Logger::AddLevel(int level)
{
    levels.push_back(level);
}
void Logger::Info(double c)
{
    cycle = c;
}
void Logger::Log ( int level , const char *format,...)
{
    bool is=false;
    for (vector<int>::iterator i = levels.begin() ; i != levels.end() ; i++ )
    {
        if ( *i == level )
        {
            is=true;
            break;
        }
    }
    if ( !is )
    {
        return;
    }
    char buffer[256];
    va_list list;
    va_start (list, format);
    vsprintf (buffer,format, list);
    string end (buffer);
    if ( type == LOG_STD )
    {
        cout  << cycle << " :"<< end << endl;
    }
    else if ( !file )
    {
        cerr << "Error ! Log Level " << level  << " :: file not set  " << endl;
    }
    else
    {
        file << cycle << " :"<< end << endl;
    }
    va_end(list);
}

void Logger::File(string Path , int num)
{
    stringstream ss;
    ss << Path << num ;
    file.open(ss.str().c_str(),ios::ate);
}

void Logger::LogType ( LogT logtype )
{
    type = logtype ;
}

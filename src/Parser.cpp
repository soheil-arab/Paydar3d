#include "Parser.h"
#include "rvdraw.h"
#include <thread>

PlayMode Parser::getPlayModeByString(string &str)
{
    if ( str == "BeforeKickOff" )
        return PM_BeforeKickOff;
    if ( str == "PlayOn" )
        return PM_PlayOn;
    if ( str == "KickOff_Left" )
        return PM_KickOff_Left;
    if ( str == "KickOff_Right" )
        return PM_KickOff_Right;
    if ( str == "KickIn_Left" )
        return PM_KickIn_Left;
    if ( str == "KickIn_Right" )
        return PM_KickIn_Right;
    if ( str == "corner_kick_left" )
        return PM_CORNER_KICK_LEFT;
    if ( str == "corner_kick_right" )
        return PM_CORNER_KICK_RIGHT;
    if ( str == "goal_kick_left" )
        return PM_GOAL_KICK_LEFT;
    if ( str == "goal_kick_right" )
        return PM_GOAL_KICK_RIGHT;
    if ( str == "Goal_Left" )
        return PM_Goal_Left;
    if ( str == "Goal_Right" )
        return PM_Goal_Right;
    if ( str == "offside_left" )
        return PM_OFFSIDE_LEFT;
    if ( str == "offside_right" )
        return PM_OFFSIDE_RIGHT;
    if ( str == "GameOver" )
        return PM_GameOver;
    if ( str == "free_kick_left" )
        return PM_FREE_KICK_LEFT;
    if ( str == "free_kick_right" )
        return PM_FREE_KICK_RIGHT;
    return PM_NONE;
}

Parser::Parser(WorldModel *wm)
{
    wm_lock = new mutex();
    WM = wm;
}

void Parser::parseHingeJoint(string &msg)
{
//    cout << msg << endl;
    /*
     * Angle Parse
     */
    string temp;
    int pos = msg.find("HJ");
    for (int i = 0; i < 22; i++)
    {
        if (pos == string::npos)
            break;
        string name;
        double angle;
        stringstream edame(msg.substr(pos));

        edame >> temp >> temp >> name >> temp >> angle;
        if ( name == "llj7" || name == "rlj7")
        {
            pos = msg.find("HJ", pos + 1);
            i--;
            continue;
        }
        //wm_lock->lock();
        WM->setJointAngle(name, angle);
        //wm_lock->unlock();
        pos = msg.find("HJ", pos + 1);
    }
}

void Parser::parseLines(string &msg)
{
    string temp;
    int pos=msg.find("L");
    while(pos !=string::npos)
    {
        stringstream edame(msg.substr(pos));
        double x1,y1,z1,x2,y2,z2;
        edame>>temp>>temp>>x1>>y1>>z1>>temp>>x2>>y2>>z2;
        //wm_lock->lock();
        WM->setSeenLines(line (Vector3f(x1,y1,z1),Vector3f(x2,y2,z2),WM->serverTime));
        //wm_lock->unlock();
        pos=msg.find("L",pos+1);
    }
}

void Parser::parseSide(string &msg)
{
    int pos = msg.find(" left");
    if (pos != string::npos)
    {
        //wm_lock->lock();
        WM->setTeamSide(Left);
        //wm_lock->unlock();
    }

    pos = msg.find(" right");
    if (pos != string::npos)
    {
        //wm_lock->lock();
        WM->setTeamSide(Right);
        //wm_lock->unlock();
    }
}

void Parser::parseFrp(string &msg)
{
    string temp;
    int pos = msg.find("FRP");
    while (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        string name;
        FootRes foot;
        edame >> temp >> temp >> name >> temp
              >> foot.c.x()
              >> foot.c.y()
              >> foot.c.z()
              >> temp
              >> foot.f.x()
              >> foot.f.y()
              >> foot.f.z();
        //wm_lock->lock();
        WM->setFootPress(name, foot);
        //wm_lock->unlock();
        pos = msg.find("FRP",pos+1);
    }


}

void Parser::parseHearMessage(string &msg)
{

    double time;
    string temp;
    int pos = msg.find("hear");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        string msg1;
        edame >> temp >> time >> temp >> msg1 ;

        if (msg1.size() ==3 && msg1[0]==msg1[1] && msg1[1] == msg1[2]){
            msg1=msg1.substr(0,1);
            //wm_lock->lock();
            WM->setLastMsg( msg1 , time );
            //wm_lock->unlock();
        }
        else
        {

            pos = msg.find("hear",pos+1);
            if (pos != string::npos)
            {
                stringstream edame(msg.substr(pos));

                edame >> temp >> time >> temp >> msg1 ;

                if (msg1.size() == 3 && msg1[0]==msg1[1] && msg1[1] == msg1[2]){
                    msg1=msg1.substr(0,1);
                    //wm_lock->lock();
                    WM->setLastMsg( msg1 , time );
                    //wm_lock->unlock();
                }

            }
        }
    }
}

void Parser::parseSense(string &msg)
{
    string temp;
    int pos = msg.find("mypos");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        double x,y,z;
        edame >> temp >> x >> y >> z ;
        //wm_lock->lock();
        WM->setSense(true);
        WM->sensedPos = Vector3f(x,y,z);
        //wm_lock->unlock();
    }

    pos = msg.find("myorien");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        double ang;
        edame >> temp >> ang ;
        //wm_lock->lock();
        WM->setSense(true);
        WM->myOrien = ang;
        //wm_lock->unlock();
    }
}

void Parser::parseFlags(string &msg)
{
    Polar goal;
    Polar flag;

    /*
     * Goal Flags Parse
     */

    string temp;
    int pos = msg.find("G1L");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> goal.dist >> goal.theta >> goal.phi;
        //wm_lock->lock();
        WM->setFlagPos("G1L", goal);
        //wm_lock->unlock();
    }
    pos = msg.find("G2L");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> goal.dist >> goal.theta >> goal.phi;
        //wm_lock->lock();
        WM->setFlagPos("G2L", goal);
        //wm_lock->unlock();
    }
    pos = msg.find("G1R");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> goal.dist >> goal.theta >> goal.phi;
        //wm_lock->lock();
        WM->setFlagPos("G1R", goal);
        //wm_lock->unlock();
    }
    pos = msg.find("G2R");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> goal.dist >> goal.theta >> goal.phi;
        //wm_lock->lock();
        WM->setFlagPos("G2R", goal);
        //wm_lock->unlock();
    }

    /*
     * Flags Parse
     */

    pos = msg.find("F1L");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> flag.dist >> flag.theta >> flag.phi;
        //wm_lock->lock();
        WM->setFlagPos("F1L", flag);
        //wm_lock->unlock();
    }
    pos = msg.find("F2L");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> flag.dist >> flag.theta >> flag.phi;
        //wm_lock->lock();
        WM->setFlagPos("F2L", flag);
        //wm_lock->unlock();
    }
    pos = msg.find("F1R");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> flag.dist >> flag.theta >> flag.phi;
        //wm_lock->lock();
        WM->setFlagPos("F1R", flag);
        //wm_lock->unlock();
    }
    pos = msg.find("F2R");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> flag.dist >> flag.theta >> flag.phi;
        //wm_lock->lock();
        WM->setFlagPos("F2R", flag);
        //wm_lock->unlock();
    }
}

void Parser::parsePlayers(string &msg)
{
    /*
     * Player Pos Parse
     */

    string temp;
    int pos = msg.find("P");
    while (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp;
        if ( temp != "team" )
        {
            pos = msg.find("P",pos+1);
            continue;
        }
        string name,place;
        int num=0;
        Polar ppol;
        edame >> name ;
        if ( name != WM->getOurName() )
        {
            //wm_lock->lock();
            WM->setOppName( name );
            //wm_lock->unlock();
        }
        edame >> temp >> num >> place >> temp >> ppol.dist >> ppol.theta >> ppol.phi ;
        if ( place == "head" )
        {
            if ( name == WM->getOurName() )
            {
                //wm_lock->lock();
                WM->setOurPlayerPos ( num , ppol ) ;
                //wm_lock->unlock();
            }
            else
            {
                //wm_lock->lock();
                WM->setOppPlayerPos ( num , ppol ) ;
                //wm_lock->unlock();
            }
        }
        else
        {
            if ( name == WM->getOurName() )
            {
                //wm_lock->lock();
                WM->setOurPlayerPartPos ( num , place , ppol ) ;
                //wm_lock->unlock();
            }
            else
            {
                //wm_lock->lock();
                WM->setOppPlayerPartPos ( num , place , ppol ) ;
                //wm_lock->unlock();
            }
        }

        pos = msg.find("P",pos+1);

    }
}

void Parser::parseTime(string &msg)
{

    double time;

    string temp;
    int pos = msg.find("time");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> time;
        //wm_lock->lock();
        WM->setServerTime(time);
        //wm_lock->unlock();
    }
}

void Parser::parseGameState(string &msg)
{
    string temp;
    int pos = msg.find("GS");
    if (pos != string::npos)
    {
        string pm;
        pos = msg.find("pm", pos + 1);
        if (pos != string::npos)
        {
            stringstream edame(msg.substr(pos));
            edame >> temp >> pm;
            //wm_lock->lock();
            WM->setPlayMode(getPlayModeByString(pm));
            //wm_lock->unlock();
        }
    }

    pos = msg.find("GS");
    if (pos != string::npos)
    {
        while (1)
        {
            string name;
            double curr;
            pos = msg.find("t", pos + 1);
            if (pos == string::npos)
            {
                break;
            }
            stringstream edame(msg.substr(pos));
            edame >> name;
            if (pos != string::npos && name.length() == 1)
            {
                edame >> curr;
                //wm_lock->lock();
                WM->setTime(curr);
                //wm_lock->unlock();
                break;
            }
        }
    }
}

void Parser::parseGyroAndAccell(string &msg)
{
    /*
     * Gyro Parse
     */

    string temp;
    int pos = msg.find("GYR");
    if (pos != string::npos)
    {
        Vector3f gyr;
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> temp >> temp >> gyr.x() >> gyr.y() >> gyr.z();
        //wm_lock->lock();
        WM->setGyro(gyr);
        //wm_lock->unlock();
    }


    /*
     * Accelerometer Parse
     */

    pos = msg.find("ACC");
    if (pos != string::npos)
    {
        Vector3f acc;
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> temp >> temp >> acc.x() >> acc.y() >> acc.z();
        //wm_lock->lock();
        WM->setACC(acc);
        //wm_lock->unlock();
    }

}

void Parser::parseBall(string &msg)
{
    /*
     * Ball Pos Parse
     */

    Polar ball;

    string temp;
    int pos = msg.find("B");
    while (1)
    {
        if (pos == string::npos)
            break;

        stringstream edame(msg.substr(pos));
        string name;
        edame >> name;
        if (name.length() != 1)
        {
            pos = msg.find("B", pos + 1);
            continue;
        }
        edame >> temp >> ball.dist >> ball.theta >> ball.phi;
        //wm_lock->lock();
        WM->setBallPolarPos(ball);
        //wm_lock->unlock();
        break;
    }
}

/**
 *
 * @param msg witch is the massage that will parse
 * @do parse message
 *
 * */
void Parser::Parse(string &msg)
{
    for ( int i = 0; i<msg.length(); i++)
        if ( msg[i]=='(' || msg[i]==')')
            msg[i]=' ';

    std::vector<std::thread> all_threads;

    all_threads.push_back(std::thread ( std::bind (&Parser::parseHingeJoint,this,msg) ));
//    all_threads.push_back(std::thread ( std::bind (&Parser::parseLines,this,msg) ));
    all_threads.push_back(std::thread ( std::bind (&Parser::parseFlags,this,msg) ));
    all_threads.push_back(std::thread ( std::bind (&Parser::parsePlayers,this,msg) ));
    all_threads.push_back(std::thread ( std::bind (&Parser::parseTime,this,msg) ));
    all_threads.push_back(std::thread ( std::bind (&Parser::parseGameState,this,msg) ));
    all_threads.push_back(std::thread ( std::bind (&Parser::parseGyroAndAccell,this,msg) ));
    all_threads.push_back(std::thread ( std::bind (&Parser::parseBall,this,msg) ));
    all_threads.push_back(std::thread ( std::bind (&Parser::parseSide,this,msg) ));
    all_threads.push_back(std::thread ( std::bind (&Parser::parseFrp,this,msg) ));
    all_threads.push_back(std::thread ( std::bind (&Parser::parseHearMessage,this,msg) ));
    all_threads.push_back(std::thread ( std::bind (&Parser::parseSense,this,msg) ));


    for ( auto &i : all_threads )
    {
        i.join();
    }
}

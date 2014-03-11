#include "Parser.h"
#include "rvdraw.h"
#include <sys/time.h>
#include <regex>

Parser::Parser(WorldModel *wm)
{
    WM = wm;
    pms["BeforeKickOff"] = PM_BeforeKickOff ;
    pms["KickOff_Left"] = PM_KickOff_Left ;
    pms["KickOff_Right"] = PM_KickOff_Right ;
    pms["PlayOn"] = PM_PlayOn ;
    pms["KickIn_Left"] = PM_KickIn_Left;
    pms["KickIn_Right"] =  PM_KickIn_Right ;
    pms["corner_kick_left"] = PM_CORNER_KICK_LEFT;
    pms["corner_kick_right"] = PM_CORNER_KICK_RIGHT;
    pms["goal_kick_left"] = PM_GOAL_KICK_LEFT ;
    pms["goal_kick_right"] = PM_GOAL_KICK_RIGHT ;
    pms["offside_left"] =  PM_OFFSIDE_LEFT ;
    pms["offside_right"] = PM_OFFSIDE_RIGHT ;
    pms["GameOver"] = PM_GameOver ;
    pms["Goal_Left"] = PM_Goal_Left ;
    pms["Goal_Right"] = PM_Goal_Right ;
    pms["free_kick_left"] = PM_FREE_KICK_LEFT ;
    pms["free_kick_right"] = PM_FREE_KICK_RIGHT ;
    pms["unknown"] = PM_NONE ;
}

/**
 *
 * @param msg witch is the massage that will parse
 * @do parse message
 *
 * */
void Parser::Parse(string msg)
{
//    cout << msg << endl;
//    for ( int i = 0; i<msg.length(); i++)
//        if ( msg[i]=='(' || msg[i]==')')
//        {
//            msg[i]=' ';
//        }
    Polar goal;
    Polar flag;
    Polar ball;
    double time;
    string temp;
//    regex rr("\\(time \\(now ([-+]?[0-9]*\.?[0-9]+)\\)\\)");
    /*
     * Time & PlayMode Parser
     */
    //    cout<<msg<<endl;
    int pos = msg.find("GS");
    if (pos != string::npos)
    {
        string pm;
        pos = msg.find("pm", pos + 1);
        if (pos != string::npos)
        {
            stringstream edame(msg.substr(pos));
            edame >> temp >> pm;
            WM->setPlayMode(pms[pm]);
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
                WM->setTime(curr);
                break;
            }

        }
    }



    /*
     * Time Parse
     */
    pos = msg.find("time");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> time;
        WM->setServerTime(time);
    }


    //kossher parsing
    //    bool not_found=false;
    //    while(!not_found)
    //    {
    pos=msg.find("L");
    while(pos !=string::npos)
    {
//        stringstream edame(msg.substr(pos));
//        double x1,y1,z1,x2,y2,z2;
//        edame>>temp>>temp>>x1>>y1>>z1>>temp>>x2>>y2>>z2;
//        line l(Vector3f(x1,y1,z1),Vector3f(x2,y2,z2),WM->serverTime);
//        cout<<l.begin<<"and "<<l.end<<endl;
//        WM->setSeenLines(l);
        //       RVDraw::instance()->drawVector3f(x1*cosDeg());
        pos=msg.find("L",pos+1);

    }

    //        else{
    //            not_found=true;
    //        }

    //    }




    /*
     * Goal Flags Parse
     */

    pos = msg.find("G1L");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> goal.dist >> goal.theta >> goal.phi;
        WM->setFlagPos("G1L", goal);
    }
    pos = msg.find("G2L");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> goal.dist >> goal.theta >> goal.phi;
        WM->setFlagPos("G2L", goal);
    }
    pos = msg.find("G1R");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> goal.dist >> goal.theta >> goal.phi;
        WM->setFlagPos("G1R", goal);
    }
    pos = msg.find("G2R");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> goal.dist >> goal.theta >> goal.phi;
        WM->setFlagPos("G2R", goal);
    }

    /*
     * Flags Parse
     */


    pos = msg.find("F1L");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> flag.dist >> flag.theta >> flag.phi;
        WM->setFlagPos("F1L", flag);
    }
    pos = msg.find("F2L");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> flag.dist >> flag.theta >> flag.phi;
        WM->setFlagPos("F2L", flag);
    }
    pos = msg.find("F1R");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> flag.dist >> flag.theta >> flag.phi;
        WM->setFlagPos("F1R", flag);
    }
    pos = msg.find("F2R");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> flag.dist >> flag.theta >> flag.phi;
        WM->setFlagPos("F2R", flag);
    }

    /*
     * Ball Pos Parse
     */

    pos = msg.find("B");
    while (1)
    {
        if (pos == string::npos)
        {
            break;
        }
        stringstream edame(msg.substr(pos));
        string name;
        edame >> name;
        if (name.length() != 1)
        {
            pos = msg.find("B", pos + 1);
            continue;
        }
        edame >> temp >> ball.dist >> ball.theta >> ball.phi;
        WM->setBallPolarPos(ball);
        break;
    }

    /*
     * Gyro Parse
     */

    pos = msg.find("GYR");
    if (pos != string::npos)
    {
        Vector3f gyr;
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> temp >> temp >> gyr.x() >> gyr.y() >> gyr.z();
        WM->setGyro(gyr);
    }

    /*
     * Angle Parse
     */
    pos = msg.find("HJ");
    for (int i = 0; i < 22; i++)
    {
        if (pos == string::npos)
        {
            break;
        }
        string name;
        double angle;
        stringstream edame(msg.substr(pos));
        edame >> temp >> temp >> name >> temp >> angle;
        WM->setJointAngle(name, angle);
        pos = msg.find("HJ", pos + 1);
    }

    /*
     * Player Pos Parse
     */

    pos = msg.find("P");
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
            WM->setOppName( name );
        }
        edame >> temp >> num >> place >> temp >> ppol.dist >> ppol.theta >> ppol.phi ;
        if ( place == "head" )
        {
            if ( name == WM->getOurName() )
            {
                WM->setOurPlayerPos ( num , ppol ) ;
            }
            else
            {
                WM->setOppPlayerPos ( num , ppol ) ;
            }
        }
        else
        {
            if ( name == WM->getOurName() )
            {
                WM->setOurPlayerPartPos ( num , place , ppol ) ;
            }
            else
            {
                WM->setOppPlayerPartPos ( num , place , ppol ) ;
            }
        }

        pos = msg.find("P",pos+1);

    }
    /*
     * Side Parser
     */
    pos = msg.find("left");
    while (1)
    {
        if (pos == string::npos)
        {
            break;
        }
        if (msg [pos - 1] == ' ')
        {
            WM->setTeamSide("left");
        }
        pos = msg.find("left", pos + 1);
    }
    pos = msg.find("right");
    while (1)
    {
        if (pos == string::npos)
        {
            break;
        }
        if (msg [pos - 1] == ' ')
        {
            WM->setTeamSide("right");
        }
        pos = msg.find("right", pos + 1);
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
        WM->setACC(acc);
    }

    /*
     * FRP Parser
     */

    pos = msg.find("FRP");
    if (pos != string::npos)
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
        WM->setFootPress(name, foot);
    }
    pos = msg.find("FRP", pos + 1);
    if (pos != string::npos)
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
        WM->setFootPress(name, foot);
    }
    //cout<<msg<<endl;
    pos = msg.find("hear");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        string msg1;
        edame >> temp >> time >> temp >> msg1 ;

        //cout << "LL! " <<msg1 << endl;
        if (msg1.size() ==3 && msg1[0]==msg1[1] && msg1[1] == msg1[2]){
            msg1=msg1.substr(0,1);
            WM->setLastMsg( msg1 , time );
        }
        else
        {

            pos = msg.find("hear",pos+1);
            if (pos != string::npos)
            {
                stringstream edame(msg.substr(pos));

                edame >> temp >> time >> temp >> msg1 ;
                //cout << "msg1 after " <<msg1 << endl;

                if (msg1.size() == 3 && msg1[0]==msg1[1] && msg1[1] == msg1[2]){
                    msg1=msg1.substr(0,1);
                    WM->setLastMsg( msg1 , time );
                }

            }
        }
    }

    pos = msg.find("mypos");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        string msg;
        double x,y,z;
        edame >> temp >> x >> y >> z ;
        WM->setSense(true);
        WM->sensedPos = Vector3f(x,y,z);
    }

    pos = msg.find("myorien");
    if (pos != string::npos)
    {
        stringstream edame(msg.substr(pos));
        string msg;
        double ang;
        edame >> temp >> ang ;
        WM->setSense(true);
        WM->myOrien = ang;
    }
}

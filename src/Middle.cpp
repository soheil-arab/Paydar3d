/*
	Middle Decision Making
*/
/*
   RRRR    OOOOO   BBBBB    OOOOO  TTTTTTTT  OOOOO   OOOOO   SSSSS
   R  R    O   O   B    B   O   O     T      O   O   O   O  S
   RRRR    O   O   BBBBB    O   O     T      O   O   O   O   SSSS
   R  R    O   O   B    B   O   O     T      O   O   O   O       S
   R   R   OOOOO   BBBBB    OOOOO     T      OOOOO   OOOOO  SSSSS
*/
#include "Decide.h"
string Decide::Middle()
{
    stringstream ss("");
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());
    VecPosition Bball=VecPosition::givePosition(ball,(VecPosition(9,0)-ball).getDirection(),-0.25);
    VecPosition Bball2=VecPosition::givePosition(ball,(VecPosition(-9,0)-ball).getDirection(),-1.5);
    SideT s;
    static int ti=0;
    static double time=0;
    static int ty=0;
    static bool beam=false;
    static bool set=false;
    static double t=WM->getServerTime();
    static bool stand=false;
    static double stan=0;
    static double st=0;
    Command com;
    double x,y,ang;
    ///////////////////// Place Your Code Here ///////////////////////
    if (canBeam() && !set && !beam)
    {
        set = true ;
        t = WM->getServerTime () ;
        return ss.str();
    }
    else if ( canBeam() && !beam && ( WM->getServerTime()-t )  >  0.5  )
    {
        ty++;
        ss<<SK->whereBeam(stand,stan);
        if (ty>2)
        {
            stand=false;
            stan=0;
            ty=0;
            beam = true;
            return ss.str();
        }
    }
    else if (!canBeam())
    {
        ty=0;
        beam=set=false;
    }
    if ( ( WM->getServerTime()-t ) < 1 )
    {
        return "";
    }
    ti++;
    if ( ti<45 )
    {
        ss<<SK->moveJoint ( "rae1",-2 ) <<SK->moveJoint ( "lae1",-2 )<<SK->Standing(stand,stan) ;
        return ss.str();
    }
    if (ti<110)
    {
        ss<<SK->Standing(stand,stan);
        return ss.str();
    }
    static double tFinal=0;
    //if(ball.getX()>4 && WM->getMyNum()==6 && me.getX()<0)
        //ss << SK->sayBallPos () ;
    //////////////////////////////////////////////////////////////////
    if (!ACT->isDone())
    {
        ss<<ACT->doCurrentAct();
        tFinal=0;
    }
    else if ((shouldStandUp(com, s)))
    {
        ACT->setCurrentAct(com, s);
        tFinal=0;
    }
    else if (shouldPlay() && parseBallPos()==WM->getMyNum())
    {
//        if (shouldClear ( x,y,s ))
//        {
//            ACT->setCurrentAct(K, s , x, y);
//        }
    ///////decide///////////////////////
//        else
            return moveToPosP(true,ball-VecPosition(0.2,0),tFinal);
    }
    ////////////positioning///////////
    else if(me.getDistanceTo(middlepositioning())>0.1 && shouldPlay2()){
        return moveToPosP(false,middlepositioning(),tFinal);
    }
    else if(fabs(WM->getMyAngleTo(myPos+Eigen::Vector3f(1,0,0)))>15 && shouldPlay2())
    {
        if(WM->getMyAngleTo(myPos+Eigen::Vector3f(1,0,0))>0)
        {
            return SK->finalAction("turnL",tFinal);
        }
        else
        {
            return SK->finalAction("turnR",tFinal);
        }
    }
    else
    {
        return SK->finalAction("",tFinal);
    }

    //////////////////////////////////////////////////////////////////
    return ss.str();
}

/*
   RRRR    OOOOO   BBBBB    OOOOO  TTTTTTTT  OOOOO   OOOOO   SSSSS
   R  R    O   O   B    B   O   O     T      O   O   O   O  S
   RRRR    O   O   BBBBB    O   O     T      O   O   O   O   SSSS
   R  R    O   O   B    B   O   O     T      O   O   O   O       S
   R   R   OOOOO   BBBBB    OOOOO     T      OOOOO   OOOOO  SSSSS
*/
#include "Decide.h"
string Decide::goalie()
{
    stringstream ss("");
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());
    static bool done=false;
    static bool stand=false;
    static double stan=0;
    static bool beam=false;
    static bool set=false;
    static double t=WM->getServerTime();
    static int tt=0;
    SideT s;
    static int ti=0;
    static double time=0;
    static bool do2=true;
    static double df=0;
    Command com;
    double x,y,ang;
    double angleToFront=WM->getMyAngleTo(myPos+salt::Vector3f(1,0,0));
    VecPosition Bball=VecPosition::givePosition(ball,(VecPosition(WM->getFieldLength()/2,0)-ball).getDirection(),-0.15);
    Rect r1(VecPosition(-WM->getFieldLength()/2,1.5),VecPosition(-WM->getFieldLength()/2+2.5,-1.5));
    string sa;
    VecPosition mvpos;
    static bool do1=false;
    ///////////////////// Place Your Code Here ///////////////////////
    if (canBeam() && !set && !beam)
    {
        set = true ;
        t = WM->getServerTime () ;
        return ss.str();
    }
    else if ( canBeam() && !beam && ( WM->getServerTime()-t )  >  0.5  )
    {
        ss << "(beam -14.5 0 0)" ;
        beam = true;
        return ss.str();
    }
    else if (!canBeam())
    {
        beam=set=false;
    }
    if ( ( WM->getServerTime()-t ) < 1 )
    {
        return "";
    }
    if (ball.getY()<-1)
    {
        mvpos=VecPosition(-WM->getFieldLength()/2+0.5,-0.55);
    }
    else if (ball.getY()>1)
    {
        mvpos=VecPosition(-WM->getFieldLength()/2+0.5,0.55);
    }
    else
    {
        mvpos=VecPosition(-WM->getFieldLength()/2+0.5,0);
    }
    if ((goaliepositioning()-me.getY())>0)
    {
        sa="sideWalkR";
    }
    else
    {
        sa="sideWalkL";
    }
    static double tFinal=0;
    static bool b=false;
    if(me.getDistanceTo(ball-VecPosition(0.07,0))<0.15)
    {
        b=false;
    }
    if(int(WM->getServerTime()*50)%2 )
        ss << SK->sayBallPos () ;
    //cout <<"close :"<<WM->getClosestOurToBall()<<endl;
        //cout<<"goalie : "<<parseBallPos()<<endl;
        //out<<"cycle:  "<<WM->getTime()<<" close:  "<<parseBallPos()<<endl;
    //////////////////////////////////////////////////////////////////
    if (!ACT->isDone())
    {
        tFinal=0;
        ss<<ACT->doCurrentAct();
    }
    else if ( WM->shouldDive(s) && ball.getX()>7.2)
    {
        tFinal=0;
        ACT->setCurrentAct(Di,s);
    }
    else if ( (  shouldStandUp ( com , s )  ) )
    {
        tFinal=0;
        Log.Log ( 1 , "Standing Up !" );
        ACT->setCurrentAct( com , s );
    }
    else
    {
//        if (shouldClear ( x,y,s ) )
//        {
//            tFinal=0;
//            ACT->setCurrentAct(K,s,x,y);
//        }
        if (shouldPlay() && (r1.isInside(ball) || me.getDistanceTo(ball)<0.4) && WM->getClosestOurToBall()==WM->getMyNum())
        {
            return moveToPosP(true,ball-VecPosition(0.2,0),tFinal);
        }
        else if (fabs(angleToFront)>12)
        {
            if(angleToFront>0)
            {
                ss<< SK->finalAction("turnL",tFinal);
            }
            else
            {
                ss<< SK->finalAction("turnR",tFinal);
            }
        }
        else if (fabs(me.getY()-goaliepositioning())>0.12 && (ball.getX()<-12 || fabs(me.getY()-goaliepositioning())>0.25))
        {
            ss<< SK->finalAction(sa,tFinal);
        }
        else if (fabs(me.getX()-mvpos.getX())>0.1)
        {
            if(me.getX()>mvpos.getX())
            {
                ss<< SK->finalAction("bwalk",tFinal);
            }
            else
            {
                ss<< SK->finalAction("walk",tFinal);
            }
        }
        else
        {
            ss<< SK->finalAction("",tFinal);
        }
    }
    //////////////////////////////////////////////////////////////////
    return ss.str();
}

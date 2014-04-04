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
    static int libT = 0 ;
    Command com;
    double x,y,ang;
    double angleToFront=WM->getMyAngleTo(myPos+Eigen::Vector3f(1,0,0));
    VecPosition Bball=VecPosition::givePosition(ball,(VecPosition(WM->getFieldLength()/2,0)-ball).getDirection(),-0.15);
    Rect r1(VecPosition(-WM->getFieldLength()/2,1.5),VecPosition(-WM->getFieldLength()/2+2.5,-1.5));
    double sa;
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
        ss << "(beam -14.75 0 0)" ;
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
        mvpos=VecPosition(-WM->getFieldLength()/2+0.25,-0.55);
    }
    else if (ball.getY()>1)
    {
        mvpos=VecPosition(-WM->getFieldLength()/2+0.25,0.55);
    }
    else
    {
        mvpos=VecPosition(-WM->getFieldLength()/2+0.25,0);
    }
    if ((goaliepositioning()-me.getY())>0)
    {
        sa=90;
    }
    else
    {
        sa=-90;
    }
    static double tFinal=0;
    static bool b=false;
    if(me.getDistanceTo(ball-VecPosition(0.07,0))<0.15)
    {
        b=false;
    }
    double finalAngToGo = 0;
    double finalAngToTurn = 0 ;
    VecPosition goal(15, 0);
    Line ballGoal = Line::makeLineFromTwoPoints(ball, goal);
    VecPosition posOnLine = ballGoal.getPointOnLineClosestTo(me);



    VecPosition behindPos = /*ball*/ VecPosition::givePosition(ball, (goal - ball).getDirection(), -0.2);
    double angToGoToBehindPos = WM->getMyAngleTo(Eigen::Vector3f(behindPos.getX(), behindPos.getY(), 0));
    double angToTurnToBehindPos = WM->getMyAngle() - (goal - behindPos).getDirection();

    double angToGoToBall = WM->getMyAngleTo(Eigen::Vector3f(ball.getX(), ball.getY(), 0));
    double angToTurnToBall = WM->getMyAngle() - (goal - ball).getDirection();


    if (fabs(angToTurnToBall) < 7) {
        angToTurnToBall = 0;
    }
    if (fabs(angToTurnToBehindPos) < 7) {
        angToTurnToBehindPos = 0;
    }
    if (fabs(angToGoToBall) < 7) {
        angToGoToBall = 0;
    }
    if (fabs(angToGoToBehindPos) < 7) {
        angToGoToBehindPos = 0;
    }

    if (fabs(angToTurnToBall) > 170) {
        angToTurnToBall = 180;
    }
    if (fabs(angToTurnToBehindPos) > 170) {
        angToTurnToBehindPos = 180;
    }
    if (fabs(angToGoToBall) > 170) {
        angToGoToBall = 180;
    }
    if (fabs(angToGoToBehindPos) > 170) {
        angToGoToBehindPos = 180;
    }

    if ( posOnLine.getDistanceTo(me) < 0.4 && ballGoal.isInBetween(posOnLine,ball,goal) )
    {
        VecPosition possibleChois1 = VecPosition::givePosition(ball,(goal - ball).getDirection() + 90,0.3);
        VecPosition possibleChois2 = VecPosition::givePosition(ball,(goal - ball).getDirection() - 90,0.3);
        VecPosition posToGoNow = possibleChois1.getDistanceTo(me) < possibleChois2.getDistanceTo(me) ? possibleChois1 : possibleChois2 ;
        finalAngToGo = WM->getMyAngleTo(Eigen::Vector3f(posToGoNow.getX(), posToGoNow.getY(), 0));
        finalAngToTurn = WM->getMyAngle() - (goal - posToGoNow).getDirection();
    } else if (me.getDistanceTo(behindPos) > 0.15) {
        finalAngToTurn = angToTurnToBehindPos ;
        finalAngToGo = angToGoToBehindPos;
    }else {
        finalAngToTurn = angToTurnToBall ;
        finalAngToGo = angToGoToBall ;
    }

    if( int(WM->getServerTime()*50)%2 && WM->isFeltDown() == false )
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
//    else if ( WM->shouldDive(s) && ball.getX()<-8)
//    {
//        tFinal=0;
//        ACT->setCurrentAct(Di,s);
//    }
    else if ( (  shouldStandUp ( com , s )  ) )
    {
        tFinal=0;
//        Log.Log ( 1 , "Standing Up !" );
        ACT->setCurrentAct( com , s );
    }
    else
    {
        if (shouldClear ( x,y,s ) )
        {
            tFinal=0;
            ACT->setCurrentAct(K,s,x,y);
        }
        if (shouldPlay() && (r1.isInside(ball) || me.getDistanceTo(ball)<0.0) && WM->getClosestOurToBall()==WM->getMyNum())
        {
            return SK->GeneralWalk(libT,finalAngToTurn,finalAngToGo,0.02);
        }
        if (fabs(angleToFront)>12)
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
            return SK->GeneralWalk(libT,0,sa,0.02);
        }
        else if (fabs(me.getX()-mvpos.getX())>0.1)
        {
            if(me.getX()>mvpos.getX())
            {
                return SK->GeneralWalk(libT,0,180,0.02);
            }
            else
            {
                return SK->GeneralWalk(libT,0,0,0.02);
            }
        }
        else
        {
//            ss<< SK->finalAction("",tFinal);
        }
    }
    //////////////////////////////////////////////////////////////////
    return ss.str();
}

/*
        Attacker Decision Making
 */

/*
   RRRR    OOOOO   BBBBB    OOOOO  TTTTTTTT  OOOOO   OOOOO   SSSSS
   R  R    O   O   B    B   O   O     T      O   O   O   O  S
   RRRR    O   O   BBBBB    O   O     T      O   O   O   O   SSSS
   R  R    O   O   B    B   O   O     T      O   O   O   O       S
   R   R   OOOOO   BBBBB    OOOOO     T      OOOOO   OOOOO  SSSSS
 */
#include "Decide.h"
#include "Geom.h"
#include "rvdraw.h"

string Decide::Attack()
{
    int closest = parseBallPos();
    int sClosest = WM->getClosestTo(closest) ;

    stringstream ss("");
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();

    static int iii = 0;
    RVDraw::instance()->drawVector3f(salt::Vector3f(myPos.x(), myPos.y(), myPos.z()), RED, iii++, 3);
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());
    VecPosition goal(15, 0);
    static bool beam = false;
    static int set =0 ;
    static int ty = 0;
    static double t = WM->getServerTime();
    static bool stand = false;
    static double stan = 0;
    SideT s;
    static double tFinal = 0;
    double x = tFinal, y/*, ang*/;
    static bool did = false;
    static double tt = 0;
    Command com;
    static int curAct = 0;
    static int libT = 0;

    if (shouldPlay2()) {
        did = false;
        tt = 0;
    }
    ///////////////////// Place Your Code Here ///////////////////////
//    if ( WM->getMyNum() == 9 )
//    {
//        cout << closest << endl;
//    }
    if (canBeam() && set < 10 && !beam) {
        set ++;
        t = WM->getServerTime();
        return ss.str();
    } else if (canBeam() && !beam && (WM->getServerTime() - t) > 1.5) {
        ty++;
        ss << SK->whereBeam(stand, stan);
        WM->clearBodyRotate();
        if (ty > 1) {
            stand = false;
            stan = 0;
            beam = true;
            ty = 0;
        }
        return ss.str();
    } else if (!canBeam()) {
        beam  = false;
        set = 0 ;
        ty = 0;
    }
    stand = false;
    if ((WM->getServerTime() - t) < 1) {
        return "";
    }

    /////////////////////////////////////////////////////////////

    if (!ACT->isDone()) {
        ss << ACT->doCurrentAct();
        tFinal = 0;
        libT = 0;
        Log.Log(2, "Doing Current Act");
        return ss.str();
    } else if ((shouldStandUp(com, s))) {
        if (curAct != 0) {
            curAct = 0;
            tFinal = 0;
            libT = 0;
        }
        ACT->setCurrentAct(com, s);
        tFinal = 0;
        libT = 0;
    } else if (!did && !shouldPlay2()) {
        Log.Log(2, "Sefring");
        tFinal = 0;
        libT = 0;
        return SK->sefr(did, tt, false);
    } else if (shouldPlay() && closest == WM->getMyNum()) {

        double finalAngToGo = 0;
        double finalAngToTurn = 0 ;

        VecPosition behindPos = VecPosition::givePosition(ball, (goal - ball).getDirection(), -0.2);
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

        Line ballGoal = Line::makeLineFromTwoPoints(ball, goal);
        VecPosition posOnLine = ballGoal.getPointOnLineClosestTo(me);


        if ( posOnLine.getDistanceTo(me) < 0.4 && ballGoal.isInBetween(posOnLine,ball,goal) )
        {
            VecPosition possibleChois1 = VecPosition::givePosition(ball,(goal - ball).getDirection() + 90,0.25);
            VecPosition possibleChois2 = VecPosition::givePosition(ball,(goal - ball).getDirection() - 90,0.25);
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

        if (  shouldClear(x, y, s) ) {
            ACT->setCurrentAct(K, s, x, y);
            tFinal = 0;
        }
        return SK->GeneralWalk(libT,finalAngToTurn,finalAngToGo,0.02);

    } else if ( WM->getMyNum() == sClosest && shouldPlay2() )
    {
        Eigen::Vector3f closestPos = WM->getOurPos(sClosest);
        VecPosition behindPos = VecPosition::givePosition(VecPosition(closestPos.x(),closestPos.y()),
                                                          (VecPosition(-15,0) - VecPosition(closestPos.x(),closestPos.y())).getDirection(), 0.7);
        double angToGoToBehindPos = WM->getMyAngleTo(Eigen::Vector3f(behindPos.getX(), behindPos.getY(), 0));
        double angToTurnToBehindPos = WM->getMyAngle() - (goal - behindPos).getDirection();

        if (fabs(angToTurnToBehindPos) < 10) {
            angToTurnToBehindPos = 0;
        }
        if (fabs(angToGoToBehindPos) < 10) {
            angToGoToBehindPos = 0;
        }
        return SK->GeneralWalk(libT,angToTurnToBehindPos,angToGoToBehindPos,0.02);
    }
    else if (shouldPlay2()) {
        VecPosition poss;
        if (WM->getMyNum() == 2 || WM->getMyNum() == 3 || WM->getMyNum() == 4 || WM->getMyNum() == 5 || WM->getMyNum() == 6)
            poss = defendpositioning();
        else if (WM->getMyNum() == 9 || WM->getMyNum() == 8)
            poss = attackpositioning();
        else if (WM->getMyNum() == 7 || WM->getMyNum() == 11 || WM->getMyNum() == 10)
            poss = middlepositioning();

        if (me.getDistanceTo(poss) > 1) {
            double angToGoPoss = WM->getMyAngleTo(Eigen::Vector3f(poss.getX(), poss.getY(), 0));
            double angToTurnToPoss = WM->getMyAngle() - (ball - poss).getDirection();
            return SK->GeneralWalk(libT, angToTurnToPoss, angToGoPoss, 0.01);
        } else if (me.getDistanceTo(poss) > 0.2) {
            double angToGoPoss = WM->getMyAngleTo(Eigen::Vector3f(poss.getX(), poss.getY(), 0));
            double angToTurnToPoss = WM->getMyAngle() - (ball - poss).getDirection();
            return SK->GeneralWalk(libT, angToTurnToPoss, angToGoPoss, -0.005);
        } else if (fabs(WM->getMyAngleTo(WM->getBallPos())) > 15 && shouldPlay2()) {
            if (WM->getMyAngleTo(WM->getBallPos()) > 0) {
                Log.Log(2, "Turn Left !");
                return SK->finalAction("turnL", tFinal);
            } else {
                Log.Log(2, "Turn Right !");
                return SK->finalAction("turnR", tFinal);
            }
        } else if (!(WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right || WM->getPlayMode() == PM_BeforeKickOff)) {
            Log.Log(2, "Darja Mizadam !");
            return "";
        }

    } else if (fabs(WM->getMyAngleTo(WM->getBallPos())) > 15 && shouldPlay2()) {
        if (WM->getMyAngleTo(WM->getBallPos()) > 0) {
            Log.Log(2, "Turn Left !");
            return SK->finalAction("turnL", tFinal);
        } else {
            Log.Log(2, "Turn Right !");
            return SK->finalAction("turnR", tFinal);
        }
    } else if (!(WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right || WM->getPlayMode() == PM_BeforeKickOff)) {
        Log.Log(2, "Darja Mizadam !");
        return SK->finalAction("darja", tFinal);
    }
    return ss.str();
}

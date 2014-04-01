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
    static int ttt = 0;
    static double deg = 0;
    static double A = 0.01;

    //    if (ttt%1000 >= 500) {
    //        deg = 10;
    //        A += 0.002;
    //    } else {
    //        deg = -10;
    //        A -= 0.002;
    //    }
    //        return SK->GeneralWalk(ttt,0,0,0.01);
    //    return SK->SideTurn(ttt,Right);
    //    return SK->WalkAngleLib(ttt,Right);
    stringstream ss("");
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());
    VecPosition goal(15, 0);
    static bool beam = false;
    double myAngle = WM->getMyAngle();
    static bool set = false;
    static int ty = 0;
    static double t = WM->getServerTime();
    static bool stand = false;
    static double stan = 0;
    SideT s;
    static double time = 0;
    static double tFinal = 0;
    double x = tFinal, y, ang;
    static bool did = false;
    static double tt = 0;
    Command com;
    static int curAct = 0;
    static int libT = 0;

    if (shouldPlay2()) {
        did = false;
        tt = 0;
    }
//        if (ttt % 1200 < 300) {
//            return SK->GeneralWalk(ttt, 30,30, 0.01);
//        } else if (ttt % 1200 < 600) {
//            return SK->GeneralWalk(ttt, -30, -30, 0.02);
//        } else if (ttt % 1200 < 900) {
//            return SK->GeneralWalk(ttt, 30, 30, 0.01);
//        } else {
//            return SK->GeneralWalk(ttt, -30, -30, 0.01);
//        }

    ///////////////////// Place Your Code Here ///////////////////////
    if (canBeam() && !set && !beam) {
        set = true;
        t = WM->getServerTime();
        return ss.str();
    } else if (canBeam() && !beam && (WM->getServerTime() - t) > 1.5) {
        //        WM->clearBodyRotate();
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
        beam = set = false;
        ty = 0;
    }
    stand = false;
    if ((WM->getServerTime() - t) < 1) {
        return "";
    }

    /////////////////////////////////////////////////////////////
    int closest = parseBallPos();

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
        VecPosition oppositeTirak, thisTirak;
        if (ball.getY() > 0) {
            oppositeTirak.setY(-1.05);
            thisTirak.setY(1.05);
        } else {
            oppositeTirak.setY(1.05);
            thisTirak.setY(-1.05);
        }

        thisTirak.setY(thisTirak.getY() + sign(thisTirak.getY()) * (ball.getDistanceTo(goal) - 1.9) * 8.0 / 15.0);
        oppositeTirak.setY(oppositeTirak.getY() + sign(oppositeTirak.getY()) * (ball.getDistanceTo(goal) - 1.9) * 8.5 / 15.0);

        oppositeTirak.setX(15);
        thisTirak.setX(15);

        Line l1 = Line::makeLineFromTwoPoints(ball, oppositeTirak);
        Line l2 = Line::makeLineFromTwoPoints(ball, thisTirak);
        Line meBall = Line::makeLineFromTwoPoints(ball, me);
        Line goalLine = Line::makeLineFromTwoPoints(thisTirak, oppositeTirak);

        double firstAngle, secondAngle;
        firstAngle = (thisTirak - ball).getDirection();
        secondAngle = (oppositeTirak - ball).getDirection();

        VecPosition meGoal = meBall.getIntersection(goalLine);
        Circle ballCircle(ball, 0.042);

        double horizonAngleLeft = VecPosition::normalizeAngle(WM->getMyAngle() + 90);
        double horizonAngleRight = VecPosition::normalizeAngle(WM->getMyAngle() - 90);

        VecPosition myLeftSide = VecPosition::givePosition(me, horizonAngleLeft, 0.1);
        VecPosition myRightSide = VecPosition::givePosition(me, horizonAngleRight, 0.1);

        Line lineFromLeftSide = Line::makeLineFromPositionAndAngle(myLeftSide, myAngle);
        Line lineFromRightSide = Line::makeLineFromPositionAndAngle(myRightSide, myAngle);

        Circle c(ball, 0.5);

        VecPosition i11, i12, i21, i22, finalPos1, finalPos2;

        l1.getCircleIntersectionPoints(c, &i11, &i12);
        l2.getCircleIntersectionPoints(c, &i21, &i22);

        if (i11.getDistanceTo(oppositeTirak) > i12.getDistanceTo(oppositeTirak))
            finalPos1 = i11;
        else
            finalPos1 = i12;

        if (i21.getDistanceTo(thisTirak) > i22.getDistanceTo(thisTirak))
            finalPos2 = i21;
        else
            finalPos2 = i22;

        stringstream temp;
        temp << WM->getMyAngleToBall();
        Triangle tri(ball, finalPos1, finalPos2);
        RVDraw::instance()->drawLine(oppositeTirak, finalPos1, RED, 5);
        RVDraw::instance()->drawLine(thisTirak, finalPos2, RED, 6);
        RVDraw::instance()->drawLine(finalPos1, finalPos2, RED, 7);
        RVDraw::instance()->drawCircle(ballPos, c.getRadius(), GREEN, 8);
        RVDraw::instance()->drawLine(me, ball, YELLO, 9);
        RVDraw::instance()->drawLine(
            me, VecPosition::givePosition(me, WM->getMyAngle(), 1), BLUE, 10);
        RVDraw::instance()->drawAnnotation(
            temp.str(), salt::Vector3f(myPos.x(), myPos.y(), myPos.z() + 0.2),
            BLACK, 11);

        VecPosition behindPos = /*ball*/ VecPosition::givePosition(ball, (VecPosition(15, 0) - ball).getDirection(), -0.2);
        double angToGoToBehindPos = WM->getMyAngleTo(salt::Vector3f(behindPos.getX(), behindPos.getY(), 0));
        double angToTurnToBehindPos = WM->getMyAngle() - (VecPosition(15, 0) - behindPos).getDirection();
        double angToGoToBall = WM->getMyAngleTo(salt::Vector3f(ball.getX(), ball.getY(), 0));
        double angToTurnToBall = WM->getMyAngle() - (VecPosition(15, 0) - ball).getDirection();

//        if (shouldClear(x, y, s,tri)) {
//            ACT->setCurrentAct(K, s, x, y);
//            tFinal = 0;
//        }
        if (fabs(angToTurnToBall) < 7) {
            angToTurnToBall = 0;
        }
        if (fabs(angToTurnToBehindPos) < 7) {
            angToTurnToBehindPos = 0;
        }

//        if (angToTurnToBall * angToGoToBall < 0) {
//            angToGoToBall *= -1;
//        }

        if (me.getDistanceTo(behindPos) > 0.15) {
            //                cout << angToGoToBehindPos << "  " << angToTurnToBehindPos << endl;
            return SK->GeneralWalk(libT, angToTurnToBehindPos, angToGoToBehindPos, 0.01);
        } else {
            ////                cout << angToGoToBall << "  " << angToTurnToBall << endl;
            return SK->GeneralWalk(libT, angToTurnToBall, angToGoToBall, 0.01);
        }

    } else if (me.getDistanceTo(attackpositioning()) > 0.1 && shouldPlay2() && WM->getMyNum() != 10 && WM->getMyNum() != 11) {
        salt::Vector3f vec(attackpositioning().getX(), attackpositioning().getY(),
                           0);
        return moveToPosP(false, attackpositioning(), tFinal);
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
        return "";
    }
    return ss.str();
}

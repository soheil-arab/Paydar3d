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
    return SK->WalkLib();
    stringstream ss("");
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());
    VecPosition goal(15,0);
    static bool beam = false;
    static bool set = false;
    static int  ty=0;
    static double t = WM->getServerTime();
    static bool stand=false;
    static double stan=0;
    SideT s;
    static double time = 0;
    static double tFinal=0;
    double x=tFinal,y, ang;
    static bool did=false;
    static double tt=0;
    Command com;

    if(shouldPlay2()){
        did=false;
        tt=0;
    }
    ///////////////////// Place Your Code Here ///////////////////////
    if (canBeam() && !set && !beam)
    {
        set = true;
        t = WM->getServerTime();
        return ss.str();
    }
    else if (canBeam() && !beam && (WM->getServerTime() - t) > 1.5)
    {
//        WM->clearBodyRotate();
        ty++;
        ss << SK->whereBeam(stand,stan);
        WM->clearBodyRotate();
        if (ty>1)
        {
            stand=false;
            stan=0;
            beam = true;
            ty=0;
        }
        return ss.str();
    }
    else if (!canBeam())
    {
        beam = set = false;
        ty=0;
    }
    stand=false;
    if ((WM->getServerTime() - t) < 1)
    {
        return "";
    }


    /////////////////////////////////////////////////////////////
    int closest = parseBallPos();

    if (!ACT->isDone())
    {
        ss << ACT->doCurrentAct();
        tFinal=0;
        Log.Log(2,"Doing Current Act");
        return ss.str();
    }
    else if ((shouldStandUp(com, s)))
    {
        ACT->setCurrentAct(com, s);
        tFinal=0;
    }
    else if(!did && !shouldPlay2())
    {
        Log.Log(2,"Sefring");
        tFinal=0;
        return SK->sefr(did,tt,false);
    }

    else if (shouldPlay() && closest==WM->getMyNum() && WM->getMyNum() !=10 && WM->getMyNum()!=11)
    {
        VecPosition dest = VecPosition::givePosition(ball,(goal-ball).getDirection(),0.1);
        VecPosition oppositeTirak ,thisTirak;
        if ( ball.getY() > 0 ){
            oppositeTirak.setY(-1.05);
            thisTirak.setY(1.05);
        }else{
            oppositeTirak.setY(1.05);
            thisTirak.setY(-1.05);
        }

        thisTirak.setY(thisTirak.getY()+sign(thisTirak.getY())*(ball.getDistanceTo(goal)-1.9)*8.0/15.0);
        oppositeTirak.setY(oppositeTirak.getY()+sign(oppositeTirak.getY())*(ball.getDistanceTo(goal)-1.9)*8.5/15.0);

        oppositeTirak.setX(15);
        thisTirak.setX(15);

        Line l1 = Line::makeLineFromTwoPoints(ball,oppositeTirak);
        Line l2 = Line::makeLineFromTwoPoints(ball,thisTirak);
        Line meBall = Line::makeLineFromTwoPoints(ball,me);
        Line goalLine = Line::makeLineFromTwoPoints(thisTirak,oppositeTirak);

        VecPosition meGoal = meBall.getIntersection(goalLine);

        Circle c(dest,0.3);

        VecPosition i11,i12,i21,i22,finalPos1,finalPos2;

        l1.getCircleIntersectionPoints(c,&i11,&i12);
        l2.getCircleIntersectionPoints(c,&i21,&i22);

        if (i11.getDistanceTo(oppositeTirak) > i12.getDistanceTo(oppositeTirak))
            finalPos1 = i11;
        else
            finalPos1 = i12;

        if (i21.getDistanceTo(thisTirak) > i22.getDistanceTo(thisTirak))
            finalPos2 = i21;
        else
            finalPos2 = i22;


        Triangle tri(ball,finalPos1,finalPos2);
        RVDraw::instance()->drawLine(oppositeTirak,finalPos1,RED,5);
        RVDraw::instance()->drawLine(thisTirak,finalPos2,RED,6);
        RVDraw::instance()->drawLine(finalPos1,finalPos2,RED,7);
        RVDraw::instance()->drawCircle(ballPos,c.getRadius(),GREEN,8);
        RVDraw::instance()->drawLine(me,ball,YELLO,9);
        RVDraw::instance()->drawLine(me,VecPosition::givePosition(me,WM->getMyAngle(),1),BLUE,10);

        double thresh = max(7,min(20 ,me.getDistanceTo(ball)*20));

        if (shouldClear ( x,y,s,tri ))
        {
            ACT->setCurrentAct(K, s, x, y);
            tFinal=0;
        }

        if ( fabs(WM->getMyAngleToBall()) >  thresh )
        {
            if ( c.isInside(me) )
            {
                Log.Log(2,"Turning");
                if ( WM->getMyAngleToBall() > 0 )
                    return SK->finalAction("turnL" ,tFinal);
                else
                    return SK->finalAction("turnR" ,tFinal);
            }
            double backAng = VecPosition::normalizeAngle( WM->getMyAngleToBall()+180 );

            cout << "threshold" << thresh <<" angle to ball :  " << WM->getMyAngleToBall() <<" my angle : " << WM->getMyAngle() << endl;

            if ( fabs(backAng)  < 8 )
            {
                Log.Log(2,"BackWalking");
                return SK->finalAction("bwalk",tFinal);
            }
           else if ( VecPosition::normalizeAngle( backAng )  < 20  && VecPosition::normalizeAngle( backAng)  > 0){
                Log.Log(2,"BackWalking In AngleL");
                return SK->finalAction("bwalkAngleL",tFinal);
            }
            else if ( VecPosition::normalizeAngle( backAng )  > -20  && VecPosition::normalizeAngle( backAng )  < 0 )
            {
                Log.Log(2,"BackWalking In AngleR");
                return SK->finalAction("bwalkAngleR",tFinal);
            }
            else if (WM->getMyAngleToBall() > 0){
                Log.Log(2,"Walking Angle L");
                return SK->finalAction("walkAngleL",tFinal,1);
            }
            else{
                    Log.Log(2,"Walking Angle R");
                    return SK->finalAction("walkAngleR",tFinal,1);
            }
         }
        else if ( c.isInside( me ) && !tri.isInside(me) && fabs(meGoal.getY()) > 0.9){
            Log.Log(2,"Side Turn");
                if (WM->getMyAngleToGoal() > 0)
                    return SK->finalAction("sideTurnL",tFinal);
                else
                    return SK->finalAction("sideTurnR",tFinal);
        }
        else
        {
            Log.Log(2,"Walk");
            return SK->finalAction("walk",tFinal,1);
        }
    }
    else if(me.getDistanceTo(attackpositioning())>0.1 && shouldPlay2()  && WM->getMyNum() !=10 && WM->getMyNum()!=11){
        salt::Vector3f vec(attackpositioning().getX(),attackpositioning().getY(),0);
        return moveToPosP(false,attackpositioning(),tFinal);
    }
    else if(fabs(WM->getMyAngleTo(WM->getBallPos()))>15 && shouldPlay2())
    {
        if(WM->getMyAngleTo(WM->getBallPos())>0)
        {
            Log.Log ( 2 , "Turn Left !" );
            return SK->finalAction("turnL",tFinal);
        }
        else
        {
            Log.Log ( 2 , "Turn Right !" );
            return SK->finalAction("turnR",tFinal);
        }
    }
    else if(!(WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right || WM->getPlayMode() == PM_BeforeKickOff ) )
    {
        Log.Log ( 2 , "Darja Mizadam !" );
        return SK->finalAction( "darja" ,tFinal);
        return "";
    }
    return ss.str();

}

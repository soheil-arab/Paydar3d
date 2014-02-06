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
#include "rvdraw.h"
string Decide::Attack()
{
    stringstream ss("");
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());
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
//    cout <<"closest : "<<closest<<endl;
    if (!ACT->isDone())
    {
        ss << ACT->doCurrentAct();
        tFinal=0;
    }
    else if ((shouldStandUp(com, s)))
    {
        ACT->setCurrentAct(com, s);
        tFinal=0;
    }
    else if(!did && !shouldPlay2())
    {
        tFinal=0;
        return SK->sefr(did,tt,false);
    }
    else if (shouldPlay() && closest==WM->getMyNum() && WM->getMyNum() !=10 && WM->getMyNum()!=11)
    {

        if ( shouldClear ( x,y,s ))
        {
            Log.Log ( 2 , "Shoooooooot va gooooooooooooooooooooooal !" );
            ACT->setCurrentAct( K, s , x, y  );
            tFinal=0;
        }
        ////isballkickable//////////
        else{
            Log.Log ( 2 , "ball is kickable !" );
            return moveToPosP(true,ball-VecPosition(0.2,0),tFinal);

        }
    }
    else if(me.getDistanceTo(attackpositioning())>0.1 && shouldPlay2()  && WM->getMyNum() !=10 && WM->getMyNum()!=11){
        Vector3f vec(attackpositioning().getX(),attackpositioning().getY(),0);
        if(WM->getMyNum()==11)
            RVDraw::instance()->drawVector3f(vec,BLUE,234423);
        return moveToPosP(false,attackpositioning(),tFinal);
    }
    else if(fabs(WM->getMyAngleTo(WM->getBallPos()))>15 && shouldPlay2())
    {
        cout <<"manam hastam\n";
        if(WM->getMyAngleTo(myPos+Vector3f(1,0,0))>0)
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
//        return SK->finalAction( "darja" ,tFinal);
        return "";
    }
//    static int ttt = 0 ;
//    ttt++;
//    ss << SK->moveJoint( "he1" ,( ttt % 100 < 50 ) ? -0.5 : 0.5  ) << SK->moveJoint( "he2" ,( ttt % 100 < 50 ) ? -0.5 : 0.5  ) ;
//    if (ttt < 80 )
//        ss << "";
//    else if ( ttt < 100 )
//        ss << SK->moveJoint("rle2" ,  -1) << SK->moveJoint("rle6" , 1) <<
//              SK->moveJoint("lle2" , -1) << SK->moveJoint("lle6" ,1) ;
//    else if ( ttt < 140 )
//      ss << SK->moveJointTo("he1" , -100 , 3) << SK->moveJointTo("he2" , -40 , 3) ;
    //////////////////////////////////////////////////////////////////
//    cout << WM->getGyro() << endl;
    return ss.str();
}

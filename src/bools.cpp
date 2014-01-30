/*
   RRRR    OOOOO   BBBBB    OOOOO  TTTTTTTT  OOOOO   OOOOO   SSSSS
   R  R    O   O   B    B   O   O     T      O   O   O   O  S
   RRRR    O   O   BBBBB    O   O     T      O   O   O   O   SSSS
   R  R    O   O   B    B   O   O     T      O   O   O   O       S
   R   R   OOOOO   BBBBB    OOOOO     T      OOOOO   OOOOO  SSSSS
*/
#include "Decide.h"

bool Decide::isBackOfBall(VecPosition pos)
{
    VecPosition ball(WM->getBallPos().x(), WM->getBallPos().y());
    VecPosition posGoal(11.5,0);
    double shib=-1/((ball.getY()-posGoal.getY())/(ball.getX()-posGoal.getX()));
    double moadele=pos.getY()-ball.getY()-(shib*pos.getX())+(shib*ball.getX());
    static bool b=true;
    if(pos.getDistanceTo(ball)<0.15 && shouldPlay())
    {
        b=false;
    }
    if(b)
    {
        return true;
    }

    if(moadele<0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
double Decide::goaliepositioning()
{
    VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
    VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
    Line t1=Line::makeLineFromTwoPoints(VecPosition(-WM->getFieldLength()/2,1.05),VecPosition(0,WM->getFieldWidth()/2));
    Line t2=Line::makeLineFromTwoPoints(VecPosition(-WM->getFieldLength()/2,-1.05),VecPosition(0,-WM->getFieldWidth()/2));
    Line Lball=Line::makeLineFromPositionAndAngle(ball,90);
    VecPosition pos1=t1.getIntersection(Lball);
    VecPosition pos2=t2.getIntersection(Lball);
    double dist;
    if(fabs(pos1.getY()-pos2.getY())<fabs(ball.getY()-pos2.getY()))
    {
        return 1.02;
    }
    if(fabs(pos1.getY()-pos2.getY())<fabs(ball.getY()-pos1.getY()))
    {
        return -1.02;
    }
    if(fabs(pos1.getY()-ball.getY())<fabs(pos2.getY()-ball.getY()))
    {
        dist=fabs(pos1.getY()-ball.getY());
    }
    else
    {
        dist=fabs(pos2.getY()-ball.getY());
    }
    double y=dist*2.1/fabs(pos1.getY()-pos2.getY());
    double f;
    if(fabs(pos1.getY()-ball.getY())<fabs(pos2.getY()-ball.getY()))
    {
        f=1.05-y;
    }
    else
    {
        f=-1.05+y;
    }
    return f;
}




bool Decide::shouldClear(double &dx, double &dy,SideT &side)
{
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());
    double myAngle = WM->getMyAngle();
    double angleToBall = fabs(WM->getMyAngleToBall());
    VecPosition start=VecPosition::givePosition(me,VecPosition::normalizeAngle(WM->getMyAngle()),+0.14);
    VecPosition end=VecPosition::givePosition(start,VecPosition::normalizeAngle(WM->getMyAngle()),+0.09);
    bool con=isInCone(45,start,end,ball);
    Rect P_A(VecPosition(15.1,1.1),VecPosition(13.2,-1.1));
    Rect O_P_A(VecPosition(-15,1.1),VecPosition(-13.2,-1.1));
    VecPosition p1,p2;
    string tname = WM->getOppName();
    transform ( tname.begin(), tname.end(), tname.begin(), ptr_fun(::tolower) );
    //cerr << tname << endl;
    //if( tname.find("kar") != string::npos && tname.find("s2")!= string::npos && fabs(ball.getX())<4)
        //return false;
    if ( angleToBall < 0 )
        side = Right;
    else
        side = Left;
    if (fabs(myAngle) < 70  && me.getDistanceTo(ball) < 0.20 && me.getDistanceTo(ball) > 0.16 && angleToBall < 20 && con && ball.getX()<13 && !WM->isOppInCircle(Circle(ball,2.5),p1,p2) && !shback() && fabs(WM->getMyAngleToGoal())<30)
    {
        return true;
    }
    if(startShoot())
        return true;
    return false;
}
bool Decide::startShoot(){
    if(WM->getMyNum()==9 && WM->getTeamSide()==Left && (WM->getPlayMode()==PM_BeforeKickOff || WM->getPlayMode()==PM_KickOff_Left || WM->getPlayMode()==PM_Goal_Left) && WM->getMyPos().x()<-0.15)
        return true;
    if(WM->getMyNum()==9 && WM->getTeamSide()==Right && (WM->getPlayMode()==PM_BeforeKickOff || WM->getPlayMode()==PM_KickOff_Right || WM->getPlayMode()==PM_Goal_Right) && WM->getMyPos().x()<-0.15)
        return true;
    return false;
}

bool Decide::shouldPlay()
{
    if (WM->getPlayMode() ==  PM_PlayOn )
    {
        return true;
    }
    if(WM->getPlayMode() == PM_Goal_Left || WM->getPlayMode() == PM_Goal_Right )
    {
        return false;
    }
    if (WM->getTeamSide() == Left )
    {
        switch ( WM->getPlayMode() )
        {
        case PM_KickOff_Left :
        case PM_KickIn_Left :
        case PM_CORNER_KICK_LEFT :
        case PM_GOAL_KICK_LEFT :
        case PM_OFFSIDE_LEFT:
        case PM_Goal_Left :
        case PM_FREE_KICK_LEFT :
            return true;
            break;

        }

    }
    if (WM->getTeamSide() == Right)
    {
        switch ( WM->getPlayMode() )
        {
        case PM_KickOff_Right :
        case PM_KickIn_Right :
        case PM_CORNER_KICK_RIGHT :
        case PM_GOAL_KICK_RIGHT :
        case PM_OFFSIDE_RIGHT:
        case PM_Goal_Right :
        case PM_FREE_KICK_RIGHT :
            return true;
            break;
        }
    }
    return false;
}

bool Decide::canBeam()
{
    if (WM->getPlayMode() == PM_BeforeKickOff || WM->getPlayMode() == PM_Goal_Left || WM->getPlayMode() == PM_Goal_Right )
    {
        return true;
    }
    return false;
}

void Decide::setBeamed()
{
    beamed = true;
}

bool Decide::isBeamed()
{
    return beamed;
}

bool Decide::shouldStandUp(Command &com, SideT &side)
{
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());

    if (!WM->isFeltDown())
    {
        return false;
    }
    if (WM->feltType() == Back)
    {
        com = SUB;
    }
    else if (WM->feltType() == Front)
    {
        com = SUF;
    }
    else
    {
        com = DP;
    }
    side = WM->feltType();
    return true;

    return false;
}
bool Decide::shouldSSideWalk(SideT &side)
{
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());
    VecPosition goalPos;
    if (fabs(me.getY()) < 0.7)
    {
        goalPos = VecPosition(11.5, me.getY());
    }
    else
    {
        goalPos = VecPosition(11.5, (0.7 * sign(me.getY())));
    }
    double myAngle = WM->getMyAngle();
    Line ballToGoal = Line::makeLineFromTwoPoints(ball, goalPos);
    Line goalLine = Line::makeLineFromPositionAndAngle(goalPos, 90);
    Line meToBall = Line::makeLineFromTwoPoints(me, ball);
    VecPosition intersect = goalLine . getIntersection(meToBall);
    if (fabs(intersect . getY()) < 0.7)
    {
        return false;
    }
    double myDistToLine = ballToGoal . getDistanceWithPoint(me);
    double ballToGoalAngle = VecPosition::normalizeAngle((goalPos - ball) . getDirection());
    double meToGoalAngle = VecPosition::normalizeAngle((goalPos - me) . getDirection());
    ballToGoalAngle > meToGoalAngle ? side = Right : side = Left;
    if (myDistToLine > 0.16 && myDistToLine < 0.22 && fabs(myAngle) < 90 && me.getDistanceTo(ball) < 0.4)
    {
        return true;
    }
    return false;
}

bool Decide::isInCone(double dWidth, VecPosition start , VecPosition end, VecPosition pos)
{
    Line line = Line::makeLineFromTwoPoints( start, end );
    VecPosition posOnLine;
    posOnLine = line.getPointOnLineClosestTo( pos );
    if (  posOnLine.getDistanceTo(pos) < dWidth*posOnLine.getDistanceTo(start)
            && line.isInBetween( posOnLine, start, end )
            && start.getDistanceTo( pos ) < start.getDistanceTo( end ) )
    {
        return true;
    }
    return false;
}
///////////////////////////////////////////////////////////////////////
bool Decide::objectIsInCone(double dWidth, VecPosition start , VecPosition end,Vector3f &posobj, object ob)
{
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());
    VecPosition pos=ball;
    bool result=false;
    int  numopp[10];
    int  numteam[10];
    int  ko=0,kt=0;
    if(ob==OBJECT_OPPONENTS || ob==OBJECT_OPPONENTS_NO_GOALIE)
    {
        int i=1;
        if(ob==OBJECT_OPPONENTS_NO_GOALIE)
            i=2;
        for(;i<=WM->getOurPlayerCount();i++)
        {
            pos=VecPosition(WM->getOppPos(i).x(),WM->getOppPos(i).y());
            if(int(WM->getOppPos(i).x())==0 && int(WM->getOppPos(i).y())==0 && int(WM->getOppPos(i).z())==0 )
                continue;
            bool cone=isInCone(dWidth,start,end,pos); 
            if(cone)
            {
                numopp[ko]=i;
                ko++;
                result=cone;
            }
        }
    }
    else if(ob==OBJECT_TEAMMATES || ob==OBJECT_OPPONENTS_NO_GOALIE)
    {
        int i=1;
        if(ob==OBJECT_TEAMMATES_NO_GOALIE)
            i=2;
        for(int i=1;i<=WM->getOurPlayerCount();i++)
        {
            pos=VecPosition(WM->getOurPos(i).x(),WM->getOurPos(i).y());
            if((int(WM->getOurPos(i).x())==0 && int(WM->getOurPos(i).y())==0 && int(WM->getOurPos(i).z())==0) || i==WM->getMyNum())
                continue; 
            bool cone=isInCone(dWidth,start,end,pos); 
            if(cone)
            {
                numteam[kt]=i;
                kt++;
                result=cone;
            }
        }
    }
    else if(ob==OBJECT_PLAYERS || ob==OBJECT_PLAYERS_NO_GOALIE)
    {
        int i=1,j=1;
        if(ob=OBJECT_PLAYERS_NO_GOALIE)
            i=j=2;
        for(;i<=WM->getOurPlayerCount();i++)
        {
            pos=VecPosition(WM->getOppPos(i).x(),WM->getOppPos(i).y());
            if(int(WM->getOppPos(i).x())==0 && int(WM->getOppPos(i).y())==0 && int(WM->getOppPos(i).z())==0 )
                continue; 
            bool cone=isInCone(dWidth,start,end,pos); 
            if(cone)
            {
                numopp[ko]=i;
                ko++;
                result=cone;
            }
        }
        for(;j<=WM->getOurPlayerCount();j++)
        {
            pos=VecPosition(WM->getOurPos(j).x(),WM->getOurPos(j).y()); 
            if((int(WM->getOurPos(j).x())==0 && int(WM->getOurPos(j).y())==0 && int(WM->getOurPos(j).z())==0) || j==WM->getMyNum()){
                continue;
            }
            bool cone=isInCone(dWidth,start,end,pos); 
            if(cone)
            {
                numteam[kt]=i;
                kt++;
                result=cone;
            }
        }
    }
    else 
    {
        bool cone=isInCone(dWidth,start,end,ball); 
        if(cone)
            result=cone;
    }
    
    int o=minDistToMeInArray(numopp,ko,true);
    int t=minDistToMeInArray(numteam,kt,false);
    posobj=Vector3f(0,0,0);
    if(o!=0 && t!=0){
        if(me.getDistanceTo(VecPosition(WM->getOppPos(o).x(),WM->getOppPos(o).y()))<me.getDistanceTo(VecPosition(WM->getOurPos(t).x(),WM->getOurPos(t).y()))   )
            posobj=WM->getOppPos(o);
        else
            posobj=WM->getOurPos(t);
    }
    else if(o!=0)
        posobj=WM->getOppPos(o);
    else if(t!=0)
        posobj=WM->getOurPos(t);
    return result;
}
int Decide::minDistToMeInArray(int a[],int len,bool opp)
{   VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
    double mindist=1000000;
    int num=0;
    if(opp)
    {
        for(int i=0;i<len;i++)
        {
            VecPosition ph=VecPosition(WM->getOppPos(a[i]).x(),WM->getOppPos(a[i]).y());
            if(ph.getDistanceTo(me)<mindist)
            {
                mindist=ph.getDistanceTo(me);
                num=a[i];
            }    
        }
    }
    else{
        for(int i=0;i<len;i++)
        {
            VecPosition ph=VecPosition(WM->getOurPos(a[i]).x(),WM->getOurPos(a[i]).y());
            if(ph.getDistanceTo(me)<mindist)
            {
                mindist=ph.getDistanceTo(me);
                num=a[i];
            }    
        }
    }
    return num;
}
////////////////////////////////////////////////////////////////////////



int Decide::parseBallPos ( )
{
    string str = WM->getLastMsg() ;
    //cout << "AS " << str << endl;
    if(atoi ( str.c_str() )==0)
    {
        //cout<<"hello\n ";
        return WM->getClosestOurToBall();
    }
    return min(9,max(1,atoi ( str.c_str() ))) ;
}



bool Decide::shSide(string &side)
{
    VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
    VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
    Line meAngle=Line::makeLineFromPositionAndAngle(ball,(VecPosition(WM->getFieldLength()/2+1,0)-ball).getDirection());
    Line goalL  =Line::makeLineFromPositionAndAngle(me,VecPosition::normalizeAngle(WM->getMyAngle()+90));
    VecPosition pos=meAngle.getIntersection(goalL);
    pos=meAngle.getPointOnLineClosestTo(me);
    if(me.getDistanceTo(pos)>0.056)
    {
        if((me-pos).getDirection()>0)
        {
            side="sideWalkL";
        }
        else
        {
            side="sideWalkR";
        }
        return true;
    }
    return false;
}
///vaghti ke toop posht paye bazikon bashad
bool Decide::shSide2(string &side)
{
    VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
    VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
    VecPosition a1=VecPosition::givePosition(me,VecPosition::normalizeAngle(WM->getMyAngle()),-0.05);
    VecPosition a2=VecPosition::givePosition(a1,VecPosition::normalizeAngle(WM->getMyAngle()),-0.13);
    VecPosition a3=VecPosition::givePosition(a1,VecPosition::normalizeAngle(WM->getMyAngle()+90),+0.1);
    VecPosition a4=VecPosition::givePosition(a2,VecPosition::normalizeAngle(WM->getMyAngle()+90),-0.1);
    side="sideWalkR";
    if(isInCone(60,a1,a2,ball))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Decide::shback()
{
    VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
    VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
    VecPosition a1=VecPosition::givePosition(me,VecPosition::normalizeAngle(WM->getMyAngle()),+0.03);
    VecPosition a2=VecPosition::givePosition(a1,VecPosition::normalizeAngle(WM->getMyAngle()+90),+0.1);
    VecPosition a3=VecPosition::givePosition(a2 ,VecPosition::normalizeAngle(WM->getMyAngle()+90),+0.16);

    VecPosition b1=VecPosition::givePosition(me,VecPosition::normalizeAngle(WM->getMyAngle()),+0.03);
    VecPosition b2=VecPosition::givePosition(b1,VecPosition::normalizeAngle(WM->getMyAngle()+90),-0.1);
    VecPosition b3=VecPosition::givePosition(b2,VecPosition::normalizeAngle(WM->getMyAngle()+90),-0.16);
    if(isInCone(60,a2,a3,ball))
    {
        return true;
    }
    if(isInCone(60,b2,b3,ball))
    {
        return true;
    }
    return false;
}
///this is the professional move to pos
string Decide::moveToPosP(bool nearball,VecPosition endpos,double &tFinal){
    VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
    VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
    Vector3f    endPos(endpos.getX(),endpos.getY(),0);
    Vector3f    posincone(0,0,0);
    VecPosition frontpos=VecPosition::givePosition(me,VecPosition::normalizeAngle(WM->getMyAngle()),0.6);
    bool        frontcon=objectIsInCone(tanDeg(70),me,frontpos,posincone,OBJECT_PLAYERS_NO_GOALIE);
    frontcon=false;
    static bool n1=true,before=true;
    if(me.getDistanceTo(ball)<0.2)
    {
        before=false;
    }
    if(me.getDistanceTo(ball)>1.8)
    {
        before=true;
    }
    if(me.getDistanceTo(ball)<1.5)
    {
        n1=true;
    }
    if(me.getDistanceTo(ball)>2)
    {
        n1=false;
    }
    if(before && nearball && me.getX()<4)
    {
        //cout<<"mash ghanbar oomad\n";
        return mvPos(ball,true,tFinal,WM->getMyAngleToBall(),n1);
    }
    ///positioning
    if(!nearball)
    {
        //cout<<"positioning\n";
        return moveToPosition(endpos,tFinal);
    }
    ///fasle door az toop
    if(me.getDistanceTo(endpos)>2.5)
    {
        //cout<<"bala\n";
        if(fabs(WM->getMyAngleTo(endPos))>45)
        {
            if(WM->getMyAngleTo(endPos)<0)
                return SK->finalAction("turnR",tFinal);
            else 
                return SK->finalAction("turnL",tFinal);
        }
        else if(frontcon)
        {
            
            if(WM->getMyAngleTo(posincone)>0)
            {
                //cout<<"door zadan1\n";
                if(me.getDistanceTo(VecPosition(posincone.x(),posincone.y()))<0.3)
                    return SK->finalAction("turnR",tFinal);
                return SK->finalAction("walkAngleR",tFinal);
            }
            else 
            {   //cout<<"door zadan100\n";
                if(me.getDistanceTo(VecPosition(posincone.x(),posincone.y()))<0.3)
                    return SK->finalAction("turnL",tFinal);
                return SK->finalAction("walkAngleL",tFinal);
            }
        }
        else if(fabs(WM->getMyAngleTo(endPos))>20)
        {
            if(WM->getMyAngleTo(endPos)<0)
                return SK->finalAction("walkAngleR",tFinal);
            else 
                return SK->finalAction("walkAngleL",tFinal);
        }
        else
            return SK->finalAction("walk",tFinal);
    }
    
    ///nazdik toop
    else if(nearball)
    {
        //cout<<"nearball\n";

        return moveToNearBall(tFinal);
    }
}
string Decide::moveToPosition(VecPosition endpos,double &tFinal){
    VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
    VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
    Vector3f    endPos(endpos.getX(),endpos.getY(),0);
    Vector3f    posincone(0,0,0);
    VecPosition frontpos=VecPosition::givePosition(me,VecPosition::normalizeAngle(WM->getMyAngle()),0.7);
    bool        frontcon=objectIsInCone(tanDeg(70),me,frontpos,posincone,OBJECT_PLAYERS_NO_GOALIE);
    frontcon=false;
    static bool dist2=true;
    double dist;
    static bool angjolo=true,door=true;
    static bool setx=true;
    if(me.getDistanceTo(endpos)>0.6)
        dist=0.4;
    if(me.getDistanceTo(endpos)<0.4)
        dist=0.2;
    if(fabs(me.getX()-endpos.getX())>dist)
        setx=true;
    if(fabs(me.getX()-endpos.getX())<0.1)
        setx=false;
    if(me.getDistanceTo(endpos)>0.4)
        dist2=true;
    if(me.getDistanceTo(endpos)<0.2)
        dist2=false;
    if(fabs(WM->getMyAngleTo(WM->getMyPos()+Vector3f(0.5,0,0)))>25)
        angjolo=true;
    if(fabs(WM->getMyAngleTo(WM->getMyPos()+Vector3f(0.5,0,0)))<5)
        angjolo=false;
    if(me.getDistanceTo(endpos)>3)
        door=true;
    if(me.getDistanceTo(endpos)<1)
        door=false;
        
    if(door)
    {
        if(fabs(WM->getMyAngleTo(endPos))>45)
        {
            if(WM->getMyAngleTo(endPos)<0)
                return SK->finalAction("turnR",tFinal);
            else 
                return SK->finalAction("turnL",tFinal);
        }
        else if(frontcon)
        {
            //cout<<"door zadan2\n";
            if(WM->getMyAngleTo(posincone)>0)
            {
                if(me.getDistanceTo(VecPosition(posincone.x(),posincone.y()))<0.3)
                    return SK->finalAction("turnR",tFinal);
                return SK->finalAction("walkAngleR",tFinal);
            }
            else 
            {
                if(me.getDistanceTo(VecPosition(posincone.x(),posincone.y()))<0.3)
                    return SK->finalAction("turnL",tFinal);
                return SK->finalAction("walkAngleL",tFinal);
            }
        }
        else if(fabs(WM->getMyAngleTo(endPos))>20)
        {
            if(WM->getMyAngleTo(endPos)<0)
                return SK->finalAction("walkAngleR",tFinal);
            else 
                return SK->finalAction("walkAngleL",tFinal);
        }
        else
            return SK->finalAction("walk",tFinal);
    }
    else 
    {            
        if(angjolo && dist2)
        {
            //cout<<"turn1\n";
            if(WM->getMyAngleTo(WM->getMyPos()+Vector3f(0.5,0,0))<0)
                return SK->finalAction("turnR",tFinal);
            else
                return SK->finalAction("turnL",tFinal);
        }
        else if(setx && me.getX()>endpos.getX() && dist2)
        {
            //cout<<"bwalk\n";
            return SK->finalAction("bwalk",tFinal);
        }
        else if(setx && me.getX()<endpos.getX() && dist2)
        {
            //cout<<"walk\n";
            return SK->finalAction("walk",tFinal);
        }
        else if(fabs(me.getY()-endpos.getY())>0.1 && dist2)
        {
            //cout<<"sidewalk\n";
            if(me.getY()<endpos.getY())
                return SK->finalAction("sideWalkR",tFinal);
            else 
                return SK->finalAction("sideWalkL",tFinal);
        }
        else if(fabs(WM->getMyAngleToBall())>10 && !dist2)
        {
            //cout<<"turn2\n";
            if(WM->getMyAngleToBall()<0)
                return SK->finalAction("turnR",tFinal);
            else 
                return SK->finalAction("turnL",tFinal);
        }
        else{
            //cout<<"darja\n";
            return SK->finalAction("",tFinal);
        }
    }
}



string Decide::moveToNearBall(double &tFinal)
{
    VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
    VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
    Vector3f    posincone(0,0,0);
    VecPosition frontpos=VecPosition::givePosition(me,VecPosition::normalizeAngle(WM->getMyAngle()),0.6);
    bool        frontcon=objectIsInCone(tanDeg(70),me,frontpos,posincone,OBJECT_PLAYERS_NO_GOALIE);
    double      ang=fabs(WM->getMyAngleTo(Vector3f(14.5,0,0)));
    double      ang2=fabs(WM->getMyAngleTo(Vector3f(15.5,0,0)));
    double      angGoal;
    if(ball.getX()>13 && fabs(ball.getY())<1.2)
        angGoal=ang2;
    else
        angGoal=ang;
    frontcon=false;
    static bool nb=true,ng=true;
    double zarib1=120/(me.getX()/3);
    if(me.getX()<0 || zarib1>110)
        zarib1=110;
    if(zarib1<60 && me.getX()<12)
        zarib1=60;
    double zarib2;
    if(me.getX()<10)
        zarib2=40;
    if(me.getX()>12)
        zarib2=20;
    //cout<<" zarib1 = "<<zarib1<<endl;
    //cout<<" zarib2 = "<<zarib2<<endl;
    //cout<< "--------------------"<<endl;    
    
    if(me.getDistanceTo(ball)<0.4)//besyar nazdik be toop
        nb=true;
    else if(me.getDistanceTo(ball)>0.6 || (me.getX()-0.03)>ball.getX())//kami door az toop
        nb=false;
    if(angGoal>zarib1 || (fabs(ball.getY())>4 && fabs(WM->getMyAngleToGoal())>35))
        ng=true;
    else if(angGoal<zarib2)
        ng=false;        
    if(shback())
    {
        return SK->finalAction("bwalk",tFinal);
    }    
    else if(!nb)//kami door az toop
    {
        if(ball.getX()+0.02>me.getX())//agar posht toop bashe
        {
            if(fabs(WM->getMyAngleToBall())>25)
            {
                if(WM->getMyAngleToBall()<0)
                    return SK->finalAction("turnR",tFinal);
                else 
                    return SK->finalAction("turnL",tFinal);
            }
            else if(frontcon)
            {
                //cout<<"door zadan3\n";
                if(WM->getMyAngleTo(posincone)>0)
                {
                    if(me.getDistanceTo(VecPosition(posincone.x(),posincone.y()))<0.3)
                        return SK->finalAction("turnR",tFinal);
                    return SK->finalAction("walkAngleR",tFinal);
                }
                else 
                {
                    if(me.getDistanceTo(VecPosition(posincone.x(),posincone.y()))<0.3)
                        return SK->finalAction("turnL",tFinal);
                    return SK->finalAction("walkAngleL",tFinal);
                }
            }
            else if(fabs(WM->getMyAngleToBall())>15)
            {
                if(WM->getMyAngleToBall()<0)
                    return SK->finalAction("walkAngleR",tFinal);
                else 
                    return SK->finalAction("walkAngleL",tFinal);
            }
            else 
                return SK->finalAction("walk",tFinal,1.5);
        }
        else //toop posht bazikon
        {//moshkel dare felan
            if(fabs(WM->getMyAngleToBall())<90)//ro be toop
            {
                if(fabs(WM->getMyAngleToBall())>30)
                {
                    if(WM->getMyAngleToBall()<0)
                        return SK->finalAction("turnR",tFinal);
                    else 
                        return SK->finalAction("turnL",tFinal);
                }
                else if(frontcon)
                {
                    //cout<<"door zadan4\n";
                    if(WM->getMyAngleTo(posincone)>0)
                    {
                        if(me.getDistanceTo(VecPosition(posincone.x(),posincone.y()))<0.3)
                            return SK->finalAction("turnR",tFinal);
                        return SK->finalAction("walkAngleR",tFinal);
                   }
                    else 
                    {
                        if(me.getDistanceTo(VecPosition(posincone.x(),posincone.y()))<0.3)
                            return SK->finalAction("turnL",tFinal);
                        return SK->finalAction("walkAngleL",tFinal);
                    }
                }
                else if(fabs(WM->getMyAngleToBall())>15)
                {
                    if(WM->getMyAngleToBall()<0)
                        return SK->finalAction("walkAngleR",tFinal);
                    else 
                        return SK->finalAction("walkAngleL",tFinal);
                }
                else 
                    return SK->finalAction("walk",tFinal,1.5);
            }
            else //posht be toop
            {
                if(fabs(WM->getMyAngleToBall())<150)
                {
                    if(WM->getMyAngleToBall()>0)
                        return SK->finalAction("turnR",tFinal);
                    else 
                        return SK->finalAction("turnL",tFinal);
                }
                else if(fabs(WM->getMyAngleToBall())<175)
                {
                    if(WM->getMyAngleToBall()>0)
                        return SK->finalAction("bwalkAngleR",tFinal);
                    else 
                        return SK->finalAction("bwalkAngleL",tFinal);
                }
                else 
                    return SK->finalAction("bwalk",tFinal);
            }
        }
    }
    
    else if(nb)//besyar nazdik be toop
    {
        cout <<"sideturn\n";
        if(ng) //az zaviye had rad shode
        {
            if(WM->getMyAngleToGoal()<0)
                return SK->finalAction("sideTurnR",tFinal);
            else 
                return SK->finalAction("sideTurnL",tFinal);
        } 
        else
        {
            if(fabs(WM->getMyAngleToBall())>20)
            {
                if(WM->getMyAngleToBall()<0)
                    return SK->finalAction("turnR",tFinal);
                else 
                    return SK->finalAction("turnL",tFinal);
            }
            else if(fabs(WM->getMyAngleToBall())>10)
            {
                if(WM->getMyAngleToBall()<0)
                    return SK->finalAction("walkAngleR",tFinal);
                else 
                    return SK->finalAction("walkAngleL",tFinal);
            }
            else 
                return SK->finalAction("walk",tFinal,1.5);
        }
    }
    else 
        return SK->finalAction("",tFinal,1.5);
}





















/// /////    positioning //// ////


VecPosition Decide::defendpositioning()
{
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());
    VecPosition posgoal(-WM->getFieldLength()/2+0.5,0);
    Line l1=Line::makeLineFromPositionAndAngle(posgoal,90);
    Line l2=Line::makeLineFromPositionAndAngle(ball,(ball-VecPosition(-WM->getFieldLength()/2,0)).getDirection());
    VecPosition intersect=l1.getIntersection(l2);
    VecPosition end=VecPosition::givePosition(intersect,(ball-VecPosition(-WM->getFieldLength()/2,0)).getDirection(),intersect.getDistanceTo(ball)/2.8);
    if(end.getX()>-3.5)
        end.setX(-3.5);
    if(end.getX()<-WM->getFieldLength()/2+1)
        end.setX(-WM->getFieldLength()/2+1);
    if(end.getY()>WM->getFieldWidth()/2-3)
        end.setY(WM->getFieldWidth()/2-3);
    if(end.getY()<-WM->getFieldWidth()/2+3)
        end.setY(-WM->getFieldWidth()/2+3);
    VecPosition endp=VecPosition(end.getX(),end.getY()+1.5);
    VecPosition endm=VecPosition(end.getX(),end.getY()-1.5);
    VecPosition endj=VecPosition::givePosition(end,(ball-VecPosition(-11.5,0)).getDirection(),2);
    VecPosition endjp=VecPosition::givePosition(endp,(ball-VecPosition(-11.5,0)).getDirection(),2);
    VecPosition endjm=VecPosition::givePosition(endm,(ball-VecPosition(-11.5,0)).getDirection(),2);
    if(ball.getY()<0)
    {
        if(WM->getMyNum()==3 || WM->getMyNum()==5){
            if(WM->getMyNum()==3 && parseBallPos()==5)
                return end;
            else if(WM->getMyNum()==5 && parseBallPos()==3)
                return end;
            else if(WM->getMyNum()==3)
                return end;
            else
                return endj;
        }
        else if(WM->getMyNum()==2 || WM->getMyNum()==4){
            if(WM->getMyNum()==2 && parseBallPos()==4)
                return endp;
            else if(WM->getMyNum()==2 && parseBallPos()==1)
                return endp;
            else if(WM->getMyNum()==2)
                return endp;
            else
                return endjp;
        }
    }
    else
    {
        if(WM->getMyNum()==3 || WM->getMyNum()==5){
            if(WM->getMyNum()==3 && parseBallPos()==5)
                return endm;
            else if(WM->getMyNum()==5 && parseBallPos()==3)
                return endm;
            else if(WM->getMyNum()==3)
                return endm;
            else
                return endjm;
        }
        else if(WM->getMyNum()==2 || WM->getMyNum()==4){
            if(WM->getMyNum()==2 && parseBallPos()==4)
                return end;
            else if(WM->getMyNum()==2 && parseBallPos()==1)
                return end;
            else if(WM->getMyNum()==2)
                return end;
            else
                return endj;
        }
    }
}


VecPosition Decide::middlepositioning(){
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());
    VecPosition posgoal(-WM->getFieldLength()/2+3,0);
    Line l1=Line::makeLineFromPositionAndAngle(posgoal,90);
    Line l2=Line::makeLineFromPositionAndAngle(ball,(ball-VecPosition(-WM->getFieldLength()/2,0)).getDirection());
    VecPosition intersect=l1.getIntersection(l2);
    VecPosition end=VecPosition::givePosition(intersect,(ball-VecPosition(-WM->getFieldLength()/2,0)).getDirection(),intersect.getDistanceTo(ball)/2);
    if(end.getX()>2)
        end.setX(2);
    if(end.getX()<-7)
        end.setX(-7);
    if(end.getY()>5.5)
        end.setY(5.5) ;
    if(end.getY()<-5.5)
        end.setY(-5.5);
    VecPosition end2=VecPosition(end.getX()+2,end.getY());
    //cout<<end<<"  hello   "<<end2 <<endl;
    if(WM->getMyNum()==6)
        return end;
    if(WM->getMyNum()==7)
        return end2;
}


VecPosition Decide::attackpositioning(){
    myPos = WM->getMyPos();
    ballPos = WM->getBallPos();
    VecPosition me(myPos.x(), myPos.y());
    VecPosition ball(ballPos.x(), ballPos.y());
    VecPosition posgoal(-WM->getFieldLength()/4,0);
    Line l1=Line::makeLineFromPositionAndAngle(posgoal,90);
    Line l2=Line::makeLineFromPositionAndAngle(ball,0);
    VecPosition intersect=l1.getIntersection(l2);
    VecPosition end=VecPosition::givePosition(intersect,0,(intersect.getDistanceTo(ball)/1.02));
    if(end.getX()>10)
        end.setX(10);
    if(end.getX()<-7)
        end.setX(-7);
    if(end.getY()>6)
        end.setY(6) ;
    if(end.getY()<-6)
        end.setY(-6);
    if(ball.getX()<-8)
        end.setX(-8);
    VecPosition end2=VecPosition(end.getX()-1,end.getY());
    VecPosition end3=VecPosition(end.getX()-0.7,end.getY()+1*sign(end.getY()));
    if(WM->getMyNum()==8 && parseBallPos()==9)
        return end2;
    if(WM->getMyNum()==9 && parseBallPos()==8)
        return end2;
    if(WM->getMyNum()==8)
        return end3;
    if(WM->getMyNum()==9)
        return end;
}

//VecPosition Decide::attackpositioning(int closest){
//    myPos = WM->getMyPos();
//    ballPos = WM->getBallPos();
//    VecPosition me(myPos.x(), myPos.y());
//    VecPosition ball(ballPos.x(), ballPos.y());
//    VecPosition posGoal(WM->getFieldLength()/2,0);
//    Line l1 = Line::makeLineFromTwoPoints(ball,posGoal);

//    VecPosition vec[3];
//    if(ball.getX()<-5)
//        vec[0].setX(-2);
//    else
//        vec[0].setX(ball.getX()-3);
//    vec[0].setY(ball.getY()/1.2);
//    vec[1] = VecPosition(vec[0].getX(),vec[0].getY()+2);
//    vec[2] = VecPosition(vec[0].getX(),vec[0].getY()-2);
//    bool player[3] = {0};
//    int j=0;
//    if(closest==8 || closest==9 || closest==11)
//        j =1;
//    for(;j<3;j++){
//        double min = 110000;
//        int num = 0;
//        for(int i=7;i<11;i++){
//            if(closest==i+1)
//                continue;
//            VecPosition our(WM->getOurPos(i,"head").x(),WM->getOurPos(i,"head").y());
//            if(min>our.getDistanceTo(vec[j]) && !player[i-7]){
//                min = our.getDistanceTo(vec[j]);
//                num = i;
//            }
//        }
//        player[num-7] = 1;
//        if(num+1==WM->getMyNum())
//            return vec[j];
//    }
//}












string Decide::mvPos(VecPosition pos,bool isball,double &tFinal,double ang,bool n1)
{

    VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
    VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
    Line l1=Line::makeLineFromTwoPoints(me,ball);
    Line l2=Line::makeLineFromTwoPoints(VecPosition(WM->getFieldLength()/2+1,1),VecPosition(WM->getFieldLength()/2+1,-1));
    VecPosition sect=l1.getIntersection(l2);
    VecPosition start=VecPosition::givePosition(me,VecPosition::normalizeAngle(WM->getMyAngle()),+0.12);
    VecPosition end=VecPosition::givePosition(start,VecPosition::normalizeAngle(WM->getMyAngle()),+0.09);
    string side;
    Rect P_A(VecPosition(WM->getFieldLength()/2+1.1,1.05),VecPosition(WM->getFieldLength()/2-1,-1.05));
    Rect R_P_A(VecPosition(WM->getFieldLength()/2+1.2,1.05),VecPosition(WM->getFieldLength()/2-1,-WM->getFieldWidth()/2-0.2));
    Rect L_P_A(VecPosition(WM->getFieldLength()/2+1.2,-1.05),VecPosition(WM->getFieldLength()/2-1,WM->getFieldWidth()/2+0.2));
    //static bool n2=true;
    /*if(me.getDistanceTo(pos)>2.5 || (me.getX()<ball.getX() && isball))
        n2=false;
    if(me.getDistanceTo(pos)<2 || !isball)
        n2=true;*/
    if(!n1)
    {
n1:
        if(me.getDistanceTo(pos)>0.4 && fabs(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0)))<35)
        {
            //cout<<"! n11\n";
            if(fabs(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0)))>10)
            {
                if(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0))<0)
                {
                    return SK->finalAction("walkAngleR",tFinal);
                }
                else
                {
                    return SK->finalAction("walkAngleL",tFinal);
                }
            }
            else
            {
                return SK->finalAction("walk",tFinal);
            }
        }
        else if(fabs(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0)))>30 && fabs(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0)))<90)
        {
            //cout<<"! n12\n";
            if(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0))>0)
            {
                return SK->finalAction("turnL",tFinal);
            }
            else
            {
                return SK->finalAction("turnR",tFinal);
            }
        }
        else if(me.getDistanceTo(pos)>0.4 && fabs(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0)))>145 && fabs(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0)))<180)
        {
            //cout<<"! n13\n";
            if(fabs(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0)))>25)
            {
                if(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0))>0)
                {
                    return SK->finalAction("bwalkAngleL",tFinal);
                }
                else
                {
                    return SK->finalAction("bwalkAngleR",tFinal);
                }
            }
            else
            {
                return SK->finalAction("bwalk",tFinal);
            }
        }
        else if(fabs(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0)))<150 && fabs(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0)))>90)
        {
            //cout<<"! n14\n";
            if(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0))>0)
            {
                return SK->finalAction("turnL",tFinal);
            }
            else
            {
                return SK->finalAction("turnR",tFinal);
            }
        }
        else
        {   //cout<<"! n15\n";
            return SK->finalAction("wlk",tFinal);
        }
    }
    ///////!isball/////////////
    if(!isball)
    {
        static bool xz=false;
        if(me.getDistanceTo(pos)>4)
            xz=true;
        if(me.getDistanceTo(pos)<1)
            xz=false;
        //~ cout<<"! isball\n";
        if(xz)
            goto n1;
        if(me.getDistanceTo(pos)<0.2 && fabs(WM->getMyAngleToBall())>10) {
            if(WM->getMyAngleToBall()>0)
            {
                return SK->finalAction("turnL",tFinal);
            }
            else
            {
                return SK->finalAction("turnR",tFinal);
            }
        }
        else if(fabs(WM->getMyAngleToGoal())>20)
        {
            if(WM->getMyAngleToGoal()>0)
            {
                return SK->finalAction("turnL",tFinal);
            }
            else
            {
                return SK->finalAction("turnR",tFinal);
            }
        }
        else if(shback())
        {
            return SK->finalAction("bwalk",tFinal);
        }
        else if(shSide2(side))
        {
            return SK->finalAction(side,tFinal);
        }
        else if(fabs(me.getY()-pos.getY())>0.1 && fabs(me.getX()-pos.getX())<0.4)
        {
            if(me.getY()>pos.getY())
            {
                return SK->finalAction("sideWalkL",tFinal);
            }
            else
            {
                return SK->finalAction("sideWalkR",tFinal);
            }
        }
        else if(fabs(me.getX()-pos.getX())>0.08)
        {
            if(me.getX()>pos.getX())
            {
                return SK->finalAction("bwalk",tFinal);
            }
            else
            {
                return SK->finalAction("walk",tFinal);
            }
        }
        else
        {
            return SK->finalAction("",tFinal);
        }

        /*if(fabs(ang)>10)
          {
            if(ang>0)
              {
                return SK->finalAction("turnL",tFinal);
              }
            else
              {
                return SK->finalAction("turnR",tFinal);
              }
          }
        else if(shback())
          {
            return SK->finalAction("bwalk",tFinal);
          }
        else if(shSide2(side))
          {
            return SK->finalAction(side,tFinal);
          }
        else if(fabs(me.getY()-pos.getY())>0.1 && me.getX()<ball.getX() && fabs(me.getX()-pos.getX())<0.4)
          {
            if(me.getY()>pos.getY())
              {
                return SK->finalAction("sideWalkL",tFinal);
              }
            else
              {
                return SK->finalAction("sideWalkR",tFinal);
              }
          }
        else if(fabs(me.getX()-pos.getX())>0.08)
          {
            if(me.getX()>pos.getX())
              {
                return SK->finalAction("bwalk",tFinal);
              }
            else
              {
                return SK->finalAction("walk",tFinal);
              }
          }
        else if(fabs(me.getY()-pos.getY())>0.1 && fabs(me.getX()-pos.getX())<0.4)
          {
            if(me.getY()>pos.getY())
              {
                return SK->finalAction("sideWalkL",tFinal);
              }
            else
              {
                return SK->finalAction("sideWalkR",tFinal);
              }
          }
        else
          {
            return SK->finalAction("",tFinal);
          }*/
    }
    ///////////Penalty area////////////////
    else if(isball && P_A.isInside(me))
    {
        //~ cout<<"isball && P_A.isInside(me)\n";
        if(fabs(sect.getY())<1 && fabs(WM->getMyAngleTo(WM->getBallPos()))<15)
        {
            return SK->finalAction("walk",tFinal);
        }
        else if(fabs(sect.getY())<1 && fabs(WM->getMyAngleTo(WM->getBallPos()))>15)
        {
            if(WM->getMyAngleTo(WM->getBallPos())>0)
            {
                return SK->finalAction("turnL",tFinal);
            }
            else
            {
                return SK->finalAction("turnR",tFinal);
            }
        }
        else if(shback())
        {
            return SK->finalAction("bwalk",tFinal);
        }
        else if(shSide2(side))
        {
            return SK->finalAction(side,tFinal);
        }
        else if(me.getX()-pos.getX()>0.1)
        {
            if(me.getX()>pos.getX())
            {
                return SK->finalAction("bwalk",tFinal);
            }
        }
        else if(me.getDistanceTo(ball)>0.4 && fabs(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0)))<45)
        {
            if(fabs(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0)))>10)
            {
                if(WM->getMyAngleTo(Vector3f(pos.getX(),pos.getY(),0))<0)
                {
                    return SK->finalAction("walkAngleR",tFinal);
                }
                else
                {
                    return SK->finalAction("walkAngleL",tFinal);
                }
            }
            else
            {
                return SK->finalAction("walk",tFinal);
            }
        }
        else if(fabs(WM->getMyAngleToGoal()>10))
        {
            if(WM->getMyAngleToGoal()>0)
            {
                return SK->finalAction("turnL",tFinal);
            }
            else
            {
                return SK->finalAction("turnR",tFinal);
            }
        }
        else if(shSide(side))
        {
            return SK->finalAction(side,tFinal);
        }
        else
        {
            return SK->finalAction("walk",tFinal);
        }
    }
    ////////Side of the penalty area/////////////////
    else if(isball && R_P_A.isInside(me))
    {

        static bool x=true,z=true;
        static bool Xa=true;
        if(me.getY()+0.1<ball.getY())
            Xa=true;
        if(me.getY()>ball.getY())
            Xa=false;
        double dang=fabs(fabs(WM->getMyAngleToGoal()-fabs(WM->getMyAngleToBall())));
        if(me.getDistanceTo(ball)>0.4 && (fabs(me.getY()-ball.getY())>0.3 || fabs(me.getX()-ball.getX())>2) && Xa)
            x=false;
        if(me.getDistanceTo(ball)>1 && (fabs(me.getY()-ball.getY())>0.7 || fabs(me.getX()-ball.getX())>2) && !Xa)
            x=false;
        if(me.getDistanceTo(ball)<0.35 || (fabs(me.getY()-ball.getY())<0.25 || fabs(me.getX()-ball.getX())<0.5) && Xa)
            x=true;
        if(me.getDistanceTo(ball)<0.95 || (fabs(me.getY()-ball.getY())<0.65 || fabs(me.getX()-ball.getX())<0.5) && !Xa)
            x=true;

        if(fabs(me.getY()-ball.getY())<0.03)
            z=true;
        if(fabs(me.getY()-ball.getY())>0.1)
            z=false;

        //cout<<WM->getMyAngle()<<endl;

        static bool as=true;
        if(fabs(me.getY()-ball.getY())>0.7)
        {
            as=true;
        }
        if(fabs(me.getY()-ball.getY())<0.45)
        {
            as=false;
        }
        if(fabs(WM->getMyAngleTo(WM->getBallPos()))<70 && fabs(WM->getMyAngleTo(WM->getBallPos()))>15 && fabs(WM->getMyAngleToGoal())<35 && as)
        {
//            cout<<"isball1\n";

            if(fabs(WM->getMyAngleTo(WM->getBallPos()))>20)
            {
                if(WM->getMyAngleTo(WM->getBallPos())<0)
                {
                    return SK->finalAction("walkAngleR",tFinal);
                }
                else
                {
                    return SK->finalAction("walkAngleL",tFinal);
                }
            }
            else
            {
                return SK->finalAction("walk",tFinal);
            }
        }
        else if(shback())
        {
            //cout<<"isball2\n";
            return SK->finalAction("bwalk",tFinal);
        }
        else if(shSide2(side))
        {
            //cout<<"isball3\n";
            return SK->finalAction(side,tFinal);
        }
        else if(shSide(side) && !as && Xa)
        {
            //cout<<"isball4\n";
            if(fabs(WM->getMyAngleToGoal())>25)
            {
                if(WM->getMyAngleToGoal()>0)
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
                return SK->finalAction(side,tFinal);
            }
        }
        else if(!z && me.getY()>ball.getY() && fabs(WM->getMyAngleToGoal())<60)
        {
            //cout<<"isball6\n";
            if(fabs(WM->getMyAngleToBall())<145 && me.getDistanceTo(ball)>0.45)
            {
                if(WM->getMyAngleToBall()<0)
                {
                    return SK->finalAction("bwalkAngleL",tFinal);
                }
                else
                {
                    return SK->finalAction("bwalkAngleR",tFinal);
                }
            }
            else
            {
                return SK->finalAction("bwalk",tFinal);
            }
        }
        else if(!x)
        {
            //cout<<"isball10\n";
            if(fabs(WM->getMyAngleToBall())<35) {
                if(WM->getMyAngleTo(WM->getBallPos())<0)
                {
                    return SK->finalAction("walkAngleR",tFinal);
                }
                else
                {
                    return SK->finalAction("walkAngleL",tFinal);
                }
            }
            else if(WM->getMyAngleTo(WM->getBallPos())>0)
                return SK->finalAction("turnL",tFinal);
            else
                return SK->finalAction("turnR",tFinal);
        }
        else if(fabs(WM->getMyAngleToGoal())>20 && x)
        {
            //cout<<"isball5\n";
            if(WM->getMyAngleToGoal()>0)
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
            //cout<<"isball9\n";
            if(shSide(side)) {
                if(side=="sideWalkR")
                    return SK->finalAction("sideWithWalkR",tFinal);
                else
                    return SK->finalAction("sideWithWalkL",tFinal);
            }
            else
                return SK->finalAction("walk",tFinal);
        }
    }
    ///////////////////////////////
    else if(isball && L_P_A.isInside(me))
    {

        static bool x=true,z=true;
        static bool Xa=true;
        if(me.getY()-0.1>ball.getY())
            Xa=true;
        if(me.getY()<ball.getY())
            Xa=false;
        double dang=fabs(fabs(WM->getMyAngleToGoal()-fabs(WM->getMyAngleToBall())));
        if(me.getDistanceTo(ball)>0.4 && (fabs(me.getY()-ball.getY())>0.3 || fabs(me.getX()-ball.getX())>2) && Xa)
            x=false;
        if(me.getDistanceTo(ball)>1 && (fabs(me.getY()-ball.getY())>0.7 || fabs(me.getX()-ball.getX())>2) && !Xa)
            x=false;
        if(me.getDistanceTo(ball)<0.35 || (fabs(me.getY()-ball.getY())<0.25 || fabs(me.getX()-ball.getX())<0.5) && Xa)
            x=true;
        if(me.getDistanceTo(ball)<0.95 || (fabs(me.getY()-ball.getY())<0.65 || fabs(me.getX()-ball.getX())<0.5) && !Xa)
            x=true;

        if(fabs(me.getY()-ball.getY())<0.03)
            z=true;
        if(fabs(me.getY()-ball.getY())>0.1)
            z=false;

        //cout<<WM->getMyAngle()<<endl;

        static bool as=true;
        if(fabs(me.getY()-ball.getY())>0.7)
        {
            as=true;
        }
        if(fabs(me.getY()-ball.getY())<0.45)
        {
            as=false;
        }
        if(fabs(WM->getMyAngleTo(WM->getBallPos()))<70 && fabs(WM->getMyAngleTo(WM->getBallPos()))>15 && fabs(WM->getMyAngleToGoal())<35 && as)
        {
            //cout<<"isball1\n";

            if(fabs(WM->getMyAngleTo(WM->getBallPos()))>20)
            {
                if(WM->getMyAngleTo(WM->getBallPos())<0)
                {
                    return SK->finalAction("walkAngleR",tFinal);
                }
                else
                {
                    return SK->finalAction("walkAngleL",tFinal);
                }
            }
            else
            {
                return SK->finalAction("walk",tFinal);
            }
        }
        else if(shback())
        {
            //cout<<"isball2\n";
            return SK->finalAction("bwalk",tFinal);
        }
        else if(shSide2(side))
        {
            //cout<<"isball3\n";
            return SK->finalAction(side,tFinal);
        }
        else if(shSide(side) && !as && Xa)
        {
            //cout<<"isball4\n";
            if(fabs(WM->getMyAngleToGoal())>25)
            {
                if(WM->getMyAngleToGoal()>0)
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
                return SK->finalAction(side,tFinal);
            }
        }
        else if(!z && me.getY()<ball.getY() && fabs(WM->getMyAngleToGoal())<60)
        {
            //cout<<"isball6\n";
            if(fabs(WM->getMyAngleToBall())<145 && me.getDistanceTo(ball)>0.45)
            {
                if(WM->getMyAngleToBall()<0)
                {
                    return SK->finalAction("bwalkAngleL",tFinal);
                }
                else
                {
                    return SK->finalAction("bwalkAngleR",tFinal);
                }
            }
            else
            {
                return SK->finalAction("bwalk",tFinal);
            }
        }
        else if(!x)
        {
            //cout<<"isball10\n";
            if(fabs(WM->getMyAngleToBall())<35) {
                if(WM->getMyAngleTo(WM->getBallPos())<0)
                {
                    return SK->finalAction("walkAngleR",tFinal);
                }
                else
                {
                    return SK->finalAction("walkAngleL",tFinal);
                }
            }
            else if(WM->getMyAngleTo(WM->getBallPos())>0)
                return SK->finalAction("turnL",tFinal);
            else
                return SK->finalAction("turnR",tFinal);
        }
        else if(fabs(WM->getMyAngleToGoal())>20 && x)
        {
            //cout<<"isball5\n";
            if(WM->getMyAngleToGoal()>0)
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
            //cout<<"isball9\n";
            if(shSide(side)) {
                if(side=="sideWalkR")
                    return SK->finalAction("sideWithWalkR",tFinal);
                else
                    return SK->finalAction("sideWithWalkL",tFinal);
            }
            else
                return SK->finalAction("walk",tFinal);
        }
    }


    //////isball/////////////
    else if(isball)
    {
        static bool x=true,z=true;
        static bool Xa=true;
        if(me.getX()+0.1<ball.getX())
            Xa=true;
        if(me.getX()>ball.getX())
            Xa=false;
        double dang=fabs(fabs(WM->getMyAngleToGoal()-fabs(WM->getMyAngleToBall())));
        if(me.getDistanceTo(ball)>0.4 && (fabs(me.getX()-ball.getX())>0.3 || fabs(me.getY()-ball.getY())>2) && Xa)
            x=false;
        if(me.getDistanceTo(ball)>1 && (fabs(me.getX()-ball.getX())>0.7 || fabs(me.getY()-ball.getY())>2) && !Xa)
            x=false;
        if(me.getDistanceTo(ball)<0.35 || (fabs(me.getX()-ball.getX())<0.25 || fabs(me.getY()-ball.getY())<0.5) && Xa)
            x=true;
        if(me.getDistanceTo(ball)<0.95 || (fabs(me.getX()-ball.getX())<0.65 || fabs(me.getY()-ball.getY())<0.5) && !Xa)
            x=true;
        if(fabs(me.getX()-ball.getX())<0.04)
            z=true;
        if(fabs(me.getX()-ball.getX())>0.1)
            z=false;

        //cout<<WM->getMyAngle()<<endl;

        static bool as=true;
        if(fabs(me.getX()-ball.getX())>0.7)
        {
            as=true;
        }
        if(fabs(me.getX()-ball.getX())<0.45)
        {
            as=false;
        }
        if(!x && fabs(WM->getMyAngleTo(WM->getBallPos()))<70 && fabs(WM->getMyAngleTo(WM->getBallPos()))>15 && fabs(WM->getMyAngleToGoal())<35 && as)
        {
            //cout<<"isball1\n";

            if(fabs(WM->getMyAngleTo(WM->getBallPos()))>20)
            {
                if(WM->getMyAngleTo(WM->getBallPos())<0)
                {
                    return SK->finalAction("walkAngleR",tFinal);
                }
                else
                {
                    return SK->finalAction("walkAngleL",tFinal);
                }
            }
            else
            {
                return SK->finalAction("walk",tFinal);
            }
        }
        else if(shback())
        {
            //cout<<"isball2\n";
            return SK->finalAction("bwalk",tFinal);
        }
        else if(shSide2(side))
        {
            //out<<"isball3\n";
            return SK->finalAction(side,tFinal);
        }
        else if(!Xa && me.getX()>ball.getX() && fabs(WM->getMyAngleToGoal())<70)
        {
            //cout<<"isball6\n";
            if(fabs(WM->getMyAngleTo(Vector3f(ball.getX(),ball.getY(),0)))<145 && fabs(ball.getX()-me.getX())>0.4 )
            {
                if(WM->getMyAngleTo(Vector3f(ball.getX(),ball.getY(),0))<0)
                {
                    return SK->finalAction("bwalkAngleL",tFinal);
                }
                else
                {
                    return SK->finalAction("bwalkAngleR",tFinal);
                }
            }
            else
            {
                return SK->finalAction("bwalk",tFinal);
            }
        }
        else if(shSide(side) && !as && Xa)
        {
            //cout<<"isball4\n";
            if(fabs(WM->getMyAngleToGoal())>25)
            {
                if(WM->getMyAngleToGoal()>0)
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
                return SK->finalAction(side,tFinal);
            }
        }
        else if(!x)
        {
            //cout<<"isball10\n";
            if(fabs(WM->getMyAngleToBall())<35) {
                if(WM->getMyAngleTo(WM->getBallPos())<0)
                {
                    return SK->finalAction("walkAngleR",tFinal);
                }
                else
                {
                    return SK->finalAction("walkAngleL",tFinal);
                }
            }
            else if(WM->getMyAngleTo(WM->getBallPos())>0)
                return SK->finalAction("turnL",tFinal);
            else
                return SK->finalAction("turnR",tFinal);
        }
        else if(fabs(WM->getMyAngleToGoal())>20 && x)
        {
            //cout<<"isball5\n";
            if(WM->getMyAngleToGoal()>0)
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
            //cout<<"isball9\n";
            if(shSide(side)) {
                if(side=="sideWalkR")
                    return SK->finalAction("sideWithWalkR",tFinal);
                else
                    return SK->finalAction("sideWithWalkL",tFinal);
            }
            else
                return SK->finalAction("walk",tFinal);
        }
    }
    return 	SK->finalAction("",tFinal);
}










bool Decide::shouldPlay2()
{
//    cout<<WM->getPlayMode()<<endl;
    if(WM->getPlayMode()==PM_BeforeKickOff || WM->getPlayMode()==PM_Goal_Left || WM->getPlayMode()==PM_Goal_Right || WM->getPlayMode()==PM_KickOff_Left || WM->getPlayMode()==PM_KickOff_Right){
        return false;
    }
    return true;
}

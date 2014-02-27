# include "Act.h"

Act::Act(WorldModel *wm,Skill *acs)
{
    WM=wm;
    ACS=acs;
    done=false;
    t=0;
    started=false;
    curr=None;
    s=Left;
    ang=0;
}

void Act::setCurrentAct(Command CURR,SideT s,double ang,double d1)
{
    bool x=false;
    curr=CURR;
    this->s=s;
    this->ang=ang;
    this->d1=d1;
    t=0;
    started=true;
    done=false;
}

Command Act::getCurrentAct()
{
    return curr;
}
///~ Return string of current Act

string Act::doCurrentAct()
{
    if (done)
    {
        curr=None;
        t=0;
        done=false;
        started=false;
        return ACS->fix();
    }

    else if ( curr == K )
    {
        bool a;
        string cmd;
        VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
        if( s == Right )
        {
            return ACS->shootR (a, done,ang) ;
        }
        else
        {
            return ACS->shootL (a, done,ang) ;
        }
        return cmd;
    }
    else if (curr==SUB)
    {
        return ACS->standUp ( Back,done ,t);
    }

    else if (curr==SUF)
    {
        return ACS->standUp ( Front,done ,t);
    }
    else if (curr==DP)
    {
        return ACS->DP ( s,done ,t);
    }
}

///~ Return Wheter Current Act Finished Or No
bool Act::isDone()
{
    if ((done && started) || curr==None)
    {
        done=false;
        started=false;
        t=0;
        curr=None;
        return true;
    }
    return false;
}

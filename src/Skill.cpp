#include "Skill.h"
#include "Skill.h"

void fk (double, double);


Skill::Skill(WorldModel*wm)
{
    WM=wm;
    f.open("ACT/standupB.txt");
    if ( !f )
    {
        cerr << "Could'nt Open File standUpB" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            f >> standupB[i][j];
        }
    f.close();
    fill1(standupF,"standupF");
    fill1(standupB,"standupB");


    f.open("ACT/standingK.txt");
    if ( !f)
    {
        cerr << "Could'nt Open File divrR" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            f >> standingK[i][j];
        }
    f.close();

    /*f.open("ACT/diveR.txt");
    if ( !f)
    {
        cerr << "Could'nt Open File divrR" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            f >> diveR[i][j];
        }
    f.close();

    f.open("ACT/diveL.txt");
    if ( !f)
    {
        cerr << "Could'nt Open File diveL" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            f >> diveL[i][j];
        }
    f.close();

    f.open("ACT/diveC.txt");
    if ( !f)
    {
        cerr << "Could'nt Open File divrC" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            f >> diveC[i][j];
        }
    f.close();
    */
    f.open("ACT/hfr.txt");
    if ( !f)
    {
        cerr << "Could'nt Open File hfr" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            f >> DPR[i][j];
        }
    f.close();

    f.open("ACT/hfl.txt");
    if ( !f)
    {
        cerr << "Could'nt Open File hfl" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            f >> DPL[i][j];
        }
    f.close();

    f.open("ACT/zero.txt");
    if ( !f)
    {
        cerr << "Could'nt Open File" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            f >> zero[i][j];
        }
    f.close();

    f.open("ACT/zero-eh.txt");
    if ( !f)
    {
        cerr << "Could'nt Open File" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            f >> zeroe[i][j];
        }
    f.close();

    f.open("ACT/standing.txt");
    if ( !f)
    {
        cerr << "Could'nt Open File" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            f >> standing[i][j];
        }
    f.close();

    f.open("ACT/standing2.txt");
    if ( !f)
    {
        cerr << "Could'nt Open File" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            f >> standing[i][j];
        }
    f.close();

    f.open("ACT/standingBack.txt");
    if ( !f)
    {
        cerr << "Could'nt Open File" << endl;
    }
    f >> x >> y;
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            f >> standingB[i][j];
        }
    f.close();

}

////////////////////////////////////////////////////////////////////

string Skill::moveJoint(string j, double val)
{
    stringstream message;
    message << "(" << j << " " << val*0.87 << ")";
    return message.str();
}
///////////////////////////////////////////////////////////////////

string Skill::moveJointTo(string j, double val, double speed)
{
    speed = fabs(speed);
    if (fabs(WM->getJointAngle(j) - val) < 0.1)
    {
        return "";
    }
    if (val - WM->getJointAngle(j) <= 0)
    {
        speed *= -1;
    }
    if (fabs(val - WM->getJointAngle(j)) < fabs(speed))
    {
        speed = val - WM->getJointAngle(j);
    }
    return moveJoint( j , speed );
}
/////////////////////////////////////////////////////////////////////
void Skill::fill1(double action[24][24],string fileName)
{

    string path="./ACT/"+fileName+".txt";
    ifstream fin ;
    fin.open(path.c_str());
    fin>>action[0][0]>>action[1][0];
    double temp;
    for(int i=0; i<action[0][0]; i++)
    {
        string joint;
        if(i>1)
        {
            fin>>action[i][0];
        }
        else
        {
            fin>>temp;
        }
        for(int j=1; j<action[1][0]; j++)
        {
            if(i==0 && j==1)
            {
                fin>>temp;
                continue;
            }
            fin>>action[i][j];
        }
    }

    fin.close();
}
/////////////////////////////////////////////////////////////////////
string Skill::act(double a[][24],double &time)
{
    int m=a[0][0];
    int n=a[1][0];

    string ss="";
    for(int i=1; i<n; i++)
    {
        if(a[0][i]-0.01<=time && time<a[1][i]-0.01)
        {
            for(int j=2; j<m; j++)
            {
                double speed=(a[j][i]-WM->getJointAngle(WM->num2Str(a[j][0]-1)))/(a[1][i]-time)/50.0;
                ss+=moveJoint(WM->num2Str(a[j][0]-1),speed);
            }
            break;
        }
        else
        {
            continue;
        }
    }
    time+=0.02;
    if(time>a[1][n-1])
    {
        ss="";
        for(int i=0; i<22; i++)
        {
            ss+=moveJoint(WM->num2Str(i),0);
        }

        return ss;
    }
    return ss;
}
/////////////////////////////////////////////////////////////////////
string Skill :: StandUpFront ( bool &done )
{
    stringstream ss;
    done = false;
    bool did=false;
    static double tt=0;
    ss<<sefr(did,tt,false);
    if(did==false)
    {
        return ss.str();
    }
    static int jjj=0;
    jjj++;
    static int co = -1 ;
    static double tmp = 0 ;
    co ++ ;
    static bool islied=false;
    if ( co == 0 )
    {
        islied=false;
    }

    if ( WM->getJointAngle("lae1")>-89 and islied == false )
    {
        stringstream ss ;
        ss << fix ( ) << moveJointTo ("rae1", -90 , -4 ) << moveJointTo ("lae1", -90 , -4 ) ;
        return ss . str ( ) ;

    }


    islied=true;
    bool dd;
    if(tmp>2.6)
    {
        jjj=0;
        did=false;
        tt=0;
        tmp=0;
        co=-1;
        done = true;
        return fix();
    }
    return act(standupF, tmp);
}

/////////////////////////////////////////////////////////////////////
string Skill::act(double a[][24], int m, int n, double &time, bool &t)
{
    if (t)
    {
        return "";
    }
    string ss = "";
    for (int i = 1; i < n; i++)
    {
        if (a[0][i] < time && time <= a[1][i])
        {
            for (int j = 2; j < m; j++)
            {
                double speed = (a[j][i] - WM->getJointAngle(WM->num2Str(a[j][0]))) / (a[1][i] - time) / 50.0;
                if (fabs(a[j][i] - WM->getJointAngle(WM->num2Str(a[j][0]))) > .4)
                {
                    ss += moveJoint(WM->num2Str(a[j][0]), speed);
                }
                else if (  ( WM->getLastJointAngle(WM->num2Str(a[j][0])) - WM->getJointAngle(WM->num2Str(a[j][0])) ) != 0 )
                {
                    ss += fix(WM->num2Str(a[j][0]));
                }
            }
            break;
        }
        else
        {
            continue;
        }
    }
    time += 0.02;
    if (time > a[1][n - 1])
    {
        t = true;
        return "";
    }
    return ss;
}
//////////////////////////////////////////////////////////////////////

string Skill::fix()
{
    stringstream ss;
    for (int i = 0; i < 22; i++)
    {
        if (  fabs( WM->getLastJointAngle(WM->num2Str(i)) - WM->getJointAngle(WM->num2Str(i)) ) >= 0.0 )
        {
            ss << moveJoint(WM->num2Str(i), 0);
        }
    }
    return ss.str();
}
//////////////////////////////////////////////////////////////////////

string Skill::feh(bool a, double s)
{
    stringstream ss("");
    for (int i = 0; i < 22; i++)
        if (fabs(WM->getJointAngle(WM->num2Str(i))) > 0.2 && ((i != 14 && i != 15) || a))
        {
            ss << moveJointTo(WM->num2Str(i), 0, s);
        }
    return ss.str();
}

//////////////////////////set////////////////////////////////////////

string Skill::set(string j, double ang, int cycle)
{
    stringstream ss;
    double speed = (ang) / cycle;
    if ( speed > 7)
    {
        speed = 7 ;
    }
    ss << moveJoint(j, speed);
    return ss.str();
}
/////////////////////////////////////////////////////////////////////
string Skill::fix(string j)
{
    if (  ( WM->getLastJointAngle(j) - WM->getJointAngle(j) ) != 0.0 )
    {
        return moveJoint(j, 0);
    }
    else
    {
        return "" ;
    }
}
/////////////////////////////////////turn//////////////////////////////////
string Skill::standUp(SideT s, bool &done, double &t)
{
    if (done)
    {
        t = 0;
        return "";
    }
    if (s == Front)
    {
        return act(standupF, standupF[0][0], standupF[1][0], t, done);
    }
    else
    {
        return StandUpBack(done);
    }
}
/////////////////////////////////////////////////////////////////////
string Skill::DP(SideT s, bool &done, double &t)
{
    if (done)
    {
        t = 0;
        return "";
    }
    if (s == Right)
    {
        return act(DPR, DPR[0][0], DPR[1][0], t, done);
    }
    else
    {
        return act(DPL, DPL[0][0], DPL[1][0], t, done);
    }
}
///////////////////////////////////////////////////////////////////////

string Skill::sefr(bool &done, double &t, bool b)
{
    if (done)
    {
        t = 0;
        return "";
    }
    if (b)
    {
        return act(zeroe, zeroe[0][0], zeroe[1][0], t, done);
    }
    else
    {
        return act(zero, zero[0][0], zero[1][0], t, done);
    }
}
////////////////////////////////////////////////////////////////////////

string Skill::Standing(bool &done, double &t)
{
    if (done)
    {
        t = 0;
        return "";
    }
    return act(standing, standing[0][0], standing[1][0], t, done);
}
//////////////////////////////////////////////////////////////////////
string Skill::Standing2(bool &done, double &t)
{
    if (done)
    {
        t = 0;
        return "";
    }
    return act(standing2, standing2[0][0], standing2[1][0], t, done);
}
///////////////////////////////////////////////////////////////////////
string Skill::StandingBack(bool &done, double &t)
{
    if (done)
    {
        t = 0;
        return "";
    }
    return act(standingB, standingB[0][0], standingB[1][0], t, done);
}
////////////////////////////////////////////////////////////////////////
string Skill::beam(double x, double y, double ang)
{

    stringstream message;
    message << "(beam " << x  << " " << y << " " << ang << ")";
    return message.str();
}
////////////////////////////////////////////////////////////////////////

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///                             Say Ball Pos
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

string Skill::sayBallPos () {
    stringstream ss;
    ss << "(say "<< WM->getClosestOurToBall() << WM->getClosestOurToBall() << WM->getClosestOurToBall() <<")";
    return ss.str();
}

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///                             Beam
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

string Skill::whereBeam(bool &stand,double &stan)
{

    stringstream ss("");
    if(WM->getMyNum()==9)
    {
        if (WM->getTeamSide() == Left)
        {
            if ( WM->getPlayMode() == PM_BeforeKickOff  || WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right )
            {
                if(WM->getServerTime()<20)
                    ss << beam(-0.19,0.0,0);
                else
                    ss << beam(-0.22,0.0,0);
            }
            else
            {
                ss << beam(-1.6,0.0,0);
            }
        }
        else
        {

            if ( WM->getPlayMode() == PM_KickOff_Right || WM->getPlayMode() == PM_Goal_Left  )
            {
                ss << beam(-0.19,0,0);
            }
            else
            {
                ss << beam(-1.6,0.0,0);
            }
        }
    }
    else if(WM->getMyNum()==8)
    {
        if (WM->getTeamSide() == Left)
        {
            if ( WM->getPlayMode( ) == PM_BeforeKickOff   || WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right )
            {
                ss << beam(-0.18,-2.5,0);
            }
            else
            {
                ss << beam(-2.6,0.7,0);
            }
        }
        else
        {
            if ( WM->getPlayMode() == PM_KickOff_Right || WM->getPlayMode() == PM_Goal_Left  )
            {
                ss << beam(-0.18,-2.5,0);
            }
            else
            {
                ss << beam(-2.6,0.7,0);
            }
        }
    }
    else if(WM->getMyNum()==6)
    {
        if (WM->getTeamSide() == Left)
        {
            if ( WM->getPlayMode( ) == PM_BeforeKickOff   || WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right )
            {
                ss << beam(-13,3,0);
            }
            else
            {
                ss << beam(-13,3,0);
            }
        }
        else
        {
            if ( WM->getPlayMode() == PM_KickOff_Right || WM->getPlayMode() == PM_Goal_Left  )
            {
                ss << beam(-13,3,0);
            }
            else
            {
                ss << beam(-13,3,0);
            }
        }
    }
    else if(WM->getMyNum()==4)
    {
        if (WM->getTeamSide() == Left)
        {
            if (WM->getPlayMode() == PM_BeforeKickOff || WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right)
            {
                ss << beam(-9,4,0);
            }
            else
            {
                ss << beam(-9,4,0);
            }
        }
        else
        {
            if ( WM->getPlayMode() == PM_KickOff_Right || WM->getPlayMode() == PM_Goal_Left )
            {
                ss << beam(-9,4,0);
            }
            else
            {
                ss << beam(-9,4,0);
            }
        }
    }
    else if(WM->getMyNum()==2)
    {
        if (WM->getTeamSide() == Left)
        {
            if ( WM->getPlayMode() == PM_BeforeKickOff || WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right )
            {
                ss << beam(-10.0,1.2,0);
            }
            else
            {
                ss << beam(-10.0,1.2,0);
            }
        }
        else
        {
            if ( WM->getPlayMode() == PM_KickOff_Right || WM->getPlayMode() == PM_Goal_Left )
            {
                ss << beam(-10,1.2,0);
            }
            else
            {
                ss << beam(-10,1.2,0);
            }
        }
    }
    else if(WM->getMyNum()==5)
    {
        if (WM->getTeamSide() == Left)
        {
            if (WM->getPlayMode() == PM_BeforeKickOff || WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right)
            {
                ss << beam(-9,-4,0);
            }
            else
            {
                ss << beam(-9,-4,0);
            }
        }
        else
        {
            if ( WM->getPlayMode() == PM_KickOff_Right || WM->getPlayMode() == PM_Goal_Left )
            {
                ss << beam(-9,-4,0);
            }
            else
            {
                ss << beam(-9,-4,0);
            }
        }
    }
    else if(WM->getMyNum()==3)
    {
        if (WM->getTeamSide() == Left)
        {
            if ( WM->getPlayMode() == PM_BeforeKickOff  || WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right )
            {
                ss << beam(-10.0,-1.2,0);
            }
            else
            {
                ss << beam(-10.0,-1.2,0);
            }
        }
        else
        {
            if ( WM->getPlayMode() == PM_KickOff_Right  || WM->getPlayMode() == PM_Goal_Left )
            {
                ss << beam(-10.0,-1.6,0);
            }
            else
            {
                ss << beam(-10.0,-1.6,0);
            }
        }
    }
    else if(WM->getMyNum()==11)
    {
        if (WM->getTeamSide() == Left)
        {
            if ( WM->getPlayMode() == PM_BeforeKickOff  || WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right )
            {
                ss << beam(-7,0,0);
            }
            else
            {
                ss << beam(-7,0,0);
            }
        }
        else
        {
            if ( WM->getPlayMode() == PM_KickOff_Right  || WM->getPlayMode() == PM_Goal_Left )
            {
                ss << beam(-7,0,0);
            }
            else
            {
                ss << beam(-7,0,0);
            }
        }
    }
    else if(WM->getMyNum()==10)
    {
        if (WM->getTeamSide() == Left)
        {
            if ( WM->getPlayMode() == PM_BeforeKickOff  || WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right )
            {
                ss << beam(-6,2,0);
            }
            else
            {
                ss << beam(-6,2,0);
            }
        }
        else
        {
            if ( WM->getPlayMode() == PM_KickOff_Right  || WM->getPlayMode() == PM_Goal_Left )
            {
                ss << beam(-6,2,0);
            }
            else
            {
                ss << beam(-6,2,0);
            }
        }
    }
    else if(WM->getMyNum()==7)
    {
        if (WM->getTeamSide() == Left)
        {
            if ( WM->getPlayMode() == PM_BeforeKickOff  || WM->getPlayMode() == PM_KickOff_Left || WM->getPlayMode() == PM_Goal_Right )
            {
                ss << beam(-13,-3,0);
            }
            else
            {
                ss << beam(-13,-3,0);
            }
        }
        else
        {
            if ( WM->getPlayMode() == PM_KickOff_Right  || WM->getPlayMode() == PM_Goal_Left )
            {
                ss << beam(-13,-3,0);
            }
            else
            {
                ss << beam(-13,-3,0);
            }
        }
    }
    return ss.str();

}
//////////////////////////////////////////////////////////////////////
string Skill::updatem(bool checkBall)
{
    static int level=0;
    if( level==6 || (WM->seeEnoughFlag() && (WM->seeBall() || !checkBall) ))
    {
        level=0;
        return fix("he1") + fix("he2");
    }
    double theta = 2*(level%2-0.5)*120.0;
    double phi   = (level/2-1)*45;

    if( fabs(WM->getJointAngle("he1")-theta)<5 &&
            fabs(WM->getJointAngle("he2")-phi)<5 )
    {
        level++;
    }

    string s1 = moveJointTo("he1",theta,6);
    string s2 = moveJointTo("he2",phi,3);
    return s1+s2;
}
//////////////////////////////////////////////////////////////////////
string Skill:: StandUpBack ( bool &done )
{
    stringstream ss;
    done = false;
    bool did=false;
    static double tt=0;
    ss<<sefr(did,tt,false);
    if(did==false)
    {
        return ss.str();
    }
    static int co = -1 ;
    static double tmp = 0 ;
    co ++ ;
    if(tmp>1.8)
    {
        did=false;
        tt=0;
        done = true;
        tmp=0;
        co=-1;
        return fix();
    }
    return act(standupB, tmp);

}
string Skill :: ToRightSide(int i,bool &done)
{
    done=false;
    stringstream ss;
    static int cmd = 0;
    static bool doneS = false;
    static double tt=0;
    cmd++;
    if(cmd<3)
    {
        ss<<fix ( )<<moveJoint("lle2",1.1)<<moveJoint("rle2",1.1)<<moveJoint("lle6",-1.1)<<moveJoint("rle6",-1.01);
    }
    else if(cmd<19)
    {
        ss<<fix()<<moveJoint("lle2",i);
    }
    else if(doneS==false)
    {
        ss<<sefr(doneS,tt,false);
    }
    else
    {
        done=true;
        cmd=0;
        doneS=false;
        tt=0;
    }
    return ss.str();
}
string Skill :: ToLeftSide(int i,bool &done)
{
    done=false;
    stringstream ss;
    static int cmd = 0;
    static bool doneS = false;
    static double tt=0;
    cmd++;
    if(cmd<3)
    {
        ss<<fix ( )<<moveJoint("lle2",-1.1)<<moveJoint("rle2",-1.1)<<moveJoint("lle6",1.1)<<moveJoint("rle6",1.01);
    }
    else if(cmd<19)
    {
        ss<<fix()<<moveJoint("rle2",i*-1);
    }
    else if(doneS==false)
    {
        ss<<sefr(doneS,tt,false);
    }

    else
    {
        done=true;
        cmd=0;
        doneS=false;
        tt=0;
    }
    return ss.str();
}
///////////////////////////////////////////////////////////////////

VecPosition Skill::FK_FOOT_HIP (double le4 , double le5)
{
    double l3 = 0.12 ;
    double l4 = 0.10 ;
    double h , w ;
    double a1 = 90 - le5 ;
    double a2 = a1 - le4 ;
    double s1 = sinDeg ( a1 )  ;
    double c1 = cosDeg ( a1 )  ;
    double s2 = sinDeg ( a2 ) ;
    double c2 = cosDeg ( a2 ) ;
    h = s1 * l4 + s2 * l3 ;
    w = c1 * l4 + c2 * l3 ;
    return VecPosition( w , h );
}
/////////////////////////////////////////////////////////////////////
bool Skill::IK_ALL ( VecPosition Hip , VecPosition RAnkle , double &rle3 , double &rle4 , double &rle5 , double &lle3 , double &lle4 , double &lle5 , double P)
{
    double l3 = 0.12 ;
    double l4 = 0.10 ;
    double Hx = Hip.getX() , Hz = Hip.getY() ;
    double Ax = RAnkle.getX()-Hx , Az = RAnkle.getY()-Hz ;
    //// LFoot
    /// First Answer
    double a51 = atan2Deg(( (Hz * Hz * Hx * Hx) -  Hx * sqrt(- (Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 +  pow( l3,  4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 +  pow( Hz,  4) +  pow( Hx,  4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 +  pow( l4,  4)))) +  (Hz * Hz * l3 * l3) +   pow( Hz,  4) -  (Hz * Hz * l4 * l4)) /  (Hz * Hz + Hx * Hx) /  Hz /  l3, ( (l3 * l3 * Hx) +   pow( Hx,  3) -  (Hx * l4 * l4) +  (Hz * Hz * Hx) + sqrt(- (Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 +  pow( l3,  4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 +  pow( Hz,  4) +  pow( Hx,  4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 +  pow( l4,  4))))) /  (Hz * Hz + Hx * Hx) /  l3);
    double a41 = atan2Deg(-(- (Hz * Hz * Hx * Hx) -  Hx * sqrt(- (Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 +  pow( l3,  4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 +  pow( Hz,  4) +  pow( Hx,  4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 +  pow( l4,  4)))) +  (Hz * Hz * l3 * l3) -   pow( Hz,  4) -  (Hz * Hz * l4 * l4)) /  (Hz * Hz + Hx * Hx) /  Hz /  l4 / 0.2e1, -( (l3 * l3 * Hx) -   pow( Hx,  3) -  (Hx * l4 * l4) -  (Hz * Hz * Hx) + sqrt(- (Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 +  pow( l3,  4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 +  pow( Hz,  4) +  pow( Hx,  4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 +  pow( l4,  4))))) /  (Hz * Hz + Hx * Hx) /  l4 / 0.2e1);

    ///Second Answer
    double a52 = atan2Deg(( (Hz * Hz * Hx * Hx) +  Hx * sqrt(- (Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 +  pow( l3,  4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 +  pow( Hz,  4) +  pow( Hx,  4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 +  pow( l4,  4)))) +  (Hz * Hz * l3 * l3) +   pow( Hz,  4) -  (Hz * Hz * l4 * l4)) /  (Hz * Hz + Hx * Hx) /  Hz /  l3, ( (l3 * l3 * Hx) +   pow( Hx,  3) -  (Hx * l4 * l4) +  (Hz * Hz * Hx) - sqrt(- (Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 +  pow( l3,  4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 +  pow( Hz,  4) +  pow( Hx,  4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 +  pow( l4,  4))))) /  (Hz * Hz + Hx * Hx) /  l3);
    double a42 = atan2Deg(-(- (Hz * Hz * Hx * Hx) +  Hx * sqrt(- (Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 +  pow( l3,  4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 +  pow( Hz,  4) +  pow( Hx,  4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 +  pow( l4,  4)))) +  (Hz * Hz * l3 * l3) -   pow( Hz,  4) -  (Hz * Hz * l4 * l4)) /  (Hz * Hz + Hx * Hx) /  Hz /  l4 / 0.2e1, -( (l3 * l3 * Hx) -   pow( Hx,  3) -  (Hx * l4 * l4) -  (Hz * Hz * Hx) - sqrt(- (Hz * Hz * (-2 * l3 * l3 * Hx * Hx - 2 * Hz * Hz * l3 * l3 +  pow( l3,  4) - 2 * Hx * Hx * l4 * l4 - 2 * l3 * l3 * l4 * l4 +  pow( Hz,  4) +  pow( Hx,  4) + 2 * Hz * Hz * Hx * Hx - 2 * Hz * Hz * l4 * l4 +  pow( l4,  4))))) /  (Hz * Hz + Hx * Hx) /  l4 / 0.2e1);

    //// RFoot
    /// First Answer
    double b41 = atan2Deg(-( (Az * Az * Ax * Ax) +  Ax * sqrt(- (Az * Az * (-2 * Az * Az * l4 * l4 +  pow( l4,  4) - 2 * l4 * l4 * Ax * Ax +  pow( Ax,  4) +  pow( Az,  4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 +  pow( l3,  4)))) -  (Az * Az * l4 * l4) +   pow( Az,  4) +  (Az * Az * l3 * l3)) /  (Az * Az + Ax * Ax) /  Az /  l3 / 0.2e1, (- (l4 * l4 * Ax) +  (Ax * l3 * l3) +   pow( Ax,  3) +  (Az * Az * Ax) - sqrt(- (Az * Az * (-2 * Az * Az * l4 * l4 +  pow( l4,  4) - 2 * l4 * l4 * Ax * Ax +  pow( Ax,  4) +  pow( Az,  4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 +  pow( l3,  4))))) /  (Az * Az + Ax * Ax) /  l3 / 0.2e1);
    double b31 = atan2Deg(-( (Az * Az * Ax * Ax) -  Ax * sqrt(- (Az * Az * (-2 * Az * Az * l4 * l4 +  pow( l4,  4) - 2 * l4 * l4 * Ax * Ax +  pow( Ax,  4) +  pow( Az,  4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 +  pow( l3,  4)))) +  (Az * Az * l4 * l4) +   pow( Az,  4) -  (Az * Az * l3 * l3)) /  (Az * Az + Ax * Ax) /  Az /  l4 / 0.2e1, ( (l4 * l4 * Ax) -  (Ax * l3 * l3) +   pow( Ax,  3) +  (Az * Az * Ax) + sqrt(- (Az * Az * (-2 * Az * Az * l4 * l4 +  pow( l4,  4) - 2 * l4 * l4 * Ax * Ax +  pow( Ax,  4) +  pow( Az,  4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 +  pow( l3,  4))))) /  (Az * Az + Ax * Ax) /  l4 / 0.2e1);

    ///Second Answer
    double b42 = atan2Deg(-( (Az * Az * Ax * Ax) -  Ax * sqrt(- (Az * Az * (-2 * Az * Az * l4 * l4 +  pow( l4,  4) - 2 * l4 * l4 * Ax * Ax +  pow( Ax,  4) +  pow( Az,  4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 +  pow( l3,  4)))) -  (Az * Az * l4 * l4) +   pow( Az,  4) +  (Az * Az * l3 * l3)) /  (Az * Az + Ax * Ax) /  Az /  l3 / 0.2e1, (- (l4 * l4 * Ax) +  (Ax * l3 * l3) +   pow( Ax,  3) +  (Az * Az * Ax) + sqrt(- (Az * Az * (-2 * Az * Az * l4 * l4 +  pow( l4,  4) - 2 * l4 * l4 * Ax * Ax +  pow( Ax,  4) +  pow( Az,  4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 +  pow( l3,  4))))) /  (Az * Az + Ax * Ax) /  l3 / 0.2e1);
    double b32 = atan2Deg(-( (Az * Az * Ax * Ax) +  Ax * sqrt(- (Az * Az * (-2 * Az * Az * l4 * l4 +  pow( l4,  4) - 2 * l4 * l4 * Ax * Ax +  pow( Ax,  4) +  pow( Az,  4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 +  pow( l3,  4)))) +  (Az * Az * l4 * l4) +   pow( Az,  4) -  (Az * Az * l3 * l3)) /  (Az * Az + Ax * Ax) /  Az /  l4 / 0.2e1, ( (l4 * l4 * Ax) -  (Ax * l3 * l3) +   pow( Ax,  3) +  (Az * Az * Ax) - sqrt(- (Az * Az * (-2 * Az * Az * l4 * l4 +  pow( l4,  4) - 2 * l4 * l4 * Ax * Ax +  pow( Ax,  4) +  pow( Az,  4) - 2 * Ax * Ax * l3 * l3 - 2 * l4 * l4 * l3 * l3 + 2 * Az * Az * Ax * Ax - 2 * Az * Az * l3 * l3 +  pow( l3,  4))))) /  (Az * Az + Ax * Ax) /  l4 / 0.2e1);

    double lle5_1 = 90 - a51;
    double lle5_2 = 90 - a52;
    double lle4_1 = a51 - a41;
    double lle4_2 = a52 - a42;

    double rle3_1 = 90 + P - b31;
    double rle3_2 = 90 + P - b32;
    double rle4_1 = b31 - b41;
    double rle4_2 = b32 - b42;

    if ( isPossible ( lle4_1 ) )
    {
        lle4 = lle4_1;
        lle5 = lle5_1;
    }
    else if ( isPossible ( lle4_2 ) )
    {
        lle4 = lle4_2;
        lle5 = lle5_2;
    }
    else
    {
        return false;
    }
    lle3 = P -lle5 - lle4 ;

    if ( isPossible ( rle4_1 ) )
    {
        rle4 = rle4_1;
        rle3 = rle3_1;
    }
    else if ( isPossible ( rle4_2 ) )
    {
        rle4 = rle4_2;
        rle3 = rle3_2;
    }
    else
    {
        return false;
    }
    rle5 = P -rle3 - rle4 ;
    return true;

}
///////////////////////////////////////////////////////////////////
bool Skill::isPossible(double l4)
{
    if ( l4 < 0 && l4 > -130 )
    {
        return true;
    }
    return false;
}
///////////////////////////////////////////////////////////////////
string Skill::shootR (bool &canshoot,bool &done,double &tFinal) {

//    VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
//    VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
//    static bool ne=true;
//    static VecPosition H = FK_FOOT_HIP (-65 , 45 ) ;
//    VecPosition pos=VecPosition::givePosition(ball,VecPosition::normalizeAngle(WM->getMyAngle()),-0.01);
//    double x1 =0.002 , y1 =0.0  ;
//    double x2 = H.getX() , y2 =H.getY();
//    double r3,r4,r5,l3,l4,l5;
//    double P = 0.2 ;
//    canshoot=IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5);

    WM->setZ(0.54);
    stringstream ss("");
    static double t= 0;
    static bool did=false;
    static double tt=0;
    double lj3 = WM->getJointAngle ( "lle3" ) ;
    double lj4 = WM->getJointAngle ( "lle4" ) ;
    double lj5 = WM->getJointAngle ( "lle5" ) ;
    double rj3 = WM->getJointAngle ( "rle3" ) ;
    double rj4 = WM->getJointAngle ( "rle4" ) ;
    double rj5 = WM->getJointAngle ( "rle5" ) ;
    static bool sw=true;
    static double sideW;


    double t1 = 0.32, t2 = 0.52, t3 = 1.22, t4 = 1.24, t5 = 1.74;

    if ( WM->isFeltDown() || t>=t5) {
        sideW = 0; t = 0; done = true;
        sw=true; did=false; tt=0;
        return ss.str();
    }

    sw=false;

//    cout << "Amin: ShootR, Joints: " << rj3 << " " << rj4 << " " << rj5 << " " << lj3 << " " << lj4 << " " << lj5 << endl;


    if (sideW < 0.8) {
        sideW += 0.02;
        static double a;
        return finalAction("sideWalkR", a);
    }

    if(!did && t<0.2) {
        ss<<sefr(did,tt,false);
        return ss.str();
    }

    t+=0.02;


    if(t < t1) {
        ss<<moveJoint("lle2",-1.2)<<moveJoint("rle2",-1.2)<<moveJoint("lle6",1.2)<<moveJoint("rle6",1.2);
        return ss.str();
    } else if(t < t2) {
        did=false;
        tt=0;
        ss<<moveJoint("rle3",2.5)<<moveJoint("rle4",-5)<<moveJoint("rle5",4.25)<<moveJoint("rle2", -0.4);
//        cout<<"Amin: t2->"<<rj3<<" "<<rj4<<" "<<rj5<<" "<<lj3<<endl;

        return ss.str();
    } else if (t < t3) {
        double r3 = 0.02*(-25-rj3)/(t3-t);
        double r4 = 0.02*(-130-rj4)/(t3-t);
        double l3 = 0.02*(25-lj3)/(t3-t);
        double r5 = 0.02*(75-rj5)/(t3-t);


//        cout<<"Amin: t3->"<<r3<<" "<<r4<<" "<<r5<<" "<<l3<<endl;
        ss<<moveJoint("rle3",r3)<<moveJoint("rle4",r4)<<moveJoint("rle5",r5)<<moveJoint("lle3" , l3);
    } else if (t < t4) {
        double r3 = 0.02*(20-rj3)/(t4*t4-t*t);
        double r4 = 0.02*(-40-rj4)/(t4*t4-t*t);
        double l3 = 0.02*(-lj3)/(t4*t4-t*t);
        double r5 = 0.02*(20-rj5)/(t4*t4-t*t );
//        cout<<"Amin: t4->"<<r3<<" "<<r4<<" "<<r5<<" "<<l3<<endl;
        ss<<moveJoint("rle3",r3)<<moveJoint("rle4",r4)<<moveJoint("rle5",r5)<<moveJoint("lle3" , l3);
    } else if (t < t5) {
        double r3 = 0.02*(70 -rj3)/(sqrt(t5)-sqrt(t));
        double r4 = 0.02*(0-rj4)/(sqrt(t5)-sqrt(t));
        double l3 = 0.02*(-25-lj3)/(sqrt(t5)-sqrt(t));
        double r5 = 2*0.02*(-45-rj5)/(sqrt(t5)-sqrt(t));

//        cout<<"Amin: t5->"<<r3<<" "<<r4<<" "<<r5<<" "<<l3<<endl;
        ss<<moveJoint("rle3",r3)<<moveJoint("rle4",r4)<<moveJoint("rle5",r5)<<moveJoint("lle3" , l3);
    }
    return ss.str();
}
///////////////////////////////////////////////////////////////////
string Skill::shootL (bool &canshoot,bool &done,double &tFinal) {
//    VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
//    static bool ne=true;
//    VecPosition pos=VecPosition::givePosition(ball,VecPosition::normalizeAngle(WM->getMyAngle()),-0.01);
//    double x1 =0.002 , y1 =0.0  ;
//    double x2 = H.getX() , y2 =H.getY();
//    VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
//    static VecPosition H = FK_FOOT_HIP (-65 , 45 ) ;


    WM->setZ(0.54);
    stringstream ss("");

    static double t= 0;
    static double sideW;
    static bool did=false;
    static double tt=0;

    double rj3 = WM->getJointAngle ( "rle3" ) ;
    double rj4 = WM->getJointAngle ( "rle4" ) ;
    double rj5 = WM->getJointAngle ( "rle5" ) ;
    double lj3 = WM->getJointAngle ( "lle3" ) ;
    double lj4 = WM->getJointAngle ( "lle4" ) ;
    double lj5 = WM->getJointAngle ( "lle5" ) ;

//    cout << "Amin: " << lj4 << " " << lj5 << endl;
    VecPosition a = FK_FOOT_HIP(lj4, lj5);
//    cout << "Amin: YrFK " << a.getX() << " " << a.getY() << endl;

    fk(lj4, lj5);
    cout << endl;
    static double l3_v0, l4_v0, l5_v0, r3_v0;
//    double r3,r4,r5,l3,l4,l5;
//    canshoot=IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5);

    static double t1 = 0.26, t2 = 0.56, t3 = 1, t4 = 1.02, t5 = 1.04;

    if ( WM->isFeltDown() || t>=t5) {
        sideW = 0;
        t1 = 0.26; t2 = 0.56; t3 = 1; t4 = 1.02; t5 = 1.04;
        l3_v0 = 0; l4_v0 = 0; l5_v0 = 0; r3_v0 = 0;
        t = 0; done = true; did=false; tt=0;
        return ss.str();
    }

//    cout << "Amin: ShootL, Joints: " << rj3 << " " << rj4 << " " << rj5 << " " << lj3 << " " << lj4 << " " << lj5 << endl;

    if (sideW < 0.8) {
        sideW += 0.02;
        static double a;
        return finalAction("sideWalkL", a);
    }

    if(!did && t<0.1) {
        ss << sefr(did,tt,false);
        return ss.str();
    }
//    cout << "Amin: sefr" << endl;
    t+=0.02;

//    cout << "Amin: t4: " << t4 << " " << t << endl;
//    cout << "Amin: t5: " << t5 << " " << l3_v0 << endl;
//    cout << "Amin: Sorat: " << l3_v0 << " " <<  l4_v0 << " " << l5_v0 << " " << r3_v0 << endl;
//      cout << "Amin: Shetab: " << l3_a << " " <<  l4_a << " " <<  l5_a << " " << r3_a << endl;

    if(t < t1) {
        ss<<moveJoint("rle2",1.2)<<moveJoint("lle2",1.2)<<moveJoint("rle6",-1.2)<<moveJoint("lle6",-1.2);
        return ss.str();
    } else if(t < t2) {
        did=false;
        tt=0;
        ss<<moveJoint("lle3",2.5)<<moveJoint("lle4",-5)<<moveJoint("lle5",4.25)<<moveJoint("lle2", 0.4);
        return ss.str();
    } else if (t < t3) {

        double l3 = 0.02*(-25-lj3)/(t3-t);
        double l4 = 0.02*(-130-lj4)/(t3-t);
        double r3 = 0.02*(25-rj3)/(t3-t);
        double l5 = 0.02*(75-lj5)/(t3-t);


//        cout<<"Amin: t3->"<<l3<<" "<<l4<<" "<<l5<<" "<<r3<<endl;
        ss<<moveJoint("lle3",l3)<<moveJoint("lle4",l4)<<moveJoint("lle5",l5)<<moveJoint("rle3" , r3)<< endl;
    } else if (t < t4) {
        double deltaX = (20-lj3);
        double l3_a = 2000 ;
        if (l3_v0*l3_v0 + 2*l3_a*deltaX >= 0)
            t4 = t+(-l3_v0+sqrt(l3_v0*l3_v0 + 2*l3_a*deltaX))/l3_a;
        t5 = t4 + 0.04;
        if (t >= t4) {
            return ss.str();
        }
//        cout << "Amin: answers  " << (-l3_v0+sqrt(l3_v0*l3_v0 + 2*l3_a*deltaX))/l3_a  << " " << (-l3_v0-sqrt(l3_v0*l3_v0 + 2*l3_a*deltaX))/l3_a << endl;
        double l4_a, l5_a, r3_a;
        if (t4 - t >= 0.02) {
            l4_a = (-40-lj4) / (0.5*(t4-t)*(t4-t)) - l4_v0 / (0.5*(t4-t));
            l5_a = (20-lj5) / (0.5*(t4-t)*(t4-t)) - l5_v0 / (0.5*(t4-t));
            r3_a = (0-rj3) / (0.5*(t4-t)*(t4-t)) - r3_v0 / (0.5*(t4-t));
        }

        double l3=0, l4=0, l5=0, r3=0;
        if (fabs(20-lj3) >= 6 ) {
            l3 = getAngle(t - t3, l3_v0, l3_a, &l3_v0);
        } else {
            l3 = 20 - lj3;
        }
        if (fabs(-40-lj4) >= 6 ) {
            l4 = getAngle(t - t3, l4_v0, l4_a, &l4_v0);
        } else {
            l4 = -40-lj4;
        }
        if (fabs(0-rj3) >= 6 ) {
            r3 = getAngle(t - t3, r3_v0, r3_a, &r3_v0);
        } else {
            r3 = 0 - rj3;
        }
        if (fabs(30-lj5) >= 6 ) {
            getAngle(t - t3, l5_v0, l5_a, &l5_v0);
            l5 = 3*0.02*(30-lj5)/(t4-t);
        } else {
            l5 = 30 - lj5;
        }


//        double l3 = 0.02*(30-lj3)/(t4*t4-t*t);
//        double l4 = 0.02*(-60-lj4)/(t4*t4-t*t);
//        double r3 = 0.02*(-rj3)/(t4-t);
//        cout<<"Amin: t4->"<<l3<<" "<<l4<<" "<<l5<<" "<<r3<<endl;
        cout << "Amin: t4: " << t4 << " " << t << endl;

        cout << "Amin: Sorat: " << l3_v0 << " " <<  l4_v0 << " " << l5_v0 << " " << r3_v0 << endl;
        cout << "Amin: Shetab: " << l3_a << " " <<  l4_a << " " <<  l5_a << " " << r3_a << endl;
        ss<<moveJoint("lle3",l3)<<moveJoint("lle4",l4)<<moveJoint("lle5",l5)<<moveJoint("rle3" , r3);


    }
    else if (t < t5) {

        double deltaX = (100-lj3);
        double l3_a = 100;

        if (l3_v0*l3_v0 + 2*l3_a*deltaX >=0 && deltaX >= 0) {
//            cout << "Amin: Here " << l3_v0 << endl;
            t5 = t+(-l3_v0+sqrt(l3_v0*l3_v0 + 2*l3_a*deltaX))/l3_a;
        } else {
            t5 = max (t4 + 0.2 , t5);
        }
        if (t >= t5)
            return ss.str();
//        cout << "Amin: answers  " << (-l3_v0+sqrt(l3_v0*l3_v0 + 2*l3_a*deltaX))/l3_a  << " " << (-l3_v0-sqrt(l3_v0*l3_v0 + 2*l3_a*deltaX))/l3_a << endl;

        double l4_a = (0-lj4) / (0.5*(t5-t)*(   t5-t)) - l4_v0 / (0.5*(t5-t));
        double l5_a = (-45-lj5) / (0.5*(t5-t)*(t5-t)) - l5_v0 / (0.5*(t5-t));
        double r3_a = (-25-rj3) / (0.5*(t5-t)*(t5-t)) - r3_v0 / (0.5*(t5-t));

        double l3=0, l4=0, l5=0, r3=0;
        if (fabs(100-lj3) >= 6 ) {
//            cout << "Amin: " << t << " -> " << lj3 << endl;
           l3 = getAngle(t - t4, l3_v0, l3_a, &l3_v0);
           if (l3 < 0)
               l3 = 0;
        } else {
            l3 = 100 - lj3;
            l3_v0 = 0;
        }
        if (fabs(0-lj4) >= 6) {
            l4 = getAngle(t - t4, l4_v0, l4_a, &l4_v0);
        } else {
            l4 = 0 - lj4;
//            l4_v0 = 0;

        }
        if (fabs(-45-lj5) >= 6) {
            l5 = -3*fabs(0.02*(-45-lj5)/(t5-t));
        } else {
            l5 = -45-lj5;
//            l5_v0 = 0;

        }
        if (fabs(-25-rj3) >= 6) {
            r3 = getAngle(t - t4, r3_v0, r3_a, &r3_v0);
        } else {
            r3 = -25-rj3;
//            r3_v0 = 0;
        }


////        double l3 = 0.02*(80-lj3)/(t-t5);
////        double l4 = 0.02*(0-lj4)/(sqrt(t5)-sqrt(t));
////        double r3 = 0.02*(-45-rj3)/(sqrt(t5)-sqrt(t));
//        cout << "Amin: Shetab: " << l3_a << " " <<  l4_a << " " <<  l5_a << " " << r3_a << endl;

//        cout<<"Amin: t5->"<<l3<<" "<<l4<<" "<<l5<<" "<<r3<<endl;
        ss<<moveJoint("lle3",l3)<<moveJoint("lle4",l4)<<moveJoint("lle5",l5)<<moveJoint("rle3" , r3);
    }

    return ss.str();

}

double Skill::getAngle (double spent, double v0, double a, double *v) {
//    cout << "Amin: getAngle(): " << a << " " << v0 << " " << fabs(a*0.02 + v0) << endl;
    *v = fabs(a*0.02 + v0);
    return v0*0.02 + 0.5*a*(0.02*0.02 + 2*spent*0.02);

}

////////////////////////////////////////////////////////////////////
bool Skill::nazdik(string type) {
    if(type=="turnL" || type=="turnR" || type=="sideWalkR" || type=="sideWalkL" || type=="sideTurnL" || type=="sideTurnR")
        return true;
    return false;
}

void fk (double lle4, double lle5) {
    double t1, t2, t3, t4, t5, t6, t8, t9, t10, t12, t13, t14, t15, t24, t28, t36, t41, t48, t50;
    double unknown[4][4];

    t1 = cosDeg(lle5); t2 = cosDeg(lle4); t3 = t1*t2; t4 = sinDeg(lle5); t5 = sinDeg(lle4); t6 = t4*t5;
    t8 = t3-1.0*t6; t9 = lle4+lle5; t10 = cosDeg(t9); t12 = t1*t5; t13 = t4*t2; t14 = t12+t13;
    t15 = sinDeg(t9); t24 = 0.4E-1*t15+0.1E-1*t10-0.1E-1; t28 = -0.4E-1*t10+0.4E-1+0.1E-1*t15;
    t36 = -0.45E-1*t5-0.1E-1+0.1E-1*t2; t41 = -0.45E-1*t2+0.45E-1-0.1E-1*t5; t48 = -t13-1.0*t12; t50 = -t6+t3;

    unknown[0][0] = 1.0; unknown[0][1] = 0.0; unknown[0][2] = 0.0; unknown[0][3] = 0.0;
    unknown[1][0] = 0.0; unknown[1][1] = t8*t10+t14*t15; unknown[1][2] = -1.0*t8*t15+t14*t10; unknown[1][3] = t8*t24+t14*t28-0.5E-2*t3+0.5E-2*t6+0.125*t12+0.125*t13+1.0*t1*t36+1.0*t4*t41+0.1E-1*t1+0.55E-1*t4;
    unknown[2][0] = 0.0; unknown[2][1] = t48*t10+t50*t15; unknown[2][2] = -1.0*t48*t15+t50*t10; unknown[2][3] = t48*t24+t50*t28+0.5E-2*t13+0.5E-2*t12-0.125*t6+0.125*t3-1.0*t4*t36+1.0*t1*t41-0.1E-1*t4+0.55E-1*t1;
    unknown[3][0] = 0.0; unknown[3][1] = 0.0; unknown[3][2] = 0.0; unknown[3][3] = 1.0;
//          cout << "Amin: MyFK " << unknown[1][3] << " " << unknown[2][3] << endl;
//          for (int i = 0 ; i < 4 ; i ++) {
//              for (int j = 0 ; j < 4 ; j ++) {
//                  cout <<  (double)((int)1000*unknown[i][j])/1000 << "        ";
//              }
//              cout << endl;
//          }
//          cout << "\n\n";
}

string Skill::finalAction (string type,double &t,double maxV)
{
    WM->setZ(0.51);
    VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
    VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
    stringstream ss;
    static double df = 0;
    static double time=0;
    static double dd=0,bb=0,sd=2;
    static bool   done2=false;
    static bool   d2=false;

    static VecPosition H = FK_FOOT_HIP ( -65 , 45 ) ;
    static double T = 15 * 0.02;

    static double w = (2*M_PI)/T;
    double a = 0.05*dd  , b = -0.017*bb;
    if(nazdik(type))  b=-0.014;

    double a1 = a/2  , b1 = b/2;
    double x1 = a*cos(w*t) , y1 =b*sin(w*t)  ;
    double x2 = H.getX()+a1*cos(w*t) , y2 =H.getY()+b1*sin(w*t) ;


    double lj1 = WM->getJointAngle ( "lle1" ) ;
    double rj1 = WM->getJointAngle ( "rle1" ) ;
    double lj2 = WM->getJointAngle ( "lle2" ) ;
    double rj2 = WM->getJointAngle ( "rle2" ) ;
    double lj3 = WM->getJointAngle ( "lle3" ) ;
    double rj3 = WM->getJointAngle ( "rle3" ) ;
    double lj4 = WM->getJointAngle ( "lle4" ) ;
    double rj4 = WM->getJointAngle ( "rle4" ) ;
    double lj5 = WM->getJointAngle ( "lle5" ) ;
    double rj5 = WM->getJointAngle ( "rle5" ) ;
    double lj6 = WM->getJointAngle ( "lle6" ) ;
    double rj6 = WM->getJointAngle ( "rle6" ) ;
    double r3,r4,r5,l3,l4,l5,l11,lf11,lf22,l22,l33,d=0,f=0;
    double P = 0.5 ;
    if(t==0)
    {
        d2=false;
        time=0;
        dd=0;
        sd=2;
        bb=1;
    }

    if(d2)
    {
        speedControler(type,t,dd,bb,sd,maxV);
    }
    t+=0.02;

    /////////
    if ( !d2 )
    {
        stringstream s;
        s << Standing ( d2, time );
        dd=0;
        return s.str();
    }
    ////////
    if(type=="walk") {
        if ( IK_ALL ( VecPosition ( x2 , y2 ) , VecPosition ( x1 , y1 ) ,r3,r4,r5,l3,l4,l5  ) )
        {
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss<<moveJoint ( "rle1" , (0-rj1)*P)<<moveJoint ( "lle1" , (0-lj1)*P)<<moveJoint ( "rle2" , (0-rj2)*P)<<moveJoint ( "lle2" , (0-lj2)*P)<<moveJoint ( "rle6" , (0-rj6)*P)<<moveJoint ( "lle6" , (0-lj6)*P);
        }
    }
    else if(type=="bwalk") {

        if(me.getDistanceTo(ball)>0.4)
        {
            a = 0.06*dd;
        }
        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss<<moveJoint ( "rle1" , (0-rj1)*P)<<moveJoint ( "lle1" , (0-lj1)*P)<<moveJoint ( "rle2" , (0-rj2)*P)<<moveJoint ( "lle2" , (0-lj2)*P)<<moveJoint ( "rle6" , (0-rj6)*P)<<moveJoint ( "lle6" , (0-lj6)*P);
        }
    }
    else if(type=="walkAngleL")
    {

        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=5;
            }
            else
            {
                d=-5;
            }

            l11=d;
            l22=(-27/45)*d;
            l33=(-10/45)*d;
            ss << moveJoint ( "lle3" , (l3-lj3+(l33))*P) << moveJoint ( "lle4" , (l4-lj4-(2*l33))*P ) << moveJoint ( "lle5" , (l5-lj5+(l33))*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss<<moveJoint ( "lle1" , (l11-lj1)*P)<<moveJoint ( "rle1" , (l11-rj1)*P) << moveJoint ( "lle2" , (l22-lj2)*P );
            ss<<moveJoint ( "rle2" , (0-rj2)*P)<<moveJoint ( "rle6" , (0-rj6)*P)<<moveJoint ( "lle6" , (0-lj6)*P);
        }
    }
    else if(type=="walkAngleR")
    {

        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=-5;
            }
            else
            {
                d=5;
            }
            l11=d;
            l22=(-27/45)*d;
            l33=(-10/45)*d;
            ss << moveJoint ( "lle3" , (l3-lj3+l33)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5+l33)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss<<moveJoint ( "lle1" , (l11-lj1)*P)<<moveJoint ( "rle1" , (l11-rj1)*P) << moveJoint ( "rle2" , (l22-rj2)*P );
            ss<<moveJoint ( "lle2" , (0-lj2)*P)<<moveJoint ( "rle6" , (0-rj6)*P)<<moveJoint ( "lle6" , (0-lj6)*P);
        }
    }
    else if(type=="bwalkAngleL") {
        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=5;
            }
            else
            {
                d=-5;
            }

            l11=d;
            l22=(-27/45)*d;
            l33=(-10/45)*d;
            ss << moveJoint ( "lle3" , (l3-lj3+l33)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5+l33)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss<<moveJoint ( "lle1" , (l11-lj1)*P)<<moveJoint ( "rle1" , (l11-rj1)*P) << moveJoint ( "lle2" , (l22-lj2)*P );
            ss<<moveJoint ( "rle2" , (0-rj2)*P)<<moveJoint ( "rle6" , (0-rj6)*P)<<moveJoint ( "lle6" , (0-lj6)*P);
        }
    }
    else if(type=="bwalkAngleR") {
        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=-5;
            }
            else
            {
                d=5;
            }

            l11=d;
            l22=(-27/45)*d;
            l33=(-10/45)*d;
            ss << moveJoint ( "lle3" , (l3-lj3+l33)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5+l33)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss<<moveJoint ( "lle1" , (l11-lj1)*P)<<moveJoint ( "rle1" , (l11-rj1)*P) << moveJoint ( "rle2" , (l22-rj2)*P );
            ss<<moveJoint ( "lle2" , (0-lj2)*P)<<moveJoint ( "rle6" , (0-rj6)*P)<<moveJoint ( "lle6" , (0-lj6)*P);
        }
    }
    else if(type=="turnR") {
        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=-5;
            }
            else
            {
                d=5;
            }

            l11=d;
            l22=(-25/45)*d;
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss<<moveJoint ( "lle1" , (l11-lj1)*P)<<moveJoint ( "rle1" , (l11-rj1)*P) << moveJoint ( "rle2" , (l22-rj2)*P );
            ss<<moveJoint ( "lle2" , (0-lj2)*P)<<moveJoint ( "rle6" , (0-rj6)*P)<<moveJoint ( "lle6" , (0-lj6)*P);
        }
    }
    else if(type=="turnL") {
        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=5;
            }
            else
            {
                d=-5;
            }

            l11=d;
            l22=(-25/45)*d;
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss<<moveJoint ( "lle1" , (l11-lj1)*P)<<moveJoint ( "rle1" , (l11-rj1)*P) << moveJoint ( "lle2" , (l22-lj2)*P )<<moveJoint ( "lle6" , (l22-lj6)*P);
            ss<<moveJoint ( "rle2" , (0-rj2)*P)<<moveJoint ( "rle6" , (0-rj6)*P)<<moveJoint ( "lle6" , (0-lj6)*P);
        }
    }
    else if(type=="sideWithWalkR") {

        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=2;
            }
            else
            {
                d=-2;
            }

            l11=d/2.5;
            l22=-d;
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss << moveJoint ( "lle2" , (l22-lj2)*P )<< moveJoint ( "rle2" , -(l22+rj2)*P ) << moveJoint ( "lle6" , -(l22+lj6)*P )<< moveJoint ( "rle6" , (l22-rj6)*P )<<moveJoint ( "lle1" , (l11-lj1)*P);
            ss<<moveJoint ( "rle1" , (0-rj1)*P);
        }
    }
    else if(type=="sideWithWalkL") {

        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=2;
            }
            else
            {
                d=-2;
            }

            l11=-d/2.5;
            l22=-d;
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss << moveJoint ( "rle2" , (l22-rj2)*P )<< moveJoint ( "lle2" , -(l22+lj2)*P ) << moveJoint ( "rle6" , -(l22+rj6)*P )<< moveJoint ( "lle6" , (l22-lj6)*P )<<moveJoint ( "rle1" , (l11-rj1)*P);
            ss<<moveJoint ( "lle1" , (0-lj1)*P);
        }
    }
    else if(type=="sideWithbWalkR") {

        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=3;
            }
            else
            {
                d=-3;
            }

            l11=d/2.5;
            l22=-d;
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss << moveJoint ( "lle2" , (l22-lj2)*P )<< moveJoint ( "rle2" , -(l22+rj2)*P ) << moveJoint ( "lle6" , -(l22+lj6)*P )<< moveJoint ( "rle6" , (l22-rj6)*P )<<moveJoint ( "lle1" , (l11-lj1)*P);
            ss<<moveJoint ( "rle1" , (0-rj1)*P);
        }
    }
    else if(type=="sideWithbWalkL") {

        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=3;
            }
            else
            {
                d=-3;
            }

            l11=-d/2.5;
            l22=-d;
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss << moveJoint ( "rle2" , (l22-rj2)*P )<< moveJoint ( "lle2" , -(l22+lj2)*P ) << moveJoint ( "rle6" , -(l22+rj6)*P )<< moveJoint ( "lle6" , (l22-lj6)*P )<<moveJoint ( "rle1" , (l11-rj1)*P);
            ss<<moveJoint ( "lle1" , (0-lj1)*P);
        }
    }
    else if(type=="sideWalkR") {
        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=sd;
            }
            else
            {
                d=-sd;
            }

            l11=d/2.5;
            l22=-d;
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss << moveJoint ( "lle2" , (l22-lj2)*P )<< moveJoint ( "rle2" , -(l22+rj2)*P ) << moveJoint ( "lle6" , -(l22+lj6)*P )<< moveJoint ( "rle6" , (l22-rj6)*P )<<moveJoint ( "lle1" , (l11-lj1)*P);
            ss<<moveJoint ( "rle1" , (0-rj1)*P);
        }
    }
    else if(type=="sideWalkL") {
        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=sd;
            }
            else
            {
                d=-sd;
            }

            l11=-d/2.5;
            l22=-d;
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss << moveJoint ( "rle2" , (l22-rj2)*P )<< moveJoint ( "lle2" , -(l22+lj2)*P ) << moveJoint ( "rle6" , -(l22+rj6)*P )<< moveJoint ( "lle6" , (l22-lj6)*P )<<moveJoint ( "rle1" , (l11-rj1)*P);
            ss<<moveJoint ( "lle1" , (0-lj1)*P);
        }
    }
    else if(type=="sideTurnL") {
        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=4;
                f=4;
            }
            else
            {
                d=-4;
                f=-4;
            }

            lf11=f;
            lf22=(-25/45)*f;
            l11=-d/2.5;
            l22=-d;
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss << moveJoint ( "rle2" , (l22-rj2)*P )<< moveJoint ( "lle2" , (-(l22+lj2)+(lf22-lj2))/2*P ) << moveJoint ( "rle6" , -(l22+rj6)*P )<< moveJoint ( "lle6" , ((l22-lj6)+(lf22-lj6))/2*P );
            ss<<moveJoint ( "lle1" , (lf11-lj1)*P)<<moveJoint ( "rle1" , (lf11-rj1)*P);
            //ss<<moveJoint ( "lle1" , (0-lj1)*P);
        }
    }
    else if(type=="sideTurnR") {
        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            if (((l4-lj4)*P)<0)
            {
                d=4;
                f=-4;
            }
            else
            {
                d=-4;
                f=4;
            }
            lf11=f;
            lf22=(-25/45)*f;
            l11=d/2.5;
            l22=-d;
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss << moveJoint ( "lle2" , (l22-lj2)*P )<< moveJoint ( "rle2" , (-(l22+rj2)+(lf22-rj2))/2*P ) << moveJoint ( "lle6" , -(l22+lj6)*P )<< moveJoint ( "rle6" , ((l22-rj6)+(lf22-rj6))/2*P );
            ss<<moveJoint ( "lle1" , (lf11-lj1)*P)<<moveJoint ( "rle1" , (lf11-rj1)*P);
            //ss<<moveJoint ( "rle1" , (0-rj1)*P);
        }
    }
    else {

        if ( IK_ALL ( VecPosition(x2,y2) , VecPosition ( x1 , y1) ,r3,r4,r5,l3,l4,l5) )
        {
            ss << moveJoint ( "lle3" , (l3-lj3)*P) << moveJoint ( "lle4" , (l4-lj4)*P ) << moveJoint ( "lle5" , (l5-lj5)*P )
               << moveJoint ( "rle3" , (r3-rj3)*P) << moveJoint ( "rle4" , (r4-rj4)*P ) << moveJoint ( "rle5" , (r5-rj5)*P ) ;
            ss<<moveJoint ( "rle1" , (0-rj1)*P)<<moveJoint ( "lle1" , (0-lj1)*P)<<moveJoint ( "rle2" , (0-rj2)*P)<<moveJoint ( "lle2" , (0-lj2)*P)<<moveJoint ( "rle6" , (0-rj6)*P)<<moveJoint ( "lle6" , (0-lj6)*P);
        }

    }

    double A1=15,A2=30,fi=0,B1=-90+A1,B2=A2;
    static double TT,ww,ta=0;
    static int Tint=55;
    TT=Tint*0.02;
    ww=(2*M_PI)/TT;
    ss	<<moveJoint("lae1",B1+   A1*sin(ww*ta+fi)-WM->getJointAngle("lae1"))
        <<moveJoint("rae1",B1+-1*A1*sin(ww*ta+fi)-WM->getJointAngle("rae1"))
        <<moveJoint("lae4",-1*B2+  -1* A2*sin(ww*ta+fi)-WM->getJointAngle("lae4"))
        <<moveJoint("rae4",B2+-1*A2*sin(ww*ta+fi)-WM->getJointAngle("rae4"));
    ta+=.02;



    return ss.str();
}
void Skill::speedControler(string type,double t,double &dd,double &bb,double &sd,double maxV)
{
    VecPosition ball(WM->getBallPos().x(),WM->getBallPos().y());
    VecPosition me(WM->getMyPos().x(),WM->getMyPos().y());
    if(t==0)
    {
        dd=0;
    }
    if(type=="walk")
    {
        if(me.getDistanceTo(ball)>0.8)
        {
            if(dd > 0.98 && dd<(maxV+0.4))
            {
                dd+=0.005;
            }
            if(dd<1)
            {
                dd+=0.01;
            }
            if(dd>1 && bb<1.4)
            {
                bb+=0.005;
            }
        }
        else
        {
            if(bb<(0.75+(maxV-1)))
            {
                bb+=0.008;
            }
            if(bb>(0.75+(maxV-1)))
            {
                bb-=0.005;
            }
            if(dd<(maxV-0.3))
            {
                dd+=0.03;
            }
            if(dd>(maxV-0.3))
            {
                dd-=0.015;
            }
        }
    }
    else if(type=="walkAngleL" || type=="walkAngleR")
    {
        if(dd<1.1)
        {
            dd+=0.01;
        }
        if(dd>1.1)
        {
            dd-=0.02;
        }
        if(bb>1)
        {
            bb-=0.012;
        }
    }
    else if(type=="bwalkAngleL" || type=="bwalkAngleR")
    {
        if(dd>-1)
        {
            dd-=0.01;
        }
        if(bb>1)
        {
            bb-=0.005;
        }
    }
    else if(type=="bwalk" || type=="sideWithbWalkR" || type=="sideWithbWalkL")
    {
        if(dd>-1)
        {
            dd-=0.02;
        }
        else if(dd>-1.5)
        {
            dd-=0.005;
        }
        if(bb>1)
        {
            bb-=0.005;
        }
    }
    else if(type=="sideWithWalkR" || type=="sideWithWalkL")
    {
        if(dd<0.9)
        {
            dd+=0.02;
        }
        if(bb>1)
        {
            bb-=0.005;
        }
    }
    else
    {
        if(fabs(dd)<0.1)
        {
            dd=0;
        }
        else if(dd>0)
        {
            dd-=0.05;
        }
        else if(dd<0)
        {
            dd+=0.025;
        }
        if(bb>0.75)
        {
            bb-=0.008;
        }
    }
    if(type=="sideWalkR" || type=="sideWalkL" || type=="sideWithWalkR" || type=="sideWithWalkL" || type=="sideWithbWalkR" || type=="sideWithbWalkL")
    {
        if(sd<6)
            sd+=0.08;
        else
            sd=6;
    }
    else
    {
        sd=2;
    }
}

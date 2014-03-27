#include "Decide.h"
Decide::Decide(WorldModel *wm)
{
    //fout.open("log.txt");
    //out.open("log1.txt");
    WM = wm;
    SK = new Skill(WM);
    center = VecPosition(10.5, 0);
    beamed = false;
    ACT = new Act(WM, SK);
    //Log.AddLevel (1); // Goalie Debug
//    Log.AddLevel (2); // Attacker Debug
    //Log.AddLevel (3); // Deffender Debug
    //Log.AddLevel (4); // Middle Debug
    //Log.AddLevel (5); // Bools Debug
}

Decide::~Decide()
{
    delete SK;
    delete ACT;
}

string Decide::decide()
{
    Log.Info (WM->getServerTime());
    string ss;
    ss+=SK->fix();
    if (WM->getMyNum() == 1)
    {
        ss += goalie();
    }
    if (WM->getMyNum() == 2 || WM->getMyNum() == 3  || WM->getMyNum() == 4 || WM->getMyNum()==5)
    {
        ss += Deffend();
    }
    if ( WM->getMyNum() == 6 || WM->getMyNum() == 7 )
    {
        ss+= Middle();
    }
    if (WM->getMyNum() == 8 || WM->getMyNum() == 9 || WM->getMyNum()==11 || WM->getMyNum()==10)
    {
        ss += Attack();
    }
//    ss +=SK->updatem ( true );
    ss = WM->uniquee ( ss );
    return ss;
}

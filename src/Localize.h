#ifndef LOCALIZE_H
#define LOCALIZE_H
#include "WorldModel.h"


using namespace std;
class Localize{
public:
    Localize(WorldModel*);
    double Distance_Point_Line(Vector3f point_polar,line l);
    void test();
    WorldModel *WM;
};
#endif

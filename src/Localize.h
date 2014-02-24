#ifndef LOCALIZE_H
#define LOCALIZE_H
#include "WorldModel.h"
#include <vector>

using namespace std;

class Localize{
public:
    Localize(WorldModel*);
    double Distance_Point_Line(Vector3f point_polar,line l);
    Vector3f calcPlaneNormal( vector<Vector3f> planePoints );
    void test();
    WorldModel *WM;
};
#endif

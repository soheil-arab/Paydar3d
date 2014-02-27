#ifndef LOCALIZE_H
#define LOCALIZE_H
#include "WorldModel.h"
#include "Svd.h"

//#include "line.h"


using namespace std;
class Localize{
public:
    Localize(WorldModel*);
    double Distance_Polar_Point_Line(Vector3f point_polar,line l);
    double Distance_Cartesian_Point_Line(Vector3f point_polar,line l);
    Vector3f calcPlaneNormal(vector<Vector3f> planePoints);
    Matrix calcGlobalCoordinate(Vector3f norm);
    void test();
    WorldModel *WM;
    map<string,line> line_global;
};
#endif

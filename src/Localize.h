#ifndef LOCALIZE_H
#define LOCALIZE_H
#include "WorldModel.h"
#include "Svd.h"
//#include "line.h"
#include "Math.h"

using namespace std;
class Localize{
public:
    Localize(WorldModel*);
    double Distance_Polar_Point_Line(Vector3f point_polar,line l);
    double Distance_Cartesian_Point_Line(Vector3f point_polar,line l);
    Vector3f calcPlaneNormal(vector<Vector3f> planePoints);
    string line_recognitation(line line_to_detect);
    Vector3f line_intersection(line l1,line l2);

    void test();
    WorldModel *WM;
    map<string,line> line_global;
    vector<string> flag_names;
    vector<string> line_names;
};
#endif

#ifndef LOCALIZE_H
#define LOCALIZE_H
#include "WorldModel.h"
#include "Svd.h"
//#include "line.h"
#include "Math.h"
#include <eigen3/Eigen/Core>

using namespace std;
class Localize{
public:
    Localize(WorldModel*);
    double Distance_Polar_Point_Line(salt::Vector3f point_polar,line l);
    double Distance_Cartesian_Point_Line(salt::Vector3f point_polar,line l);
    salt::Vector3f calcPlaneNormal(vector<salt::Vector3f> planePoints);
    string line_recognitation(line line_to_detect);
    salt::Vector3f line_intersection(line l1,line l2);
    Eigen::MatrixXi overdetermined(Eigen::MatrixXi A,Eigen::VectorXf b);


    void test();
    WorldModel *WM;
    map<string,line> line_global;
    vector<string> flag_names;
    vector<string> line_names;
};
#endif

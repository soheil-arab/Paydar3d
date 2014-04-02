#ifndef LOCALIZE_H
#define LOCALIZE_H
#include "WorldModel.h"
#include "Math.h"
//#define EIGEN2_SUPPORT
#include <eigen3/Eigen/Core>

using namespace std;
class Localize{
public:
    Localize(WorldModel*);
    void finalLocalize();
    double Distance_Polar_Point_Line(salt::Vector3f point_polar,line l);
    double Distance_Cartesian_Point_Line(salt::Vector3f point_polar,line l);
    string line_recognitation(line line_to_detect);
    salt::Vector3f line_intersection(line l1,line l2);
    Eigen::MatrixXf overdetermined(Eigen::MatrixXf A,Eigen::MatrixXf b);
    double Distance_Cartesian_Point_Segment(salt::Vector3f point_cartesian, line l);
    void setLocalLines();
    void setImportantPoints();
    void resetEveryThing();
    salt::Vector3f find_other_end(line l,salt::Vector3f begin,double length);
    double lorentz(Eigen::MatrixXf A,Eigen::MatrixXf B);
    salt::Vector3f localize_with_lines();
    vector<double> quadratic_solver(double a, double b, double c);
    double find_distance_to_intesection(line l1,line l2);
    double Area(double a,double b,double c);
    void setX();
    void setY();
    Eigen::Matrix3d calcOrient();
    salt::Vector3f calcPlaneNormal(vector<salt::Vector3f> planePoints);
    bool isBeamed();
    void set_second_x_estimation();
    void add_position();
    void add_median_of_last_positions();
    void set_head_rotate(salt::Vector3f position);
    void gyroLocalize();
    salt::Vector3f general_rotation(salt::Vector3f,salt::Vector3f,double);
    void localizeOtherElement(salt::Vector3f myPos);
    void erfanKiriestLocalize();









    void test();
    WorldModel *WM;
    unordered_map<string,line> line_global;
    vector<string> flag_names;
    vector<string> line_names;
    unordered_map<string, salt::Vector3f> important_points_global;
    bool fr,fn,fs,pr,prn,prs,m,fl,pl,pln,pls;
    bool frfn,frfs,frprn,frprs,prprn,prprs,ms,mn,flfn,flfs,flpln,flpls,plpln,plpls;

    int counter_rvdraw;


private:

    unordered_map<string , line> lines_cartesian_local_position;
    unordered_map<string, line> lines_polar_local_position;
    unordered_map<string, double> distance_of_important_points;
    unordered_map<string , salt::Vector3f> importan_points;
    vector<double> X;
    vector<double> Y;
    double Y_initial_estimation,X_initial_estimation,X_second_estimation;
    bool beamed;
    vector<salt::Vector3f> last_positions;
    vector<double> median_of_last_positions_x;
    vector<double> median_of_last_positions_y;

    double sum;
    int counter;

};
#endif

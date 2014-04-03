

#include "Localize.h"
#include "WorldModel.h"
#include "line.h"
#include "string.h"
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/LU>
#include <eigen3/Eigen/SVD>
#include <eigen3/Eigen/Geometry>

using namespace std;

Localize::Localize(WorldModel* wm)
{
    WM = wm;

    counter_rvdraw = 0;
    Y_initial_estimation = 0;
    X_initial_estimation = 0;
    sum = 0;
    counter = 0;

    /*
     *             ---------------------------FR--------------------------
     *             |               |                    |                |
     *             |              PRN                  PRS               |
     *             |               |                    |                |
     *             |               |                    |                |
     *             |               ---------PR----------                 |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             FN                                                    FS
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             MN------------------------M---------------------------MS
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |                                                     |
     *             |               ---------PL----------                 |
     *             |               |                    |                |
     *             |               PLN                  PLS              |
     *             |               |                    |                |
     *             |               |                    |                |
     *              -----------------------FL-----------------------------
     */

    //defining the global position of the lines
    line_global["FR"] = line(Eigen::Vector3f(15, -10, 0), Eigen::Vector3f(15, 10, 0));
    line_global["FL"] = line(Eigen::Vector3f(-15, -10, 0), Eigen::Vector3f(-15, 10, 0));
    line_global["FN"] = line(Eigen::Vector3f(15, 10, 0), Eigen::Vector3f(-15, 10, 0));
    line_global["FS"] = line(Eigen::Vector3f(15, -10, 0), Eigen::Vector3f(-15, -10, 0));
    line_global["PRN"] = line(Eigen::Vector3f(15, 3, 0), Eigen::Vector3f(13.2, 3, 0));
    line_global["PRS"] = line(Eigen::Vector3f(13.2, -3, 0), Eigen::Vector3f(15, -3, 0));
    line_global["PR"] = line(Eigen::Vector3f(13.2, -3, 0), Eigen::Vector3f(13.2, 3, 0));
    line_global["PLN"] = line(Eigen::Vector3f(-15, 3, 0), Eigen::Vector3f(-13.2, 3, 0));
    line_global["PL"] = line(Eigen::Vector3f(-13.2, -3, 0), Eigen::Vector3f(-13.2, 3, 0));
    line_global["PLS"] = line(Eigen::Vector3f(-15, -3, 0), Eigen::Vector3f(-13.2, -3, 0));
    line_global["M"] = line(Eigen::Vector3f(0, 10, 0), Eigen::Vector3f(0, -10, 0));

    important_points_global["G1R"] = Eigen::Vector3f(15, 1.05, 0.8);
    important_points_global["G2R"] = Eigen::Vector3f(15, -1.05, 0.8);
    important_points_global["F1R"] = Eigen::Vector3f(15, 10, 0);
    important_points_global["F2R"] = Eigen::Vector3f(15, -10, 0);
    important_points_global["G1L"] = Eigen::Vector3f(-15, 1.05, 0.8);
    important_points_global["G2L"] = Eigen::Vector3f(-15, -1.05, 0.8);
    important_points_global["F1L"] = Eigen::Vector3f(-15, 10, 0);
    important_points_global["F2L"] = Eigen::Vector3f(-15, -10, 0);
    important_points_global["FRFS"] = Eigen::Vector3f(15, -10, 0);
    important_points_global["FRFN"] = Eigen::Vector3f(15, 10, 0);
    important_points_global["FRPRS"] = Eigen::Vector3f(15, -3, 0);
    important_points_global["FRPRN"] = Eigen::Vector3f(15, 3, 0);
    important_points_global["PRPRS"] = Eigen::Vector3f(13.2, -3, 0);
    important_points_global["PRPRN"] = Eigen::Vector3f(13.2, 3, 0);
    important_points_global["MS"] = Eigen::Vector3f(0, -10, 0);
    important_points_global["MN"] = Eigen::Vector3f(0, 10, 0);

    important_points_global["FLFS"] = Eigen::Vector3f(-15, -10, 0);
    important_points_global["FLFN"] = Eigen::Vector3f(-15, 10, 0);
    important_points_global["FLPLS"] = Eigen::Vector3f(-15, -3, 0);
    important_points_global["FLPLN"] = Eigen::Vector3f(-15, 3, 0);
    important_points_global["PLPLS"] = Eigen::Vector3f(-13.2, -3, 0);
    important_points_global["PLPLN"] = Eigen::Vector3f(-13.2, 3, 0);

    flag_names.push_back("G1L");
    flag_names.push_back("G2L");
    flag_names.push_back("G1R");
    flag_names.push_back("G2R");
    flag_names.push_back("F1L");
    flag_names.push_back("F2L");
    flag_names.push_back("F1R");
    flag_names.push_back("F2R");

    line_names.push_back("FL");
    line_names.push_back("FR");
    line_names.push_back("FN");
    line_names.push_back("FS");
    line_names.push_back("PRN");
    line_names.push_back("PRS");
    line_names.push_back("PR");
    line_names.push_back("PLN");
    line_names.push_back("PL");
    line_names.push_back("PLS");
    line_names.push_back("M");
}

void Localize::finalLocalize()
{
    setLocalLines();
    setImportantPoints();
    setX();
    setY();
    set_second_x_estimation();
    add_position();
    add_median_of_last_positions();
    static bool recalib = true;
    if (isBeamed()) {
        recalib = false;
        cout << "salam" << endl;
    } else if (recalib)
        gyroLocalize();
    if (!WM->isFeltDown() && !recalib) {
        //        cout << "halllle " << endl;
        set_head_rotate(Eigen::Vector3f(X_second_estimation, Y_initial_estimation, 0.516));
        WM->setMyPos(Eigen::Vector3f(X_second_estimation, Y_initial_estimation, 0.516));
        localizeOtherElement(WM->getMyPos());
        recalib = true;
    }
    resetEveryThing();
    static int c = 0;
    c++;
    //    RVDraw::instance()->drawEigen::Vector3f(WM->getMyPos(), RED, c,3);
    c++;
    //    RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos,BLACK,c,3);
}

void Localize::setLocalLines()
{
    vector<line> lines = WM->getLastSeenLines();
    int size = lines.size();
    line l_polar;
    line l_cartesian;
    string line_name;

    for (int i = 0; i < size; i++) {
        l_polar = lines.at(i);
        l_cartesian = l_polar.line_PolarToCartesian();
//        cout<<"tool: "<<l_cartesian.getLength()<<endl;
        line_name = line_recognitation(l_cartesian);
        if (line_name != "the others") {
            lines_polar_local_position[line_name] = l_polar;
            lines_cartesian_local_position[line_name] = l_cartesian;

        }
    }
}

void Localize::setImportantPoints()
{
    fr = false;
    fn = false;
    fs = false;
    pr = false;
    prn = false;
    prs = false;
    m = false;
    fl = false;
    pl = false;
    pln = false;
    pls = false;
    frfn = false;
    frfs = false;
    frprn = false;
    frprs = false;
    prprn = false;
    prprs = false;
    ms = false;
    mn = false,
            flfn = false, flfs = false, flpln = false, flpls = false, plpln = false, plpls = false;

    //    if(!(lines_cartesian_local_position["FR"].begin.x() ==0 && lines_cartesian_local_position["FR"].begin.y() == 0 && lines_cartesian_local_position["FR"].begin.z() ==0 ))
    if (lines_cartesian_local_position.find("FR") != lines_cartesian_local_position.end()) {
        fr = true;
    }
    //    if(!(lines_cartesian_local_position["FN"].begin.x() ==0 && lines_cartesian_local_position["FN"].begin.y() == 0 && lines_cartesian_local_position["FN"].begin.z() ==0 ))
    if (lines_cartesian_local_position.find("FN") != lines_cartesian_local_position.end()) {
        fn = true;
    }
    //    if(!(lines_cartesian_local_position["FS"].begin.x() ==0 && lines_cartesian_local_position["FS"].begin.y() == 0 && lines_cartesian_local_position["FS"].begin.z() ==0 ))
    if (lines_cartesian_local_position.find("FS") != lines_cartesian_local_position.end()) {
        fs = true;
    }
    //    if(!(lines_cartesian_local_position["PR"].begin.x() ==0 && lines_cartesian_local_position["PR"].begin.y() == 0 && lines_cartesian_local_position["PR"].begin.z() ==0 ))
    if (lines_cartesian_local_position.find("PR") != lines_cartesian_local_position.end()) {
        pr = true;
    }
    //    if(!(lines_cartesian_local_position["PRN"].begin.x() ==0 && lines_cartesian_local_position["PRN"].begin.y() == 0 && lines_cartesian_local_position["PRN"].begin.z() ==0 ))
    if (lines_cartesian_local_position.find("PRN") != lines_cartesian_local_position.end()) {
        prn = true;
    }
    //    if(!(lines_cartesian_local_position["PRS"].begin.x() ==0 && lines_cartesian_local_position["PRS"].begin.y() == 0 && lines_cartesian_local_position["PRS"].begin.z() ==0 ))
    if (lines_cartesian_local_position.find("PRS") != lines_cartesian_local_position.end()) {
        prs = true;
    }
    //    if(!(lines_cartesian_local_position["M"].begin.x() ==0 && lines_cartesian_local_position["M"].begin.y() == 0 && lines_cartesian_local_position["M"].begin.z() ==0 ))
    if (lines_cartesian_local_position.find("M") != lines_cartesian_local_position.end()) {
        m = true;
    }

    //    if(!(lines_cartesian_local_position["FL"].begin.x() ==0 && lines_cartesian_local_position["FL"].begin.y() == 0 && lines_cartesian_local_position["FL"].begin.z() ==0 ))
    if (lines_cartesian_local_position.find("FL") != lines_cartesian_local_position.end()) {
        fl = true;
    }
    //    if(!(lines_cartesian_local_position["PL"].begin.x() ==0 && lines_cartesian_local_position["PL"].begin.y() == 0 && lines_cartesian_local_position["PL"].begin.z() ==0 ))
    if (lines_cartesian_local_position.find("PL") != lines_cartesian_local_position.end()) {
        pl = true;
    }
    //    if(!(lines_cartesian_local_position["PLN"].begin.x() ==0 && lines_cartesian_local_position["PLN"].begin.y() == 0 && lines_cartesian_local_position["PLN"].begin.z() ==0 ))
    if (lines_cartesian_local_position.find("PLN") != lines_cartesian_local_position.end()) {
        pln = true;
    }
    //    if(!(lines_cartesian_local_position["PLS"].begin.x() ==0 && lines_cartesian_local_position["PLS"].begin.y() == 0 && lines_cartesian_local_position["PLS"].begin.z() ==0 ))
    if (lines_cartesian_local_position.find("PLS") != lines_cartesian_local_position.end()) {
        pls = true;
    }

    //set the flag pos
    if (WM->getFlagLastSeen("G1R") == WM->serverTime) {
        importan_points["G1R"] = WM->flag["G1R"];
        distance_of_important_points["G1R"] = importan_points["G1R"].norm();
    }
    if (WM->getFlagLastSeen("G2R") == WM->serverTime) {
        importan_points["G2R"] = WM->flag["G2R"];
        distance_of_important_points["G2R"] = importan_points["G2R"].norm();
    }
    if (WM->getFlagLastSeen("F1R") == WM->serverTime && !frfn) {
        importan_points["FRFN"] = WM->flag["F1R"];
        distance_of_important_points["FRFN"] = importan_points["FRFN"].norm();
        frfn = true;
    }
    if (WM->getFlagLastSeen("F2R") == WM->serverTime && !frfs) {
        importan_points["FRFS"] = WM->flag["F2R"];
        distance_of_important_points["FRFS"] = importan_points["FRFS"].norm();
        frfs = true;
    }

    if (WM->getFlagLastSeen("G1L") == WM->serverTime) {
        importan_points["G1L"] = WM->flag["G1L"];
        distance_of_important_points["G1L"] = importan_points["G1L"].norm();
    }
    if (WM->getFlagLastSeen("G2L") == WM->serverTime) {
        importan_points["G2L"] = WM->flag["G2L"];
        distance_of_important_points["G2L"] = importan_points["G2L"].norm();
    }
    if (WM->getFlagLastSeen("F1L") == WM->serverTime && !flfn) {
        importan_points["FLFN"] = WM->flag["F1L"];
        distance_of_important_points["FLFN"] = importan_points["FLFN"].norm();
        flfn = true;
    }
    if (WM->getFlagLastSeen("F2L") == WM->serverTime && !flfs) {
        importan_points["FLFS"] = WM->flag["F2L"];
        distance_of_important_points["FLFS"] = importan_points["FLFS"].norm();
        flfs = true;
    }

    //here we set the important points of the intersections of the Right Side

    if (fr && fn) {
        importan_points["FRFN"] = line_intersection(lines_cartesian_local_position["FR"], lines_cartesian_local_position["FN"]);
        distance_of_important_points["FRFN"] = importan_points["FRFN"].norm();
        frfn = true;
    }
    if (fr && fs) {
        importan_points["FRFS"] = line_intersection(lines_cartesian_local_position["FR"], lines_cartesian_local_position["FS"]);
        distance_of_important_points["FRFS"] = importan_points["FRFS"].norm();
        frfs = true;
    }
    if (fr && prn) {
        importan_points["FRPRN"] = line_intersection(lines_cartesian_local_position["FR"], lines_cartesian_local_position["PRN"]);
        distance_of_important_points["FRPRN"] = importan_points["FRPRN"].norm();
        frprn = true;
    }
    if (fr && prs) {
        importan_points["FRPRS"] = line_intersection(lines_cartesian_local_position["FR"], lines_cartesian_local_position["PRS"]);
        distance_of_important_points["FRPRS"] = importan_points["FRPRS"].norm();
        frprs = true;
    }
    if (pr && prn) {
        importan_points["PRPRN"] = line_intersection(lines_cartesian_local_position["PR"], lines_cartesian_local_position["PRN"]);
        distance_of_important_points["PRPRN"] = importan_points["PRPRN"].norm();
        prprn = true;
    }
    if (pr && prs) {
        importan_points["PRPRS"] = line_intersection(lines_cartesian_local_position["PR"], lines_cartesian_local_position["PRS"]);
        distance_of_important_points["PRPRS"] = importan_points["PRPRS"].norm();
        prprs = true;
    }
    if (m && fs) {
        importan_points["MS"] = line_intersection(lines_cartesian_local_position["FS"], lines_cartesian_local_position["M"]);
        distance_of_important_points["MS"] = importan_points["MS"].norm();
        ms = true;
    }
    if (m && fn) {
        importan_points["MN"] = line_intersection(lines_cartesian_local_position["FN"], lines_cartesian_local_position["M"]);
        distance_of_important_points["MN"] = importan_points["MN"].norm();
        mn = true;
    }

    //here we set the important points of the intersections of the Left side
    if (fl && fn) {
        importan_points["FLFN"] = line_intersection(lines_cartesian_local_position["FL"], lines_cartesian_local_position["FN"]);
        distance_of_important_points["FLFN"] = importan_points["FLFN"].norm();
        flfn = true;
    }
    if (fl && fs) {
        importan_points["FLFS"] = line_intersection(lines_cartesian_local_position["FL"], lines_cartesian_local_position["FS"]);
        distance_of_important_points["FLFS"] = importan_points["FLFS"].norm();
        flfs = true;
    }
    if (fl && pln) {
        importan_points["FLPLN"] = line_intersection(lines_cartesian_local_position["FL"], lines_cartesian_local_position["PLN"]);
        distance_of_important_points["FLPLN"] = importan_points["FLPLN"].norm();
        flpln = true;
    }
    if (fl && pls) {
        importan_points["FLPLS"] = line_intersection(lines_cartesian_local_position["FL"], lines_cartesian_local_position["PLS"]);
        distance_of_important_points["FLPLS"] = importan_points["FLPLS"].norm();
        flpls = true;
    }
    if (pl && pln) {
        importan_points["PLPLN"] = line_intersection(lines_cartesian_local_position["PL"], lines_cartesian_local_position["PLN"]);
        //        distance_of_important_points["PLPLN"] = find_distance_to_intesection(lines_polar_local_position["PL"] , lines_polar_local_position["PLN"]);
        distance_of_important_points["PLPLN"] = importan_points["PLPLN"].norm();
        plpln = true;
    }
    if (pl && pls) {
        importan_points["PLPLS"] = line_intersection(lines_cartesian_local_position["PL"], lines_cartesian_local_position["PLS"]);
        //        distance_of_important_points["PLPLS"] = find_distance_to_intesection(lines_polar_local_position["PL"] , lines_polar_local_position["PLS"]);
        distance_of_important_points["PLPLS"] = importan_points["PLPLS"].norm();
        plpls = true;
    }
    if (m && fs) {
        importan_points["MS"] = line_intersection(lines_cartesian_local_position["FS"], lines_cartesian_local_position["M"]);
        distance_of_important_points["MS"] = importan_points["MS"].norm();
        ms = true;
    }
    if (m && fn) {
        importan_points["MN"] = line_intersection(lines_cartesian_local_position["FN"], lines_cartesian_local_position["M"]);
        distance_of_important_points["MN"] = importan_points["MN"].norm();
        mn = true;
    }

    //set the other ends of the lines of the Left side

    if (m && ms && !mn) {
        importan_points["MN"] = find_other_end(lines_cartesian_local_position["M"], importan_points["MS"], 20);
        distance_of_important_points["MN"] = importan_points["MN"].norm();
        mn = true;
    }
    if (m && mn && !ms) {
        importan_points["MS"] = find_other_end(lines_cartesian_local_position["M"], importan_points["MN"], 20);
        distance_of_important_points["MS"] = importan_points["MS"].norm();
        ms = true;
    }

    if (pl && plpls && !plpln) {
        importan_points["PLPLN"] = find_other_end(lines_cartesian_local_position["PL"], importan_points["PLPLS"], 6);
        distance_of_important_points["PLPLN"] = importan_points["PLPLN"].norm();
        plpln = true;
    }
    if (pl && plpln && !plpls) {
        importan_points["PLPLS"] = find_other_end(lines_cartesian_local_position["PL"], importan_points["PLPLN"], 6);
        distance_of_important_points["PLPLS"] = importan_points["PLPLS"].norm();
        plpls = true;
    }
    if (pls && flpls && !plpls) {
        importan_points["PLPLS"] = find_other_end(lines_cartesian_local_position["PLS"], importan_points["FLPLS"], 1.8);
        distance_of_important_points["PLPLS"] = importan_points["PLPLS"].norm();
        plpls = true;
    }
    if (pls && plpls && !flpls) {
        importan_points["FLPLS"] = find_other_end(lines_cartesian_local_position["PLS"], importan_points["PLPLS"], 1.8);
        distance_of_important_points["FLPLS"] = importan_points["FLPLS"].norm();
        flpls = true;
    }
    if (pln && plpln && !flpln) {
        importan_points["FLPLN"] = find_other_end(lines_cartesian_local_position["PLN"], importan_points["PLPLN"], 1.8);
        distance_of_important_points["FLPLN"] = importan_points["FLPLN"].norm();
        flpln = true;
    }
    if (pln && flpln && !plpln) {
        importan_points["PLPLN"] = find_other_end(lines_cartesian_local_position["PLN"], importan_points["FLPLN"], 1.8);
        distance_of_important_points["PLPLN"] = importan_points["PLPLN"].norm();
        plpln = true;
    }
    if (fs && flfs && !ms) {
        importan_points["MS"] = find_other_end((lines_cartesian_local_position["FS"]), importan_points["FLFS"], 15);
        distance_of_important_points["MS"] = importan_points["MS"].norm();
        ms = true;
    }
    if (fn && flfn && !mn) {
        importan_points["MN"] = find_other_end((lines_cartesian_local_position["FN"]), importan_points["FLFN"], 15);
        distance_of_important_points["MN"] = importan_points["MN"].norm();
        mn = true;
    }

    //set the other ends of the right side

    if (fr && frfn && !frfs) {
        importan_points["FRFS"] = find_other_end(lines_cartesian_local_position["FR"], importan_points["FRFN"], 20);
        distance_of_important_points["FRFS"] = importan_points["FRFS"].norm();
        frfs = true;
    }
    if (fr && frfn && !frfs) {
        importan_points["FRFS"] = find_other_end(lines_cartesian_local_position["FR"], importan_points["FRFN"], 20);
        distance_of_important_points["FRFS"] = importan_points["FRFS"].norm();
        frfs = true;
    }
    if (fr && frfs && !frfn) {
        importan_points["FRFN"] = find_other_end(lines_cartesian_local_position["FR"], importan_points["FRFS"], 20);
        distance_of_important_points["FRFN"] = importan_points["FRFN"].norm();
        frfn = true;
    }
    if (pr && prprs && !prprn) {
        importan_points["PRPRN"] = find_other_end(lines_cartesian_local_position["PR"], importan_points["PRPRS"], 6);
        distance_of_important_points["PRPRN"] = importan_points["PRPRN"].norm();
        prprn = true;
    }
    if (pr && prprn && !prprs) {
        importan_points["PRPRS"] = find_other_end(lines_cartesian_local_position["PR"], importan_points["PRPRN"], 6);
        distance_of_important_points["PRPRS"] = importan_points["PRPRS"].norm();
        prprs = true;
    }
    if (prs && frprs && !prprs) {
        importan_points["PRPRS"] = find_other_end(lines_cartesian_local_position["PRS"], importan_points["FRPRS"], 1.8);
        distance_of_important_points["PRPRS"] = importan_points["PRPRS"].norm();
        prprs = true;
    }
    if (prs && prprs && !frprs) {
        importan_points["FRPRS"] = find_other_end(lines_cartesian_local_position["PRS"], importan_points["PRPRS"], 1.8);
        distance_of_important_points["FRPRS"] = importan_points["FRPRS"].norm();
        frprs = true;
    }
    if (prn && prprn && !frprn) {
        importan_points["FRPRN"] = find_other_end(lines_cartesian_local_position["PRN"], importan_points["PRPRN"], 1.8);
        distance_of_important_points["FRPRN"] = importan_points["FRPRN"].norm();
        frprn = true;
    }
    if (prn && frprn && !prprn) {
        importan_points["PRPRN"] = find_other_end(lines_cartesian_local_position["PRN"], importan_points["FRPRN"], 1.8);
        distance_of_important_points["PRPRN"] = importan_points["PRPRN"].norm();
        prprn = true;
    }
    /*    if(fs && frfs && !ms)
    {
        importan_points["MS"] = find_other_end((lines_cartesian_local_position["FS"]) , importan_points["FRFS"],15);
        distance_of_important_points["MS"] = importan_points["MS"].norm();
        ms = true;

    }
    if(fn && frfn && !mn)
    {
        importan_points["MN"] = find_other_end((lines_cartesian_local_position["FN"]) , importan_points["FRFN"],15);
        distance_of_important_points["MN"] = importan_points["MN"].norm();
        mn = true;

    }

*/
}

///this function will reset every importnat point,line, and lines_we_see in world model
void Localize::resetEveryThing()
{
    lines_cartesian_local_position.clear();
    lines_polar_local_position.clear();
    importan_points.clear();
    distance_of_important_points.clear();
    X.clear();
    Y.clear();

    WM->resetLastSeenLines();
}

void Localize::setX()
{
    double h;
    double s;
    double x1, x2;
    double len1, len2;
    double base;
    string name1, name2;

    if (frprs && prprs) {
        name1 = "frprs";
        name2 = "prprs";
        base = 1.8;
        len1 = distance_of_important_points["FRPRS"];
        len2 = distance_of_important_points["PRPRS"];
        s = Area(len1, len2, base);
        h = 2 * s / base;
        x1 = sqrt(len1 * len1 - h * h);
        x2 = sqrt(len2 * len2 - h * h);

        X.push_back(15 - x1);
    }

    if (flpls && plpls) {
        name1 = "flpls";
        name2 = "plpls";
        base = 1.8;
        len1 = distance_of_important_points["FLPLS"];
        len2 = distance_of_important_points["PLPLS"];
        s = Area(len1, len2, base);
        h = 2 * s / base;
        x1 = sqrt(len1 * len1 - h * h);
        x2 = sqrt(len2 * len2 - h * h);

        X.push_back(15 - x1);
    }

    if (frprn && prprn) {

        name1 = "frprn";
        name2 = "prprn";
        base = 1.8;
        len1 = distance_of_important_points["FRPRN"];
        len2 = distance_of_important_points["PRPRN"];
        s = Area(len1, len2, base);
        h = 2 * s / base;
        x1 = sqrt(len1 * len1 - h * h);
        x2 = sqrt(len2 * len2 - h * h);

        X.push_back(15 - x1);
    }

    if (flpln && plpln) {

        name1 = "flpln";
        name2 = "plpln";
        base = 1.8;
        len1 = distance_of_important_points["FLPLN"];
        len2 = distance_of_important_points["PLPLN"];
        s = Area(len1, len2, base);
        h = 2 * s / base;
        x1 = sqrt(len1 * len1 - h * h);
        x2 = sqrt(len2 * len2 - h * h);

        X.push_back(15 - x1);
    }

    if (ms && frfs) {

        name1 = "ms";
        name2 = "frfs";
        base = 15;

        len1 = distance_of_important_points["FRFS"];
        len2 = distance_of_important_points["MS"];
        s = Area(len1, len2, base);
        h = 2 * s / base;
        x1 = sqrt(len1 * len1 - h * h);
        x2 = sqrt(len2 * len2 - h * h);

        X.push_back(15 - x1);
    }

    if (ms && flfs) {

        name1 = "ms";
        name2 = "flfs";
        base = 15;

        len1 = distance_of_important_points["FLFS"];
        len2 = distance_of_important_points["MS"];
        s = Area(len1, len2, base);
        h = 2 * s / base;
        x1 = sqrt(len1 * len1 - h * h);
        x2 = sqrt(len2 * len2 - h * h);

        X.push_back(15 - x1);
    }

    if (mn && frfn) {

        name1 = "mn";
        name2 = "frfn";
        base = 15;
        len1 = distance_of_important_points["FRFN"];
        len2 = distance_of_important_points["MN"];
        s = Area(len1, len2, base);
        h = 2 * s / base;
        x1 = sqrt(len1 * len1 - h * h);
        x2 = sqrt(len2 * len2 - h * h);

        X.push_back(15 - x1);
    }

    if (mn && flfn) {

        name1 = "mn";
        name2 = "flfn";
        base = 15;
        len1 = distance_of_important_points["FLFN"];
        len2 = distance_of_important_points["MN"];
        s = Area(len1, len2, base);
        h = 2 * s / base;
        x1 = sqrt(len1 * len1 - h * h);
        x2 = sqrt(len2 * len2 - h * h);

        X.push_back(15 - x1);
    }

    if (X.size() > 0) {
        sort(X.begin(), X.end());
        X_initial_estimation = X.at(X.size() / 2);

    } else {
        //        cout<<WM->serverTime<<endl;
    }
}

void Localize::setY()
{
    double h;
    double s;
    double y1, y2;
    double len1, len2;
    double base;
    string name1, name2;

    //    if(m && ms && mn && Distance_Cartesian_Point_Line(Eigen::Vector3f(0,0,0),lines_cartesian_local_position["M"]) > 1 && lines_cartesian_local_position["M"].begin.x() > 0.5 )
    //    {
    //        name1="mn";
    //        name2="ms";
    //        base = 20;
    //        len1 = distance_of_important_points["MN"];len2 = distance_of_important_points["MS"] ;
    //        s=Area(len1 ,len2 ,base);
    //        h=2*s/base;
    //        y1=sqrt(len1*len1 - h*h);
    //        y2=sqrt(len2*len2 - h*h);

    //        if(y2 > y1 && y2 >base)
    //        {
    //            Y.push_back(10 + y1);
    //            //            cout<<name1<<","<<name2<<" is"<<10 + y1<<" real "<<WM->sensedPos.y()<<endl;
    //        }
    //        else
    //        {
    //            Y.push_back(10 - y1);
    //            //            cout<<name1<<","<<name2<<" is"<<10 - y1<<" real "<<WM->sensedPos.y()<<endl;
    //        }
    //        cout<<"avalie"<<endl;
    //    }

    //    else{
    //        cout<<"dovomie"<<endl;
    if (m && ms && mn) {
        name1 = "mn";
        name2 = "ms";
        base = 20;
        len1 = distance_of_important_points["MN"];
        len2 = distance_of_important_points["MS"];
        s = Area(len1, len2, base);
        h = 2 * s / base;
        y1 = sqrt(len1 * len1 - h * h);
        y2 = sqrt(len2 * len2 - h * h);


        if (!(gIsNan(y1) || gIsNan(y2))){
            if (y2 > y1 && y2 > base) {
                Y.push_back(10 + y1);
                //            cout<<name1<<","<<name2<<" is"<<10 + y1<<" real "<<WM->sensedPos.y()<<endl;
            } else {
                Y.push_back(10 - y1);
                //            cout<<name1<<","<<name2<<" is"<<10 - y1<<" real "<<WM->sensedPos.y()<<endl;
            }
        }
    }

    if ((frfn && frfs) || (flfn && flfs)) {
        if (frfn && frfs) {
            name1 = "frfn";
            name2 = "frfs";
            len1 = distance_of_important_points["FRFN"];
            len2 = distance_of_important_points["FRFS"];
        } else {
            name1 = "flfn";
            name2 = "flfs";
            len1 = distance_of_important_points["FLFN"];
            len2 = distance_of_important_points["FLFS"];
        }

        base = 20;
        s = Area(len1, len2, base);
        h = 2 * s / base;
        y1 = sqrt(len1 * len1 - h * h);
        y2 = sqrt(len2 * len2 - h * h);


        if (!(gIsNan(y1) || gIsNan(y2))){
            if (y2 > y1 && y2 > base) {
                Y.push_back(10 + y1);
                //            cout<<name1<<","<<name2<<" is"<<10 + y1<<" real "<<WM->sensedPos.y()<<endl;
            } else {
                Y.push_back(10 - y1);
                //            cout<<name1<<","<<name2<<" is"<<10 - y1<<" real "<<WM->sensedPos.y()<<endl;
            }
        }
    }
    if ((frfn && frprs) || (flfn && flpls)) {

        if (frfn && frprs) {
            name1 = "frfn";
            name2 = "frprs";
            len1 = distance_of_important_points["FRFN"];
            len2 = distance_of_important_points["FRPRS"];
        } else {
            name1 = "flfn";
            name2 = "flpls";
            len1 = distance_of_important_points["FLFN"];
            len2 = distance_of_important_points["FLPLS"];
        }

        base = 13;
        s = Area(len1, len2, base);
        h = 2 * s / base;
        y1 = sqrt(len1 * len1 - h * h);
        y2 = sqrt(len2 * len2 - h * h);


        if (!(gIsNan(y1) || gIsNan(y2))){
            if (y2 > y1 && y2 > base) {
                Y.push_back(10 + y1);
                //            cout<<name1<<","<<name2<<" is"<<10 + y1<<" real "<<WM->sensedPos.y()<<endl;
            } else {
                Y.push_back(10 - y1);
                //            cout<<name1<<","<<name2<<" is"<<10 - y1<<" real "<<WM->sensedPos.y()<<endl;
            }
        }
    }
    if ((frprn && frfs) || (flpln && flfs)) {
        if ((frprn && frfs)) {
            name1 = "frprn";
            name2 = "frfs";
            len1 = distance_of_important_points["FRPRN"];
            len2 = distance_of_important_points["FRFS"];

        } else {
            name2 = "flpln";
            name2 = "flfs";
            len1 = distance_of_important_points["FLPLN"];
            len2 = distance_of_important_points["FLFS"];
        }

        base = 13;

        s = Area(len1, len2, base);
        h = 2 * s / base;
        y1 = sqrt(len1 * len1 - h * h);
        y2 = sqrt(len2 * len2 - h * h);

        if (!(gIsNan(y1) || gIsNan(y2))){
            if (y2 > y1 && y2 > base) {
                Y.push_back(3 + y1);
                //            cout<<name1<<","<<name2<<" is"<<3 + y1<<" real "<<WM->sensedPos.y()<<endl;

            } else {
                Y.push_back(3 - y1);
                //            cout<<name1<<","<<name2<<" is"<<3 - y1<<" real "<<WM->sensedPos.y()<<endl;
            }
        }
    }
    if ((frprs && frfs) || (flpls && flfs)) {
        if ((frprs && frfs)) {
            name1 = "frprs";
            name2 = "frfs";
            len1 = distance_of_important_points["FRPRS"];
            len2 = distance_of_important_points["FRFS"];
        } else {
            name1 = "flpls";
            name2 = "flfs";
            len1 = distance_of_important_points["FLPLS"];
            len2 = distance_of_important_points["FLFS"];
        }

        base = 7;
        s = Area(len1, len2, base);
        h = 2 * s / base;
        y1 = sqrt(len1 * len1 - h * h);
        y2 = sqrt(len2 * len2 - h * h);


        if (!(gIsNan(y1) || gIsNan(y2))){
            if (y2 > y1 && y2 > base) {
                Y.push_back(-3 + y1);
                //            cout<<name1<<","<<name2<<" is"<<-3 + y1<<" real "<<WM->sensedPos.y()<<endl;
            } else {
                Y.push_back(-3 - y1);
                //            cout<<name1<<","<<name2<<" is"<<-3 - y1<<" real "<<WM->sensedPos.y()<<endl;
            }
        }
    }
    if ((frfn && frprn) || (flfn && flpln)) {
        if ((frfn && frprn)) {
            name1 = "frfn";
            name2 = "frprn";
            len1 = distance_of_important_points["FRFN"];
            len2 = distance_of_important_points["FRPRN"];
        } else {
            name1 = "flfn";
            name2 = "flpln";
            len1 = distance_of_important_points["FLFN"];
            len2 = distance_of_important_points["FLPLN"];
        }

        base = 7;
        s = Area(len1, len2, base);
        h = 2 * s / base;
        y1 = sqrt(len1 * len1 - h * h);
        y2 = sqrt(len2 * len2 - h * h);

        if (!(gIsNan(y1) || gIsNan(y2))){
            if (y2 > y1 && y2 > base) {
                Y.push_back(10 + y1);
                //            cout<<name1<<","<<name2<<" is"<<10 + y1<<" real "<<WM->sensedPos.y()<<endl;
            } else {
                Y.push_back(10 - y1);
                //            cout<<name1<<","<<name2<<" is"<<10 - y1<<" real "<<WM->sensedPos.y()<<endl;
            }
        }
    }

    if ((frprn && frprs) || (flpln && flpls)) {
        if (frprn && frprs) {
            name1 = "frprn";
            name2 = "frprs";
            len1 = distance_of_important_points["FRPRN"];
            len2 = distance_of_important_points["FRPRS"];

        } else {
            name1 = "flpln";
            name2 = "flpls";
            len1 = distance_of_important_points["FLPLN"];
            len2 = distance_of_important_points["FLPLS"];
        }

        base = 6;
        s = Area(len1, len2, base);
        h = 2 * s / base;
        y1 = sqrt(len1 * len1 - h * h);
        y2 = sqrt(len2 * len2 - h * h);

        if (!(gIsNan(y1) || gIsNan(y2))){
            if (y2 > y1 && y2 > base) {
                //            cout<<name1<<","<<name2<<" is"<<3 + y1<<" real "<<WM->sensedPos.y()<<endl;
                Y.push_back(3 + y1);
            } else {
                //            cout<<name1<<","<<name2<<" is"<<3 - y1<<" real "<<WM->sensedPos.y()<<endl;
                Y.push_back(3 - y1);
            }
        }
    }

    if ((prprn && prprs) || (plpln && plpls)) {
        if ((prprn && prprs)) {
            name1 = "prprs";
            name2 = "prprn";
            len1 = distance_of_important_points["PRPRN"];
            len2 = distance_of_important_points["PRPRS"];
        } else {
            name1 = "plpls";
            name2 = "plpln";
            len1 = distance_of_important_points["PLPLN"];
            len2 = distance_of_important_points["PLPLS"];
        }

        base = 6;
        s = Area(len1, len2, base);
        h = 2 * s / base;
        y1 = sqrt(len1 * len1 - h * h);
        y2 = sqrt(len2 * len2 - h * h);

        if (!(gIsNan(y1) || gIsNan(y2))){
            if (y2 > y1 && y2 > base) {
                Y.push_back(3 + y1);
                //            cout<<name1<<","<<name2<<" is"<<3 + y1<<" real "<<WM->sensedPos.y()<<endl;
            } else {
                Y.push_back(3 - y1);
                //            cout<<name1<<","<<name2<<" is"<<3 - y1<<" real "<<WM->sensedPos.y()<<endl;
            }
        }
    }
    //    }
    if (Y.size() > 0) {
        sort(Y.begin(), Y.end());
        Y_initial_estimation = Y.at(Y.size() / 2);

        //        cout<<"tokhmi-----------------------------------------------------------------------"<<endl;
        //        counter_rvdraw++;
        //        RVDraw::instance()->drawEigen::Vector3f(Eigen::Vector3f(WM->sensedPos.x(),Y_initial_estimation,WM->sensedPos.z()),BLACK, counter_rvdraw,3);
        //        counter_rvdraw++;
        //        RVDraw::instance()->drawLine(Eigen::Vector3f(X_initial_estimation,10,0),Eigen::Vector3f(X_initial_estimation,-10,0),ORANGE,counter_rvdraw);
        //        RVDraw::instance()->drawLine(Eigen::Vector3f(-15, Y_initial_estimation, 0), Eigen::Vector3f(15, Y_initial_estimation, 0), BLACK, 331);
    } /*else {
//        cout << "can not set password" << endl;
    }*/
}

///we have found one of the ends of a line, with this method we will find the other end in local position
///and we will add that end to the important points
Eigen::Vector3f Localize::find_other_end(line l, Eigen::Vector3f one_end, double length)
{
    Eigen::Vector3f the_other_end;
    Eigen::Vector3f begin = l.begin;
    Eigen::Vector3f end = l.end;

    if ((one_end - begin).norm() < (one_end - end).norm()) {
        the_other_end = one_end + ((end - begin) * length / ((end - begin).norm()));
    } else {
        the_other_end = one_end + ((begin - end) * length / (begin - end).norm());
    }

    return the_other_end;
}

void Localize::test()
{
    //    setLocalLines();
    //    setImportantPoints();
    //    setX();
    //    setY();
    //    set_second_x_estimation();
    //    add_position();
    //    add_median_of_last_positions();
    //    isBeamed();
    //    set_head_rotate(Eigen::Vector3f(WM->sensedPos.x(),WM->sensedPos.y(),WM->sensedPos.z()));

    //    RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(importan_points["MN"]), ORANGE , 113);

    for(unordered_map<string,Eigen::Vector3f>::iterator i=importan_points.begin(); i != importan_points.end() ; i++)
    {
        //            if(i->first == "FRFS")
        //            {
        //                counter_rvdraw++;
        //                if((i->second).norm() > 0.1)
        //                    RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),WHITE,counter_rvdraw);
        //            }
        //            if(i->first == "FRFN")
        //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),BLACK,31);
        //            if(i->first == "FRPRS")
        //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),BLUE,32);
        //            if(i->first == "FRPRN")
        //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),RED,33);
        //            if(i->first == "PRPRN")
        //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),YELLO,34);
        //            if(i->first == "PRPRS")
        //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),PINK,35);
        //            if(i->first == "MS")
        //            {
        //                counter_rvdraw++;
        //                if((i->second).norm() > 0.1)
        //                    RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),BLACK,counter_rvdraw);
        //            }
        //            if(i->first == "MN")
        //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),GREEN,37);

        //            if(i->first == "FLFS")
        //            {
        //                counter_rvdraw++;
        //                if((i->second).norm() > 0.1)
        //                    RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),WHITE,30);
        //            }
        //            if(i->first == "FLFN")
        //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),BLACK,31);
        //            if(i->first == "FLPLS")
        //            {
        //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),BLUE,counter_rvdraw++);
        //            }
        //            if(i->first == "FLPLN")
        //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),RED,33);
        //            if(i->first == "PLPLN")
        //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),YELLO,34);
        //            if(i->first == "PLPLS")
        //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),PINK,35);
        //            if(i->first == "MS")
        //            {
        //                counter_rvdraw++;
        //                if((i->second).norm() > 0.1)
        //                    RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),BLACK,counter_rvdraw);
        //            }
        //            if(i->first == "MN")
        //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(i->second),GREEN,37);

    }

    //    localize_with_lines();
    //            calcOrient();

    //    for(int counter=0;counter<WM->getLastSeenLines().size();counter++)
    //    {
    //        line l=WM->getLastSeenLines()[counter];
    //        Eigen::Vector3f beg=PolarToCartecian(l.begin);
    //        Eigen::Vector3f end=PolarToCartecian(l.end);
    //        line lcart=line(beg,end);
    //        string l_name=line_recognitation(lcart);

    //        if(l_name=="PR")
    //        {
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),GREEN,12);
    //        }
    //        if(l_name=="PRN")
    //        {
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),BLACK,13);
    //        }
    //        if(l_name=="PRS")
    //        {
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),YELLO,17);
    //        }
    //        if(l_name=="FR")
    //        {
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),RED,14);
    //        }
    //        if(l_name=="FS"){
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),BLUE,37);
    //        }
    //        if(l_name=="FN"){
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),GREEN,16);
    //        }
    //        if(l_name=="M"){
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),WHITE,18);
    //        }

    //        if(l_name=="PL")
    //        {
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),GREEN,12);
    //            //                    RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),GREEN,counter_rvdraw);

    //        }
    //        if(l_name=="PLN")
    //        {
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),BLACK,13);
    //            //                              RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),BLACK,counter_rvdraw);
    //        }
    //        if(l_name=="PLS")
    //        {
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),YELLO,17);
    //            //                            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),YELLO,counter_rvdraw);
    //        }
    //        if(l_name=="FL")
    //        {
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),RED,counter_rvdraw);
    //            //                            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),RED,18);
    //        }
    //        if(l_name=="FS"){
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),BLUE,37);
    //            //                            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),BLUE,counter_rvdraw);

    //        }
    //        if(l_name=="FN"){
    //            counter_rvdraw++;
    //            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),GREEN,16);
    //            //                            RVDraw::instance()->drawLine(WM->sensedPos+WM->headRotate*(lcart.begin),WM->sensedPos+WM->headRotate*(lcart.end),GREEN,counter_rvdraw);

    //        }
    //    }
    //    //        }

    //    resetEveryThing();
}

////this method will find the robot position with
///the help of the lines that it saw through the
///last cycle
///this function is using the method of  "http://web.archive.org/web/20110719232148/http://www.macalester.edu/~halverson/math36/GPS.pdf"
Eigen::Vector3f Localize::localize_with_lines()
{
}

///this method computes the area of a rectangle with heron method
double Localize::Area(double a, double b, double c)
{
    double p = (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

///Lorentz innert product
double Localize::lorentz(Eigen::MatrixXf A, Eigen::MatrixXf B)
{
    return (A(0, 0) * B(0, 0) + A(1, 0) * B(1, 0) + A(2, 0) * B(2, 0) - A(3, 0) * B(3, 0));
    //    return (A(0,0)*B(0,0) + A(1,0)*B(1,0) + A(2,0)*B(2,0));
}

///solving the 2degree equations
vector<double> Localize::quadratic_solver(double a, double b, double c)
{
    vector<double> result;

    if (b * b - 4 * a * c >= 0) {
        result.push_back((-b + sqrt(b * b - 4 * a * c)) / (2 * a));
        result.push_back((-b - sqrt(b * b - 4 * a * c)) / (2 * a));
        return result;
    }
}

///calculate the distance of a point and a line

double Localize::Distance_Polar_Point_Line(Eigen::Vector3f point_polar, line l)
{
    Eigen::Vector3f begin_polar = l.begin;
    Eigen::Vector3f end_polar = l.end;
    Eigen::Vector3f begin_cartesian = PolarToCartecian(begin_polar);
    Eigen::Vector3f end_cartesian = PolarToCartecian(end_polar);
    Eigen::Vector3f point_cartesian = PolarToCartecian(point_polar);

    Eigen::Vector3f begin_to_point = point_cartesian - begin_cartesian;
    Eigen::Vector3f begin_to_end = end_cartesian - begin_cartesian;

    double angle_rad = acos((begin_to_end.dot(begin_to_point)) / (begin_to_end.norm() * begin_to_point.norm()));
    return begin_to_point.norm() * sin(angle_rad);
}

double Localize::Distance_Cartesian_Point_Line(Eigen::Vector3f point_cartesian, line l)
{
    Eigen::Vector3f begin_cartesian = l.begin;
    Eigen::Vector3f end_cartesian = l.end;

    Eigen::Vector3f begin_to_point = point_cartesian - begin_cartesian;
    Eigen::Vector3f begin_to_end = end_cartesian - begin_cartesian;
    Eigen::Vector3f end_to_point = end_cartesian - point_cartesian;

    return (begin_to_point - (((begin_to_end.dot(begin_to_point)) / begin_to_end.norm()) * (begin_to_end / begin_to_end.norm()))).norm();

    //    if(begin_to_end.norm() < 0.1 || begin_to_end.norm() < 0.1 || end_to_point.norm() < 0.1)
    //        return 0;

    //    double angle_rad=acos((begin_to_end.dot(begin_to_point))/(begin_to_end.norm()*begin_to_point.norm()));
    //    return begin_to_point.norm()*sin(angle_rad);
}

///this function returns the distance of the point and the middle of a segment
///
double Localize::Distance_Cartesian_Point_Segment(Eigen::Vector3f point_cartesian, line l)
{
    return (point_cartesian - (l.begin + l.end) / 2).norm();
}

///this method will solve an overdetermiedn system of equation of the form A*x=b which A[m,n] and
///x[n,1] which we know that m>=n and
///returns the best answert with respect to min square
Eigen::MatrixXf Localize::overdetermined(Eigen::MatrixXf A, Eigen::MatrixXf b)
{

    return (A.transpose() * A).inverse() * A.transpose() * b;
}

///finds the intersection of two given lines,lines l1,l2 are supposed to be represented in cartesian mode
///
Eigen::Vector3f Localize::line_intersection(line l1, line l2)
{
    Eigen::Vector3f b1 = l1.begin;
    Eigen::Vector3f b2 = l2.begin;
    Eigen::Vector3f e1 = l1.end;
    Eigen::Vector3f e2 = l2.end;

    if ((b1 - b2).norm() < 0.1)
        return (b1 + b2) / 2;
    if ((b1 - e2).norm() < 0.1)
        return (b1 + e2) / 2;
    if ((e1 - e2).norm() < 0.1)
        return (e1 + e2) / 2;
    if ((e1 - b2).norm() < 0.1)
        return (e1 + b2) / 2;

    Eigen::Vector3f result = Eigen::Vector3f(0, 0, 0);
    Eigen::Vector3f normal_l1 = (l1.end - l1.begin) / ((l1.end - l1.begin).norm());
    Eigen::Vector3f normal_l2 = (l2.end - l2.begin) / ((l2.end - l2.begin).norm());

    if ((normal_l1 + normal_l2).norm() < 0.1 || (normal_l1 - normal_l2).norm() < 0.1) {
        return result;
    } else {
        Eigen::Vector3f b1 = l1.begin;
        Eigen::Vector3f e1 = l1.end;

        Eigen::Vector3f b2 = l2.begin;
        Eigen::Vector3f e2 = l2.end;

        Eigen::MatrixXf b(3,1);
        b(0, 0) = b1.x() - b2.x();
        b(1, 0) = b1.y() - b2.y();
        b(2, 0) = b1.z() - b2.z();

        Eigen::MatrixXf A(3, 2);
        A(0, 0) = e2.x() - b2.x();
        A(0, 1) = b1.x() - e1.x();

        A(1, 0) = e2.y() - b2.y();
        A(1, 1) = b1.y() - e1.y();

        A(2, 0) = e2.z() - b2.z();
        A(2, 1) = b1.z() - e1.z();

        double t2 = overdetermined(A, b)(0, 0);
        result.x() = b2.x() + (e2.x() - b2.x()) * t2;
        result.y() = b2.y() + (e2.y() - b2.y()) * t2;
        result.z() = b2.z() + (e2.z() - b2.z()) * t2;

        return result;
    }
}

///this method will find the distance of our head to the intersection
///of two lines wich are given in polar coordination

double Localize::find_distance_to_intesection(line l1, line l2)
{
    double d1 = l1.begin.x();
    double d2 = l1.end.x();
    double d3 = l2.begin.x();
    double d4 = l2.end.x();

    if (fabs(d1 - d3) < 0.5)
        return (d1 + d3) / 2;
    if (fabs(d1 - d4) < 0.5)
        return (d1 + d4) / 2;
    if (fabs(d2 - d3) < 0.5)
        return (d2 + d3) / 2;
    if (fabs(d2 - d4) < 0.5)
        return (d2 + d4) / 2;
}

unordered_map<string, Eigen::Vector3f> WorldModel::getLastFlagSeen()
{
    unordered_map<string, Eigen::Vector3f> result;
    for (unordered_map<string, Eigen::Vector3f>::iterator i = flag.begin(); i != flag.end(); i++) {
        if (flagLastSeen[i->first] == serverTime)
            result.insert(*i);
    }

    return result;
}
string Localize::line_recognitation(line line_to_detect)
{
    unordered_map<string, Eigen::Vector3f> flagLastSeen = WM->getLastFlagSeen();
    string flagName;
    Eigen::Vector3f flagPos;
    double distance_point_line;
    double distance_point_segment;
    Eigen::Vector3f the_other_end = line_to_detect.begin;

    for (unordered_map<string, Eigen::Vector3f>::iterator f_iterator = flagLastSeen.begin(); f_iterator != flagLastSeen.end(); f_iterator++) {

        flagName = f_iterator->first;
        flagPos = f_iterator->second;
        distance_point_line = Distance_Cartesian_Point_Line(flagPos, line_to_detect);
        distance_point_segment = Distance_Cartesian_Point_Segment(flagPos, line_to_detect);

        if(fabs(line_to_detect.getLength() - 1.25) < 0.1)
            return "the others";

        if (fabs(distance_point_line - 0.8) < 0.1 && fabs(distance_point_segment - 1.32004) < 5 && (flagName == "G1R" || flagName == "G2R")) {
            return "FR";
        }
        if (fabs(distance_point_line - 0.8) < 0.1 && fabs(distance_point_segment - 1.32004) < 5 && (flagName == "G1L" || flagName == "G2L")) {
            return "FL";
        }

        if (fabs(distance_point_line - 4.12826) < 0.1 && fabs(distance_point_segment - 4.22522) < 1.5) {
            if (flagName == "G1R") {
                return "PRS";
            }
            if (flagName == "G2R") {
                return "PRN";
            }
            if (flagName == "G1L") {
                return "PLS";
            }
            if (flagName == "G2L") {
                return "PLN";
            }
        }

        //        if(fabs(distance_point_line - 0) < 0.1  && fabs(distance_point_segment ) < 10 )
        if (fabs(distance_point_line - 0) < 0.1) {
            if ((the_other_end - flagPos).norm() < 1) {
                the_other_end = line_to_detect.end;
            }

            if (flagName == "F1R") {
                if (CartecianToPolar(the_other_end).theta > CartecianToPolar(flagPos).theta) {
                    return "FN";
                } else {
                    return "FR";
                }
            }

            if (flagName == "F2R") {
                if (CartecianToPolar(the_other_end).theta > CartecianToPolar(flagPos).theta) {

                    return "FR";
                } else {
                    return "FS";
                }
            }

            if (flagName == "F1L") {
                if (CartecianToPolar(the_other_end).theta > CartecianToPolar(flagPos).theta) {

                    return "FL";
                } else {
                    return "FN";
                }
            }

            if (flagName == "F2L") {

                if (CartecianToPolar(the_other_end).theta > CartecianToPolar(flagPos).theta) {
                    return "FS";
                } else {
                    return "FL";
                }
            }
        }


        if (fabs(distance_point_line - 13) < 0.3 && fabs(distance_point_segment - 13.0311) < 2) {
            if (flagName == "F1R")
                return "PRS";
            else {

                if (flagName == "F2R")
                    return "PRN";
                else {
                    if (flagName == "F1L") {
                        return "PLS";
                    } else {
                        if (flagName == "F2L") {
                            return "PLN";
                        }
                    }
                }
            }
        }

        if (fabs(distance_point_line - 7) < 0.2 && fabs(distance_point_segment - 7.05762) < 2) {
            if (flagName == "F1R")
                return "PRN";
            else {
                if (flagName == "F2R")
                    return "PRS";
                else {
                    if (flagName == "F1L") {
                        return "PLN";
                    } else {
                        if (flagName == "F2L") {
                            return "PLS";
                        }
                    }
                }
            }
        }




        if (fabs(distance_point_line - 1.8) < 0.1 && (flagName == "F1R" || flagName == "F2R") && fabs(distance_point_segment - 10.1607) < 3)
            return "PR";
        if (fabs(distance_point_line - 1.8) < 0.1 && (flagName == "F1L" || flagName == "F2L") && fabs(distance_point_segment - 10.1607) < 3)
            return "PL";
    }

    for (unordered_map<string, Eigen::Vector3f>::iterator f_iterator = flagLastSeen.begin(); f_iterator != flagLastSeen.end(); f_iterator++) {

        flagName = f_iterator->first;
        flagPos = f_iterator->second;
        distance_point_line = Distance_Cartesian_Point_Line(flagPos, line_to_detect);
        distance_point_segment = Distance_Cartesian_Point_Segment(flagPos, line_to_detect);

        if (fabs(distance_point_line - 2.10772) > fabs(distance_point_line - 1.96977) && (flagName == "G1R" || flagName == "G2R") && fabs(distance_point_line - 1.96977) < 0.1 && fabs(distance_point_segment - 2.23215) < 1)
            return "PR";
        else {
            if (fabs(distance_point_line - 2.10772) < fabs(distance_point_line - 1.96977) && fabs(distance_point_line - 2.10772) < 0.1 && fabs(distance_point_segment - 2.29183) < 1)
                if (flagName == "G1R")
                    return "PRN";
                else {
                    if (flagName == "G2R")
                        return "PRS";
                }
        }

        if (fabs(distance_point_line - 2.10772) > fabs(distance_point_line - 1.96977) && (flagName == "G1L" || flagName == "G2L") && fabs(distance_point_line - 1.96977) < 0.1 && fabs(distance_point_segment - 2.23215) < 1)
            return "PL";
        else {
            if (fabs(distance_point_line - 2.10772) < fabs(distance_point_line - 1.96977) && fabs(distance_point_line - 2.10772) < 0.1 && fabs(distance_point_segment - 2.29183) < 1)
                if (flagName == "G1L")
                    return "PLN";
                else {
                    if (flagName == "G2L")
                        return "PLS";
                }
        }

        //        if((fabs(distance_point_line - 15)< 0.3 && (fabs(distance_point_segment -18.0278) < 1) && (flagName == "F1R" || flagName == "F2R" || flagName == "F1L" || flagName == "F2L"))
        //                || (fabs(distance_point_line - 15.0213) < 0.3 && fabs(distance_point_segment - 15.058) < 0.1 && (flagName == "G1R" || flagName == "G2R" || flagName == "G1L" || flagName == "G2L"))){
        if ((/*line_to_detect.getLength() <0.9 || */ line_to_detect.getLength() > 1.4) && ((fabs(distance_point_line - 15) < 0.3 && (flagName == "F1R" || flagName == "F2R" || flagName == "F1L" || flagName == "F2L"))
                                                                                           || (fabs(distance_point_line - 15.0213) < 0.3 && (flagName == "G1R" || flagName == "G2R" || flagName == "G1L" || flagName == "G2L")))) {
            //            cout<<line_to_detect.getLength()<<"length"<<endl;
            return "M";
        }
    }
    return "the others";

}

//    for (unordered_map<string, Eigen::Vector3f>::iterator f_iterator = flagLastSeen.begin(); f_iterator != flagLastSeen.end(); f_iterator++) {
//        flagName = f_iterator->first;
//        flagPos = f_iterator->second;
//        distance_point_line = Distance_Cartesian_Point_Line(flagPos, line_to_detect);
//        distance_point_segment = Distance_Cartesian_Point_Segment(flagPos, line_to_detect);

//        if (fabs(distance_point_line - 13) < 0.3 && fabs(distance_point_segment - 13.0311) < 2) {
//            if (flagName == "F1R")
//                return "PRS";
//            else {

//                if (flagName == "F2R")
//                    return "PRN";
//                else {
//                    if (flagName == "F1L") {
//                        return "PLS";
//                    } else {
//                        if (flagName == "F2L") {
//                            return "PLN";
//                        }
//                    }
//                }
//            }
//        }

//        if (fabs(distance_point_line - 7) < 0.2 && fabs(distance_point_segment - 7.05762) < 2) {
//            if (flagName == "F1R")
//                return "PRN";
//            else {
//                if (flagName == "F2R")
//                    return "PRS";
//                else {
//                    if (flagName == "F1L") {
//                        return "PLN";
//                    } else {
//                        if (flagName == "F2L") {
//                            return "PLS";
//                        }
//                    }
//                }
//            }
//        }
//    }

//    for (unordered_map<string, Eigen::Vector3f>::iterator f_iterator = flagLastSeen.begin(); f_iterator != flagLastSeen.end(); f_iterator++) {
//        flagName = f_iterator->first;
//        flagPos = f_iterator->second;
//        distance_point_line = Distance_Cartesian_Point_Line(flagPos, line_to_detect);
//        distance_point_segment = Distance_Cartesian_Point_Segment(flagPos, line_to_detect);

//        if (fabs(distance_point_line - 1.8) < 0.1 && (flagName == "F1R" || flagName == "F2R") && fabs(distance_point_segment - 10.1607) < 3)
//            return "PR";
//        if (fabs(distance_point_line - 1.8) < 0.1 && (flagName == "F1L" || flagName == "F2L") && fabs(distance_point_segment - 10.1607) < 3)
//            return "PL";
//    }

//    for (unordered_map<string, Eigen::Vector3f>::iterator f_iterator = flagLastSeen.begin(); f_iterator != flagLastSeen.end(); f_iterator++) {

//        flagName = f_iterator->first;
//        flagPos = f_iterator->second;
//        distance_point_line = Distance_Cartesian_Point_Line(flagPos, line_to_detect);
//        distance_point_segment = Distance_Cartesian_Point_Segment(flagPos, line_to_detect);

//        if (fabs(distance_point_line - 2.10772) > fabs(distance_point_line - 1.96977) && (flagName == "G1R" || flagName == "G2R") && fabs(distance_point_line - 1.96977) < 0.1 && fabs(distance_point_segment - 2.23215) < 1)
//            return "PR";
//        else {
//            if (fabs(distance_point_line - 2.10772) < fabs(distance_point_line - 1.96977) && fabs(distance_point_line - 2.10772) < 0.1 && fabs(distance_point_segment - 2.29183) < 1)
//                if (flagName == "G1R")
//                    return "PRN";
//                else {
//                    if (flagName == "G2R")
//                        return "PRS";
//                }
//        }

//        if (fabs(distance_point_line - 2.10772) > fabs(distance_point_line - 1.96977) && (flagName == "G1L" || flagName == "G2L") && fabs(distance_point_line - 1.96977) < 0.1 && fabs(distance_point_segment - 2.23215) < 1)
//            return "PL";
//        else {
//            if (fabs(distance_point_line - 2.10772) < fabs(distance_point_line - 1.96977) && fabs(distance_point_line - 2.10772) < 0.1 && fabs(distance_point_segment - 2.29183) < 1)
//                if (flagName == "G1L")
//                    return "PLN";
//                else {
//                    if (flagName == "G2L")
//                        return "PLS";
//                }
//        }

//        //        if((fabs(distance_point_line - 15)< 0.3 && (fabs(distance_point_segment -18.0278) < 1) && (flagName == "F1R" || flagName == "F2R" || flagName == "F1L" || flagName == "F2L"))
//        //                || (fabs(distance_point_line - 15.0213) < 0.3 && fabs(distance_point_segment - 15.058) < 0.1 && (flagName == "G1R" || flagName == "G2R" || flagName == "G1L" || flagName == "G2L"))){
//        if ((/*line_to_detect.getLength() <0.9 || */ line_to_detect.getLength() > 1.4) && ((fabs(distance_point_line - 15) < 0.3 && (flagName == "F1R" || flagName == "F2R" || flagName == "F1L" || flagName == "F2L"))
//                                                                                           || (fabs(distance_point_line - 15.0213) < 0.3 && (flagName == "G1R" || flagName == "G2R" || flagName == "G1L" || flagName == "G2L")))) {
//            //            cout<<line_to_detect.getLength()<<"length"<<endl;
//            return "M";
//        }
//    }
//    return "the others";

//}

void Localize::add_position()
{
    if (last_positions.size() == 5) {
        last_positions.erase(last_positions.begin());
        last_positions.push_back(Eigen::Vector3f(X_second_estimation, Y_initial_estimation, 0.516));

    } else {
        last_positions.push_back(Eigen::Vector3f(X_second_estimation, Y_initial_estimation, 0.516));
    }
}

void Localize::add_median_of_last_positions()
{
    vector<double> x, y;

    if (last_positions.size() == 5) {
        for (int i = 0; i < last_positions.size(); i++) {
            //            lengthes.push_back((last_positions.at(i)).norm());
            x.push_back((last_positions.at(i)).x());
            y.push_back((last_positions.at(i)).y());
        }

        sort(x.begin(), x.end());
        sort(y.begin(), y.end());

        if (median_of_last_positions_x.size() == 5) {
            median_of_last_positions_x.erase(median_of_last_positions_x.begin());
            median_of_last_positions_x.push_back(x.at(x.size() / 2));

            median_of_last_positions_y.erase(median_of_last_positions_y.begin());
            median_of_last_positions_y.push_back(y.at(y.size() / 2));
        } else {
            median_of_last_positions_x.push_back(x.at(x.size() / 2));

            median_of_last_positions_y.push_back(y.at(y.size() / 2));
        }
    }
}

bool Localize::isBeamed()
{
    if (median_of_last_positions_x.size() == 5) {
        if (fabs(median_of_last_positions_x.at(4) - median_of_last_positions_x.at(0)) > 1) {
            return true;
            cout << "beamed x: " << WM->serverTime << endl;
        }
    }

    if (median_of_last_positions_y.size() == 5) {
        if (fabs(median_of_last_positions_y.at(4) - median_of_last_positions_y.at(0)) > 1) {
            return true;
            cout << "beamed y: " << WM->serverTime << endl;
        }
    }

    return false;
}

void Localize::set_second_x_estimation()
{
    double diff;
    double length;
    Eigen::Vector3f fl;
    double sum = 0;
    int counter = 0;
    double x;

    for (unordered_map<string, Eigen::Vector3f>::iterator f = importan_points.begin(); f != importan_points.end(); f++) {
        fl = important_points_global[f->first];
        length = (f->second).norm();

        if (length != 0 && !(f->first == "MS" || f->first == "MN") /*checks that we realy see a good flag*/) {
            diff = sqrt(length * length - (fl.z() - 0.516) * (fl.z() - 0.516) - (fl.y() - Y_initial_estimation) * (fl.y() - Y_initial_estimation));
            if (fabs((fl.x() - diff) - X_initial_estimation) < fabs(fl.x() + diff - X_initial_estimation) && (fl.x() - diff < 16 && fl.x() - diff > -16)) {
                x = fl.x() - diff;
                sum += fl.x() - diff;
                counter++;
                if (x - WM->sensedPos.x() > 0.5) {
                    //cout<<"name "<<f->first<<" and the global position "<<fl<<endl;
                    //                    cout<<"local "<<f->second<<endl;
                    //                    cout<<"in dare be fuck mide"<<X_initial_estimation<<endl;
                    //                    cout<<"fl.x "<<fl.x()<<endl;
                    //                    cout<<WM->sensedPos+WM->headRotate*(f->second)<<"ine "<<endl;
                    //                    cout<<"x is "<<x<<"the real one is: "<<WM->sensedPos.x()<<endl;
                }

            } else {
                if ((fl.x() + diff < 16 && fl.x() + diff > -16)) {
                    counter_rvdraw++;
                    //                RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos+WM->headRotate*(f->second),BLACK,counter_rvdraw);
                    x = fl.x() + diff;
                    sum += fl.x() + diff;
                    counter++;
                    if (x - WM->sensedPos.x() > 0.5) {
                        //                        cout<<"name "<<f->first<<" and the global position "<<fl<<endl;
                        //                        cout<<"local "<<f->second<<endl;
                        //                        cout<<"in dare be fuck mide"<<X_initial_estimation<<endl;
                        //                        cout<<"fl.x "<<fl.x()<<endl;
                        //                        cout<<WM->sensedPos+WM->headRotate*(f->second)<<"ine "<<endl;
                        //                        cout<<"x is "<<x<<"the real one is: "<<WM->sensedPos.x()<<endl;
                    }
                }
            }
        }
    }

    if (counter != 0)
        X_second_estimation = sum / counter;

    counter_rvdraw++;
    //    RVDraw::instance()->drawEigen::Vector3f(Eigen::Vector3f(X_second_estimation,Y_initial_estimation,0.516), GREEN, counter_rvdraw,3);
    counter_rvdraw++;
    //    RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos, BLACK, counter_rvdraw,3);
}

void Localize::set_head_rotate(Eigen::Vector3f position)
{
    if ( importan_points.size() > 2 )
    {
        Eigen::MatrixXf I_P_local(3, importan_points.size());
        Eigen::MatrixXf I_P_glabal(3, importan_points.size());
        Eigen::Vector3f temp_flag;
        string temp_name;
        int counter = 0;

        //    Eigen::Matrix3f hr(3,3);
        //    hr(0,0) = WM->headRotate(0,0);
        //    hr(1,0) = WM->headRotate(1,0);
        //    hr(2,0) = WM->headRotate(2,0);

        //    hr(0,1) = WM->headRotate(0,1);
        //    hr(1,1) = WM->headRotate(1,1);
        //    hr(2,1) = WM->headRotate(2,1);

        //    hr(0,2) = WM->headRotate(0,2);
        //    hr(1,2) = WM->headRotate(1,2);
        //    hr(2,2) = WM->headRotate(2,2);

        for (unordered_map<string, Eigen::Vector3f>::iterator f = importan_points.begin(); f != importan_points.end(); f++) {
            temp_name = f->first;
            temp_flag = f->second;
            //        cout<<"flage :"<<temp_name<<endl;

            I_P_local(0, counter) = temp_flag.x();
            I_P_local(1, counter) = temp_flag.y();
            I_P_local(2, counter) = temp_flag.z();

            I_P_glabal(0, counter) = important_points_global[temp_name].x() - position.x();
            I_P_glabal(1, counter) = important_points_global[temp_name].y() - position.y();
            I_P_glabal(2, counter) = important_points_global[temp_name].z() - position.z();

            counter++;
        }
        //    cout<<"I_P_global: "<<endl;
        //    cout<<I_P_glabal<<endl;
        //    cout<<"I_P_local"<<endl;
        //    cout<<I_P_local<<endl;

        //    cout<<"calculated: "<<endl;
        //    cout<<I_P_glabal*(I_P_local.transpose())*((I_P_local*I_P_local.transpose()).inverse())<<endl;
        //    cout<<"the real one: "<<endl;
        //    cout<<WM->headRotate(0,0)<<WM->headRotate(0,1)<<WM->headRotate(0,2)<<endl;
        //    cout<<WM->headRotate(1,0)<<WM->headRotate(1,1)<<WM->headRotate(1,2)<<endl;
        //    cout<<WM->headRotate(2,0)<<WM->headRotate(2,1)<<WM->headRotate(2,2)<<endl;
        Eigen::Matrix3f head_orient = I_P_glabal * (I_P_local.transpose()) * ((I_P_local * I_P_local.transpose()).inverse());
        Eigen::Vector3f l1 = Eigen::Vector3f(head_orient(0, 0), head_orient(1, 0), head_orient(2, 0)).normalized();
        Eigen::Vector3f l2 = Eigen::Vector3f(head_orient(0, 1), head_orient(1, 1), head_orient(2, 1)).normalized();
        Eigen::Vector3f l3 = Eigen::Vector3f(head_orient(0, 2), head_orient(1, 2), head_orient(2, 2)).normalized();
        if ( WM->getTeamSide() == Right )
        {
            l1.x()*=-1;
            l1.y()*=-1;

            l2.x()*=-1;
            l2.y()*=-1;

            l3.x()*=-1;
            l3.y()*=-1;
        }
        Eigen::Matrix3f myOr ;
        myOr << l1.x(), l2.x(), l3.x(),
                l1.y(), l2.y(), l3.y(),
                l1.z(), l2.z(), l3.z();
        WM->headRotate = myOr;
        //    cout << head_orient << endl;
        Eigen::Vector3f brX = myOr*(Eigen::Vector3f(1, 0, 0));
        Eigen::Vector3f brY = myOr*(Eigen::Vector3f(0, 1, 0));
        Eigen::Vector3f brZ = myOr*(Eigen::Vector3f(0, 0, 1));

        Eigen::Vector3f x_before_he2_rotation = WM->general_rotation(brX, brY, Deg2Rad(WM->getJointAngle("he2")));
        Eigen::Vector3f y_before_he2_rotation = WM->general_rotation(brY, brY, Deg2Rad(WM->getJointAngle("he2")));
        Eigen::Vector3f z_before_he2_rotation = WM->general_rotation(brZ, brY, Deg2Rad(WM->getJointAngle("he2")));

        Eigen::Vector3f mhrX = WM->general_rotation(x_before_he2_rotation, z_before_he2_rotation, -Deg2Rad(WM->getJointAngle("he1")));
        Eigen::Vector3f mhrY = WM->general_rotation(y_before_he2_rotation, z_before_he2_rotation, -Deg2Rad(WM->getJointAngle("he1")));
        Eigen::Vector3f mhrZ = WM->general_rotation(z_before_he2_rotation, z_before_he2_rotation, -Deg2Rad(WM->getJointAngle("he1")));

        //besme rabe shohada va sedighein ...ya allah :)
        WM->bodyRotate << mhrX.x(), mhrY.x(), mhrZ.x(),
                mhrX.y(), mhrY.y(), mhrZ.y(),
                mhrX.z(), mhrY.z(), mhrZ.z()
                ;

        //        cout << mhrX << endl;
        WM->setMyAngle(atan2Deg(mhrX.y(), mhrX.x()));

        //    return I_P_glabal*(I_P_local.transpose())*((I_P_local*I_P_local.transpose()).inverse());

        //    cout<<hr - I_P_glabal*(I_P_local.transpose())*((I_P_local*I_P_local.transpose()).inverse())<<endl;
    }
}

Eigen::Vector3f Localize::calcPlaneNormal(vector<Eigen::Vector3f> planePoints)
{
    if (planePoints.size() < 3) {
        return Eigen::Vector3f();
    }
    double x = 0, y = 0, z = 0;
    for (int i = 0; i < planePoints.size(); i++) {
        x += planePoints.at(i).x();
        y += planePoints.at(i).y();
        z += planePoints.at(i).z();
    }

    Eigen::Vector3f cntPoint(x / planePoints.size(), y / planePoints.size(), z / planePoints.size());
    Eigen::Matrix3f covm(planePoints.size(), 3);
    for (int i = 0; i < planePoints.size(); i++) {
        covm(i, 0) = planePoints.at(i).x() - cntPoint.x();
        covm(i, 1) = planePoints.at(i).y() - cntPoint.y();
        covm(i, 2) = planePoints.at(i).z() - cntPoint.z();
    }
    Eigen::JacobiSVD<Eigen::Matrix3f> svd(covm);
    Eigen::Matrix3f V = svd.matrixV();
    Eigen::Vector3f res(V(0, 2), V(1, 2), V(2, 2));

    cntPoint.normalize();

    if (cntPoint.dot(res) > 0) {
        res *= -1;
    }

    return res;
}

//axis1 ----> Z
//axis2 ----> X
//axis3 ----> Y
Eigen::Matrix3f Localize::calcOrient()
{
    static int il = 1, jl = -1, cycleCntr = 0;
    static int missCycle = 0;
    static double mind = 1;
    static bool recalib = false;
    int c = 0;
    Eigen::Matrix3f orient;
    orient = Eigen::Matrix3f::Zero();
    vector<Eigen::Vector3f> planePoints;
    Eigen::Vector3f axis2, axis3;
    for (unordered_map<string, Eigen::Vector3f>::iterator i = WM->flag.begin(); i != WM->flag.end(); i++) {
        if (WM->flagLastSeen[i->first] != WM->serverTime) {
            continue;
        }
        if (WM->flagGlobal[i->first].z() < 0.1) {
            planePoints.push_back(i->second);
        }
    }

    vector<line> lastseen = WM->getLastSeenLines();
    for (int counter = 0; counter < lastseen.size(); counter++) {

        line l = lastseen[counter];
        if (l.time_we_saw_it != WM->serverTime) {
            continue;
        }
        Eigen::Vector3f Cr1 = PolarToCartecian(l.begin);
        Eigen::Vector3f Cr2 = PolarToCartecian(l.end);
        planePoints.push_back(Cr1);
        planePoints.push_back(Cr2);
    }
    Eigen::Vector3f axis1;
    if (planePoints.size() >= 3) {
        axis1 = calcPlaneNormal(planePoints);
    } else {
        return Eigen::Matrix3f::Zero();
    }

    for (int counter = 0; counter < lastseen.size(); counter++) {
        line l = lastseen[counter];
        Eigen::Vector3f Cr1 = PolarToCartecian(l.begin);
        Eigen::Vector3f Cr2 = PolarToCartecian(l.end);
        line newL = line(Cr1, Cr2);
        string l_name = line_recognitation(newL);
        Polar p1 = CartecianToPolar(Cr1);
        Polar p2 = CartecianToPolar(Cr2);
        if (1) {

            //khate vasate zamin :D

            /*         if(WM->getMyPos().x() < -0.5) {
                if(l_name == "M"){
                    //cout<< "khodafez" << endl;
                    if(p1.theta>p2.theta)
                        axis3 = Cr1-Cr2;
                    else
                        axis3 = Cr2-Cr1;
                    axis3.normalize();
                    axis2 = axis3.cross(axis1) ;
                    Eigen::Matrix3f globalOrient;
                    globalOrient(0,0)=axis2.x();
                    globalOrient(1,0)=axis2.y();
                    globalOrient(2,0)=axis2.z();
                    globalOrient(0,1)=axis3.x();
                    globalOrient(1,1)=axis3.y();
                    globalOrient(2,1)=axis3.z();
                    globalOrient(0,2)=axis1.x();
                    globalOrient(1,2)=axis1.y();
                    globalOrient(2,2)=axis1.z();
                    orient += (globalOrient.transpose());
                    c++;

                }
            }

            if(WM->getMyPos().x() > 0.5) {
                if(l_name == "M"){
                    //cout<< "salam" << endl;
                    if(p1.theta>p2.theta)
                        axis3 = Cr2-Cr1;
                    else
                        axis3 = Cr1-Cr2;
                    axis3.normalize();
                    axis2 = axis3.cross(axis1) ;
                    Eigen::Matrix3f globalOrient;
                    globalOrient(0,0)=axis2.x();
                    globalOrient(1,0)=axis2.y();
                    globalOrient(2,0)=axis2.z();
                    globalOrient(0,1)=axis3.x();
                    globalOrient(1,1)=axis3.y();
                    globalOrient(2,1)=axis3.z();
                    globalOrient(0,2)=axis1.x();
                    globalOrient(1,2)=axis1.y();
                    globalOrient(2,2)=axis1.z();
                    orient += (globalOrient.transpose());
                    c++;

                }
            }
            */
            //khotoote ofoghi penalty rast :D
            if (Y_initial_estimation < -3.2) {
                if (l_name == "PRN" || l_name == "PRS") {
                    if (p1.theta < p2.theta)
                        axis2 = Cr1 - Cr2;
                    else
                        axis2 = Cr2 - Cr1;
                    axis2.normalize();
                    axis3 = axis1.cross(axis2);
                    Eigen::Matrix3f globalOrient;
                    globalOrient(0, 0) = axis2.x();
                    globalOrient(1, 0) = axis2.y();
                    globalOrient(2, 0) = axis2.z();
                    globalOrient(0, 1) = axis3.x();
                    globalOrient(1, 1) = axis3.y();
                    globalOrient(2, 1) = axis3.z();
                    globalOrient(0, 2) = axis1.x();
                    globalOrient(1, 2) = axis1.y();
                    globalOrient(2, 2) = axis1.z();
                    orient += (globalOrient.transpose());
                    c++;
                }
            }

            if (Y_initial_estimation > 3.2) {
                if (l_name == "PRN" || l_name == "PRS") {
                    if (p1.theta > p2.theta)
                        axis2 = Cr1 - Cr2;
                    else
                        axis2 = Cr2 - Cr1;
                    axis2.normalize();
                    axis3 = axis1.cross(axis2);
                    Eigen::Matrix3f globalOrient;
                    globalOrient(0, 0) = axis2.x();
                    globalOrient(1, 0) = axis2.y();
                    globalOrient(2, 0) = axis2.z();
                    globalOrient(0, 1) = axis3.x();
                    globalOrient(1, 1) = axis3.y();
                    globalOrient(2, 1) = axis3.z();
                    globalOrient(0, 2) = axis1.x();
                    globalOrient(1, 2) = axis1.y();
                    globalOrient(2, 2) = axis1.z();
                    orient += (globalOrient.transpose());
                    c++;
                }
            }

            if (Y_initial_estimation > -2.8 && Y_initial_estimation < 2.8) {
                if (l_name == "PRN") {
                    if (p1.theta < p2.theta)
                        axis2 = Cr1 - Cr2;
                    else
                        axis2 = Cr2 - Cr1;
                    axis2.normalize();
                    axis3 = axis1.cross(axis2);
                    Eigen::Matrix3f globalOrient;
                    globalOrient(0, 0) = axis2.x();
                    globalOrient(1, 0) = axis2.y();
                    globalOrient(2, 0) = axis2.z();
                    globalOrient(0, 1) = axis3.x();
                    globalOrient(1, 1) = axis3.y();
                    globalOrient(2, 1) = axis3.z();
                    globalOrient(0, 2) = axis1.x();
                    globalOrient(1, 2) = axis1.y();
                    globalOrient(2, 2) = axis1.z();
                    orient += (globalOrient.transpose());
                    c++;

                } else if (l_name == "PRS") {
                    if (p1.theta > p2.theta)
                        axis2 = Cr1 - Cr2;
                    else
                        axis2 = Cr2 - Cr1;
                    axis2.normalize();
                    axis3 = axis1.cross(axis2);
                    Eigen::Matrix3f globalOrient;
                    globalOrient(0, 0) = axis2.x();
                    globalOrient(1, 0) = axis2.y();
                    globalOrient(2, 0) = axis2.z();
                    globalOrient(0, 1) = axis3.x();
                    globalOrient(1, 1) = axis3.y();
                    globalOrient(2, 1) = axis3.z();
                    globalOrient(0, 2) = axis1.x();
                    globalOrient(1, 2) = axis1.y();
                    globalOrient(2, 2) = axis1.z();
                    orient += (globalOrient.transpose());
                    c++;
                }
            }

            //khotoote ofoghi penalty chap :D
            /*          if(Y_initial_estimation < -4) {
                if(l_name == "PLN" || l_name == "PLS"){
                    if(p1.theta<p2.theta)
                        axis2 = Cr1-Cr2;
                    else
                        axis2 = Cr2-Cr1;
                    axis2.normalize();
                    axis3 = axis1.cross(axis2) ;
                    Eigen::Matrix3f globalOrient;
                    globalOrient(0,0)=axis2.x();
                    globalOrient(1,0)=axis2.y();
                    globalOrient(2,0)=axis2.z();
                    globalOrient(0,1)=axis3.x();
                    globalOrient(1,1)=axis3.y();
                    globalOrient(2,1)=axis3.z();
                    globalOrient(0,2)=axis1.x();
                    globalOrient(1,2)=axis1.y();
                    globalOrient(2,2)=axis1.z();
                    orient += (globalOrient.transpose());
                    c++;

                }
            }

            if(Y_initial_estimation > 4) {
                if(l_name == "PLN" || l_name == "PLS"){
                    if(p1.theta>p2.theta)
                        axis2 = Cr1-Cr2;
                    else
                        axis2 = Cr2-Cr1;
                    axis2.normalize();
                    axis3 = axis1.cross(axis2) ;
                    Eigen::Matrix3f globalOrient;
                    globalOrient(0,0)=axis2.x();
                    globalOrient(1,0)=axis2.y();
                    globalOrient(2,0)=axis2.z();
                    globalOrient(0,1)=axis3.x();
                    globalOrient(1,1)=axis3.y();
                    globalOrient(2,1)=axis3.z();
                    globalOrient(0,2)=axis1.x();
                    globalOrient(1,2)=axis1.y();
                    globalOrient(2,2)=axis1.z();
                    orient += (globalOrient.transpose());
                    c++;

                }
            }

            if(Y_initial_estimation > -2 || Y_initial_estimation < 2 ) {
                if(l_name == "PLN"){
                    if(p1.theta<p2.theta)
                        axis2 = Cr1-Cr2;
                    else
                        axis2 = Cr2-Cr1;
                    axis2.normalize();
                    axis3 = axis1.cross(axis2) ;
                    Eigen::Matrix3f globalOrient;
                    globalOrient(0,0)=axis2.x();
                    globalOrient(1,0)=axis2.y();
                    globalOrient(2,0)=axis2.z();
                    globalOrient(0,1)=axis3.x();
                    globalOrient(1,1)=axis3.y();
                    globalOrient(2,1)=axis3.z();
                    globalOrient(0,2)=axis1.x();
                    globalOrient(1,2)=axis1.y();
                    globalOrient(2,2)=axis1.z();
                    orient += (globalOrient.transpose());
                    c++;

                }
                else if(l_name == "PLS"){
                    if(p1.theta>p2.theta)
                        axis2 = Cr1-Cr2;
                    else
                        axis2 = Cr2-Cr1;
                    axis2.normalize();
                    axis3 = axis1.cross(axis2) ;
                    Eigen::Matrix3f globalOrient;
                    globalOrient(0,0)=axis2.x();
                    globalOrient(1,0)=axis2.y();
                    globalOrient(2,0)=axis2.z();
                    globalOrient(0,1)=axis3.x();
                    globalOrient(1,1)=axis3.y();
                    globalOrient(2,1)=axis3.z();
                    globalOrient(0,2)=axis1.x();
                    globalOrient(1,2)=axis1.y();
                    globalOrient(2,2)=axis1.z();
                    orient += (globalOrient.transpose());
                    c++;

                }

            }

            //khate amooD e penalty :D

            if(WM->getMyPos().x() < 12) {
                if(l_name == "PR"){
                    //cout<< "Ya Ali" << endl;
                    if(p1.theta>p2.theta)
                        axis3 = Cr1-Cr2;
                    else
                        axis3 = Cr2-Cr1;
                    axis3.normalize();
                    axis2 = axis3.cross(axis1) ;
                    Eigen::Matrix3f globalOrient;
                    globalOrient(0,0)=axis2.x();
                    globalOrient(1,0)=axis2.y();
                    globalOrient(2,0)=axis2.z();
                    globalOrient(0,1)=axis3.x();
                    globalOrient(1,1)=axis3.y();
                    globalOrient(2,1)=axis3.z();
                    globalOrient(0,2)=axis1.x();
                    globalOrient(1,2)=axis1.y();
                    globalOrient(2,2)=axis1.z();
                    orient += (globalOrient.transpose());
                    c++;

                }
            }

            if(WM->getMyPos().x() > -12) {
                if(l_name == "PL"){
                    if(p1.theta>p2.theta)
                        axis3 = Cr1-Cr2;
                    else
                        axis3 = Cr2-Cr1;
                    axis3.normalize();
                    axis2 = axis3.cross(axis1) ;
                    Eigen::Matrix3f globalOrient;
                    globalOrient(0,0)=axis2.x();
                    globalOrient(1,0)=axis2.y();
                    globalOrient(2,0)=axis2.z();
                    globalOrient(0,1)=axis3.x();
                    globalOrient(1,1)=axis3.y();
                    globalOrient(2,1)=axis3.z();
                    globalOrient(0,2)=axis1.x();
                    globalOrient(1,2)=axis1.y();
                    globalOrient(2,2)=axis1.z();
                    orient += (globalOrient.transpose());
                    c++;

                }
            }

            //khotoote dore zamin :D

            if( l_name == "FR" || l_name == "FS" )
            {
                if(p1.theta>p2.theta)
                    axis2 = Cr1-Cr2;
                else
                    axis2 = Cr2-Cr1;
                axis2.normalize();
                if(l_name == "FR"){
                    axis3 = axis2 ;
                    axis2 = axis3.cross(axis1) ;
                }
                else
                {
                    axis3 = axis1.cross(axis2);
                }
                Eigen::Matrix3f globalOrient;
                globalOrient(0,0)=axis2.x();
                globalOrient(1,0)=axis2.y();
                globalOrient(2,0)=axis2.z();
                globalOrient(0,1)=axis3.x();
                globalOrient(1,1)=axis3.y();
                globalOrient(2,1)=axis3.z();
                globalOrient(0,2)=axis1.x();
                globalOrient(1,2)=axis1.y();
                globalOrient(2,2)=axis1.z();
                orient += (globalOrient.transpose());
                c++;

            }

            else if ( l_name == "FL" ||  l_name == "FN" )
            {
                if(p1.theta>p2.theta)
                    axis2 = Cr2-Cr1;
                else
                    axis2 = Cr1-Cr2;
                axis2.normalize();
                if(l_name == "FL")
                {
                    axis3 = axis2 ;
                    axis2 = axis3.cross(axis1);
                }
                else
                {
                    axis3 = axis1.cross(axis2);
                }
                Eigen::Matrix3f globalOrient;
                globalOrient(0,0)=axis2.x();
                globalOrient(1,0)=axis2.y();
                globalOrient(2,0)=axis2.z();
                globalOrient(0,1)=axis3.x();
                globalOrient(1,1)=axis3.y();
                globalOrient(2,1)=axis3.z();
                globalOrient(0,2)=axis1.x();
                globalOrient(1,2)=axis1.y();
                globalOrient(2,2)=axis1.z();
                orient += (globalOrient.transpose());
                c++;

            }*/
        }
    }
    if (c > 0) {
        Eigen::Vector3f x = orient.col(0);
        Eigen::Vector3f y = orient.col(1);
        Eigen::Vector3f z = orient.col(2);
        x.normalize();
        y.normalize();
        z.normalize();
        orient.col(0) = x;
        orient.col(1) = y;
        orient.col(2) = z;

        //besme rabe shohada va sedighein
        Eigen::Vector3f myPos(0, 0, 0);
        Eigen::Matrix3f myOr;
        myOr << orient(0, 0), orient(0, 1), orient(0, 2),
                orient(1, 0), orient(1, 1), orient(1, 2),
                orient(2, 0), orient(2, 1), orient(2, 2)
                ;
        int numberOfFlags = 0;
        for (unordered_map<string, Eigen::Vector3f>::iterator i = WM->flag.begin(); i != WM->flag.end(); i++) {
            if (WM->flagLastSeen[i->first] != WM->serverTime) {
                continue;
            }
            Eigen::Vector3f poss = WM->flagGlobal[i->first] - myOr*(i->second);
            //        RVDraw::instance()->drawLine(sensedPos,sensedPos+headRotate*(i->second),GREEN,numberOfFlags);

            myPos += poss;
            numberOfFlags++;
        }

        if (numberOfFlags) {
            WM->Localed = true;
            myPos /= numberOfFlags;
            //            cout<<"mypos : "<<myPos << endl;
            //            if (WM->getTeamSide() == Left)
            //                RVDraw::instance()->drawEigen::Vector3f(myPos, RED, il, 3);
            /*else*/ if (WM->getTeamSide() == Right) {
                Eigen::Vector3f tmp;
                tmp.x() = -myPos.x();
                tmp.y() = -myPos.y();
                tmp.z() = myPos.z();
                //                RVDraw::instance()->drawEigen::Vector3f(tmp,RED,1,3);
            }
            il += 2;
            //            RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos,BLUE,jl,3);
            jl -= 2;
        }

        //cout<<(myPos - WM->gyroPos ).norm()<<endl;

        Eigen::Vector3f hrX = WM->headRotate*(Eigen::Vector3f(1, 0, 0));
        Eigen::Vector3f hrY = WM->headRotate*(Eigen::Vector3f(0, 1, 0));
        Eigen::Vector3f hrZ = WM->headRotate*(Eigen::Vector3f(0, 0, 1));

        Eigen::Vector3f brX = myOr*(Eigen::Vector3f(1, 0, 0));
        Eigen::Vector3f brY = myOr*(Eigen::Vector3f(0, 1, 0));
        Eigen::Vector3f brZ = myOr*(Eigen::Vector3f(0, 0, 1));
        /*( pow(orient(0,0) - hrX.x(),2) + pow(orient(1,0)-hrX.y(),2) + pow(orient(2,0)-hrX.z(),2) ) >0.0005 ||
                   ( pow(orient(0,1) - hrY.x(),2) + pow(orient(1,1)-hrY.y(),2) + pow(orient(2,1)-hrY.z(),2) ) >0.0005 ||
                   ( pow(orient(0,2) - hrZ.x(),2) + pow(orient(1,2)-hrZ.y(),2) + pow(orient(2,2)-hrZ.z(),2) ) >0.0005*/
        if (WM->isServerBeamed() && recalib) {
            //            missCycle++;
            //            cout<<"diff : "<<(myPos - WM->gyroPos ).norm()<<endl;
            //            if(missCycle > 2)
            //            {
            //                missCycle=0;

            //                Eigen::Vector3f x_before_he2_rotation=WM->general_rotation(brX,brY,Deg2Rad(WM->getJointAngle("he2")));
            //                Eigen::Vector3f y_before_he2_rotation=WM->general_rotation(brY,brY,Deg2Rad(WM->getJointAngle("he2")));
            //                Eigen::Vector3f z_before_he2_rotation=WM->general_rotation(brZ,brY,Deg2Rad(WM->getJointAngle("he2")));

            //                Eigen::Vector3f mhrX=WM->general_rotation(x_before_he2_rotation,z_before_he2_rotation,-Deg2Rad(WM->getJointAngle("he1")));
            //                Eigen::Vector3f mhrY=WM->general_rotation(y_before_he2_rotation,z_before_he2_rotation,-Deg2Rad(WM->getJointAngle("he1")));
            //                Eigen::Vector3f mhrZ=WM->general_rotation(z_before_he2_rotation,z_before_he2_rotation,-Deg2Rad(WM->getJointAngle("he1")));

            //                //besme rabe shohada va sedighein ...ya allah :)
            //                WM->bodyRotate.Set(mhrX.x(),mhrY.x(),mhrZ.x(),0,
            //                                   mhrX.y(),mhrY.y(),mhrZ.y(),0,
            //                                   mhrX.z(),mhrY.z(),mhrZ.z(),0,
            //                                    0, 0, 0, 1);
            //                if ( WM->ballLastSeen == WM->serverTime )          /// if See Ball In This Cycle Set Its Pos !
            //                {
            //                    Eigen::Vector3f ballPos = myPos + myOr*(WM->ball);
            //                    WM->setBallVel(ballPos-WM->getBallPos());
            //                    WM->setBallPos(ballPos);
            //                    if(ballPos.z()<=0)
            //                    {
            //                        ballPos.Set(Eigen::Vector3f(ballPos.x(),ballPos.y(),0.042));
            //                    }
        }

        //                cout<<"salam"<<endl;
        //                cout<<"hoora :"<<endl
        //                    <<"X : "<<( pow(orient(0,0) - hrX.x(),2) + pow(orient(1,0)-hrX.y(),2) + pow(orient(2,0)-hrX.z(),2) ) <<endl
        //                    <<"Y : "<<( pow(orient(0,1) - hrY.x(),2) + pow(orient(1,1)-hrY.y(),2) + pow(orient(2,1)-hrY.z(),2) ) <<endl
        //                    <<"Z : "<<( pow(orient(0,2) - hrZ.x(),2) + pow(orient(1,2)-hrZ.y(),2) + pow(orient(2,2)-hrZ.z(),2) ) <<endl;
        //            }

        //        }
        //        else
        //            missCycle =0;

        //        cout<<"Man rotate :"<<endl<<orient<<endl;
        //       <<"X: "<<globalOrient*(Eigen::Vector3f(1,0,0)).normalize()<<endl
        //      <<"Y: "<<globalOrient*(Eigen::Vector3f(0,1,0)).normalize()<<endl
        //     <<"Z: "<<globalOrient*(Eigen::Vector3f(0,0,1)).normalize()<<endl;
        //        cout<<"head rotate :"<<endl
        //           <<"X: "<<WM->headRotate*(Eigen::Vector3f(1,0,0)).normalize()<<endl
        //          <<"Y: "<<WM->headRotate*(Eigen::Vector3f(0,1,0)).normalize()<<endl
        //         <<"Z: "<<WM->headRotate*(Eigen::Vector3f(0,0,1)).normalize()<<endl;
        //        cout<<"hoora :"<<endl
        //           <<"X : "<<sqrt( pow(orient(0,0) - hrX.x(),2) + pow(orient(1,0)-hrX.y(),2) + pow(orient(2,0)-hrX.z(),2) ) <<endl
        //           <<"Y : "<<sqrt( pow(orient(0,1) - hrY.x(),2) + pow(orient(1,1)-hrY.y(),2) + pow(orient(2,1)-hrY.z(),2) ) <<endl
        //           <<"Z : "<<sqrt( pow(orient(0,2) - hrZ.x(),2) + pow(orient(1,2)-hrZ.y(),2) + pow(orient(2,2)-hrZ.z(),2) ) <<endl;

        return orient;

    } else
        ;
    //cout<<"ridi"<<endl;
}

void Localize::gyroLocalize()
{
    if (WM->getPlayMode() == PM_BeforeKickOff) {
        WM->clearBodyRotate();
    }
    WM->initDimentions();
    WM->initFlags();

    Eigen::Vector3f rightGyro(WM->gyro.y(), -WM->gyro.x(), WM->gyro.z());
    Eigen::Vector3f newGyro = WM->bodyRotate*(rightGyro);

    double theta = Deg2Rad(newGyro.norm() * 0.02);

    if (newGyro.norm() > 0.2)
        newGyro = newGyro / newGyro.norm();

    Eigen::Vector3f x = WM->bodyRotate*(Eigen::Vector3f(1, 0, 0));
    Eigen::Vector3f y = WM->bodyRotate*(Eigen::Vector3f(0, 1, 0));
    Eigen::Vector3f z = WM->bodyRotate*(Eigen::Vector3f(0, 0, 1));

    Eigen::Vector3f newx = x * cos(theta) + (newGyro.cross(x)) * sin(theta) + newGyro * (newGyro.dot(x)) * (1 - cos(theta));
    Eigen::Vector3f newy = y * cos(theta) + (newGyro.cross(y)) * sin(theta) + newGyro * (newGyro.dot(y)) * (1 - cos(theta));
    Eigen::Vector3f newz = newx.cross(newy);

    WM->bodyRotate << newx.x(), newy.x(), newz.x(),
            newx.y(), newy.y(), newz.y(),
            newx.z(), newy.z(), newz.z();

    WM->setMyAngle(atan2Deg(newx.y(), newx.x()));

    Eigen::Vector3f x_after_he1_rotation = general_rotation(newx, newz, Deg2Rad(WM->getJointAngle("he1")));
    Eigen::Vector3f y_after_he1_rotation = general_rotation(newy, newz, Deg2Rad(WM->getJointAngle("he1")));
    Eigen::Vector3f z_after_he1_rotation = general_rotation(newz, newz, Deg2Rad(WM->getJointAngle("he1")));

    Eigen::Vector3f x_after_he2_rotation = general_rotation(x_after_he1_rotation, y_after_he1_rotation, -Deg2Rad(WM->getJointAngle("he2")));
    Eigen::Vector3f y_after_he2_rotation = general_rotation(y_after_he1_rotation, y_after_he1_rotation, -Deg2Rad(WM->getJointAngle("he2")));
    Eigen::Vector3f z_after_he2_rotation = general_rotation(z_after_he1_rotation, y_after_he1_rotation, -Deg2Rad(WM->getJointAngle("he2")));
    //    cout<<"gyro :"<<endl
    //        <<x_after_he2_rotation<<endl
    //        <<y_after_he2_rotation<<endl
    //        <<z_after_he2_rotation<<endl;

    WM->headRotate << x_after_he2_rotation.x(), y_after_he2_rotation.x(), z_after_he2_rotation.x(),
            x_after_he2_rotation.y(), y_after_he2_rotation.y(), z_after_he2_rotation.y(),
            x_after_he2_rotation.z(), y_after_he2_rotation.z(), z_after_he2_rotation.z();

    int numberOfFlags = 0;

    Eigen::Vector3f myPos(0, 0, 0);
    for (unordered_map<string, Eigen::Vector3f>::iterator i = WM->flag.begin(); i != WM->flag.end(); i++) {
        if (WM->flagLastSeen[i->first] != WM->serverTime) {
            continue;
        }

        Eigen::Vector3f poss = WM->flagGlobal[i->first] - WM->headRotate*(i->second);

        myPos += poss;
        numberOfFlags++;
    }

    /**
      for testing purpose
    */

    if (numberOfFlags) {
        WM->Localed = true;
        myPos /= numberOfFlags;
        WM->setGyroPos(myPos);
        WM->setMyPos(myPos);

        //        static int c = 0;
        //        c++;
        //                RVDraw::instance()->drawEigen::Vector3f(myPos, RED, c,3);
        //        c++;
        //        RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos,BLACK,c,3);
    }
    localizeOtherElement(myPos);
}

//this function will rotate an orirentation around an axis with specified angle, the angle should be in radian

Eigen::Vector3f Localize::general_rotation(Eigen::Vector3f initial, Eigen::Vector3f axis, double angle)
{
    //    cout<<"angle: "<<angle<<endl;
    //    cout<<"axis: "<<axis<<endl;
    Eigen::Vector3f normaled_axis(axis.x() / axis.norm(), axis.y() / axis.norm(), axis.z() / axis.norm());
    double ux = normaled_axis.x();
    double uy = normaled_axis.y();
    double uz = normaled_axis.z();
    Eigen::Matrix3f Rotation;
    Rotation << cos(angle) + ux * ux * (1 - cos(angle)), ux * uy * (1 - cos(angle)) - uz * sin(angle), ux * uz * (1 - cos(angle)) + uy * sin(angle),
            ux * uy * (1 - cos(angle)) + uz * sin(angle), cos(angle) + uy * uy * (1 - cos(angle)), uy * uz * (1 - cos(angle)) - ux * sin(angle),
            ux * uz * (1 - cos(angle)) - uy * sin(angle), uy * uz * (1 - cos(angle)) + ux * sin(angle), cos(angle) + uz * uz * (1 - cos(angle));
    Eigen::Vector3f rotated = Rotation*(initial);
    //    cout<<"rotated: "<<rotated<<endl;
    return rotated;
}

void Localize::localizeOtherElement(Eigen::Vector3f myPos)
{

    if (WM->ballLastSeen == WM->serverTime) /// if See Ball In This Cycle Set Its Pos !
    {
        Eigen::Vector3f ballPos = myPos + WM->headRotate*(WM->ball);
        WM->setBallVel(ballPos - WM->getBallPos());
        WM->setBallPos(ballPos);
        if (ballPos.z() <= 0) {
            ballPos << Eigen::Vector3f(ballPos.x(), ballPos.y(), 0.042) ;
        }
    }
    for (unordered_map<int, RelPlayerInfo>::iterator i = WM->ourRel.begin(); i != WM->ourRel.end(); i++) {
        if (i->first != WM->getMyNum()) {
            WM->our[i->first].head = myPos + WM->headRotate*(PolarToCartecian(i->second.head));
        }
        WM->our[i->first].lfoot = myPos + WM->headRotate*(PolarToCartecian(i->second.lfoot));
        WM->our[i->first].rfoot = myPos + WM->headRotate*(PolarToCartecian(i->second.rfoot));
        WM->our[i->first].rlowerarm = myPos + WM->headRotate*(PolarToCartecian(i->second.rlowerarm));
        WM->our[i->first].llowerarm = myPos + WM->headRotate*(PolarToCartecian(i->second.llowerarm));
    }

    for (unordered_map<int, RelPlayerInfo>::iterator i = WM->theirRel.begin(); i != WM->theirRel.end(); i++) {
        WM->their[i->first].head = myPos + WM->headRotate*(PolarToCartecian(i->second.head));
        WM->their[i->first].lfoot = myPos + WM->headRotate*(PolarToCartecian(i->second.lfoot));
        WM->their[i->first].rfoot = myPos + WM->headRotate*(PolarToCartecian(i->second.rfoot));
        WM->their[i->first].rlowerarm = myPos + WM->headRotate*(PolarToCartecian(i->second.rlowerarm));
        WM->their[i->first].llowerarm = myPos + WM->headRotate*(PolarToCartecian(i->second.llowerarm));
    }
}

void Localize::erfanKiriestLocalize()
{
    setLocalLines();
    setImportantPoints();
    setX();
    setY();
    set_second_x_estimation();
    add_position();
    add_median_of_last_positions();
    set_head_rotate(Eigen::Vector3f(X_second_estimation, Y_initial_estimation, 0.516));

    if ( WM->getTeamSide() == Left )
        WM->setMyPos(Eigen::Vector3f(X_second_estimation, Y_initial_estimation, 0.516));
    else
        WM->setMyPos(Eigen::Vector3f(-X_second_estimation, -Y_initial_estimation, 0.516));

    //    cout << Y_initial_estimation << endl;
    //    cout << Eigen::Vector3f(X_second_estimation, Y_initial_estimation, 0.516) << endl;
    localizeOtherElement(WM->getMyPos());
    resetEveryThing();
    //    static int c=0;
    //    c++;
    //    RVDraw::instance()->drawEigen::Vector3f(WM->getMyPos(), RED, c,3);
    //    c++;
    //    RVDraw::instance()->drawEigen::Vector3f(WM->sensedPos,BLACK,c,3);
}

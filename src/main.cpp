/*
   RRRR    OOOOO   BBBBB    OOOOO  TTTTTTTT  OOOOO   OOOOO   SSSSS
   R  R    O   O   B    B   O   O     T      O   O   O   O  S
   RRRR    O   O   BBBBB    O   O     T      O   O   O   O   SSSS
   R  R    O   O   B    B   O   O     T      O   O   O   O       S
   R   R   OOOOO   BBBBB    OOOOO     T      OOOOO   OOOOO  SSSSS
*/

///==========================================
///==========================================
///|| Robotoos3D Base By Mohammad Razeghi  ||
///||       razeghi71@gmail.com            ||
///     And Mr IrFun Tvakoli
///         erfan.tavakoli71@gmail.com
///==========================================
///==========================================
/*     Members :
       Mohammad Razeghi
       Hamed Ghayour
*/
#include "Parser.h"
#include "Decide.h"
//#include "ConnectionManager.h"
#include "connection.h"
#include <cstring>
#include <cstdio>
#include <string>
#include <unistd.h>
#include "Localize.h"

using namespace std;

///~ argument arg is name of execution file
void printHelp(char arg[]);
int main(int argc, char* argv[])
{

    /// create a connection
    //        ConnectionManager *sockClient = new ConnectionManager ;
    connection* sockClient = new connection;
    ///~ WorldModel to Save World Info
    WorldModel* WM = new WorldModel;
    /// ~ Class Decide For Decition making
    Decide* DC = new Decide(WM);
    /// ~ Parser To Parse Server Strings
    Parser* P = new Parser(WM);

    /// ~ Localizer class
    Localize* localize = new Localize(WM);

    ///~  Default Team Name is Robotoos3D
    WM->setOurName("PaYdar3D");
    ///~ Default Player is Attacker (Number 9)
    WM->setMyNum(9);
    ///~ standing Z is 0.54q
    WM->setZ(0.54);
    ///~ By Default Agent Connect To Local Host
    string ip = "127.0.0.1";
    ///~ Default Server Port is 3100
    int port = 3100;
    for (int i = 1; i < argc; i++) {
        ///~ Print Help
        if (!strcmp(argv[i], "--help")) {
            printHelp(argv[0]);
            exit(0);
        }
        ///~ Set Ip
        if (!strcmp(argv[i], "--ip")) {
            ip = argv[++i];
        }
        ///~ Set Port
        else if (!strcmp(argv[i], "--port")) {
            port = atoi(argv[++i]);
        }
        ///~ Set Team Name
        else if (!strcmp(argv[i], "--name")) {
            WM->setOurName(argv[++i]);
        }
        ///~ Set Agent Number
        else if (!strcmp(argv[i], "--num")) {
            WM->setMyNum(atoi(argv[++i]));
        }
        ///~ if Option is Illegal Giva an Error
        else {
            cerr << "Unkown Input :\" " << argv[i] << " \" ?" << endl;
            cerr << "Use --help Option To See Arguments" << endl;
            exit(1);
        }
    }
    /// connect To Server
    try
    {
        //        sockClient->connect_to(ip.c_str(),port);
        sockClient->connect_to(ip.c_str(), port);
        /// send scene Message
        //        sockClient->Receive();
        int num = WM->getMyNum();

        if ( num == 8 || num == 9 || num == 10 || num == 11  || num == 2 || num == 3 || num == 4 )
        {
            sockClient->send ("(scene rsg/agent/nao/nao_hetero.rsg 0)");
        } else if ( num == 7  || num == 6 )
        {
            sockClient->send ("(scene rsg/agent/naotoe/naotoe_hetero.rsg 0)");
        } else
        {
            sockClient->send ("(scene rsg/agent/nao/nao_hetero.rsg 2)");
        }
        string get;
        sockClient->receive(get);

        stringstream ss;
        usleep(500);
        /// init Agent and Send number and our team name (Robotoos3D)
        ss << "(init (unum " << WM->getMyNum() << ")(teamname " << WM->getOurName() << "))";
        sockClient->send(ss.str());
        string dec;
        timespec* start, *end;
        start = new timespec;
        end = new timespec;
        double parseTime, localizeTime, decideTime;

        while (1) {
            sockClient->receive(get);

            clock_gettime(CLOCK_REALTIME, start); // Works on Linux
            P->Parse(get);
            clock_gettime(CLOCK_REALTIME, end); // Works on Linux
            parseTime = (end->tv_nsec - start->tv_nsec) / 1000000000.0;

            clock_gettime(CLOCK_REALTIME, start); // Works on Linux
//            WM->Localize();
            localize->erfanKiriestLocalize();
//            localize->test();
            clock_gettime(CLOCK_REALTIME, end); // Works on Linux
            localizeTime = (end->tv_nsec - start->tv_nsec) / 1000000000.0 ;

            clock_gettime(CLOCK_REALTIME, start); // Works on Linux
            dec = DC->decide();
            clock_gettime(CLOCK_REALTIME, end); // Works on Linux
            decideTime = (end->tv_nsec - start->tv_nsec) / 1000000000.0 ;

            if (parseTime + localizeTime + decideTime > 0.02) {
                cout <<  "p : " << parseTime << " l: " << localizeTime << " d: " << decideTime << endl;
//                cout << "Rinesh Happend : " << (end->tv_nsec - start->tv_nsec) / 1000000000.0 << endl;
                //                exit(0);
            }
            sockClient->send(dec);
        }
        ///~ Close Connection
    }
    catch (exception& e)
    {
        sockClient->disconnect();
        cout << e.what() << endl; /// if any exception happend Handle it & Print Error
    }
    return 0;
}

/// ~ Argument List Help
void printHelp(char arg[])
{
    cerr << "Usage :"
         << arg << " [Options] "
         << "\n    Options :"
         << "\n    --ip : ip Of Host"
         << "\n    --name : team name"
         << "\n    --num : player Number"
         << "\n    --port : Connecting port"
         << endl;
}

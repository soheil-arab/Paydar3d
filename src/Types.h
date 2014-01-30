#ifndef TYPES
#define TYPES
#include "salt.h"
#include "Geom.h"
#include <algorithm>
using namespace salt;
enum SideT
{
  Right = 0 ,
  Left ,
  Back ,
  Front ,
  Unknown
};

enum LogT
{
  LOG_FILE,
  LOG_STD,
};


struct FootRes
{
  Vector3f c;
  Vector3f f;
};

struct Polar
{
  double phi;
  double theta;
  double dist;
};
enum Command
{
  W,
  Di,
  T90,
  TU,
  SW,
  K,
  SSW,
  SSW2,
  SUB,
  SUF,
  DP,
  back,
  None,

};

struct RelPlayerInfo
{
  Polar head;
  Polar lfoot;
  Polar rfoot;
  Polar llowerarm;
  Polar rlowerarm;
  double lastSeen;
};

struct GloPlayerInfo
{
  Vector3f head;
  Vector3f lfoot;
  Vector3f rfoot;
  Vector3f llowerarm;
  Vector3f rlowerarm;
};

enum PlayMode
{
  PM_BeforeKickOff = 0,      /*!< before_kick_off:   before the match  */
  PM_KickOff_Left = 1,       /*!< kick_off_left:     kick off for the left team  */
  PM_KickOff_Right = 2,      /*!< kick_off_right:    kick off for the right team */
  PM_PlayOn,                 /*!< play_on:           regular game play */
  PM_KickIn_Left,
  PM_KickIn_Right,
  PM_CORNER_KICK_LEFT,       /*!< corner_kick_l:     corner kick left team   */
  PM_CORNER_KICK_RIGHT,      /*!< corner_kick_r:     corner kick right team  */
  PM_GOAL_KICK_LEFT,         /*!< goal_kick_l:       goal kick for left team */
  PM_GOAL_KICK_RIGHT,        /*!< goal_kick_r:       goal kick for right team*/
  PM_OFFSIDE_LEFT,           /*!< offside_l:         offside for left team   */
  PM_OFFSIDE_RIGHT,          /*!< offside_r:         offside for right team  */
  PM_GameOver,
  PM_Goal_Left,
  PM_Goal_Right,
  PM_FREE_KICK_LEFT,         /*!< free_kick_l:       free kick for left team */
  PM_FREE_KICK_RIGHT,        /*!< free_kick_r:       free kick for right team*/
  PM_NONE                    /*!< no play mode, this must be the last entry */
};

enum object 
{
  OBJECT_BALL,
  OBJECT_OPPONENTS,
  OBJECT_TEAMMATES,
  OBJECT_PLAYERS,
  OBJECT_TEAMMATES_NO_GOALIE,
  OBJECT_OPPONENTS_NO_GOALIE,
  OBJECT_PLAYERS_NO_GOALIE
  
};


#endif

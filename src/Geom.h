#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>       // needed for M_PI constant
#include <string>       // needed for string
#include <iostream>

using namespace std;

typedef double AngRad;  /*!< Type definition for angles in degrees. */
typedef double AngDeg;  /*!< Type definition for angles in radians. */

#define EPSILON 0.0001  /*!< Value used for doubleing point equality tests. */

// auxiliary numeric functions for determining the
// maximum and minimum of two given double values and the sign of a value
double max     ( double d1, double d2 );
double min     ( double d1, double d2 );
int    sign    ( double d1            );

// auxiliary goniometric functions which enable you to
// specify angles in degrees rather than in radians
AngDeg Rad2Deg ( AngRad x             );
AngRad Deg2Rad ( AngDeg x             );
double cosDeg  ( AngDeg x             );
double sinDeg  ( AngDeg x             );
double tanDeg  ( AngDeg x             );
AngDeg atanDeg ( double x             );
double atan2Deg( double x,  double y  );
AngDeg acosDeg ( double x             );
AngDeg asinDeg ( double x             );

// various goniometric functions
bool   isAngInInterval     ( AngDeg ang,    AngDeg angMin,    AngDeg angMax );
AngDeg getBisectorTwoAngles( AngDeg angMin, AngDeg angMax );

/*! CoordSystem is an enumeration of the different specified
	coordinate systems.  The two possibilities are CARTESIAN or
	POLAR. These values are for instance used in the initializing a
	VecPosition. The CoordSystem indicates whether the supplied
	arguments represent the position in cartesian or in polar
	coordinates. */
enum CoordSystemT
{
  CARTESIAN,
  POLAR
};

/*****************************************************************************/
/********************   CLASS VECPOSITION   **********************************/
/*****************************************************************************/

/*! This class contains an x- and y-coordinate of a position (x,y) as
	member data and methods which operate on this position. The
	standard arithmetic operators are overloaded and can thus be
	applied to positions (x,y). It is also possible to represent a
	position in polar coordinates (r,phi), since the class contains a
	method to convert these into cartesian coordinates (x,y). */
class VecPosition
{
  // private member data
private:

  double m_x;   /*!< x-coordinate of this position */
  double m_y;   /*!< y-coordinate of this position */

  // public methods
public:
  // constructor for VecPosition class
  VecPosition                               ( double            vx = 0,
      double            vy = 0,
      CoordSystemT      cs =CARTESIAN);

  // overloaded arithmetic operators
  VecPosition        operator -             (                                );
  VecPosition        operator +             ( const double      &d           );
  VecPosition        operator +             ( const VecPosition &p           );
  VecPosition        operator -             ( const double      &d           );
  VecPosition        operator -             ( const VecPosition &p           );
  VecPosition        operator *             ( const double      &d           );
  VecPosition        operator *             ( const VecPosition &p           );
  VecPosition        operator /             ( const double      &d           );
  VecPosition        operator /             ( const VecPosition &p           );
  void               operator =             ( const double      &d           );
  void               operator +=            ( const VecPosition &p           );
  void               operator +=            ( const double      &d           );
  void               operator -=            ( const VecPosition &p           );
  void               operator -=            ( const double      &d           );
  void               operator *=            ( const VecPosition &p           );
  void               operator *=            ( const double      &d           );
  void               operator /=            ( const VecPosition &p           );
  void               operator /=            ( const double      &d           );
  bool               operator !=            ( const VecPosition &p           );
  bool               operator !=            ( const double      &d           );
  bool               operator ==            ( const VecPosition &p           );
  bool               operator ==            ( const double      &d           );

  // methods for producing output
  friend ostream&    operator <<            ( ostream           &os,
      VecPosition       p            );
  void               show                   ( CoordSystemT      cs =CARTESIAN);
  string             str                    ( CoordSystemT      cs =CARTESIAN);

  // set- and get methods for private member variables
  bool               setX                   ( double            dX           );
  double             getX                   (                          ) const;
  bool               setY                   ( double            dY           );
  double             getY                   (                          ) const;

  // set- and get methods for derived position information
  void               setVecPosition         ( double            dX = 0,
      double            dY = 0,
      CoordSystemT      cs =CARTESIAN);
  double             getDistanceTo          ( const VecPosition p            );
  VecPosition        setMagnitude           ( double            d            );
  double             getMagnitude           (                          ) const;
  AngDeg             getDirection           (                          ) const;

  // comparison methods for positions
  bool               isInFrontOf            ( const VecPosition &p           );
  bool               isInFrontOf            ( const double      &d           );
  bool               isBehindOf             ( const VecPosition &p           );
  bool               isBehindOf             ( const double      &d           );
  bool               isLeftOf               ( const VecPosition &p           );
  bool               isLeftOf               ( const double      &d           );
  bool               isRightOf              ( const VecPosition &p           );
  bool               isRightOf              ( const double      &d           );
  bool               isBetweenX             ( const VecPosition &p1,
      const VecPosition &p2          );
  bool               isBetweenX             ( const double      &d1,
      const double      &d2          );
  bool               isBetweenY             ( const VecPosition &p1,
      const VecPosition &p2          );
  bool               isBetweenY             ( const double      &d1,
      const double      &d2          );

  // conversion methods for positions
  VecPosition        normalize              (                                );
  VecPosition        rotate                 ( AngDeg            angle        );
  VecPosition        globalToRelative       ( VecPosition       orig,
      AngDeg            ang          );
  VecPosition        relativeToGlobal       ( VecPosition       orig,
      AngDeg            ang          );
  VecPosition        getVecPositionOnLineFraction( VecPosition  &p,
      double            dFrac        );
  static VecPosition givePosition(VecPosition start,  double angle,  double distance);

  // static class methods
  static VecPosition getVecPositionFromPolar( double            dMag,
      AngDeg            ang          );
  static AngDeg      normalizeAngle         ( AngDeg            angle        );
};

/*****************************************************************************/
/*********************   CLASS GEOMETRY   ************************************/
/*****************************************************************************/

/*! This class contains several static methods dealing with geometry.*/
class Geometry
{

public:

  // geometric series
  static double getLengthGeomSeries(double dFirst,double dRatio,double dSum  );
  static double getSumGeomSeries   (double dFirst,double dRatio,double dLen  );
  static double getSumInfGeomSeries(double dFirst,double dRatio              );
  static double getFirstGeomSeries (double dSum,  double dRatio,double dLen  );
  static double getFirstInfGeomSeries(double dSum,double dRatio              );

  // abc formula
  static int    abcFormula(double a,double b, double c, double *s1,double *s2);
};

/*****************************************************************************/
/********************** CLASS CIRCLE *****************************************/
/*****************************************************************************/

/*!This class represents a circle. A circle is defined by one VecPosition
   (which denotes the center) and its radius. */
class Circle
{
  VecPosition m_posCenter;            /*!< Center of the circle  */
  double      m_dRadius;              /*!< Radius of the circle  */

public:
  Circle( );
  Circle( VecPosition pos, double dR );

  void        show                  ( ostream& os = cout );

  // get and set methods
  bool        setCircle             ( VecPosition pos,
                                      double      dR  );
  bool        setRadius             ( double dR       );
  double      getRadius             (                 );
  bool        setCenter             ( VecPosition pos );
  VecPosition getCenter             (                 );
  double      getCircumference      (                 );
  double      getArea               (                 );

  // calculate intersection points and area with other circle
  bool        isInside              ( VecPosition pos );
  int         getIntersectionPoints ( Circle      c,
                                      VecPosition *p1,
                                      VecPosition *p2 );
  double      getIntersectionArea   ( Circle c        );


}  ;

/*****************************************************************************/
/*********************** CLASS LINE ******************************************/
/*****************************************************************************/

/*!This class contains the representation of a line. A line is defined
   by the formula ay + bx + c = 0. The coefficients a, b and c are stored
   and used in the calculations. */
class Line
{
  // a line is defined by the formula: ay + bx + c = 0
  double m_a; /*!< This is the a coefficient in the line ay + bx + c = 0 */
  double m_b; /*!< This is the b coefficient in the line ay + bx + c = 0 */
  double m_c; /*!< This is the c coefficient in the line ay + bx + c = 0 */

public:
  Line( double a, double b, double c );

  // print methods
  void        show( ostream& os = cout );
  friend      ostream& operator << (ostream & os, Line l);

  // get intersection points with this line
  VecPosition getIntersection            ( Line        line                  );
  int         getCircleIntersectionPoints( Circle      circle,
      VecPosition *posSolution1,
      VecPosition *posSolution2         );
  Line        getTangentLine             ( VecPosition pos                   );
  VecPosition getPointOnLineClosestTo    ( VecPosition pos                   );
  double      getDistanceWithPoint       ( VecPosition pos                   );
  bool        isInBetween                ( VecPosition pos,
      VecPosition point1,
      VecPosition point2                );

  // calculate associated variables in the line
  double      getYGivenX                 ( double      x );
  double      getXGivenY                 ( double      y );
  double      getACoefficient            (               ) const;
  double      getBCoefficient            (               ) const;
  double      getCCoefficient            (               ) const;

  // static methods to make a line using an easier representation.
  static Line makeLineFromTwoPoints      ( VecPosition pos1,
      VecPosition pos2                  );
  static Line makeLineFromPositionAndAngle( VecPosition vec,
      AngDeg angle                      );
};

/*****************************************************************************/
/********************** CLASS RECTANGLE **************************************/
/******************************************************************************/
/*!This class represents a rectangle. A rectangle is defined by two
   VecPositions the one at the upper left corner and the one at the
   right bottom. */
class Rect
{
  VecPosition m_posLeftTop;     /*!< top left position of the rectangle      */
  VecPosition m_posRightBottom; /*!< bottom right position of the rectangle  */

public:
  Rect                          ( VecPosition pos, VecPosition pos2 );

  void        show              ( ostream& os = cout                );

  // checks whether point lies inside the rectangle
  bool        isInside          ( VecPosition pos                   );

  // standard get and set methosd
  void        setRectanglePoints( VecPosition pos1,
                                  VecPosition pos2                  );
  bool        setPosLeftTop     ( VecPosition pos                   );
  VecPosition getPosLeftTop     (                                   );
  bool        setPosRightBottom ( VecPosition pos                   );
  VecPosition getPosRightBottom (                                   );
};


#endif

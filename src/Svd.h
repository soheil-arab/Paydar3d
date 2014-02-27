#ifndef SVD_H
#define SVD_H

#include <algorithm>
#include <math.h>
using namespace std;
class SVD
{
public:
    SVD();
    SVD(double** A, int rows, int cols);
    double hypot(double a, double b);
    double** getV();
    double*  getS();
    double** getU();
private:
    /*
     * Arrays for internal storage of U and V.
     */
    double** U;
    double** V;

    /*
     * Array for internal storage of singular values.
     */
    double* s;

    /*
     * Row and column dimensions.
     */
    int m, n;
};

#endif // SVD_H

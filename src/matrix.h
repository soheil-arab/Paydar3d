#ifndef SALT_MATRIX_H
#define SALT_MATRIX_H

#include "defines.h"
#include "vector.h"
#include <memory.h>

namespace salt
{
#if 0
}
#endif

/** Matrix provides a 4x4 double Matrix along with methods to set
  *     up and manipulate it.
  */
class Matrix
{
public:
  /** the values of the matrix */
  double m[16];

  /** do nothing constructor, the matrix values are undefined for performance reasons*/
  f_inline Matrix() {}

  /** copy constructs the matrix from newMatrix, a pointer to a double array*/
  f_inline Matrix(double *newMatrix)
  {
    memcpy(m, newMatrix,    sizeof(double)*16);
  }

  /** copy constructs the matrix from newMatrix, a reference to another Matrix*/
  f_inline Matrix(const Matrix &newMatrix)
  {
    memcpy(m, newMatrix.m,  sizeof(double)*16);
  }

  /** copy constructs the matrix from newMatrix, a pointer to another Matrix*/
  f_inline Matrix(Matrix *newMatrix)
  {
    memcpy(m, newMatrix->m, sizeof(double)*16);
  }

  /** constructs the matrix from 16 double values */
  f_inline Matrix(double m00, double m01, double m02, double m03,
                  double m10, double m11, double m12, double m13,
                  double m20, double m21, double m22, double m23,
                  double m30, double m31, double m32, double m33);

  /** sets up the identity matrix */
  f_inline void Identity()
  {
    memcpy(m, mIdentity,    sizeof(double)*16);
  }

  /** returns a pointer to a double array describing an identity matrix */
  static double* GetIdentity()
  {
    return mIdentity;
  }

  /** sets up a X-rotation matrix with inAngle degrees */
  f_inline void RotationX(double inAngle);

  /** sets up a Y-rotation matrix with inAngle degrees */
  f_inline void RotationY(double inAngle);

  /** sets up a Z-rotation matrix with inAngle degrees */
  f_inline void RotationZ(double inAngle);

  /** sets up a translation matrix with inVector */
  f_inline void Translation(const Vector3f &inVector);

  /** sets up a scaling matrix with inVector */
  f_inline void Scale(const Vector3f & inVector);

  /** sets up a rotation matrix, looking from inEye in inDirection,
   *  with inUp pointing up
   */
  void LookAt(const Vector3f & inEye, const Vector3f & inDirection, const Vector3f & inUp);

  /** print the matrix to stdout for debugging purposes*/
  void        Dump() const;

  /** sets up the matrix from 16 double values */
  f_inline void       Set(double m00, double m01, double m02, double m03,
                          double m10, double m11, double m12, double m13,
                          double m20, double m21, double m22, double m23,
                          double m30, double m31, double m32, double m33);

  /** rotate the matrix around the axis X with inAngle radians */
  f_inline const Matrix &     RotateX(double inAngle);

  /** Rotate the matrix around the axis Y with inAngle radians */
  f_inline const Matrix &     RotateY(double inAngle);

  /** Rotate the matrix around the axis Z with inAngle radians */
  f_inline const Matrix &     RotateZ(double inAngle);

  /** Translate the matrix by vector inVector */
  f_inline const Matrix &     Translate(const Vector3f & inVector);

  /** returns a const reference to the right vector of the matrix */
  f_inline const Vector3f &   Right() const
  {
    return *(const Vector3f*) &El(0, 0);
  }

  /** returns a reference to the right vector of the matrix */
  f_inline Vector3f & Right()
  {
    return *(Vector3f*)  &El(0, 0);
  }

  /** returns a const reference to the up vector of the matrix */
  f_inline const Vector3f &   Up() const
  {
    return *(const Vector3f*) &El(0, 1);
  }

  /** returns a reference to the up vector of the matrix */
  f_inline Vector3f & Up()
  {
    return *(Vector3f*)  &El(0, 1);
  }

  /** returns a const reference to the forward vector of the matrix */
  f_inline const Vector3f &   Forward() const
  {
    return *(const Vector3f*) &El(0, 2);
  }

  /** returns a reference to the forward vector of the matrix */
  f_inline Vector3f & Forward()
  {
    return *(Vector3f*)  &El(0, 2);
  }

  /** returns a const reference to the pos vector of the matrix */
  f_inline const Vector3f &   Pos() const
  {
    return *(const Vector3f*) &El(0, 3);
  }

  /** returns a reference to the pos vector of the matrix */
  f_inline Vector3f & Pos()
  {
    return *(Vector3f*)  &El(0, 3);
  }

  /** returns true if this matrix is equal to <matrix> */
  bool        IsEqual(const Matrix& matrix) const;

  /** inverts a matrix, spezialized for rotation matrices only */
  f_inline void               InvertRotationMatrix();

  /** inverts a matrix */
  void InvertMatrix();

  /** multiplies the matrix with inVector */
  f_inline Vector3f   Transform(const Vector3f & inVector) const;

  /** rotates the matrix by inVector */
  f_inline Vector3f   Rotate(const Vector3f & inVector) const;

  /** inverse rotates the matrix by inVector */
  f_inline Vector3f   InverseRotate(const Vector3f & inVector) const;
  // special lighting matrices

  /** sets up an attenuation matrix without rotation */
  void CalcAttenuationNoRotation(const Vector3f &pos, double radius);

  /** sets up an attenuation matrix with rotation */
  void CalcAttenuationWithRotation(const Matrix &lightWorldMatrix, double radius);

  /** sets up an infinite projection matrix */
  void CalcInfiniteProjection(double width, double height, double fov, double zNear);

  /** sets up an infinite frustum */
  void CalcInfiniteFrustum(double left, double right, double bottom, double top, double zNear);

  /** sets up a special lighting matrix for a spotlight */
  void CalcSpotLight(const Matrix &lightWorldTransform, double fov, double width, double height, double zNear);

  // Operators

  /** multiplies the matrix with another matrix */
  f_inline const Matrix       operator*(const Matrix &inRHS) const;

  /** multiplies the matrix with another Matrix */
  f_inline const Matrix &     operator*=(const Matrix &inRHS);
  f_inline const Matrix &     operator+=(const Matrix &inRHS);

  f_inline const Matrix       operator+(const Matrix &inRHS) const;
  f_inline const Matrix       operator/(const int &num) const;
  /** Multipies the matrix with a double */
  f_inline  Matrix       zero() ;


  /** Multipies the matrix with a 3 dimensional vector */
  f_inline const Vector3f     operator*(const Vector3f &inRHS) const;

  /** multiplies the matrix with a 2 dimensional vector */
  f_inline const Vector3f     operator*(const Vector2f &inRHS) const;

  // Element access operators

  /** returns a reference to a single element of the matrix */
  f_inline double& operator() (int inRow, int inColumn)
  {
    return El(inRow, inColumn);
  }

  /** returns a const reference to a single element of the matrix */
  f_inline const double& operator() (int inRow, int inColumn) const
  {
    return El(inRow, inColumn);
  }

protected:
  /** returns a reference to a single element of the matrix */
  f_inline double& El(int inRow, int inColumn)
  {
    return m[inColumn*4 + inRow];
  }

  /** returns a const reference to a single element of the matrix */
  f_inline const double& El(int inRow, int inColumn) const
  {
    return m[inColumn*4 + inRow];
  }

private:
  /** the identity matrix */
  static double mIdentity[16];
};

f_inline Matrix::Matrix(double m00, double m01, double m02, double m03,
                        double m10, double m11, double m12, double m13,
                        double m20, double m21, double m22, double m23,
                        double m30, double m31, double m32, double m33)
{
  m[0] = m00;
  m[4] = m01;
  m[8] = m02;
  m[12] = m03;
  m[1] = m10;
  m[5] = m11;
  m[9] = m12;
  m[13] = m13;
  m[2] = m20;
  m[6] = m21;
  m[10]= m22;
  m[14] = m23;
  m[3] = m30;
  m[7] = m31;
  m[11]= m32;
  m[15] = m33;
}

f_inline void Matrix::RotationX(double inAngle)
{
  double c=gCos(inAngle), s=gSin(inAngle);

  // Create X-Rotation matrix
  Identity();
  El(1, 1) = c;
  El(2, 1) = s;
  El(1, 2) = -s;
  El(2, 2) = c;
}

f_inline void Matrix::RotationY(double inAngle)
{
  double c=gCos(inAngle), s=gSin(inAngle);

  // Create Y-Rotation matrix
  Identity();
  El(0, 0) = c;
  El(2, 0) = -s;
  El(0, 2) = s;
  El(2, 2) = c;
}

f_inline void Matrix::RotationZ(double inAngle)
{
  double c=gCos(inAngle), s=gSin(inAngle);

  // Create Z-Rotation matrix
  Identity();
  El(0, 0) = c;
  El(1, 0) = s;
  El(0, 1) = -s;
  El(1, 1) = c;
}

f_inline void Matrix::Translation(const Vector3f & inVector)
{
  // Create translation matrix
  Identity();
  El(0, 3) = inVector.x();
  El(1, 3) = inVector.y();
  El(2, 3) = inVector.z();
}

f_inline void Matrix::Scale(const Vector3f & inVector)
{
  // Create translation matrix
  Identity();
  El(0, 0) = inVector.x();
  El(1, 1) = inVector.y();
  El(2, 2) = inVector.z();
}

f_inline void Matrix::Set(double m00, double m01, double m02, double m03,
                          double m10, double m11, double m12, double m13,
                          double m20, double m21, double m22, double m23,
                          double m30, double m31, double m32, double m33)
{
  m[0] = m00;
  m[4] = m01;
  m[8] = m02;
  m[12] = m03;
  m[1] = m10;
  m[5] = m11;
  m[9] = m12;
  m[13] = m13;
  m[2] = m20;
  m[6] = m21;
  m[10]= m22;
  m[14] = m23;
  m[3] = m30;
  m[7] = m31;
  m[11]= m32;
  m[15] = m33;
}


// 'multiplies' the current matrix by a x-rotation matrix
f_inline const Matrix & Matrix::RotateX(double inAngle)
{
  double c=gCos(inAngle), s=gSin(inAngle);

  // calculate the changed values
  double m01 = c*El(0,1) + s*El(0,2);
  double m11 = c*El(1,1) + s*El(1,2);
  double m21 = c*El(2,1) + s*El(2,2);
  double m31 = c*El(3,1) + s*El(3,2);
  double m02 =-s*El(0,1) + c*El(0,2);
  double m12 =-s*El(1,1) + c*El(1,2);
  double m22 =-s*El(2,1) + c*El(2,2);
  double m32 =-s*El(3,1) + c*El(3,2);

  // transfer them to the matrix
  El(0,1) = m01;
  El(1,1) = m11;
  El(2,1) = m21;
  El(3,1) = m31;
  El(0,2) = m02;
  El(1,2) = m12;
  El(2,2) = m22;
  El(3,2) = m32;

  return (*this);
}

// 'multiplies' the current matrix by a y-rotation matrix
f_inline const Matrix & Matrix::RotateY(double inAngle)
{
  double c=gCos(inAngle), s=gSin(inAngle);

  // calculate the changed values
  double m00 = c*El(0,0) - s*El(0,2);
  double m10 = c*El(1,0) - s*El(1,2);
  double m20 = c*El(2,0) - s*El(2,2);
  double m30 = c*El(3,0) - s*El(3,2);
  double m02 = s*El(0,0) + c*El(0,2);
  double m12 = s*El(1,0) + c*El(1,2);
  double m22 = s*El(2,0) + c*El(2,2);
  double m32 = s*El(3,0) + c*El(3,2);

  // transfer them to the matrix
  El(0,0) = m00;
  El(1,0) = m10;
  El(2,0) = m20;
  El(3,0) = m30;
  El(0,2) = m02;
  El(1,2) = m12;
  El(2,2) = m22;
  El(3,2) = m32;

  return (*this);
}

// 'multiplies' the current matrix by a z-rotation matrix
f_inline const Matrix & Matrix::RotateZ(double inAngle)
{
  double c=gCos(inAngle), s=gSin(inAngle);

  // calculate the changed values
  double m00 = c*El(0,0) + s*El(0,1);
  double m10 = c*El(1,0) + s*El(1,1);
  double m20 = c*El(2,0) + s*El(2,1);
  double m30 = c*El(3,0) + s*El(3,1);
  double m01 =-s*El(0,0) + c*El(0,1);
  double m11 =-s*El(1,0) + c*El(1,1);
  double m21 =-s*El(2,0) + c*El(2,1);
  double m31 =-s*El(3,0) + c*El(3,1);

  // transfer them to the matrix
  El(0,0) = m00;
  El(1,0) = m10;
  El(2,0) = m20;
  El(3,0) = m30;
  El(0,1) = m01;
  El(1,1) = m11;
  El(2,1) = m21;
  El(3,1) = m31;

  return (*this);
}

// 'multiplies' the current matrix by a translation matrix (last row is interpreted as 0 0 0 1)
f_inline const Matrix & Matrix::Translate(const Vector3f & inVector)
{
  double x = inVector.x(), y = inVector.y(), z = inVector.z();

  // transfer them to the matrix
  El(0,3) = x*m[0] + y*m[4] + z*m[8] + m[12];
  El(1,3) = x*m[1] + y*m[5] + z*m[9] + m[13];
  El(2,3) = x*m[2] + y*m[6] + z*m[10]+ m[14];

  return (*this);
}

f_inline void Matrix::InvertRotationMatrix()
{
  // transpose the rotation part
  gSwap(El(0, 1), El(1, 0));
  gSwap(El(0, 2), El(2, 0));
  gSwap(El(1, 2), El(2, 1));

  // Set new position
  Pos() = -Rotate(Pos());
}

// full matrix application (last row is interpreted as 0 0 0 1)
f_inline Vector3f Matrix::Transform(const Vector3f & inVector) const
{
  double x = inVector.x(), y = inVector.y(), z = inVector.z();

  return Vector3f(    x*m[0] + y*m[4] + z*m[8] + m[12],
                      x*m[1] + y*m[5] + z*m[9] + m[13],
                      x*m[2] + y*m[6] + z*m[10]+ m[14]);
}

// this only applies the rotation part of the matrix
f_inline Vector3f Matrix::Rotate(const Vector3f & inVector) const
{
  double x = inVector.x(), y = inVector.y(), z = inVector.z();
  return Vector3f(    x*m[0] + y*m[4] + z*m[8],
                      x*m[1] + y*m[5] + z*m[9],
                      x*m[2] + y*m[6] + z*m[10]);
}

// this applies the transpose of the rotation part of the matrix (inverse rotation)
f_inline Vector3f Matrix::InverseRotate(const Vector3f & inVector) const
{
  double x = inVector.x(), y = inVector.y(), z = inVector.z();
  return Vector3f(x*m[0] + y*m[1] + z*m[2],
                  x*m[4] + y*m[5] + z*m[6],
                  x*m[8] + y*m[9] + z*m[10]);
}

// Multiply this Matrix with another Matrix
f_inline const Matrix Matrix::operator*(const Matrix & inRHS) const
{
  Matrix r;

  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      {
        r(i,j) = El(i,0)*inRHS(0,j)+El(i,1)*inRHS(1,j)+El(i,2)*inRHS(2,j)+El(i,3)*inRHS(3,j);
      }

  return r;
}

f_inline const Matrix Matrix::operator+(const Matrix & inRHS) const
{
  Matrix r;

  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      {
        r(i,j) = El(i,j)+inRHS(i,j);
      }
  return r;
}

f_inline const Matrix & Matrix::operator+=(const Matrix &inRHS)
{
  *this = *this + inRHS;
  return *this;
}

f_inline const Matrix Matrix::operator/(const int &num) const
{
  Matrix r;

  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      {
        r(i,j) = El(i,j)/num;
      }
  return r;
}

// Multiply a double with this Matrix
f_inline  Matrix  Matrix::zero()
{
  for (int i=0; i<4; i++)
    {
      for (int j=0; j<4; j++)
        {
          El(i,j)=0.0;
        }
    }

}

// Multiply a Vector3f with this Matrix
f_inline const Vector3f Matrix::operator*(const Vector3f &inRHS) const
{
  double x = inRHS.x(), y = inRHS.y(), z = inRHS.z();

  return Vector3f(x*m[0] + y*m[4] + z*m[8] + m[12],
                  x*m[1] + y*m[5] + z*m[9] + m[13],
                  x*m[2] + y*m[6] + z*m[10]+ m[14]);
}

// Multiply a Vector2f with this Matrix
f_inline const Vector3f Matrix::operator*(const Vector2f &inRHS) const
{
  double x = inRHS.x(), y = inRHS.y();

  return Vector3f(x*m[0] + y*m[4] + m[12],
                  x*m[1] + y*m[5] + m[13],
                  x*m[2] + y*m[6] + m[14]);
}

f_inline const Matrix & Matrix::operator*=(const Matrix &inRHS)
{
  *this = *this * inRHS;
  return *this;
}

} //namespace salt

#endif // SALT_MATRIX_H

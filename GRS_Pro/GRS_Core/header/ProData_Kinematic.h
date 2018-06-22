
#ifndef KINEMATIC_H
#define KINEMATIC_H


//#include "ITP_Interface.h"
#include "ITP_Interface.h"

#if defined(__cplusplus)
extern "C"
{
#endif



void Kinematic_CreateRotationMatrix(double mat[4][4],double a,double alpha,double d,double theta);

void Kinematic_MultiplyMatrix(double matOutput[4][4],double matLeft[4][4],double matRight[4][4]);

int Kinematic__mat4_inv(double a[4][4],double ainv[4][4]);

int Kinematics_XYZToQuat(double *pXYZ,ITP_Orient *quat);
int Kinematics_XYZToMat(double *pXYZ, double m[4][4]);
int Kinematics_Mat2xyz(double m[4][4], ITP_EulerXYZ *Exyz);
int Kinematics_MatToQuat(double m[4][4], ITP_Orient *q);
int Kinematics_Quat2Mat(ITP_Orient *q, ITP_Pos *pPosition,double m[4][4]);


#if defined(__cplusplus)
}
#endif 

#endif

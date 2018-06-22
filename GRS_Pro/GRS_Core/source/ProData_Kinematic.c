


#include "ProData_Kinematic.h"
#include <math.h>
#include <string.h>

int Kinematics_Quat2Mat(ITP_Orient *q, ITP_Pos *pPosition,double m[4][4])
{//ITP_Orient必须时单位四元组………………………………………………………………
	/* from space book where e1=q->x e2=q->y e3=q->z e4=q->s */
	double det;
	m[0][0] = 1 - 2 * (ITP_Square(q->y) + ITP_Square(q->z));
	m[0][1] = 2 * (q->x * q->y - q->z * q->s);
	m[0][2] = 2 * (q->z * q->x + q->y * q->s);
	m[0][3]	= pPosition->x;

	m[1][0] = 2 * (q->x * q->y + q->z * q->s);
	m[1][1] = 1 - 2 * (ITP_Square(q->z) + ITP_Square(q->x));
	m[1][2] = 2 * (q->y * q->z - q->x * q->s);
	m[1][3]	= pPosition->y;

	m[2][0] = 2 * (q->z * q->x - q->y * q->s);
	m[2][1] = 2 * (q->y * q->z + q->x * q->s);
	m[2][2] = 1 - 2 * (ITP_Square(q->x) + ITP_Square(q->y));
	m[2][3]	= pPosition->z;

	m[3][0]	= 0;
	m[3][1]	= 0;
	m[3][2]	= 0;
	m[3][3]	= 1;

	//det=KinematicsPuma_Mat33_Det(m);


	return 1;
}

void Kinematic_CreateRotationMatrix(double mat[4][4],double a,double alpha,double d,double theta)
{
	int i=0,j=0;

	//角度转弧度
	double alpha1=alpha;
	double theta1=theta;			//已经是弧度

	double costhera=cos(theta1);
	double cosalpha=cos(alpha1);
	double sinthera=sin(theta1);
	double sinalpha=sin(alpha1);
	if (ITP_ABS(costhera)<ITP_Zero)
	{
		costhera=0;
	}
	if (ITP_ABS(cosalpha)<ITP_Zero)
	{
		cosalpha=0;
	}
	if (ITP_ABS(sinthera)<ITP_Zero)
	{
		sinthera=0;
	}
	if (ITP_ABS(sinalpha)<ITP_Zero)
	{
		sinalpha=0;
	}
	mat[i++][j]=costhera;
	mat[i++][j]=sinthera*cosalpha;
	mat[i++][j]=sinthera*sinalpha;
	mat[i++][j]=0;

	i=0;
	j++;
	mat[i++][j]=-sinthera;
	mat[i++][j]=costhera*cosalpha;
	mat[i++][j]=costhera*sinalpha;
	mat[i++][j]=0;

	i=0;
	j++;
	mat[i++][j]=0;
	mat[i++][j]=-sinalpha;
	mat[i++][j]=cosalpha;
	mat[i++][j]=0;

	i=0;
	j++;
	mat[i++][j]=a;
	mat[i++][j]=-d*sinalpha;
	mat[i++][j]=d*cosalpha;
	mat[i++][j]=1;
}




int Kinematic__mat4_inv(double a[4][4],double ainv[4][4])
{
	double denom;

	double a11a22,a11a23,a11a24,a11a33,a11a32,a11a34;
	double a12a21,a12a23,a12a24,a12a31,a12a33,a12a34;
	double a13a21,a13a22,a13a24,a13a31,a13a32,a13a34;
	double a14a21,a14a22,a14a23,a14a31,a14a32,a14a33;
	double a21a32,a21a33,a21a34;
	double a22a31,a22a33,a22a34;
	double a23a31,a23a32,a23a34;
	double a24a31,a24a32,a24a33;
	double a11a22a33,a11a22a34,a11a23a32,a11a23a34,a11a24a32,a11a24a33;
	double a12a21a33,a12a21a34,a12a23a31,a12a23a34,a12a24a31,a12a24a33;
	double a13a21a32,a13a21a34,a13a22a31,a13a22a34,a13a24a31,a13a24a32;
	double a14a21a32,a14a21a33,a14a22a31,a14a22a33,a14a23a31,a14a23a32;

	a11a22 = a[0][0]*a[1][1];
	a11a23 = a[0][0]*a[1][2];
	a11a24 = a[0][0]*a[1][3];
	a11a33 = a[0][0]*a[2][2];
	a11a32 = a[0][0]*a[2][1];
	a11a34 = a[0][0]*a[2][3];

	a12a21 = a[0][1]*a[1][0];
	a12a23 = a[0][1]*a[1][2];
	a12a24 = a[0][1]*a[1][3];
	a12a31 = a[0][1]*a[2][0];
	a12a33 = a[0][1]*a[2][2];
	a12a34 = a[0][1]*a[2][3];

	a13a21 = a[0][2]*a[1][0];
	a13a22 = a[0][2]*a[1][1];
	a13a24 = a[0][2]*a[1][3];
	a13a31 = a[0][2]*a[2][0];
	a13a32 = a[0][2]*a[2][1];
	a13a34 = a[0][2]*a[2][3];

	a14a21 = a[0][3]*a[1][0];
	a14a22 = a[0][3]*a[1][1];
	a14a23 = a[0][3]*a[1][2];
	a14a31 = a[0][3]*a[2][0];
	a14a32 = a[0][3]*a[2][1];
	a14a33 = a[0][3]*a[2][2];

	a21a32 = a[1][0]*a[2][1];
	a21a33 = a[1][0]*a[2][2];
	a21a34 = a[1][0]*a[2][3];

	a22a31 = a[1][1]*a[2][0];
	a22a33 = a[1][1]*a[2][2];
	a22a34 = a[1][1]*a[2][3];

	a23a31 = a[1][2]*a[2][0];
	a23a32 = a[1][2]*a[2][1];
	a23a34 = a[1][2]*a[2][3];

	a24a31 = a[1][3]*a[2][0];
	a24a32 = a[1][3]*a[2][1];
	a24a33 = a[1][3]*a[2][2];

	a11a22a33 = a11a22*a[2][2];
	a11a22a34 = a11a22*a[2][3];
	a11a23a32 = a11a23*a[2][1];
	a11a23a34 = a11a23*a[2][3];
	a11a24a32 = a11a24*a[2][1];
	a11a24a33 = a11a24*a[2][2];
	a12a21a33 = a12a21*a[2][2];
	a12a21a34 = a12a21*a[2][3];
	a12a23a31 = a12a23*a[2][0];
	a12a23a34 = a12a23*a[2][3];
	a12a24a31 = a12a24*a[2][0];
	a12a24a33 = a12a24*a[2][2];
	a13a21a32 = a13a21*a[2][1];
	a13a21a34 = a13a21*a[2][3];
	a13a22a31 = a13a22*a[2][0];
	a13a22a34 = a13a22*a[2][3];
	a13a24a31 = a13a24*a[2][0];
	a13a24a32 = a13a24*a[2][1];
	a14a21a32 = a14a21*a[2][1];
	a14a21a33 = a14a21*a[2][2];
	a14a22a31 = a14a22*a[2][0];
	a14a22a33 = a14a22*a[2][2];
	a14a23a31 = a14a23*a[2][0];
	a14a23a32 = a14a23*a[2][1];

	denom =				//求行列式
		+ a12a24a33*a[3][0]
	+ a13a24a31*a[3][1]
	- a12a21a33*a[3][3]
	- a13a24a32*a[3][0]
	- a14a21a32*a[3][2]
	- a12a24a31*a[3][2]
	+ a14a23a32*a[3][0]
	+ a13a21a32*a[3][3]
	+ a14a21a33*a[3][1]
	+ a12a23a31*a[3][3]
	- a14a23a31*a[3][1]
	+ a12a21a34*a[3][2]
	- a12a23a34*a[3][0]
	- a13a21a34*a[3][1]
	- a13a22a31*a[3][3]
	+ a14a22a31*a[3][2]
	+ a13a22a34*a[3][0]
	- a14a22a33*a[3][0]
	+ a11a22a33*a[3][3]
	- a11a22a34*a[3][2]
	+ a11a24a32*a[3][2]
	- a11a23a32*a[3][3] 
	+ a11a23a34*a[3][1]
	- a11a24a33*a[3][1];

	if (denom<ITP_Zero)
	{
		return -1;
	}

	denom = 1.0/denom;

	ainv[0][0] = (+ a22a33*a[3][3]
	- a22a34*a[3][2]
	+ a24a32*a[3][2]
	- a23a32*a[3][3]
	+ a23a34*a[3][1]
	- a24a33*a[3][1])*denom;
	ainv[0][1] = (- a12a33*a[3][3]
	+ a12a34*a[3][2]
	- a13a34*a[3][1]
	+ a13a32*a[3][3]
	- a14a32*a[3][2]
	+ a14a33*a[3][1])*denom;
	ainv[0][2] = (+ a12a23*a[3][3]
	- a12a24*a[3][2]
	- a13a22*a[3][3]
	+ a13a24*a[3][1]
	+ a14a22*a[3][2]
	- a14a23*a[3][1])*denom;
	ainv[0][3] = (- a12a23a34
		+ a12a24a33
		+ a13a22a34
		- a13a24a32
		- a14a22a33
		+ a14a23a32)*denom;

	ainv[1][0] = (+ a23a31*a[3][3]
	- a23a34*a[3][0]
	- a21a33*a[3][3]
	+ a21a34*a[3][2]
	+ a24a33*a[3][0]
	- a24a31*a[3][2])*denom;
	ainv[1][1] = (+ a11a33*a[3][3]
	- a11a34*a[3][2]
	- a13a31*a[3][3]
	+ a14a31*a[3][2]
	+ a13a34*a[3][0]
	- a14a33*a[3][0])*denom;
	ainv[1][2] = (- a11a23*a[3][3]
	+ a11a24*a[3][2]
	+ a13a21*a[3][3]
	+ a14a23*a[3][0]
	- a14a21*a[3][2]
	- a13a24*a[3][0])*denom;
	ainv[1][3] = (+ a11a23a34
		- a11a24a33
		- a13a21a34
		+ a14a21a33
		- a14a23a31
		+ a13a24a31)*denom;

	ainv[2][0] = (+ a22a34*a[3][0]
	- a22a31*a[3][3]
	- a24a32*a[3][0]
	+ a21a32*a[3][3]
	- a21a34*a[3][1]
	+ a24a31*a[3][1])*denom;
	ainv[2][1] = (+ a11a34*a[3][1]
	- a11a32*a[3][3]
	- a12a34*a[3][0]
	- a14a31*a[3][1]
	+ a12a31*a[3][3]
	+ a14a32*a[3][0])*denom;
	ainv[2][2] = (- a11a24*a[3][1]
	- a14a22*a[3][0]
	+ a11a22*a[3][3]
	+ a12a24*a[3][0]
	+ a14a21*a[3][1]
	- a12a21*a[3][3])*denom;
	ainv[2][3] = (- a11a22a34
		+ a11a24a32
		+ a14a22a31
		- a12a24a31
		+ a12a21a34
		- a14a21a32)*denom;

	ainv[3][0] = (+ a21a33*a[3][1]
	+ a23a32*a[3][0]
	- a22a33*a[3][0]
	- a23a31*a[3][1]
	+ a22a31*a[3][2]
	- a21a32*a[3][2])*denom;
	ainv[3][1] = (+ a12a33*a[3][0]
	- a13a32*a[3][0]
	- a12a31*a[3][2]
	+ a13a31*a[3][1]
	+ a11a32*a[3][2]
	- a11a33*a[3][1])*denom;
	ainv[3][2] = (- a12a23*a[3][0]
	+ a12a21*a[3][2]
	- a13a21*a[3][1]
	+ a13a22*a[3][0]
	- a11a22*a[3][2]
	+ a11a23*a[3][1])*denom;
	ainv[3][3] = (+ a13a21a32
		+ a12a23a31
		- a12a21a33
		- a13a22a31
		+ a11a22a33
		- a11a23a32)*denom;

	return 1;
}

void Kinematic_MultiplyMatrix(double matOutput[4][4],double matLeft[4][4],double matRight[4][4])
{
	int i,j;
	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			matOutput[i][j]=matLeft[i][0]*matRight[0][j] 
			+ matLeft[i][1]*matRight[1][j]
			+ matLeft[i][2]*matRight[2][j]
			+ matLeft[i][3]*matRight[3][j];
		}
	}
}


int Kinematics_XYZToQuat(double *pXYZ,ITP_Orient *quat)
{
	double m[4][4];
	Kinematics_XYZToMat(pXYZ,m);
	Kinematics_MatToQuat(m,quat);
}


int Kinematics_Mat2xyz(double m[4][4], ITP_EulerXYZ *Exyz)//……………………待检验…………………………………………
{
	Exyz->ey = atan2(-m[2][0], sqrt(ITP_Square(m[0][0]) + ITP_Square(m[1][0])));

	if (fabs(Exyz->ey-ITP_HalfPI)<ITP_Zero) {
		Exyz->ex = atan2(m[0][1], m[1][1]);
		Exyz->ey = ITP_HalfPI;		/* force it */
		Exyz->ez = 0;
	} else if (fabs(Exyz->ey + ITP_HalfPI) < ITP_Zero) {
		Exyz->ex = -atan2(m[2][1], m[1][1]);
		Exyz->ey = -ITP_HalfPI;		/* force it */
		Exyz->ez = 0;
	} else {
		Exyz->ex = atan2(m[2][1], m[2][2]);
		Exyz->ez = atan2(m[1][0], m[0][0]);
	}
	return 1;
}


//矩阵的最后一行一列无用
int Kinematics_XYZToMat(double *pXYZ, double m[4][4])
{
	double sa, sb, sg;
	double ca, cb, cg;

	sa = sin(pXYZ[2]);
	sb = sin(pXYZ[1]);
	sg = sin(pXYZ[0]);

	ca = cos(pXYZ[2]);
	cb = cos(pXYZ[1]);
	cg = cos(pXYZ[0]);

	m[0][0] = ca * cb;
	m[0][1] = ca * sb * sg - sa * cg;
	m[0][2] = ca * sb * cg + sa * sg;

	m[1][0] = sa * cb;
	m[1][1] = sa * sb * sg + ca * cg;
	m[1][2] = sa * sb * cg - ca * sg;

	m[2][0] = -sb;
	m[2][1] = cb * sg;
	m[2][2] = cb * cg;

	return 1;
}


int KinematicsPuma_QuatNorm(ITP_Orient *quat)
{
	double size = sqrt(ITP_Square(quat->s) + ITP_Square(quat->x) + ITP_Square(quat->y) + ITP_Square(quat->z));

	if (size == 0.0) 
	{
		quat->s = 1;
		quat->x = 0;
		quat->y = 0;
		quat->z = 0;
		return 0;
	}

	if (quat->s >= 0.0) 
	{
		quat->s = quat->s / size;
		quat->x = quat->x / size;
		quat->y = quat->y / size;
		quat->z = quat->z / size;


	} 
	else 
	{
		quat->s = -quat->s / size;
		quat->x = -quat->x / size;
		quat->y = -quat->y / size;
		quat->z = -quat->z / size;

	}
	return 1;
}


int Kinematics_MatToQuat(double m[4][4], ITP_Orient *q)
{
	double discr;
	double a;

	//判断矩阵横列式是否为+1

	discr = 1.0 + m[0][0] + m[1][1] + m[2][2];
	if (discr < 0.0) discr = 0.0;	/* give sqrt some slack for tiny negs */

	q->s = 0.5 * sqrt(discr);

	if (ITP_ABS(q->s)<ITP_Zero) 
	{
		q->s = 0;
		discr = 1.0 + m[0][0] - m[1][1] - m[2][2];
		if (discr < 0.0) 
			discr = 0.0;
		q->x = sqrt(discr) / 2.0;
		discr = 1.0 + m[1][1] - m[0][0] - m[2][2];
		if (discr < 0.0) discr = 0.0;
		q->y = sqrt(discr) / 2.0;
		discr = 1.0 + m[2][2] - m[1][1] - m[0][0];
		if (discr < 0.0) discr = 0.0;
		q->z = sqrt(discr) / 2.0;


		if (q->x > q->y && q->x > q->z) {
			if (m[1][0] < 0.0) {
				q->y *= -1;
			}
			if (m[2][0] < 0.0) {
				q->z *= -1;
			}
		} else if (q->y > q->z) {
			if (m[1][0] < 0.0) {
				q->x *= -1;
			}
			if (m[2][1] < 0.0) {
				q->z *= -1;
			}
		} else {
			if (m[2][0] < 0.0) {
				q->x *= -1;
			}
			if (m[2][1] < 0.0) {
				q->y *= -1;
			}
		}
	} else {
		q->x = (m[2][1] - m[1][2]) / (a = 4 * q->s);
		q->y = (m[0][2] - m[2][0]) / a;
		q->z = (m[1][0] - m[0][1]) / a;
	}
	return 1;
}

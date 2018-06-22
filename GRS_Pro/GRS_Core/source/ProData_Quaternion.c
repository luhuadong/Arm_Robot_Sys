

#include "ProData_Quaternion.h"
#include <math.h>

//以进给速度变化（四元组）通过v_ort（degree/s）来控制旋转
//四元组的性质与矩阵 左右乘的性质一样

//pPoint为局部坐标点，pOutputPoint原始坐标点
void ITPQuaternion_RotatePoint(ITP_Orient *pQuat,ITP_Pos *pPoint,ITP_Pos *pOutputPoint)
{
	ITP_Pos c;
	//ITP_Orient temp,temp1;
	//ITP_Orient quatInv;
	//ITP_Orient quatOutput;
	//temp.s=0;
	//temp.x=pPoint->x;
	//temp.y=pPoint->y;
	//temp.z=pPoint->z;
	//ITPQuaternion_Inv(pQuat,&quatInv);
	//ITPQuaternion_Mult(&quatInv,&temp,&temp1);
	//ITPQuaternion_Mult(&temp1,pQuat,&quatOutput);
	////ITPQuaternion_Mult(pQuat,&temp,&temp1);
	////ITPQuaternion_Mult(&temp1,&quatInv,&quatOutput);

	//pOutputPoint->x=quatOutput.x;
	//pOutputPoint->y=quatOutput.y;
	//pOutputPoint->z=quatOutput.z;

	
	c.x = pQuat->y * pPoint->z - pQuat->z * pPoint->y;
	c.y = pQuat->z * pPoint->x - pQuat->x * pPoint->z;
	c.z = pQuat->x * pPoint->y - pQuat->y * pPoint->x;

	pOutputPoint->x = pPoint->x + 2 * (pQuat->s * c.x + pQuat->y * c.z - pQuat->z * c.y);
	pOutputPoint->y = pPoint->y + 2 * (pQuat->s * c.y + pQuat->z * c.x - pQuat->x * c.z);
	pOutputPoint->z = pPoint->z + 2 * (pQuat->s * c.z + pQuat->x * c.y - pQuat->y * c.x);

}

int ITPQuaternion_Inv(ITP_Orient *pQuat,ITP_Orient *pOutQuat)//求逆
{//注：输入的pQuat需为单位四元组
	pOutQuat->s = pQuat->s;
	pOutQuat->x = -pQuat->x;
	pOutQuat->y = -pQuat->y;
	pOutQuat->z = -pQuat->z;
	return 1;
}

int ITPQuaternion_Mult(ITP_Orient *pLeftQuat,ITP_Orient *pRightQuat,ITP_Orient *pOutQuat)//求积
{
	ITP_Orient leftQuat,rightQuat;
	leftQuat=*pLeftQuat;
	rightQuat=*pRightQuat;
	pOutQuat->s = leftQuat.s * rightQuat.s - leftQuat.x * rightQuat.x - leftQuat.y * rightQuat.y - leftQuat.z * rightQuat.z;

	if (pOutQuat->s >= 0) 
	{
		pOutQuat->x = leftQuat.s * rightQuat.x + leftQuat.x * rightQuat.s + leftQuat.y * rightQuat.z - leftQuat.z * rightQuat.y;
		pOutQuat->y = leftQuat.s * rightQuat.y - leftQuat.x * rightQuat.z + leftQuat.y * rightQuat.s + leftQuat.z * rightQuat.x;
		pOutQuat->z = leftQuat.s * rightQuat.z + leftQuat.x * rightQuat.y - leftQuat.y * rightQuat.x + leftQuat.z * rightQuat.s;
	} 
	else 
	{
		pOutQuat->s = -pOutQuat->s;
		pOutQuat->x = -leftQuat.s * rightQuat.x - leftQuat.x * rightQuat.s - leftQuat.y * rightQuat.z + leftQuat.z * rightQuat.y;
		pOutQuat->y = -leftQuat.s * rightQuat.y + leftQuat.x * rightQuat.z - leftQuat.y * rightQuat.s - leftQuat.z * rightQuat.x;
		pOutQuat->z = -leftQuat.s * rightQuat.z - leftQuat.x * rightQuat.y + leftQuat.y * rightQuat.x - leftQuat.z * rightQuat.s;
	}
	return 1;
}

void ITPQuaternion_CreateQuat(double angle,ITP_Vecter *pVecter,ITP_Orient *pOutQuat)
{
	double angle_half=angle/2,sa=sin(angle_half);
	pOutQuat->s=cos(angle_half);
	pOutQuat->x=pVecter->vx*sa;
	pOutQuat->y=pVecter->vy*sa;
	pOutQuat->z=pVecter->vz*sa;
}

//angle旋转角度，pVecter旋转方向(应为单位向量)，pQuat初始四元组，pOutQuat输出四元组
int ITPQuaternion_Rotate(double angle,ITP_Vecter *pVecter,ITP_Orient *pQuat,ITP_Orient *pOutQuat)//求旋转后的四元组
{
	ITP_Orient leftQuat,invQuat;
	ITP_Orient temp;

	ITPQuaternion_CreateQuat(angle,pVecter,&leftQuat);
	//ITPQuaternion_Inv(&leftQuat,&invQuat);//求逆

	ITPQuaternion_Mult(&leftQuat,pQuat,pOutQuat);
	//ITPQuaternion_Mult(&temp,&invQuat,pOutQuat);
}



double ITPQuaternion_DotMult(ITP_Orient *q1,ITP_Orient *q2)//点乘
{
	return (q1->s*q2->s+q1->x*q2->x+q1->y*q2->y+q1->z*q2->z);
}

double ITPQuaternion_GetCos(ITP_Orient *q1,ITP_Orient *q2)//点乘
{
	double dot;
	double L1=sqrt(q1->s*q1->s+q1->x*q1->x+q1->y*q1->y+q1->z*q1->z);
	double L2=sqrt(q2->s*q2->s+q2->x*q2->x+q2->y*q2->y+q2->z*q2->z);
	dot=ITPQuaternion_DotMult(q1,q2);
	return dot/(L1*L2);
}

void ITPQuaternion_Normalize(ITP_Orient *pQuat,ITP_Orient *pOutQuat)
{
	double size;
	size = sqrt(ITP_Square(pQuat->s) + ITP_Square(pQuat->x) + ITP_Square(pQuat->y) + ITP_Square(pQuat->z));
	if (ITP_ABS(size-1)<ITP_Zero)
	{
		*pOutQuat=*pQuat;
		return;
	}
	if (size<ITP_Zero) 
	{
		pOutQuat->s = 1;
		pOutQuat->x = 0;
		pOutQuat->y = 0;
		pOutQuat->z = 0;
		return;
	}
	size = 1.0 / size;
	if (pQuat->s >= 0) 
	{
		pOutQuat->s = pQuat->s * size;
		pOutQuat->x = pQuat->x * size;
		pOutQuat->y = pQuat->y * size;
		pOutQuat->z = pQuat->z * size;
	} 
	else 
	{
		pOutQuat->s = -pQuat->s * size;
		pOutQuat->x = -pQuat->x * size;
		pOutQuat->y = -pQuat->y * size;
		pOutQuat->z = -pQuat->z * size;
	}
}

void ITPQuaternion_Add(ITP_Orient *pQuat1,ITP_Orient *pQuat2,ITP_Orient *pOutQuat)
{
	pOutQuat->s=pQuat1->s+pQuat2->s;
	pOutQuat->x=pQuat1->x+pQuat2->x;
	pOutQuat->y=pQuat1->y+pQuat2->y;
	pOutQuat->z=pQuat1->z+pQuat2->z;
}

void ITPQuaternion_ConstMult(double val,ITP_Orient *pOutQuat)
{
	pOutQuat->s=val*pOutQuat->s;
	pOutQuat->x=val*pOutQuat->x;
	pOutQuat->y=val*pOutQuat->y;
	pOutQuat->z=val*pOutQuat->z;
}


double ITPQuaternion_GetCornerOfQuat(ITP_Orient *pQuat1,ITP_Orient *pQuat2)
{
	double dCos=ITPQuaternion_GetCos(pQuat1,pQuat2);
	return acos(dCos);
}

//时间分割来进行插补	//flagDirection表示旋转方向,//flagDirection表示角度的方向,1表示输出大角度，0表示输出小角度
int ITPQuaternion_InterpolateTime(double dt,int flagDirection,ITP_Orient *pQuat1,ITP_Orient *pQuat2,ITP_Orient *pOutQuat)
{
	double dCos=ITPQuaternion_GetCos(pQuat1,pQuat2);//cos的值-1～1(好像可以表示0-180)
	double dSin,dAngle,dCoeff0,dCoeff1,temp;
	ITP_Orient quat2=*pQuat2,quat1=*pQuat1;

	//double temp1=acos(dCos)*180.0/ITP_PI;
	//if(flagDirection)
	if(dCos<0)//………………………………………………………………待验证………………………………
	{
		dCos=-dCos;
		ITPQuaternion_ConstMult(-1,&quat2);
	}	

	if(abs(dCos)< (1 - 1e-6))//表示在接近0度或180度时采用线性插值
	{
		dSin=sqrt(1-dCos*dCos);
		dAngle=atan2(dSin,dCos);//这样写后面的结果为1，无需将结果单位化
		temp=1/dSin;
		dCoeff0=sin((1-dt)*dAngle)*temp;
		dCoeff1=sin(dt*dAngle)*temp;
		ITPQuaternion_ConstMult(dCoeff0,&quat1);
		ITPQuaternion_ConstMult(dCoeff1,&quat2);
		ITPQuaternion_Add(&quat1,&quat2,pOutQuat);
	}
	else//采用线性插值
	{
		ITPQuaternion_ConstMult(1.0 - dt,&quat1);
		ITPQuaternion_ConstMult(dt,&quat2);
		ITPQuaternion_Add(&quat1,&quat2,pOutQuat);
	}

	ITPQuaternion_Normalize(pOutQuat,pOutQuat);
	return 1;
}

int ITPQuaternion_InterpolateTime_Line(double dt,int flagDirection,ITP_Orient *pQuat1,ITP_Orient *pQuat2,ITP_Orient *pOutQuat)
{
	ITP_Orient quat2=*pQuat2,quat1=*pQuat1;


	if(flagDirection)
	{
		ITPQuaternion_ConstMult(-1,&quat2);
	}

	ITPQuaternion_ConstMult(1.0 - dt,&quat1);
	ITPQuaternion_ConstMult(dt,&quat2);
	ITPQuaternion_Add(&quat1,&quat2,pOutQuat);
	
	ITPQuaternion_Normalize(pOutQuat,pOutQuat);
	return 1;
}



//注：似乎没必要进行以下计算，可以通过求总夹角，后通过需要进给的夹角直接推算时间t。
//问题的关键是，实际方向夹角和quat夹角的对应关系!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//double tempT=0;
////进给角度来进行插补
//int ITPQuaternion_InterpolateAngle(double angle,int flagDirection,ITP_Orient *pQuat1,ITP_Orient *pQuat2,ITP_Orient *pOutQuat)
//{
//	double f0,f1,f2,f3,f4,f5;
//	double ca=cos(angle),ca_2=ca*ca,t=0,t1,t2;
//	double q1s=pQuat1->s,q1x=pQuat1->x,q1y=pQuat1->y,q1z=pQuat1->z;
//	double q2s=pQuat2->s,q2x=pQuat2->x,q2y=pQuat2->y,q2z=pQuat2->z;
//	double q1s_2=q1s*q1s,q1x_2=q1x*q1x,q1y_2=q1y*q1y,q1z_2=q1z*q1z;
//	double q2s_2=q2s*q2s,q2x_2=q2x*q2x,q2y_2=q2y*q2y,q2z_2=q2z*q2z;
//	double q1s_4=q1s_2*q1s_2,q1x_4=q1x_2*q1x_2,q1y_4=q1y_2*q1y_2,q1z_4=q1z_2*q1z_2;
//	double q1s_3=q1s_2*q1s,q1x_3=q1x_2*q1x,q1y_3=q1y_2*q1y,q1z_3=q1z_2*q1z;
//	//double q2s_4=q2s_2*q2s_2,q2x_4=q2x_2*q2x_2,q2y_4=q2y_2*q2y_2,q2z_4=q2z_2*q2z_2;
//
//
//	f0=2*q1s_2*q1x_2+2*q1s_2*q1y_2+2*q1s_2*q1z_2+q1y_4+q1z_4+2*q1x_2*q1y_2+2*q1x_2*q1z_2+2*q1y_2*q1z_2+q1x_4+q1s_4;
//	f1=2*q1s_2*q1x*q2x-2*q1s_4-2*q1x_4-2*q1y_4-2*q1z_4-4*q1s_2*q1x_2-4*q1s_2*q1y_2-4*q1s_2*q1z_2-4*q1x_2*q1y_2-4*q1x_2*q1z_2-4*q1y_2*q1z_2+2*q1s_3*q2s+2*q1x_3*q2x+2*q1y_3*q2y+2*q1z_3*q2z+2*q1s_2*q1y*q2y+2*q1s_2*q1z*q2z+2*q1s*q2s*q1x_2+2*q1s*q2s*q1y_2+2*q1s*q2s*q1z_2+2*q1x_2*q1y*q2y+2*q1x_2*q1z*q2z+2*q1x*q2x*q1y_2+2*q1x*q2x*q1z_2+2*q1y_2*q1z*q2z+2*q1y*q2y*q1z_2;
//	f2=-2*q1s_2*q1x*q2x+q1s_4+q1x_4+q1y_4+q1z_4+2*q1s_2*q1x_2+2*q1s_2*q1y_2+2*q1s_2*q1z_2+2*q1x_2*q1y_2+2*q1x_2*q1z_2+2*q1y_2*q1z_2-2*q1s_3*q2s+q1s_2*q2s_2-2*q1x_3*q2x+q1x_2*q2x_2-2*q1y_3*q2y+q1y_2*q2y_2-2*q1z_3*q2z+q1z_2*q2z_2+2*q1s*q2s*q1x*q2x-2*q1s_2*q1y*q2y-2*q1s_2*q1z*q2z-2*q1s*q2s*q1x_2-2*q1s*q2s*q1y_2-2*q1s*q2s*q1z_2-2*q1x_2*q1y*q2y-2*q1x_2*q1z*q2z-2*q1x*q2x*q1y_2-2*q1x*q2x*q1z_2-2*q1y_2*q1z*q2z-2*q1y*q2y*q1z_2+2*q1s*q2s*q1y*q2y+2*q1s*q2s*q1z*q2z+2*q1x*q2x*q1y*q2y+2*q1x*q2x*q1z*q2z+2*q1y*q2y*q1z*q2z;
//
//	f3=2*ca_2*q1s_2*q1z_2+ca_2*q1x_4+ca_2*q1y_4+ca_2*q1z_4+2*ca_2*q1y_2*q1z_2+ca_2*q1s_4+2*ca_2*q1x_2*q1z_2+2*ca_2*q1x_2*q1y_2+2*ca_2*q1s_2*q1x_2+2*ca_2*q1s_2*q1y_2;
//	f4=-2*ca_2*q1x_4-2*ca_2*q1y_4-2*ca_2*q1z_4-2*ca_2*q1s_4-4*ca_2*q1s_2*q1x_2-4*ca_2*q1s_2*q1y_2-4*ca_2*q1s_2*q1z_2-4*ca_2*q1x_2*q1y_2-4*ca_2*q1x_2*q1z_2-4*ca_2*q1y_2*q1z_2+2*ca_2*q1s_2*q1x*q2x+2*ca_2*q1s_2*q1y*q2y+2*ca_2*q1s_3*q2s+2*ca_2*q1x_3*q2x+2*ca_2*q1y_3*q2y+2*ca_2*q1z_3*q2z+2*ca_2*q1s_2*q1z*q2z+2*ca_2*q1s*q2s*q1x_2+2*ca_2*q1s*q2s*q1y_2+2*ca_2*q1s*q2s*q1z_2+2*ca_2*q1x_2*q1y*q2y+2*ca_2*q1x_2*q1z*q2z+2*ca_2*q1x*q2x*q1y_2+2*ca_2*q1x*q2x*q1z_2+2*ca_2*q1y_2*q1z*q2z+2*ca_2*q1y*q2y*q1z_2;
//	f5=ca_2*q1x_4+ca_2*q1y_4+ca_2*q1z_4+ca_2*q1s_4+2*ca_2*q1s_2*q1x_2+2*ca_2*q1s_2*q1y_2+2*ca_2*q1s_2*q1z_2+2*ca_2*q1x_2*q1y_2+2*ca_2*q1x_2*q1z_2+2*ca_2*q1y_2*q1z_2-2*ca_2*q1s_2*q1x*q2x-2*ca_2*q1s_2*q1y*q2y-2*ca_2*q1s_3*q2s+ca_2*q1s_2*q2s_2-2*ca_2*q1x_3*q2x+ca_2*q1x_2*q2x_2-2*ca_2*q1y_3*q2y+ca_2*q1y_2*q2y_2-2*ca_2*q1z_3*q2z+ca_2*q1z_2*q2z_2+ca_2*q2s_2*q1z_2+ca_2*q2s_2*q1x_2+ca_2*q2s_2*q1y_2+ca_2*q2x_2*q1s_2+ca_2*q2x_2*q1z_2+ca_2*q2x_2*q1y_2+ca_2*q2y_2*q1s_2+ca_2*q2y_2*q1z_2+ca_2*q2y_2*q1x_2+ca_2*q2z_2*q1s_2+ca_2*q2z_2*q1x_2+ca_2*q2z_2*q1y_2-2*ca_2*q1s_2*q1z*q2z-2*ca_2*q1s*q2s*q1x_2-2*ca_2*q1s*q2s*q1y_2-2*ca_2*q1s*q2s*q1z_2-2*ca_2*q1x_2*q1y*q2y-2*ca_2*q1x_2*q1z*q2z-2*ca_2*q1x*q2x*q1y_2-2*ca_2*q1x*q2x*q1z_2-2*ca_2*q1y_2*q1z*q2z-2*ca_2*q1y*q2y*q1z_2;
//
//
//	t1= 0.5/(f2-f5)*(-f1+f4+sqrt(f1*f1-2*f1*f4+f4*f4-4*f2*f0+4*f2*f3+4*f5*f0-4*f5*f3));
//	t2= 0.5/(f2-f5)*(-f1+f4-sqrt(f1*f1-2*f1*f4+f4*f4-4*f2*f0+4*f2*f3+4*f5*f0-4*f5*f3));
//	if (t1<0 || t1>1 || t1<tempT)
//	{
//		t=t2;
//	}
//	if (t2<0 || t2>1 || t2<tempT)
//	{
//		t=t1;
//	}
//	if (t==0)//取小值
//	{
//		if (t1<t2)
//		{
//			t=t1;
//		}
//		else
//			t=t2;
//	}
//	tempT=t;
//	ITPQuaternion_InterpolateTime_Line(t,flagDirection,pQuat1,pQuat2,pOutQuat);
//}





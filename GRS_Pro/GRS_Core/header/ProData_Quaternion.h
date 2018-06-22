
#ifndef ITP_Quaternion_H
#define ITP_Quaternion_H


//四元组运算，插补/插值
//#include "ITP_Interface.h"
#include "ITP_Interface.h"



#if defined(__cplusplus)
extern "C"
{
#endif



int ITPQuaternion_Inv(ITP_Orient *pQuat,ITP_Orient *pOutQuat);//求逆

void ITPQuaternion_RotatePoint(ITP_Orient *pQuat,ITP_Pos *pPoint,ITP_Pos *pOutputPoint);

void ITPQuaternion_Normalize(ITP_Orient *pQuat,ITP_Orient *pOutQuat);

int ITPQuaternion_Mult(ITP_Orient *pLeftQuat,ITP_Orient *pRightQuat,ITP_Orient *pOutQuat);//求积

void ITPQuaternion_CreateQuat(double angle,ITP_Vecter *pVecter,ITP_Orient *pOutQuat);

//angle旋转角度，pVecter旋转方向，pQuat初始坐标（注：s应该为0），pOutQuat输出四元组
int ITPQuaternion_Rotate(double angle,ITP_Vecter *pVecter,ITP_Orient *pQuat,ITP_Orient *pOutQuat);//求旋转后的四元组

double ITPQuaternion_GetCornerOfQuat(ITP_Orient *pQuat1,ITP_Orient *pQuat2);
int ITPQuaternion_InterpolateTime(double dt,int flagShortestPath,ITP_Orient *pQuat1,ITP_Orient *pQuat2,ITP_Orient *pOutQuat);
//int ITPQuaternion_InterpolateAngle(double angle,int flagShortestPath,ITP_Orient *pQuat1,ITP_Orient *pQuat2,ITP_Orient *pOutQuat);
#if defined(__cplusplus)
}
#endif 

#endif

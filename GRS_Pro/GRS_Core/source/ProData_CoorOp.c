

#include "ProData_CoorOp.h"

#include "ProData_Quaternion.h"
#include "ProData_Kinematic.h"
#include <math.h>


void DataManage_PosNormalize(ITP_Pos *pPos,ITP_Pos *pOutPos)
{
	double size;
	size = sqrt(ITP_Square(pPos->x) + ITP_Square(pPos->y) + ITP_Square(pPos->z));
	if (ITP_ABS(size-1)<ITP_Zero)
	{
		*pOutPos=*pPos;
	}

	size = 1.0 / size;
	pOutPos->x = pPos->x * size;
	pOutPos->y = pPos->y * size;
	pOutPos->z = pPos->z * size;

}



//用户坐标系创建
//x1为原点坐标，x1到x2为x轴正方向，x1到y1为Y轴正方向(注意：基坐标系是哪个)
int ProData_CreateUserCoord(ITP_Pose *pPoseX1,ITP_Pose *pPoseX2,ITP_Pose *pPoseY1,ITP_Pose *pUserCoor)
{
	ITP_Pos posX2,posX1,posXRotate,posZ1,posZ,posY1,posZRotate;
	ITP_Orient rot1,rot2,rot;
	double angle,sinA;
	pUserCoor->trans=pPoseX1->trans;

	posX1.x=1;
	posX1.y=0;
	posX1.z=0;
	posX2.x=pPoseX2->trans.x-pPoseX1->trans.x;
	posX2.y=pPoseX2->trans.y-pPoseX1->trans.y;
	posX2.z=pPoseX2->trans.z-pPoseX1->trans.z;
	DataManage_PosNormalize(&posX2,&posX2);//单位化

	posXRotate.x=posX1.y*posX2.z-posX1.z*posX2.y;//posXRotate表示旋转轴向量
	posXRotate.y=posX1.z*posX2.x-posX1.x*posX2.z;
	posXRotate.z=posX1.x*posX2.y-posX1.y*posX2.x;
	angle=acos((posX1.x*posX2.x+posX1.y*posX2.y+posX1.z*posX2.z)/(sqrt(posX2.x*posX2.x+posX2.y*posX2.y+posX2.z*posX2.z)));

	//构建四元组（X轴）(旋转到X轴平行且同向)
	angle=angle/2;
	rot1.s=cos(angle);
	sinA=sin(angle);
	rot1.x=posXRotate.x*sinA;
	rot1.y=posXRotate.y*sinA;
	rot1.z=posXRotate.z*sinA;

	posZ1.x=0;
	posZ1.y=0;
	posZ1.z=1;//旋转后Z轴的坐标系

	//posZ为posZ1在原坐标系的坐标
	ITPQuaternion_RotatePoint(&rot1,&posZ1,&posZ);

	posY1.x=pPoseY1->trans.x-pPoseX1->trans.x;
	posY1.y=pPoseY1->trans.y-pPoseX1->trans.y;
	posY1.z=pPoseY1->trans.z-pPoseX1->trans.z;
	DataManage_PosNormalize(&posY1,&posY1);

	posZ1.x=posX2.y*posY1.z-posX2.z*posY1.y;//这里的posZ1表示工件坐标系Z在原始坐标系的向量
	posZ1.y=posX2.z*posY1.x-posX2.x*posY1.z;
	posZ1.z=posX2.x*posY1.y-posX2.y*posY1.x;
	DataManage_PosNormalize(&posZ1,&posZ1);

	posZRotate.x=posZ.y*posZ1.z-posZ.z*posZ1.y;
	posZRotate.y=posZ.z*posZ1.x-posZ.x*posZ1.z;
	posZRotate.z=posZ.x*posZ1.y-posZ.y*posZ1.x;
	angle=acos((posZ.x*posZ1.x+posZ.y*posZ1.y+posZ.z*posZ1.z)/(sqrt(posZ.x*posZ.x+posZ.y*posZ.y+posZ.z*posZ.z)*sqrt(posZ1.x*posZ1.x+posZ1.y*posZ1.y+posZ1.z*posZ1.z)));

	//构建四元组（Z轴）(旋转到Z轴平行且同向)
	angle=angle/2;
	rot2.s=cos(angle);
	sinA=sin(angle);
	rot2.x=posZRotate.x*sinA;
	rot2.y=posZRotate.y*sinA;
	rot2.z=posZRotate.z*sinA;

	ITPQuaternion_Mult(&rot2,&rot1,&rot);

	ITPQuaternion_Normalize(&rot,&rot);
	pUserCoor->rot=rot;
	return 1;
}


//x1为起点坐标，x1到x2为x轴正方向，x1到z1为Z轴正方向(注意：基坐标系是哪个)
int DataManage_CalToolQuaternion(ITP_Pose *pPoseP,ITP_Pose *pPoseZ1,ITP_Pose *pPoseX1,ITP_Pose *pToolCoor)
{
	ITP_Pos posZ2,posZ1,posZRotate,posX1,posY1,posY,posYRotate;
	ITP_Orient rot1,rot2,rot;
	double angle,sinA;

	if (pPoseZ1==0)
	{
		return 0;
	}
	posZ1.x=0;		//由Z开始
	posZ1.y=0;
	posZ1.z=1;
	posZ2.x=pPoseP->trans.x-pPoseZ1->trans.x;	
	posZ2.y=pPoseP->trans.y-pPoseZ1->trans.y;
	posZ2.z=pPoseP->trans.z-pPoseZ1->trans.z;
	//posZ2.x=pPoseZ1->trans.x-pPoseP->trans.x;	
	//posZ2.y=pPoseZ1->trans.y-pPoseP->trans.y;
	//posZ2.z=pPoseZ1->trans.z-pPoseP->trans.z;
	DataManage_PosNormalize(&posZ2,&posZ2);//单位化

	posZRotate.x=posZ1.y*posZ2.z-posZ1.z*posZ2.y;//posXRotate表示旋转轴向量
	posZRotate.y=posZ1.z*posZ2.x-posZ1.x*posZ2.z;
	posZRotate.z=posZ1.x*posZ2.y-posZ1.y*posZ2.x;
	angle=acos((posZ1.x*posZ2.x+posZ1.y*posZ2.y+posZ1.z*posZ2.z)/(sqrt(posZ2.x*posZ2.x+posZ2.y*posZ2.y+posZ2.z*posZ2.z)));

	//构建四元组（X轴）(旋转到X轴平行且同向)
	angle=angle/2;
	rot1.s=cos(angle);
	sinA=sin(angle);
	rot1.x=posZRotate.x*sinA;
	rot1.y=posZRotate.y*sinA;
	rot1.z=posZRotate.z*sinA;

	if (pPoseX1==0)
	{
		ITPQuaternion_Normalize(&rot1,&rot1);
		pToolCoor->rot=rot1;//相对与基坐标系的旋转四元组
	}
	else
	{
		posY1.x=0;
		posY1.y=1;
		posY1.z=0;//旋转后Y轴的坐标系
		ITPQuaternion_RotatePoint(&rot1,&posY1,&posY);

		posX1.x=pPoseP->trans.x-pPoseX1->trans.x;
		posX1.y=pPoseP->trans.y-pPoseX1->trans.y;
		posX1.z=pPoseP->trans.z-pPoseX1->trans.z;
		DataManage_PosNormalize(&posX1,&posX1);

		posY1.x=posZ2.y*posX1.z-posZ2.z*posX1.y;//这里的posZ1表示工件坐标系Z在原始坐标系的向量
		posY1.y=posZ2.z*posX1.x-posZ2.x*posX1.z;
		posY1.z=posZ2.x*posX1.y-posZ2.y*posX1.x;
		DataManage_PosNormalize(&posY1,&posY1);

		posYRotate.x=posY.y*posY1.z-posY.z*posY1.y;
		posYRotate.y=posY.z*posY1.x-posY.x*posY1.z;
		posYRotate.z=posY.x*posY1.y-posY.y*posY1.x;
		angle=acos((posY.x*posY1.x+posY.y*posY1.y+posY.z*posY1.z)/(sqrt(posY.x*posY.x+posY.y*posY.y+posY.z*posY.z)*sqrt(posY1.x*posY1.x+posY1.y*posY1.y+posY1.z*posY1.z)));

		//构建四元组（Z轴）(旋转到Z轴平行且同向)
		angle=angle/2;
		rot2.s=cos(angle);
		sinA=sin(angle);
		rot2.x=posYRotate.x*sinA;
		rot2.y=posYRotate.y*sinA;
		rot2.z=posYRotate.z*sinA;

		ITPQuaternion_Mult(&rot2,&rot1,&rot);
		ITPQuaternion_Normalize(&rot,&rot);
		pToolCoor->rot=rot;
	}

	return 1;
}

//根据工具坐标系在基坐标的pose和工具偏移pose计算TCP坐标
void DataManage_ToolPoseToTCP(ITP_Pose *pPose,ITP_Pose *pToolPose,ITP_Pose *pTCPPose)
{
	double matTool[4][4],matTool_inv[4][4],matPose[4][4],matTCP[4][4];
	Kinematics_Quat2Mat(&pPose->rot,&pPose->trans,matPose);
	Kinematics_Quat2Mat(&pToolPose->rot,&pToolPose->trans,matTool);
	Kinematic__mat4_inv(matTool,matTool_inv);
	Kinematic_MultiplyMatrix(matTCP,matPose,matTool_inv);//得出TCP点的矩阵

	pTCPPose->trans.x=matTCP[0][3];	//x
	pTCPPose->trans.y=matTCP[1][3];	//y
	pTCPPose->trans.z=matTCP[2][3];	//z
	Kinematics_MatToQuat(matTCP,&pTCPPose->rot);
}

void DataManage_PoseToToolOffset(ITP_Pose *pTCPPose,ITP_Pose *pToolPose,ITP_Pose *pToolCoor)
{
	double matTool[4][4],matTCP[4][4],matTCP_inv[4][4],matToolOffset[4][4];
	Kinematics_Quat2Mat(&pTCPPose->rot,&pTCPPose->trans,matTCP);
	Kinematics_Quat2Mat(&pToolPose->rot,&pToolPose->trans,matTool);
	Kinematic__mat4_inv(matTCP,matTCP_inv);

	Kinematic_MultiplyMatrix(matToolOffset,matTCP_inv,matTool);

	pToolCoor->trans.x=matToolOffset[0][3];	//x
	pToolCoor->trans.y=matToolOffset[1][3];	//y
	pToolCoor->trans.z=matToolOffset[2][3];	//z
	Kinematics_MatToQuat(matToolOffset,&pToolCoor->rot);
}


//返回：0表示无解，1表示有解
//工具坐标系创建,numPos为点的个数，pToolPose表示当前选取点所选取的工具坐标系，如果pToolPose为0表示TCP的点
int ProData_CreateToolCoord(int numPos,ITP_Pose *pPose,ITP_Pose *pToolPose,ITP_Pose *pElongZ,ITP_Pose *pElongX,ITP_Pose *pToolCoor)
{
	double p1_x,p1_y,p1_z,p2_x,p2_y,p2_z,p3_x,p3_y,p3_z,p4_x,p4_y,p4_z;
	double a1,b1,c1,d1,a2,b2,c2,d2,a3,b3,c3,d3;
	double x,y,z;
	ITP_Pose tcpPose,toolPose;
	p1_x=pPose[0].trans.x;
	p1_y=pPose[0].trans.y;
	p1_z=pPose[0].trans.z;

	p2_x=pPose[1].trans.x;
	p2_y=pPose[1].trans.y;
	p2_z=pPose[1].trans.z;

	p3_x=pPose[2].trans.x;
	p3_y=pPose[2].trans.y;
	p3_z=pPose[2].trans.z;

	p4_x=pPose[3].trans.x;
	p4_y=pPose[3].trans.y;
	p4_z=pPose[3].trans.z;

	a1 =-2*p1_x+2*p2_x;
	b1 =-2*p1_y+2*p2_y;
	c1 =-2*p1_z+2*p2_z;
	d1 =ITP_Square(p1_x)-ITP_Square(p2_z)+ITP_Square(p1_y)-ITP_Square(p2_y)+ITP_Square(p1_z)-ITP_Square(p2_x);
	a2 =-2*p1_x+2*p3_x;
	b2 =-2*p1_y+2*p3_y;
	c2 =-2*p1_z+2*p3_z;
	d2 =ITP_Square(p1_x)+ITP_Square(p1_y)+ITP_Square(p1_z)-ITP_Square(p3_x)-ITP_Square(p3_y)-ITP_Square(p3_z);
	a3 =-2*p1_x+2*p4_x;
	b3 =-2*p1_y+2*p4_y;
	c3 =-2*p1_z+2*p4_z;
	d3 =ITP_Square(p1_x)-ITP_Square(p4_z)+ITP_Square(p1_y)-ITP_Square(p4_y)+ITP_Square(p1_z)-ITP_Square(p4_x);


	if (a1==0 || (b2*a1-a2*b1)==0 || ITP_ABS(a3*b1*c2-a3*c1*b2-b3*a1*c2+b3*a2*c1+c3*a1*b2-c3*a2*b1)<ITP_Zero 
		|| ITP_ABS(a3*b1*c2-a3*c1*b2-b3*a1*c2+b3*a2*c1+c3*a1*b2-c3*a2*b1)<ITP_Zero 
		|| ITP_ABS(a3*b1*c2-a3*c1*b2-b3*a1*c2+b3*a2*c1+c3*a1*b2-c3*a2*b1)<ITP_Zero )
	{
		return 0;
	}

	//得出工具端点在基坐标系的坐标(类似求球心坐标)
	z =-(a3*b1*d2-a3*d1*b2+b3*a2*d1-b3*a1*d2+d3*a1*b2-d3*a2*b1)/(a3*b1*c2-a3*c1*b2-b3*a1*c2+b3*a2*c1+c3*a1*b2-c3*a2*b1);
	y =(a1*c2*d3-a1*d2*c3-c2*a3*d1+d2*a3*c1+a2*d1*c3-a2*c1*d3)/(a3*b1*c2-a3*c1*b2-b3*a1*c2+b3*a2*c1+c3*a1*b2-c3*a2*b1);
	x =-(b1*c2*d3-d2*c3*b1+c1*b3*d2-c1*d3*b2-d1*b3*c2+d1*c3*b2)/(a3*b1*c2-a3*c1*b2-b3*a1*c2+b3*a2*c1+c3*a1*b2-c3*a2*b1);
	toolPose.trans.x=x;
	toolPose.trans.y=y;
	toolPose.trans.z=z;

	//计算工具与法兰盘的位置偏移,求第4点法兰盘的基坐标
	if (pToolPose==0)
	{
		tcpPose=pPose[3];
	}
	else
	{
		DataManage_ToolPoseToTCP(&pPose[3],pToolPose,&tcpPose);
	}

	//计算四元组(基于基坐标系)
	if(0==DataManage_CalToolQuaternion(&pPose[3],pElongZ,pElongX,&toolPose))
	{
		toolPose.rot=tcpPose.rot;
	}



	//计算工具的偏移pose
	DataManage_PoseToToolOffset(&tcpPose,&toolPose,pToolCoor);

	return 1;
}

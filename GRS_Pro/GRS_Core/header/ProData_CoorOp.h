
#ifndef DataManage_CoorOp_H
#define DataManage_CoorOp_H


//#include "ITP_Interface.h"
#include "ITP_Interface.h"




#if defined(__cplusplus)
extern "C"
{
#endif


//用户坐标系创建 pPoseX1 为原点， pPoseX2 为X轴正方向， pPoseY1 表示Y轴方向, pUserCoor 为输出的用户坐标系
//int DataManage_CreateUserCoordinate(ITP_Pose *pPoseX1,ITP_Pose *pPoseX2,ITP_Pose *pPoseY1,ITP_Pose *pUserCoor);
int ProData_CreateUserCoord(ITP_Pose *pPoseX1,ITP_Pose *pPoseX2,ITP_Pose *pPoseY1,ITP_Pose *pUserCoor);

//工具坐标系创建 numPos>=4， pPose 为点指针数组（ numPos 个）， pToolPose 为 pPose 相对与法兰盘的工具偏移量（为0时表示法兰盘位置）
//pElongZ 指向工具的Z轴反方向， pElongX 指向工具的X轴反方向， pToolCoor 为相对与法兰盘的工具坐标系偏移
//int DataManage_CreateToolCoordinate(int numPos,ITP_Pose *pPose,ITP_Pose *pToolPose,ITP_Pose *pElongZ,ITP_Pose *pElongX,ITP_Pose *pToolCoor);
int ProData_CreateToolCoord(int numPos,ITP_Pose *pPose,ITP_Pose *pToolPose,ITP_Pose *pElongZ,ITP_Pose *pElongX,ITP_Pose *pToolCoor);


#if defined(__cplusplus)
}
#endif 

#endif

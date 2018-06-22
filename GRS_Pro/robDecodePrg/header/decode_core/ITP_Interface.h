#ifndef ITP_Interface_H
#define ITP_Interface_H

#include "decode_instructions.h"

#define ITP_Key_InputShareMem	150
#define ITP_Key_OutputShareMem	130
#define ITP_Key_ParaShareMem	140

#define ITP_KinematicType_Puma		101
#define ITP_KinematicType_Scara		201
#define ITP_KinematicType_Delta		301

#define ITP_KinematicType	ITP_KinematicType_Puma//ITP_KinematicType_Scara//运动学模型


/*****************************************************************
与数控系统插补模块接口差异
1、该机器人接口模块将工件坐标系和工具坐标放到插补模块中进行变换，
原因1：工件坐标系可固定到外部设备
原因2：动力学分析时需要用到工具坐标数据
原因3：静态工具功能
2、插补轴数：一个机器人最多支持12轴的联动插补，6轴机器人本体，6轴外部轴
3、负载：机器人包括工具负载和抓取工件负载
*****************************************************************/
//参考手册1	《RAPID Instructions,Functions and Data types》

#define ITP_RobotJointNum	6	//关节数目
#define ITP_RobotMoveJointNum	3		//移动关节数目
#define ITP_RobotCoordinateNum	6	//坐标个数
#define ITP_ExtaxAxisNum	6	//外轴数目

//#define ITP_ExtAxisInvalidate	999999999

#define ITP_AllAxisNum	(ITP_RobotJointNum+ITP_ExtaxAxisNum)//总轴数

#define ITP_PI	3.14159265358979323846
#define ITP_HalfPI	1.57079632679489661923
#define ITP_3HalfPI	(3*ITP_HalfPI)
#define ITP_2PI	(2*ITP_PI)

#define ITP_Square(x) ((x)*(x))
#define ITP_angle2radian(x) (x*ITP_PI/180)
#define ITP_radian2angle(x) (x*180/ITP_PI)

#define ITP_Zero	((double)0.000001)//单位：毫米，针对 double float表示0
#define ITP_MaxSpeed	((double)12000)//单位：mm/min

#define ITP_ABS(a) ((a)>=0?(a):(-(a)))

#define ITP_Error	-1
#define ITP_Done		1

/****************************************************************
插补模块
****************************************************************/
#define ITP_Mode_JointOp_SpeedAndIncMode	2	//插补方法：关节控制；控制方式：速度或增量
#define ITP_Mode_JointOp_PosMode	3	//插补方法：关节控制；控制方式：定位


#define ITP_Mode_CoordinateOp_SpeedAndIncMode	12	//插补方法：联动插补；控制方式：速度或增量
#define ITP_Mode_CoordinateOp_PosMode	13	//插补方法：联动插补；控制方式：译码输入


/*****************************************************************
指令格式:指令传输时使用
int cmd;		//指令号（不同指令具有不同的数据类型和大小）
(可选)int id;			//指令ID，多通道同步时使用
(可选)ITP_RobTarget topoint;	//目标点
(可选)ITP_RobTarget cirpoint;	//圆心点
(可选)IO数据

(可选、模态指令)ITP_SpeedData speed;	//速度0
(可选、模态指令)ITP_ZoneData zone;		//平滑或停止处理数据
(可选、模态指令)ITP_ToolData tool;		//工具数据
(可选、模态指令)ITP_WobjData wobj;		//工件坐标系
(可选、模态指令)ITP_LoadData load;		//总负载（机器人上的工具负载+机器人上的工件负载）
*****************************************************************/
#define ITP_BgnNum_VelAcc	100
#define ITP_BgnNum_Set		200
#define ITP_BgnNum_Move		500

//速度和加速度相关指令
//#define ITP_CMD_AccSet			(ITP_BgnNum_VelAcc+0)
//#define ITP_CMD_WorldAccLim		(ITP_BgnNum_VelAcc+1)
//#define ITP_CMD_PathAccLim		(ITP_BgnNum_VelAcc+2)
//#define ITP_CMD_SpeedLimAxis	(ITP_BgnNum_VelAcc+3)
//#define ITP_CMD_SpeedLimChec	(ITP_BgnNum_VelAcc+4)
//#define ITP_CMD_SpeedRefresh	(ITP_BgnNum_VelAcc+5)
//#define ITP_CMD_VelSet

//运动相关配置
//#define ITP_CMD_ConfJ			(ITP_BgnNum_Set+0)
//#define ITP_CMD_ConfL			(ITP_BgnNum_Set+1)
//#define ITP_CMD_ActUnit			(ITP_BgnNum_Set+2)
//#define ITP_CMD_DeactUnit		(ITP_BgnNum_Set+3)
//#define ITP_CMD_MechUnitLoad	(ITP_BgnNum_Set+4)
//#define ITP_CMD_GripLoad		(ITP_BgnNum_Set+5)
//#define ITP_CMD_MotionSup		(ITP_BgnNum_Set+6)
//#define ITP_CMD_SingArea		(ITP_BgnNum_Set+7)
//#define ITP_CMD_CirPathMode		(ITP_BgnNum_Set+8)
////#define ITP_CMD_PathResol		(ITP_BgnNum_Set+9)
////#define ITP_CMD_TuneReset		(ITP_BgnNum_Set+10)
////#define ITP_CMD_TuneServo		(ITP_BgnNum_Set+11)
////#define ITP_CMD_WaitRob			(ITP_BgnNum_Set+12)
//
//#define ITP_CMD_EOffsOn			(ITP_BgnNum_Set+13)
//#define ITP_CMD_EOffsOff		(ITP_BgnNum_Set+14)
//#define ITP_CMD_EOffsSet		(ITP_BgnNum_Set+15)
//
//#define ITP_CMD_PDispSet		(ITP_BgnNum_Set+16)
//#define ITP_CMD_PDispOn			(ITP_BgnNum_Set+17)
//#define ITP_CMD_PDispOff		(ITP_BgnNum_Set+18)
//
//#define ITP_CMD_TriggCheckIO	(ITP_BgnNum_Set+19)//触发环境需要定义结构体
//#define ITP_CMD_TriggIO			(ITP_BgnNum_Set+20)
//#define ITP_CMD_TriggEquip		(ITP_BgnNum_Set+21)	
//#define ITP_CMD_TriggInt		(ITP_BgnNum_Set+22)
//#define ITP_CMD_TriggRampAO		(ITP_BgnNum_Set+23)
//#define ITP_CMD_TriggSpeed		(ITP_BgnNum_Set+24)

//运动指令
#define ITP_CMD_MoveAbsJ		(ITP_BgnNum_Move+0)

#define ITP_CMD_MoveJ			(ITP_BgnNum_Move+2)
//#define ITP_CMD_MoveJAO			(ITP_BgnNum_Move+3)
//#define ITP_CMD_MoveJDO			(ITP_BgnNum_Move+4)
//#define ITP_CMD_MoveJGO			(ITP_BgnNum_Move+5)
//#define ITP_CMD_MoveJSync		(ITP_BgnNum_Move+6)

#define ITP_CMD_MoveL			(ITP_BgnNum_Move+7)
//#define ITP_CMD_MoveLAO			(ITP_BgnNum_Move+8)
//#define ITP_CMD_MoveLDO			(ITP_BgnNum_Move+9)
//#define ITP_CMD_MoveLGO			(ITP_BgnNum_Move+10)
//#define ITP_CMD_MoveLSync		(ITP_BgnNum_Move+11)

#define ITP_CMD_MoveC			(ITP_BgnNum_Move+12)
//#define ITP_CMD_MoveCAO			(ITP_BgnNum_Move+13)
//#define ITP_CMD_MoveCDO			(ITP_BgnNum_Move+14)
//#define ITP_CMD_MoveCGO			(ITP_BgnNum_Move+15)
//#define ITP_CMD_MoveCSync		(ITP_BgnNum_Move+16)

//#define ITP_CMD_SearchC			(ITP_BgnNum_Move+17)
//#define ITP_CMD_SearchExtJ		(ITP_BgnNum_Move+18)
//#define ITP_CMD_SearchL			(ITP_BgnNum_Move+19)
//
//#define ITP_CMD_TriggJ			(ITP_BgnNum_Move+20)
//#define ITP_CMD_TriggL			(ITP_BgnNum_Move+21)
//#define ITP_CMD_TriggC			(ITP_BgnNum_Move+22)
//#define ITP_CMD_TriggLIOs		(ITP_BgnNum_Move+23)
////#define ITP_CMD_TriggStopProc	(ITP_BgnNum_Move+24)
//
//
//#define ITP_CMD_ClearPath		(ITP_BgnNum_Move+25)
//#define ITP_CMD_StorePath		(ITP_BgnNum_Move+26)
//#define ITP_CMD_RestoPath		(ITP_BgnNum_Move+27)
//
//#define ITP_CMD_StartMove		(ITP_BgnNum_Move+28)
//#define ITP_CMD_StartMoveRetry	(ITP_BgnNum_Move+29)
//#define ITP_CMD_StopMove		(ITP_BgnNum_Move+30)
//#define ITP_CMD_StopMoveReset	(ITP_BgnNum_Move+31)



//插补周期定义
#define interpolatePeriod	((double)1)	//插补周期为几个毫秒
#define interpolatePeriod_2	((double)interpolatePeriod*interpolatePeriod)	//插补周期为几个毫秒
#define interpolatePeriod_3	((double)interpolatePeriod*interpolatePeriod*interpolatePeriod)//插补周期为几个毫秒

#define ITP_SecondToPeriod(x)	(x*1000/interpolatePeriod)		//将单位秒转插补周期

#define ITP_SpeedUnitToPeriod(x)	(x*interpolatePeriod/60000)			//用于将mm/min的速度转周期速度
#define ITP_AccUnitToPeriod(x)	(x*interpolatePeriod_2/3600000000)	//用于将mm/min*min的加速度转周期加速度
#define ITP_JUnitToPeriod(x)	(x*interpolatePeriod_3/216000000000000)	//用于将mm/min*min*min的加加速度转周期

#define ITP_PeriodToSpeedUnit(x)	(x*60000/interpolatePeriod)				//用于将周期速度转mm/min的速度
#define ITP_PeriodToAccUnit(x)	(x*3600000000/interpolatePeriod_2)		//用于将周期加速度转mm/min*min的加速度
#define ITP_PeriodToJUnit(x)	(x*216000000000000/interpolatePeriod_3)	//用于将加加速度转周期转mm/min*min*min

//加速度采用时间表示
#define ITP_AccTimeToPeriod(t)	(ITP_SpeedUnitToPeriod(ITP_MaxSpeed)*interpolatePeriod/t)	//t表示时间，vMax最大速度单位mm/min


#define ITP_AccUnit_sToPeriod(x)	(x*interpolatePeriod_2/1000000)	//用于将mm/s*s的加速度转周期加速度
#define ITP_PeriodToAccUnit_s(x)	(x*1000000/interpolatePeriod_2)		//用于将mm/s*s的加速度转周期加速度
//将mm/min的单位转换成脉冲
//#define interpolatePulse_1mm	((double)100000)	//1mm的脉冲个数
//
//#define ITP_mmToPulse(x)		(x*interpolatePulse_1mm)
//#define ITP_SpeedUnitToPulse(x)	(x*interpolatePeriod*interpolatePulse_1mm/60000)			//用于将mm/min的速度转脉冲速度
//#define ITP_AccUnitToPulse(x)	(x*interpolatePeriod_2*interpolatePulse_1mm/3600000000)	//用于将mm/min*min的加速度转脉冲加速度
//#define ITP_JUnitToPulse(x)		(x*interpolatePeriod_3*interpolatePulse_1mm/216000000000000)	//用于将mm/min*min*min的加加速度转脉冲
//
//#define ITP_PulseTomm(x)		(x/interpolatePulse_1mm)
//#define ITP_PulseToSpeedUnit(x)	(x*60000/(interpolatePeriod*interpolatePulse_1mm))				//用于将mm/min的速度转周期速度
//#define ITP_PulseToAccUnit(x)	(x*3600000000/(interpolatePeriod_2*interpolatePulse_1mm))		//用于将mm/min*min的加速度转周期加速度
//#define ITP_PulseToJUnit(x)		(x*216000000000000/(interpolatePeriod_3*interpolatePulse_1mm))	//用于将mm/min*min*min的加加速度转周期
//
//
//#define ITP_AccTimeToPulse(t)	(ITP_SpeedUnitToPulse(ITP_MaxSpeed)*interpolatePeriod/t)	//t表示时间，vMax最大速度单位mm/min
//



//弧度范围判读
//#define ITP_AngleIn1Quad(radian)  (radian>=0 && radian<ITP_HalfPI) ? 1:0		//在第1象限
//#define ITP_AngleIn2Quad(radian)  (radian>=ITP_HalfPI && radian<ITP_PI) ? 1:0	//在第2象限
//#define ITP_AngleIn3Quad(radian)  (radian>=ITP_PI && radian<ITP_3HalfPI) ? 1:0	//在第3象限
//#define ITP_AngleIn4Quad(radian)  (radian>=ITP_3HalfPI && radian<ITP_2PI) ? 1:0	//在第4象限




//统一规定，界面显示相关用角度，计算用弧度

#if defined(__cplusplus)
extern "C"
{
#endif


//基本数据类型
typedef struct ITP_Pos			//位置类型
{
	double x;
	double y;
	double z;
}ITP_Pos;

typedef struct ITP_Orient		//四元组，定义的方向类型
{
	double s;
	double x;
	double y;
	double z;
}ITP_Orient;

typedef struct ITP_Vecter
{
	double vx;
	double vy;
	double vz;
}ITP_Vecter;

typedef struct ITP_EulerXYZ		//欧拉角，定义的方向类型
{
	double ex;
	double ey;
	double ez;
}ITP_EulerXYZ;

typedef struct ITP_Pose			//位姿，由位置和四元组组成
{
	ITP_Pos trans;
	ITP_Orient rot;
}ITP_Pose;

typedef struct ITP_ConfData		//机器人姿态配置,具体请参考手册1的confdata数据类型说明
{								
	int cf1;			//0表示配置无效
	int cf4;
	int cf6;
	int cfx;
}ITP_ConfData;

typedef struct ITP_LoadData		//负载数据
{
	double mass;	//负载质量
	ITP_Pos cog;	//质心到指定坐标系的位置（区分机器人装配工具和静态工具,具体请参考手册1的loaddata数据类型）
	ITP_Orient aom;	//方向坐标

	double ix;	//绕X轴的转动惯量
	double iy;	//绕Y轴的转动惯量
	double iz;	//绕Z轴的转动惯量

}ITP_LoadData;


typedef struct ITP_ToolData		//工具数据
{
	char robhold;	//1表示工具固定在机器人法兰盘上，0表示工具不固定在机器人上
	ITP_Pose tframe;//工具相对机器人法兰盘坐标系的姿态,如果robhold为0则为该坐标为相对大地坐标系的坐标
	ITP_LoadData tload;//工具负载
}ITP_ToolData;

typedef struct ITP_WobjData		//用户坐标系
{
	char robhold;	//1表示用户坐标系固定在机器人上，0表示用户坐标系不固定在机器人上
	char ufprog;	//1表示用户坐标系固定，0表示用户坐标系可随指定机械单元移动
	char data[80];	//机械单元名称，当ufprog==0时有效………………………………………………………………………………………………


	ITP_Pose uframe;	//当静态刀具时，该坐标相对于机器人法兰盘坐标系，
						//当刀具固定在机器人上，该坐标系相对于大地坐标系
	ITP_Pose oframe;	//相对于uframe坐标系
}ITP_WobjData;

typedef struct ITP_Inpos	//精确停止位置参数
{
	double position;	//在fine或z0精确距离的比例
	double speed;	//在fine或z0精确速度的比例
	double mintime;	//单位秒
	double maxtime;
}ITP_Inpos;

typedef struct ITP_StopPointData	//停止数据
{
	int stoppoint;	//1表示inpos,2表示stoptime,3表示followb										//译码实现
	char progsynch;	//0表示该程序段未执行完可进行下一程序段的执行，1相反						//译码实现
	ITP_Inpos inpos;	//停止精度数据(准确停止设置,需要检测伺服或传感器反馈计算的坐标数据)		//译码实现
	double stoptime;	//停止时间(从插补输出停止开始算停止的时间)								//译码实现

	double followtime;	//跟随时间（使用传送装置(就是移动的工件坐标系)）

}ITP_StopPointData;

typedef struct ITP_ZoneDistanceData	//平滑数据
{
	//double rate;	//平滑倍率，范围（0.5-1），1表示无平滑处理，数值越小表示平滑越大
	double pzone_tcp;//在该半径范围内开始工具轨迹平滑
	double pzone_ori;//在该半径范围内开始工具方向平滑
	double pzone_eax;//在该半径范围内开始外部轴的平滑
	double zone_ori;	//接近该角度时，开始工具方向平滑
	double zone_leax;//对于直线外轴，当在该距离内开始外轴平滑
	double zone_reax;//对于旋转外轴，当在该角度范围内开始外轴平滑
}ITP_ZoneDistanceData;

typedef struct ITP_ZoneData		//指定平滑开始距离和角度
{
	char finep;		//1表示准确停止，0表示平滑
	union {
		ITP_StopPointData inpos;
		ITP_ZoneDistanceData bgnzone;
	};
}ITP_ZoneData;

typedef struct ITP_SpeedData_v	//速度数据
{
	double v_tcp;	//工具轨迹速度 mm/插补周期
	double v_ort;	//工具方向速度 radian/插补周期
}ITP_SpeedData_v;

typedef struct ITP_SpeedData	//速度数据//单位mm/period 或 radian/period
{
	double time;		//指定完成指令的时间，如果设置该指令，以下设置无效	<=0无效(单位：插补周期)
	double v_tcp;	//工具轨迹速度	
	double v_ort;	//工具方向速度
	double v_leax;	//直线外轴速度
	double v_reax;	//旋转外轴速度
}ITP_SpeedData;

typedef struct ITP_RobJoint		//关节坐标
{
	double rax[ITP_RobotJointNum];	//关节坐标
}ITP_RobJoint;

typedef struct ITP_ExtJoint		//外轴坐标
{
	double eax[ITP_ExtaxAxisNum];	//外轴a坐标
}ITP_ExtJoint;

typedef struct ITP_JointTarget	//关节坐标
{
	ITP_RobJoint robax;	//机器人1-6关节坐标
	ITP_ExtJoint extax;	//外轴a-f坐标
}ITP_JointTarget;

typedef struct ITP_JointSpeed	//关节速度
{
	ITP_RobJoint robax;	//机器人1-6关节坐标
	ITP_ExtJoint extax;	//外轴a-f坐标
}ITP_JointSpeed;

typedef struct ITP_JointAcc	//关节速度
{
	ITP_RobJoint robax;	//机器人1-6关节坐标
	ITP_ExtJoint extax;	//外轴a-f坐标
}ITP_JointAcc;


typedef struct ITP_RobTarget	//位姿坐标
{
	ITP_Pose pose;

	ITP_ConfData robconf;	//机器人姿态配置
	ITP_ExtJoint extax;		//外轴坐标

	//ITP_EulerXYZ euler;		//欧拉角与四元组重复
}ITP_RobTarget;

typedef struct ITP_ProgDisp//偏移数据类型
{
	ITP_Pose pdisp;		//机器人姿态偏移
	ITP_ExtJoint eoffs;	//外轴偏移
}ITP_ProgDisp;


typedef struct ITP_MotSetData	//运动参数(单位为插补周期)
{
//加速度
	double acc_acc;//加速度				单位：radian/周期*周期	(用于关节加减速和旋转加减速)	radian/s*s  radian/min*min
	double dec_acc;//减加速度
	double acc_ramp;//加加速度
	double acc_finepramp;//到fine点的加加速度
	double accMax;		//最大加速度
	double decMax;	//最大减速度

	double worldAcc_acc;//加速度		//单位：mm/周期*周期		mm/s*s		mm/min*min
	double worldDec_acc;//减加速度
	double worldAcc_ramp;//加加速度
	double worldAcc_finepramp;//到fine点的加加速度
	double worldAccMax;	//最大世界坐标系加速度
	double worldDecMax;	//最大笛卡尔坐标系减速度

//奇异点处理方法(注：该参数修改后立即生效)
	char singularPointOperation;//0表示不处理,1表示工具方向发生偏移以避开奇异点


//机器人配置
	char acc_way;	//加减速方法：0表示S曲线，1表示直线

//拐角相关参数设置(通过以下两个参数计算平滑时的尾点速度)
	double permitCornerJointSpeedChange;	//允许的拐角关节速度变化
	//double permitCornerJointAccChange;	//允许的拐角关节加速度变化

//变位机
	ITP_Pose assistOffset;		//相对于基坐标系的偏移

	ITP_ProgDisp progdisp;//偏移参数
}ITP_MotSetData;

typedef struct ITP_CmdProNo //参数标识
{
	int module;	//参数类型
	int progNo;	//该类型参数位置号
}ITP_CmdProNo;


typedef struct ITP_CmdData	//插补内部数据结构，整合读取的指令数据，用于加减速和插补
{
	int cmd;//指令号
	
	/****************************************************
	指令的所有数据都已经保存到插补内存中，传输时仅需要传输参数类型和参数号
	****************************************************/
	ITP_RobTarget P;//位姿或关节坐标
	ITP_JointTarget J;//moveAbs指令的目标位置
	ITP_RobTarget P_Circle;//圆上起点与终点之间的一点坐标

	ITP_ToolData T;//工具坐标系
	ITP_WobjData W;//用户坐标系


	ITP_SpeedData V;//速度
	ITP_ZoneData Z;//停止方式
	//输出或输入类型，中断处理
	
	ITP_CmdProNo pc;
}ITP_CmdData;




//机器人坐标系：大地坐标系，任务坐标系，基坐标系，工件坐标系，工具坐标系

typedef struct ITP_CoordinateData		//坐标系数据类型(大地坐标系，任务坐标系，基坐标系)
{
	ITP_Pose tframe;//工具相对机器人法兰盘坐标系的姿态
}ITP_CoordinateData;



typedef struct ITP_InputData	//输入
{
	char emg;	//0非急停，1急停
	char reset;	//复位
	char invalidSoftLimit;//0软限位有效，1软限位无效

	/*************************************************************
	#define ITP_Mode_JointOp_SpeedAndIncMode	2	//速度运行
	#define ITP_Mode_JointOp_AngleMode	3	//给定角度运行

	#define ITP_Mode_CoordinateOp_SpeedAndIncMode	12	//速度运行
	#define ITP_Mode_CoordinateOp_PosMode	13	//位置控制
	*************************************************************/
	char mode;//模式

	ITP_Pose toolPose;	//手动模式下的工具坐标系（基坐标：法兰盘坐标系）
	ITP_Pose userCoor;	//用户定义的坐标系（这里通过位姿来表示,父坐标系为基坐标系）

	int flagIncUpdate;	//通过该变量来更新增量进给
	double jogMoveVal;	//0表示手动速度模式，非0表示手动位置模式，且表示位移（包含方向）
    //当模式为 ITP_Mode_JointOp_SpeedAndIncMode 时以下触发信号有效
	//单位为：弧度/插补周期或mm/插补周期
	double robotMove[ITP_RobotJointNum];		//关节正反向，笛卡尔坐标系轴正反向,0表示停止，正数表示正向运动速度，负数表示反向运动速度
	double extaxRotate[ITP_ExtaxAxisNum];		//外部轴正反运动 0表示停止，正数表示正向运动速度，负数表示反向运动速度


    //当模式为 ITP_Mode_CoordinateOp_SpeedAndIncMode 时以下触发信号有效
	double moveSpeed;
	char flagMoveItem;	//0表示X轴，1表示Y轴，2表示Z轴，3表示绕X轴旋转，4表示绕Y轴旋转，5表示绕Z轴旋转
	char flagCoordinate;//0表示基坐标系，1表示工具坐标系，2表示用户自定义坐标系(基准坐标系为基坐标系)
	

	//当模式为ITP_Mode_JointOp_PosMode时以下信号有效
	ITP_SpeedData_v robotPosSpeed;	//表示工具轨迹速度,工具方向速度。当两个值有一个不为0时，以该值为速度运动
	ITP_RobTarget tarRob;	//直接手动运动到指定位置(基坐标系)

	//当模式为ITP_Mode_CoordinateOp_PosMode时以下信号有效
	char start;//1执行，0暂停					
	char skip;	//1跳过当前段,上升和下降沿触发
	char sim;	//1空运行
	double rate;//速度倍率
	
	decode_cmd_data cmd;
	//ITP_CmdData cmd;		//指令(插补模块自己缓存指令)
	//ITP_CmdData cmdPrio;	//优先指令(小线段插补时该指令无效，如果有指令时优先执行)
}ITP_InputData;

#define ITP_Alarm_SingularPoint			(-10)	//奇异点
#define ITP_Alarm_OverRobJointRange		(-11)	//超程报警
#define ITP_Alarm_OverExtAxisRange		(-12)	//指令的外轴超出范围
#define ITP_Alarm_NoConfJoint			(-13)	//无法达到配置点
#define ITP_Alarm_InverseError			(-14)	//运动学反算异常

#define ITP_Alarm_MoveError				(-20)	//运动异常(如加减速数据异常)
#define ITP_Alarm_MoveExMaxAcc			(-21)	//超出最大加速度

#define ITP_Alarm_UserCoorTrackError	(-30)	//工件坐标系随动异常
#define ITP_Alarm_UserToolCoorError		(-31)	//工具或用户坐标系数据异常

#define ITP_Alarm_ExtAxisError			(-40)	//直线或圆弧插补指令不支持单独外周控制

typedef struct ITP_OutputData	//输出
{
	char inpos;	//1定位完成
	char moving;	//1运动中，0停止中

	char modeReal;	//当前所处模式

//插补报警（提示报警，插补模块自己作并紧急停止）
	int alarmNo;	//输出报警号

//插补预处理报警(仅针对自动运行模式)
	ITP_CmdProNo	pc_error;//预处理异常报警程序号(插补只输出报警，不做处理)
	int alarmNo2;	//预处理异常报警号

	ITP_CmdProNo pc_exe;

	ITP_CmdProNo pc_read;

	//ITP_RobTarget probePoint;//探针功能记录点

	ITP_RobTarget curRob;			//当前位姿(基坐标系)
	ITP_RobTarget curRobUserCoor;	//当前位姿(用户自定义坐标系)

	ITP_JointTarget curJointTarget;//当前关节坐标

	ITP_JointSpeed curJointSpeed;//当前关节速度
	ITP_JointAcc curJointAcc;		//当前关节加速度

	ITP_Pose curExtAxisTarget;	//外轴相对于基坐标系的坐标
}ITP_OutputData;

#if defined(__cplusplus)
}
#endif 

#endif

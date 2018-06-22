// 定义SYSTEM_GLOBALS中声明的变量
#define SYSTEM_GLOBALS

// 定义PROGRAMDATA_GLOBALS中声明的变量
#define PROGRAMDATA_GLOBALS

// 定义JOGGING_GLOBALS中声明的变量
#define JOGGING_GLOBALS

// 定义PROGRAMDATA_GLOBALS中声明的变量
#define PROGRAMDATA_GLOBALS

#include "GRS_Common.h"

#include <QHostInfo>

void joggingInit()
{
    /* ******************** 最大速度定义 ********************* */
    MaxSpeed_Joint[0] = ITP_angle2radian(156);   // 六个关节轴的最大速度（单位：度每秒）[156 140 156 270 180 430]
    MaxSpeed_Joint[1] = ITP_angle2radian(140);
    MaxSpeed_Joint[2] = ITP_angle2radian(156);
    MaxSpeed_Joint[3] = ITP_angle2radian(270);
    MaxSpeed_Joint[4] = ITP_angle2radian(180);
    MaxSpeed_Joint[5] = ITP_angle2radian(430);
    MaxSpeed_Linear = 1500;     // 线性运动最大速度（单位：毫米每秒）1500
    MaxSpeed_Reorient = ITP_angle2radian(180);   // 重定向最大速度（单位：毫米每秒）1200，这里设为180度每秒

    JoggingSpeedRate = 0.5; // 手动操纵的最大速度倍率
    SysSpeedRate = 0.5;     // 机器人系统的速度倍率

    /* ************* 以下是手动操纵界面中用到的结构体（初始化） ************* */
    joggingPar.mechanicalUnit = Jogging::PAR_ROB_1; // 机械单元默认选择ROB_1
    joggingPar.absoluteAccuracy = Jogging::PAR_OFF; // 绝对精度默认选择OFF
    joggingPar.motionMode = Jogging::PAR_AXIS1_6;   // 运动模式默认选择AXIS1_6（关节运动模式）
    joggingPar.coordinateSystem = Jogging::PAR_BASE;// 坐标系默认选择BASE（基坐标）【注意：当运动模式为AXIS1_6时该选项无效】
    joggingPar.increment = Jogging::PAR_NONE;       // 增量默认选择NONE
    joggingPar.maxSpeed = ITP_MaxSpeed;  // 手动操作的最大速度（12000 mm/min）
    joggingPar.curSpeed = 0.0;  // 手动操作的当前速度（单位已转换）

    SW_Increment = SW_OFF;  // 默认关闭增量开关
    incVal[Jogging::PAR_NONE].axis      = 0.0;      // (rad)    无
    incVal[Jogging::PAR_NONE].linear    = 0.0;      // (mm)
    incVal[Jogging::PAR_NONE].reorient  = 0.0;      // (rad)
    incVal[Jogging::PAR_SMALL].axis     = 0.0001;   // (rad)    小
    incVal[Jogging::PAR_SMALL].linear   = 0.005;    // (mm)
    incVal[Jogging::PAR_SMALL].reorient = 0.0005;   // (rad)
    incVal[Jogging::PAR_MEDIUM].axis    = 0.0004;   // (rad)    中
    incVal[Jogging::PAR_MEDIUM].linear  = 1.0;      // (mm)
    incVal[Jogging::PAR_MEDIUM].reorient= 0.004;    // (rad)
    //incVal[Jogging::PAR_LARGE].axis     = 0.0025;   // (rad)    大
    incVal[Jogging::PAR_LARGE].axis     = 0.025;   // (rad)    大
    incVal[Jogging::PAR_LARGE].linear   = 5.0;      // (mm)
    incVal[Jogging::PAR_LARGE].reorient = 0.009;    // (rad)
    incVal[Jogging::PAR_USER] = incVal[Jogging::PAR_SMALL]; // 用户增量的缺省值等于小增量的值
}

void programDataInit()
{
    // tooldata: robhold, tframe, tload
    defaultTool0 = {1, {{0,0,0}, {1,0,0,0}}, {-1, {0,0,0}, {1,0,0,0}, 0, 0, 0}};

    // wobjdata: robhold, ufprog, ufmec, uframe, oframe
    decode_string ufmecStr = {""};
    defaultWobj0 = {0, 1, ufmecStr, {{0,0,0}, {1,0,0,0}}, {{0,0,0}, {1,0,0,0}}};

    // loaddata: mass, cog, aom, ix, iy, iz
    defaultLoad0 = {0, {0,0,0}, {1,0,0,0}, 0, 0, 0};

    G_CurToolData.dataType = D_DATATYPE_TOOLDATA;
    strcpy(G_CurToolData.dataProperty.typeName, "tool0");
    G_CurToolData.dataValue.tooldataData = defaultTool0;

    G_CurWobjData.dataType = D_DATATYPE_WOBJDATA;
    strcpy(G_CurWobjData.dataProperty.typeName, "wobj0");
    G_CurWobjData.dataValue.wobjdataData = defaultWobj0;

    G_CurLoadData.dataType = D_DATATYPE_LOADDATA;
    strcpy(G_CurLoadData.dataProperty.typeName, "load0");
    G_CurLoadData.dataValue.loaddataData = defaultLoad0;
}

/*
 * Global functions as follows:
 */
int G_CommonInit()
{
    // 注册一个自定义事件
    myCustomEvent = (QEvent::Type)QEvent::registerEventType(5000);

//    G_SysName = QHostInfo::localHostName();
    G_ControllerName = QString("GSK_RB08B");

    // 为一些全局参数设置初始值，后续会增加关机保存当前参数的功能，可以从XML或INI文件中读取参数信息
    G_OperatingMode = OM_Auto;
    G_ControllerStatus = CS_MotorOff;
    G_ProductionStatus = PS_Stopped;
    G_SpeedScaleFactor = 0.2;

    joggingInit();
    programDataInit();

    return 0;
}




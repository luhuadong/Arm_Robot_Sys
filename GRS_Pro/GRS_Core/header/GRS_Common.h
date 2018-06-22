/*
 * @file	common.h
 * @brief	Common resource of GSK robot system
 * @author  luhuadong
 * @version 1.0
 * @date	2015-12-28
 *
 * # update (更新日志)
 *
 * [2015-12-28] <luhuadong> v1.0
 *
 * + v1.0版发布
 *
 */

#ifndef __GRS_COMMON_H__
#define __GRS_COMMON_H__

#include <QApplication>
#include <QStringList>
#include <QEvent>

// 包含类型ITP_InputData
#include "ITP_Interface.h"
#include "decode_typestructs.h"
#include "robotdecodepublic.h"


#if defined(__cplusplus)
extern "C"
{
#endif

#define LCD_SCALE_WIDTH     4
#define LCD_SCALE_HEIGHT    3
/*
 * In this project, the LCD screen scale we used is 4:3.
 * Generally, there are some screen size as follows:
 * (width * height, px)
*/

//#define UI_SCREEN_WIDTH     1600
//#define UI_SCREEN_HEIGHT    1200
//#define UI_SCREEN_WIDTH     1440
//#define UI_SCREEN_HEIGHT    1080
//#define UI_SCREEN_WIDTH     1280
//#define UI_SCREEN_HEIGHT    960
//#define UI_SCREEN_WIDTH     960
//#define UI_SCREEN_HEIGHT    720
#define UI_SCREEN_WIDTH     800
#define UI_SCREEN_HEIGHT    600
//#define UI_SCREEN_WIDTH     640
//#define UI_SCREEN_HEIGHT    480
//#define UI_SCREEN_WIDTH     480
//#define UI_SCREEN_HEIGHT    360

#define UI_PANEL_WIDTH      (UI_SCREEN_WIDTH / 4)
#define UI_PANEL_HEIGHT     (UI_SCREEN_HEIGHT)

#define UI_TOPBAR_HEIGHT    (UI_SCREEN_HEIGHT / 10)
#define UI_BOTTOMBAR_HEIGHT (UI_SCREEN_HEIGHT / 10)

#define TaskBarMaxNum 6

/* 用户管理
 * 系统维护人员：拥有最高权限；
 * 机器人编程人员：无需开放过多的底层资源，可以开放对机器人某些模块的设置，对程序的修改、编写、调试等权限；
 * 生产线的操作员：不允许修改和编辑，只允许查看和运行；
 *
 * 系统针对这个结构体维护一个固化在FLASH中的用户文件，用户每次登录系统时，系统根据用户的输入去匹配这个用户文件，
 * 根据匹配结果得到相应的用户级别，开放不同的系统资源。
 * 级别高的用户可以创建、注销低级别的用户
 * 用户可以根据需要修改自己的登录密码。
*/
typedef struct{
    unsigned char userName[32];
    unsigned char password[32];
    unsigned char level;
}USER;

/*
 * 事件类型（按事件的严重程度分为三种事件类型）
 * 1、信息指一般系统事件，例如启动和停止程序、更改操作模式以及开关电机。
 * 2、警告是您需要关注的事件，但是，它没有非常严重，需要停止进程或 RAPID 程序。
 * 3、错误是防止机器人系统继续执行程序的事件。运行的进程或RAPID程序不能继续，因此将停止。
 *
 */
typedef enum{
    EVENT_Information,	// Information
    EVENT_Warning,		// Warning
    EVENT_Error			// Error
}eventTypes;

/* 系统层面要维护一个任务链表，包括每个任务所拥有的模块及其模块下的例行程序。
 * 而界面并不需要知道有这样一个链表树的存在，系统只需提供获取当前任务、模块以及例行程序等信息的接口函数，
 * 和相关的创建、修改、删除、保存等操作的接口函数即可。
 */

#ifdef SYSTEM_GLOBALS
#define SYSTEM_EXT
#else
#define SYSTEM_EXT extern
#endif

// 自定义事件
SYSTEM_EXT QEvent::Type myCustomEvent;// = (QEvent::Type)QEvent::registerEventType();

// 系统名称
//SYSTEM_EXT char G_SysName[32];
SYSTEM_EXT QString G_SysName;

// 控制器名称
//SYSTEM_EXT char G_ControllerName[32];
SYSTEM_EXT QString G_ControllerName;


/* 控制器状态——
 * 1.Initializing
 * 2.电机停止
 * 3.电机开启
 * 4.常规停止
 * 5.紧急停止
 * 6.急停后等待电机开启
 * 7.系统故障
*/
typedef enum{
    CS_Initializing,
    CS_MotorOff,
    CS_MotorOn,
    CS_NormalStop,
    CS_EmergencyStop,
    CS_WaitingMotorOn,
    CS_SystemFault
}controllerStatusTypes;

SYSTEM_EXT controllerStatusTypes G_ControllerStatus;

/* 程序执行状态（标记机器人是否正在运行程序）
 * 1.Running	正在运行程序
 * 2.Ready		程序已经加载，PP（程序执行起始点）已经设定，可以开始运行程序。
 * 3.Stopped	程序已经加载
 * 4.Uninitialized	程序内存未初始化。这表明当前存在错误。
 *
*/
typedef enum{
    PS_Running,
    PS_Ready,
    PS_Stopped,
    PS_Uninitialized
}productionStatusTypes;

SYSTEM_EXT productionStatusTypes G_ProductionStatus;

/* 控制器的操作模式
 * 1.Initializing
 * 2.自动
 * 3.手动
 * 4.Manual full speed
 * 5.Waiting for acknowledge
*/
typedef enum{
    OM_Initializing,
    OM_Auto,
    OM_Manual,
    OM_ManualFullSpeed,
    OM_WaitingAck
}operatingModeTypes;

SYSTEM_EXT operatingModeTypes G_OperatingMode;

// 系统速度倍率（Scale Factor或者Ratio）
SYSTEM_EXT double G_SpeedScaleFactor;
// 实际速度倍率
SYSTEM_EXT double G_ActualSpeedScaleFactor;

// 保存当前任务
//int G_CurTask;

// 保存当前模块
//int G_CurModule;


// 保存上一次增量选项（增量开关用：增量由关转开时回到上一次开时状态）


// 菜单选项标识
typedef enum {
    Menu_none = -1,
    Menu_HotEdit = 0,
    Menu_IO,
    Menu_Jogging,
    Menu_AutoProduction,
    Menu_ProgramEditor,
    Menu_ProgramData,
    Menu_Arc,
    Menu_LogOff = 8,
    Menu_Backup = 9,
    Menu_Calibration,
    Menu_ControlPanel,
    Menu_EventLog,
    Menu_Explorer,
    Menu_SystemInfo,
    Menu_Restart = 17
}menuItemTypes;





/* ************* 注意命名空间【Jogging】 ************* */
namespace Jogging {

/* 语言 */
typedef enum{
    UI_EN,
    UI_ZH
}LANGUAGE;


typedef enum{
    MECHANICAL_UNIT,
    ABSOLUTE_ACCURACY,
    MOTION_MODE,
    COORDINATE_SYSTEM,
    TOOL,
    WORK_OBJECT,
    PAY_LOAD,
    JOYSTICK_LOCK,
    INCREMENT,
    POSITION_FORMAT,
    ALIGN,
    GO_TO,
    ACTIVATE
}MANUALSUBWIDGET;


/* 机械单元参数标识 */
typedef enum{
    PAR_ROB_1
}TypeMechanicalUnit;

/* 绝对精度参数标识 */
typedef enum{
    PAR_ON,
    PAR_OFF
}TypeAbsoluteAccuracy;

/* 动作模式参数标识 */
typedef enum{
    PAR_AXIS1_6,
    PAR_LINEAR,
    PAR_REORIENT
}TypeMotionMode;

/* 坐标系参数标识 */
typedef enum{
    PAR_WORLD,
    PAR_BASE,
    PAR_TOOL,
    PAR_WORKOBJECT
}TypeCoordinateSystem;

/* 增量参数标识 */
typedef enum{
    PAR_NONE,
    PAR_SMALL,
    PAR_MEDIUM,
    PAR_LARGE,
    PAR_USER
}TypeIncrement;

typedef struct inc_Val{
    double axis;
    double linear;
    double reorient;
}Struct_IncVal;

/* 记录手动操纵中选择的参数的结构体 */
typedef struct joggingPar{
    TypeMechanicalUnit mechanicalUnit;
    TypeAbsoluteAccuracy absoluteAccuracy;
    TypeMotionMode motionMode;
    TypeCoordinateSystem coordinateSystem;
    TypeIncrement increment;
    double maxSpeed;    // 手动操作的最大速度（单位mm/min）
    double curSpeed;    // 手动操作的当前速度（已转换）
}TypeJoggingPar;


/* 位置显示方式（Position shown in） */
typedef enum{
    PAR_SHOW_WORLD,
    PAR_SHOW_BASE,
    PAR_SHOW_WOBJ
}TypePositionCoo;

/* 方向格式（Orientation format） */
typedef enum{
    PAR_SHOW_QUATERNION,
    PAR_SHOW_EULERANGLES
}TypePositionOri;

/* 角度单位（Angle unit） */
typedef enum{
    PAR_SHOW_DEGREES,
    PAR_SHOW_RADIANS
}TypePositionAnU;

/* 位置格式结构体，用于保存【位置格式】界面操作的选项 */
typedef struct positionFmt{
    TypePositionCoo curCoord;
    TypePositionOri curOrientFmt;
    TypePositionAnU curUnit;
}TypePositionFmt;

}   // end of namespace Jogging



#define PROPERTY_ROWS 9
#define PROPERTY_COLUMNS 2
#define POSITION_ROWS 7
#define POSITION_COLUMNS 3

#define SW_ON   0   // 开关宏定义：开
#define SW_OFF  1   // 开关宏定义：关



#ifdef JOGGING_GLOBALS
#define JOGGING_EXT
#else
#define JOGGING_EXT extern
#endif

JOGGING_EXT Jogging::TypeJoggingPar joggingPar;     // 用于保存【属性表】操作的选项
JOGGING_EXT Jogging::TypePositionFmt displayFmt;   // 用于保存【位置格式】界面操作的选项

/* ************* 以下是手动操纵界面中用到的全局字符串或字符串链表 ************* */
JOGGING_EXT QStringList joggingPropertyStrList;     // 手动操纵属性表的文字
JOGGING_EXT QStringList mechanicalUnitStrList;      // 机械单元选项文字
JOGGING_EXT QStringList absoluteAccuracyStrList;    // 绝对精度选项文字
JOGGING_EXT QStringList motionModeStrList;          // 动作模式选项文字
JOGGING_EXT QStringList coordinateSystemStrList;    // 坐标系选项文字
//JOGGING_EXT QStringList toolStrList;                // 工具坐标选项文字
//JOGGING_EXT QStringList workObjectStrList;          // 工件坐标选项文字
//JOGGING_EXT QStringList payloadStrList;             // 有效载荷选项文字
//JOGGING_EXT QStringList joystickLockStrList;        // 操纵杆锁定选项文字
JOGGING_EXT QStringList incrementStrList;           // 增量选项文字
JOGGING_EXT QStringList propertyTipsStrList;        // 【属性】表中各选项的提示

JOGGING_EXT QStringList jointOpKeyStrList;          // 关节操作时在【位置】中显示的字符串（1、2、3、4、5、6）
JOGGING_EXT QStringList cooOpKeyStrList_Qua;        // 坐标操作（四元数）时在【位置】中显示的字符串（X、Y、Z、q1、q2、q3、q4）
JOGGING_EXT QStringList cooOpKeyStrList_Eul;        // 关节操作（欧拉角）时在【位置】中显示的字符串（X、Y、Z、EZ、EY、EX）
JOGGING_EXT QStringList jointOpUnitStrList_Deg;     // 关节操作（单位度）时在【位置】中显示的单位（°、...、°）
JOGGING_EXT QStringList jointOpUnitStrList_Rad;     // 关节操作（单位弧度）时在【位置】中显示的单位（rad、...、rad）
JOGGING_EXT QStringList cooOpUnitStrList_Qua;       // 坐标操作（四元数）时在【位置】中显示的单位（mm、mm、mm、无、无、无、无）
JOGGING_EXT QStringList cooOpUnitStrList_Eul_Deg;   // 坐标操作（欧拉角、单位度）时在【位置】中显示的单位（mm、mm、mm、°、°、°）
JOGGING_EXT QStringList cooOpUnitStrList_Eul_Rad;   // 坐标操作（欧拉角、单位弧度）时在【位置】中显示的单位（mm、mm、mm、rad、rad、rad）

JOGGING_EXT QStringList jointMoveBtnStrList;        // 关节操作时的按键文字（J1+/J1-/J2+/J2-/J3+/J3-/J4+/J4-/J5+/J5-/J6+/J6-）
JOGGING_EXT QStringList coordMoveBtnStrList;        // 坐标操作时的按键文字（X+/X-/Y+/Y-/Z+/Z-/A+/A-/B+/B-/C+/C-）

JOGGING_EXT int SW_Increment;   // 增量开关（0表示开，1表示关）
JOGGING_EXT Jogging::Struct_IncVal incVal[5];      // 增量选项为5，即无、小、中、大、用户

JOGGING_EXT double MaxSpeed_Joint[ITP_RobotJointNum];   // 六个关节轴的最大速度（单位：度每秒）[156 140 156 270 180 430]
JOGGING_EXT double MaxSpeed_Linear;     // 线性运动最大速度（单位：毫米每秒）1500
JOGGING_EXT double MaxSpeed_Reorient;   // 重定向最大速度（单位：毫米每秒）1200，这里设为180度每秒
JOGGING_EXT double JoggingSpeedRate;    // 手动操纵的最大速度倍率，安全起见，要求手动操纵时不能全速运行，该值一般为0.5或0.8
JOGGING_EXT double SysSpeedRate;        // 机器人系统的速度倍率

//JOGGING_EXT ITP_RobTarget sysInitRobTarget; // 定义一个机器人位置点，用于测试【转到】功能




namespace ProgramData {

typedef enum{
    ROB_TARGET,     // robtarget 机器人与外轴的位置数据
    JOINT_TARGET,   // jointtarget 关节位置数据
    TOOL_DATA,      // tooldata 工具数据
    WOBJ_DATA,      // wobjdata 工件数据
    LOAD_DATA,      // loaddata 负荷数据
    ZONE_DATA,      // zonedata TCP转弯半径数据
    SPEED_DATA,     // speeddata 机器人与外轴的速度数据
    BOOL,           // bool 布尔型
    NUM,            // num 数值型
    STRING          // string 字符串型
}DataTypes;     // 当前系统支持的程序数据类型


typedef enum{
    DATA_DEC_NEW,
    DATA_DEC_EDIT
}DataDeclarationTypes;

typedef enum{
    SCOPE_GLOBAL,
    SCOPE_LOCAL,
    SCOPE_TASK
}ScopeTypes;

typedef enum{
    STORAGE_VAR,
    STORAGE_PERS,
    STORAGE_CONST
}StorageTypes;

typedef enum{
    DIMENSION_NONE,
    DIMENSION_1,
    DIMENSION_2,
    DIMENSION_3
}DimensionTypes;

typedef struct{
    decode_typestruct dataType;
    decode_type_property dataProperty;
    decode_type_data dataValue;
}proData;

}   // end of namespace ProgramData

#ifdef PROGRAMDATA_GLOBALS
#define PROGRAMDATA_EXT
#else
#define PROGRAMDATA_EXT extern
#endif


//PROGRAMDATA_EXT QString curTask;    // 记录机器人当前任务
PROGRAMDATA_EXT QStringList dataTypesStrList;   // 记录当前系统支持的程序数据类型的字符串列表（robtarget、jointtarget ...）

PROGRAMDATA_EXT decode_tooldata defaultTool0;
PROGRAMDATA_EXT decode_wobjdata defaultWobj0;
PROGRAMDATA_EXT decode_loaddata defaultLoad0;

// 用于保存手动操纵中选择的工具、工件、载荷变量
PROGRAMDATA_EXT ProgramData::proData G_CurToolData;
PROGRAMDATA_EXT ProgramData::proData G_CurWobjData;
PROGRAMDATA_EXT ProgramData::proData G_CurLoadData;



/*
 * Global functions as follows:
 */
int G_CommonInit();




#if defined(__cplusplus)
}
#endif

#endif // __GRS_COMMON_H__

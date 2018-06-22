#ifndef ROBOTPRGFILESTRUCT_H
#define ROBOTPRGFILESTRUCT_H

//#include <QApplication>
#include <QDomDocument>
#include <QXmlStreamWriter>

#include <QList>

#include "decode_typestructs.h"
#include "decode_lexical_analysis.h"
#include "robotvariablestruct.h"
#include "robotdecodepublic.h"


//#define LINE_TYPE_NULL      (0) //空字符行
//#define LINE_TYPE_NOTE      (1) //注释行
//#define LINE_TYPE_MODULE    (2) //MODULE开始
//#define LINE_TYPE_MODULEEND (3) //MODULEEND
//#define LINE_TYPE_PROC      (4) //PROC开始
//#define LINE_TYPE_PROCEND   (5) //PROCEND
//#define LINE_TYPE_VARIABLE  (6) //变量类型
//#define LINE_TYPE_IF        (7) //IF开始
//#define LINE_TYPE_IFEND     (8) //IFEND



//typedef struct {//程序指针存储结构
//    int curModule;  //当前所在的模块
//    int curModuleLine;  //当前所在模块的行
//    int curRoutine; //当前所在的例行程序
//    int curRoutineLine;//当前所在的例行程序的行
//}program_pointer;




typedef struct {//一行字符串存储结构
//该行字符串类型，变量 or 程序指令等LINE_TYPE_NOTE:注释，LINE_TYPE_DATA：数据行，LINE_TYPE_INSTRUCTION：指令行
//低8位为decode_line_type类型，
//bit8~bit15如果是数据行LINE_TYPE_VAR：VAR，LINE_TYPE_PERS：PERS ，LINE_TYPE_CONST：CONST，如果是指令行，则为相应类型，如LINE_TYPE_IF等
//bit16~bit24是变量范围0：全局， LINE_TYPE_LOCAL：LOCAL， LINE_TYPE_TASK：TASK
//高8位暂定指令制表符缩进个数，
    int type:8;//此处需要改为位域方式，暂时未改
    int varType:8;
    int scope:8;
    int tabCnt:8;//制表符缩进次数
//    QList<char *> str;//该行字符串指针，用于存储每一个段，已经过滤空格和逗号，
    char *str;  //该行字符串指针
}line_str_data;

typedef struct {//例行程序所需参数存储结构，一个参数
    int type;//参数类型索引
    decode_type_data paraDaa;//此处再添加一个数据结构体
}parameter_struct;

//typedef struct {//例行程序属性
//    char routineName[32];//例行程序名
//    int routineType;//例行程序类型procedure function trap
//    int routineFlag;//标志位，参看ABB用来标记，从低字节开始local backward error undo
//    int parentModule;//所属模块索引
//    char parameter[256];//参数字符串
//}routine_prg_property;

typedef struct {//例行程序存储结构
    routine_prg_property routinePrgPro;//例行程序属性
    QList<line_str_data> lineStrData;//例行程序中所有字符串行
    QList<parameter_struct> parameterStruct;//例行程序所需参数，首个元素表示返回值
    robot_var_struct robotVarArry[D_DATATYPE_END_CNT];//例行程序中的变量
    int flag;//作为标志，暂时使用bit0为是否该例行程序有效，为0则无效
}routine_prg;

//typedef struct {//模块属性结构体
//    char moduleName[32];//模块名，ABB允许的最长长度
//    int moduleType;//模块类型，系统或者程序，0为程序，1为系统
//}module_prg_property;

typedef struct {//程序模块存储结构
    module_prg_property modulePrgPro;//模块属性
    QList<line_str_data> lineStrData;//模块内部存储的程序行
    QList<routine_prg> routinePrg;//模块包含的例行程序
    robot_var_struct robotVarArry[D_DATATYPE_END_CNT];//模块的变量
    int flag;//作为标志，暂时使用bit0为是否该模块有效，为0则无效
}module_prg;

typedef struct { //文件信息存储结构
    char fileName[32];//文件名
    char filePath[256];//文件路径
    int parentTask;//所属任务，也可作为所在通道
    int editFlag;//编辑状态标志
    program_pointer programPoniter;//当前程序指针，注意，自动方式下加载文件后要设置为main开头第一个有效指令
    QList<int> dataTypeUseTable;//文件中使用的机器人数据类型表
    QList<module_prg> modulePrg;//包含的模块，注意，在加载模块时
    //注意此处还要添加剪切板
}prg_file_info;


class RobotPrgFileStruct
{
public:
    RobotPrgFileStruct();
    ~RobotPrgFileStruct();
    bool readRobotPrgFile(const char *filePathName);
    bool writeRobotPrgFile(const char *filePathName);
    bool parseProgramElement(const QDomElement &element);
    bool parseModuleElement(const QDomElement &element);
    bool writeModuleElement(QXmlStreamWriter *xmlWriter, int modulePos);
    program_pointer getSelectModuleLinePos(int selMod, int lineNum);
    int getSelectModuleLineStr(int selMod, int lineNum, char *str);
    bool createNewModule(module_prg_property modPrgPro);
    bool createNewRoutine(int modNum, routine_prg_property routinePrgPro);
    bool insertNewLineStr(int modNum, int lineNum, const char *str);
    bool appendLineStr(int modNum, int routineNum, const char *str);
    int getModulePos(char *modName);
    int getRoutinePos(int modNum, char *routineName);
    bool printModuleLineStrTest(int modNum);
    int createNewVariable(int modNum, int routineNum, char *str);
    bool deleteOneLineStr(int modNum, int lineNum);
    bool editOneLineStr(int modNum, int lineNum, const char *str);
    int getFileModuleCnt(void);
    int getSelModRoutineCnt(int modNum);
    bool modifyOneLineStr(int modNum, int lineNum, const char *str);
    int modifyOneVariable(int modNum, int routineNum, char *varName, char *str);
    int deleteOneVariable(int modNum, int routineNum, char *varName);
    int usePosGetSelVarProperty(int modNum, int routineNum, int pos, int varType, decode_type_property *pro);
    int usePosGetSelVarVal(int modNum, int routineNum, int pos, int varType, decode_type_data *dataVal);
    int useNameSetSelVarData(int modNum, int routineNum, char *varName, decode_type_data dataVal);
    int useNameGetSelVarData(int modNum, int routineNum, char *varName, decode_type_data *dataVal);
    int useNameGetSelVarProperty(int modNum, int routineNum, char *varName, decode_type_property *pro);
    int deleteSelRoutine(int modNum, int routineNum);
    int deleteSelModule(int modNum);
    int getSelModuleProperty(int modNum, module_prg_property *modPro);
    int modifySelModuleProperty(int modNum, module_prg_property modPro);
    int getSelRoutineProperty(int modNum, int routineNum, routine_prg_property *routinePro);
    int modifySelRoutineProperty(int modNum, int routineNum, routine_prg_property routinePro);
    int isCanSelLineStr(int modNum, int lineNum);
    int isCanEditLineStr(int modNum, int lineNum);
    int getSelRoutineFirstLinePos(int modNum, int routineNum);
    int getFileLineTotalCnt(void);
    int getSelModuleFileLineTotalCnt(int modNum);
    int getSelRoutineFileLineTotalCnt(int modNum, int routineNum);
    int clearFileStructInfo(void);
    int useNameGetModuleProperty(char *modName, module_prg_property *modPro);
    int useNameGetRoutineProperty(char *routineName, routine_prg_property *routinePro);
    int parseModLineToRelation(int *modNum, int *modLine, int *routine, int *routineLine);
    int getSelRoutineLineStr(int modNum, int routineNum, int routineLineNum, char *str);
    int getFirstInstructionLinePos(int modNum, int routineNum);

private:
    prg_file_info prgFileInfo;

};

#endif // ROBOTPRGFILESTRUCT_H

#include "robotvariablestruct.h"
#include "decode_lexical_analysis.h"

class RobotVariableStruct
{
public:
    RobotVariableStruct();
    ~RobotVariableStruct();
    bool createNewRobotVariable(int varType, decode_type_property typePro);
    bool deleteRobotVariable(int varType, const char *varName);
    bool getRobotVarTypePropertyUseName(int varType, const char *varName, decode_type_property *typePro);
    bool getRobotVarTypePropertyUsePos(int varType, int pos, decode_type_property *typePro);
    bool setRobotVarValData(int varType, const char *varName, decode_type_data varData);
    bool getRobotVarValData(int varType, const char *varName, decode_type_data *varData);
private:
    robot_var_struct robotVarArry[D_DATATYPE_END_CNT];

};

RobotVariableStruct::RobotVariableStruct()
{
}

RobotVariableStruct::~RobotVariableStruct()
{
}

bool RobotVariableStruct::createNewRobotVariable(int varType, decode_type_property typePro)
{
    int i = 0;
    robot_variable robotVariable;

    robotVariable.valIndex = 0;//此处要调用函数创建
    if (robotVariable.valIndex == -1) {//申请失败
        return false;
    }
    robotVariable.varType = varType;
    robotVariable.typePro = typePro;

    while (i < robotVarArry[varType].robotVar.size()) {
        if (decodeStringCompare(typePro.typeName, robotVarArry[varType].robotVar[i].typePro.typeName, 0) < 0) {//按名称排序
            robotVarArry[varType].robotVar.insert(i, robotVariable);
            return true;
        }
        i++;
    }
    robotVarArry[varType].robotVar.append(robotVariable);
    return true;
}

bool RobotVariableStruct::deleteRobotVariable(int varType, const char *varName)
{
    int i = 0;

    while (i < robotVarArry[varType].robotVar.size()) {
        if (decodeStringCompare(varName, robotVarArry[varType].robotVar[i].typePro.typeName, 0) == 0) {//变量名相同
            robotVarArry[varType].robotVar.removeAt(i);
            //此处要加调用删除变量值存储空间标记，使无效
            return true;
        }
        i++;
    }
    return false;
}

bool RobotVariableStruct::getRobotVarTypePropertyUseName(int varType, const char *varName, decode_type_property *typePro)
{
    int i = 0;

    while (i < robotVarArry[varType].robotVar.size()) {
        if (decodeStringCompare(varName, robotVarArry[varType].robotVar[i].typePro.typeName, 0) == 0) {//变量名相同
            *typePro = robotVarArry[varType].robotVar[i].typePro;
            return true;
        }
        i++;
    }
    return false;
}

bool RobotVariableStruct::getRobotVarTypePropertyUsePos(int varType, int pos, decode_type_property *typePro)
{
    if (pos < robotVarArry[varType].robotVar.size()) {
        *typePro = robotVarArry[varType].robotVar[pos].typePro;
        return true;
    }
    return false;
}

bool RobotVariableStruct::setRobotVarValData(int varType, const char *varName, decode_type_data varData)
{
    int i = 0;

    while (i < robotVarArry[varType].robotVar.size()) {
        if (decodeStringCompare(varName, robotVarArry[varType].robotVar[i].typePro.typeName, 0) == 0) {//变量名相同
            //此处要调用变量值修改
            return true;
        }
        i++;
    }
    return false;
}

bool RobotVariableStruct::getRobotVarValData(int varType, const char *varName, decode_type_data *varData)
{
    int i = 0;

    while (i < robotVarArry[varType].robotVar.size()) {
        if (decodeStringCompare(varName, robotVarArry[varType].robotVar[i].typePro.typeName, 0) == 0) {//变量名相同
            //此处要调用变量值读取
            return true;
        }
        i++;
    }
    return false;
}

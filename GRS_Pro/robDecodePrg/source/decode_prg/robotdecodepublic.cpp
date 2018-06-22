#include "robotdecodepublic.h"
#include "robotprgfilestruct.h"

#include <QList>


typedef struct {
    int cnt[32];//ABB允许名称32字符，记录每一个长度的数目
    QList<decode_character_property> charPro;
}character_index;


character_index CharacterIndexGlobal;


RobotPrgFileStruct robotPrgFileStructGlobal[MAX_CHANNEL];

int createOneCharacterIndex(decode_character_property charPro)
{
#if 0
    int len;
    int i = 0;
    int offset = 0;
    int result;


    len = strlen(charPro.charName);

    if (len >= 32 || len == 0) {//超过允许长度
        return -1;
    }
    len = len - 2;//因为从0开始，并且从该字长之前的所有计数，所以减2
    while (i < len) {
        offset += CharacterIndexGlobal.cnt[i];
        i++;
    }

    while (offset < CharacterIndexGlobal.charPro.size()) {
        result = decodeStringCompare(charPro.charName, CharacterIndexGlobal.charPro[offset].charName, 0);
        if (result == 0) {//如果相等
            switch (CharacterIndexGlobal.charPro[offset].charType) {
            case D_CHAR_TYPE_INSTRUCTION:
            case D_CHAR_TYPE_VARTYPE:
            case D_CHAR_TYPE_MODNAME:
            case D_CHAR_TYPE_PRCNAME:
                return -1;
                break;
            default:
                if (((charPro.modParent == CharacterIndexGlobal.charPro[offset].modParent && charPro.routineParent != CharacterIndexGlobal.charPro[offset].modParent) && charPro.scope2 == 2)//如果是LOCAL
                    || (charPro.modParent != CharacterIndexGlobal.charPro[offset].modParent) && charPro.scope2 == 2  ) {//不同模块下，且为LOCAL范围
                    break;
                } else {
                    return -1;
                }
            }


        } else if (result < 0) {//排序
            CharacterIndexGlobal.charPro.insert(offset, charPro);
            CharacterIndexGlobal.cnt[len] += 1;
            return 1;
        }
        offset++;
    }

    CharacterIndexGlobal.charPro.append(charPro);//末尾则直接添加到末尾
    CharacterIndexGlobal.cnt[len] += 1;
#else
    int len;
    int i = 0;
    int offset = 0;
    int result;


    len = strlen(charPro.charName);

    if (len >= 32 || len == 0) {//超过允许长度
        return -1;
    }
    len = len - 1;//因为从0开始，并且从该字长之前的所有计数，所以减1
    while (i < len) {
        offset += CharacterIndexGlobal.cnt[i];
        i++;
    }
    i = 0;

 //   while (offset < CharacterIndexGlobal.charPro.size()) {
    while (i < CharacterIndexGlobal.cnt[len]) {
        result = decodeStringCompare(charPro.charName, CharacterIndexGlobal.charPro[offset].charName, 0);
        if (result == 0) {//如果相等
            switch (CharacterIndexGlobal.charPro[offset].charType) {
            case D_CHAR_TYPE_INSTRUCTION:
            case D_CHAR_TYPE_VARTYPE:
            case D_CHAR_TYPE_MODNAME:
            case D_CHAR_TYPE_PRCNAME:
                return -1;
                break;
            default:
                if (((charPro.modParent == CharacterIndexGlobal.charPro[offset].modParent && charPro.routineParent != CharacterIndexGlobal.charPro[offset].modParent) && charPro.scope2 == 2)//如果是LOCAL
                    || (charPro.modParent != CharacterIndexGlobal.charPro[offset].modParent) && charPro.scope2 == 2  ) {//不同模块下，且为LOCAL范围
                    break;
                } else {
                    return -1;
                }
            }
        }
        offset++;
        i++;
    }
    CharacterIndexGlobal.charPro.insert(offset, charPro);
    CharacterIndexGlobal.cnt[len] += 1;
#endif
    return 1;
}


int getOneCharacterIndex(int pos, char *str, decode_character_property *charPro)//根据序号依次取相同名称的匹配
{
    int len = strlen(str);
    int i = 0;
    int offset = 0;
    int result;
    int cnt = 0;

    if (len > 32) {
        return -1;
    }
    len = len - 1;//放在此处很有必要，因为在该长度之前的计算偏移
    while (i < len) {
        offset += CharacterIndexGlobal.cnt[i];
        i++;
    }
    i = 0;
    while (i < CharacterIndexGlobal.cnt[len]) {
//        offset = offset + i;
        result = decodeStringCompare(str, CharacterIndexGlobal.charPro[offset].charName, 0);
        if (result == 0) {//如果比较相等
            if (cnt == pos) {
                *charPro = CharacterIndexGlobal.charPro[offset];
                return 1;
            }
            cnt++;
        }
        offset++;
        i++;
    }

    return -1;
}

int deleteOneCharacterIndex(decode_character_property charPro)//删除一个查询表记录
{
    int len = strlen(charPro.charName);
    int i = 0;
    int offset = 0;
    int result;
    int cnt = 0;
    decode_character_property *pCharPro;

    if (len > 32) {
        return -1;
    }
    len = len - 1;//放在此处很有必要，因为在该长度之前的计算偏移
    while (i < len) {
        offset += CharacterIndexGlobal.cnt[i];
        i++;
    }
    i = 0;
    while (i < CharacterIndexGlobal.cnt[len]) {
//        offset = offset + i;
        pCharPro = &CharacterIndexGlobal.charPro[offset];
        result = decodeStringCompare(charPro.charName, pCharPro->charName, 0);
        if (result == 0) {//如果比较相等
            if (memcmp(pCharPro, &charPro, sizeof(decode_character_property)) == 0) {//如果内存比较相等
               CharacterIndexGlobal.cnt[len] -= 1;
               CharacterIndexGlobal.charPro.removeAt(offset);//移除记录
               return 1;
            }
        }
        offset++;
        i++;
    }

    return -1;
}


BOOLEAN readRobotPrgFile(int channel, const char *filePathName)
{
    return robotPrgFileStructGlobal[channel].readRobotPrgFile(filePathName);
}

BOOLEAN writeRobotPrgFile(int channel, const char *filePathName)
{
    return robotPrgFileStructGlobal[channel].writeRobotPrgFile(filePathName);
}

int getSelectModuleLineStr(int channel, int selMod, int lineNum, char *str)
{
    return robotPrgFileStructGlobal[channel].getSelectModuleLineStr(selMod, lineNum, str);
}

BOOLEAN createNewModule(int channel, module_prg_property modPrgPro)
{
    return robotPrgFileStructGlobal[channel].createNewModule(modPrgPro);
}

BOOLEAN createNewRoutine(int channel, int modNum, routine_prg_property routinePrgPro)
{
    return robotPrgFileStructGlobal[channel].createNewRoutine(modNum, routinePrgPro);
}

BOOLEAN insertNewLineStr(int channel, int modNum, int lineNum, const char *str)
{
    return robotPrgFileStructGlobal[channel].insertNewLineStr(modNum, lineNum, str);
}

int getModulePos(int channel, char *modName)
{
    return robotPrgFileStructGlobal[channel].getModulePos(modName);
}

int getRoutinePos(int channel, int modNum, char *routineName)
{
    return robotPrgFileStructGlobal[channel].getRoutinePos(modNum, routineName);
}

int createNewVariable(int channel, int modNum, int routineNum, char *str)
{
    return robotPrgFileStructGlobal[channel].createNewVariable(modNum, routineNum, str);
}

BOOLEAN deleteOneLineStr(int channel, int modNum, int lineNum)
{
    return robotPrgFileStructGlobal[channel].deleteOneLineStr(modNum, lineNum);
}

int getFileModuleCnt(int channel)
{
    return robotPrgFileStructGlobal[channel].getFileModuleCnt();
}

int getSelModRoutineCnt(int channel, int modNum)
{
    return robotPrgFileStructGlobal[channel].getSelModRoutineCnt(modNum);
}

BOOLEAN modifyOneLineStr(int channel, int modNum, int lineNum, const char *str)
{
    return robotPrgFileStructGlobal[channel].modifyOneLineStr(modNum, lineNum, str);
}

int modifyOneVariable(int channel, int modNum, int routineNum, char *varName, char *str)
{
    return robotPrgFileStructGlobal[channel].modifyOneVariable(modNum, routineNum, varName, str);
}

int deleteOneVariable(int channel, int modNum, int routineNum, char *varName)
{
    return robotPrgFileStructGlobal[channel].deleteOneVariable(modNum, routineNum, varName);
}

int usePosGetSelVarProperty(int channel, int modNum, int routineNum, int pos, int varType, decode_type_property *pro)
{
    return robotPrgFileStructGlobal[channel].usePosGetSelVarProperty(modNum, routineNum, pos, varType, pro);
}

int usePosGetSelVarVal(int channel, int modNum, int routineNum, int pos, int varType, decode_type_data *pro)
{
    return robotPrgFileStructGlobal[channel].usePosGetSelVarVal(modNum, routineNum, pos, varType, pro);
}

int useNameGetSelVarProperty(int channel, int modNum, int routineNum, char *varName, decode_type_property *pro)
{
    return robotPrgFileStructGlobal[channel].useNameGetSelVarProperty(modNum, routineNum, varName, pro);
}

int deleteSelRoutine(int channel, int modNum, int routineNum)
{
    return robotPrgFileStructGlobal[channel].deleteSelRoutine(modNum, routineNum);
}

int deleteSelModule(int channel, int modNum)
{
    return robotPrgFileStructGlobal[channel].deleteSelModule(modNum);
}

int getSelModuleProperty(int channel, int modNum, module_prg_property *modPro)
{
    return robotPrgFileStructGlobal[channel].getSelModuleProperty(modNum, modPro);
}

int modifySelModuleProperty(int channel, int modNum, module_prg_property modPro)
{
    return robotPrgFileStructGlobal[channel].modifySelModuleProperty(modNum, modPro);
}

int getSelRoutineProperty(int channel, int modNum, int routineNum, routine_prg_property *routinePro)
{
    return robotPrgFileStructGlobal[channel].getSelRoutineProperty(modNum, routineNum, routinePro);
}

int modifySelRoutineProperty(int channel, int modNum, int routineNum, routine_prg_property routinePro)
{
    return robotPrgFileStructGlobal[channel].modifySelRoutineProperty(modNum, routineNum, routinePro);
}

int isCanSelLineStr(int channel, int modNum, int lineNum)
{
    return robotPrgFileStructGlobal[channel].isCanSelLineStr(modNum, lineNum);
}

int isCanEditLineStr(int channel, int modNum, int lineNum)
{
    return robotPrgFileStructGlobal[channel].isCanEditLineStr(modNum, lineNum);
}

int getSelRoutineFirstLinePos(int channel, int modNum, int routineNum)
{
    return robotPrgFileStructGlobal[channel].getSelRoutineFirstLinePos(modNum, routineNum);
}

int transVarValueToStr(int type, char *str, decode_type_data *varVal)
{
    return decodeTypestructTransValToStr(type, str, varVal);
}

int getFileLineTotalCnt(int channel)
{
    return robotPrgFileStructGlobal[channel].getFileLineTotalCnt();
}

int getSelModuleFileLineTotalCnt(int channel, int modNum)
{
    return robotPrgFileStructGlobal[channel].getSelModuleFileLineTotalCnt(modNum);
}

int getSelRoutineFileLineTotalCnt(int channel, int modNum, int routineNum)
{
    return robotPrgFileStructGlobal[channel].getSelRoutineFileLineTotalCnt(modNum, routineNum);
}

int clearFileStructInfo(int channel)
{
    return robotPrgFileStructGlobal[channel].clearFileStructInfo();
}

int useNameGetSelVarData(int channel, int modNum, int routineNum, char *varName, decode_type_data *dataVal)
{
    return robotPrgFileStructGlobal[channel].useNameGetSelVarData(modNum, routineNum, varName, dataVal);
}



#include "robotprgfilestruct.h"
#include "robotdecodepublic.h"
#include <QFile>
#include <QtDebug>

#define PROGRAM_NAME    ("Program")
#define MODULE_NAME     ("Module")




RobotPrgFileStruct::RobotPrgFileStruct()
{
//    qDebug("fileStruct create");
}

RobotPrgFileStruct::~RobotPrgFileStruct()
{
#if 0
    int i = 0;
    int j = 0;
    int m = 0;
    int n = 0;
//    qDebug("fileStruct distroy start");
    while (i < prgFileInfo.modulePrg.size()) {
        while (j < prgFileInfo.modulePrg[i].routinePrg.size()) {
            while (m < prgFileInfo.modulePrg[i].routinePrg[j].lineStrData.size())
            {
//                delete prgFileInfo.modulePrg[i].routinePrg[j].lineStrData[m].str;
                m++;
            }
            prgFileInfo.modulePrg[i].routinePrg[j].lineStrData.clear();
            prgFileInfo.modulePrg[i].routinePrg.clear();
            j++;
        }

        while (n < prgFileInfo.modulePrg[i].lineStrData.size()) {
//          delete prgFileInfo.modulePrg[i].lineStrData[n].str;
            n++;
        }
         prgFileInfo.modulePrg[i].lineStrData.clear();
        i++;
    }
    prgFileInfo.modulePrg.clear();
//    qDebug("fileStruct distroy end");
#endif
}

bool RobotPrgFileStruct::readRobotPrgFile(const char *filePathName)
{
    QFile file(QObject::tr(filePathName));
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    QDomElement root;
    int len;
    char *tmpPtr = (char *)filePathName;
    char *lastPtr = tmpPtr;
    char *testPtr;

//    char filePathName[256];
//    strcpy(filePathName, filePath);
//    strcat(filePathName, fileName);
//    strcpy(prgFileInfo.fileName, fileName);
//    strcpy(prgFileInfo.filePath, filePath);


    if (!file.open(QFile::ReadOnly|QFile::Text)) {
        return false;
    }

    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        return false;
    }

    root = doc.documentElement();

    if (root.tagName()!= PROGRAM_NAME) {
        return false;
    }
//    tmpPtr=strchr(tmpPtr, '/');
    testPtr = tmpPtr;
//    while (((tmpPtr=strchr(tmpPtr, '\\')) != NULL)) { //注意此处要区分windows和linux路径
    while (((testPtr=strchr(tmpPtr, '\\')) != NULL) || ((testPtr=strchr(tmpPtr, '/')) != NULL)) { //注意此处要区分windows和linux路径
        tmpPtr = testPtr;
        tmpPtr++;//跳过'\\'字符
        lastPtr = tmpPtr;
    }
    clearFileStructInfo();
    len = lastPtr - filePathName;
    strncpy(prgFileInfo.filePath, filePathName, len);
    len = strlen(lastPtr);//去掉'\\'
    strncpy(prgFileInfo.fileName, lastPtr, len);    
    prgFileInfo.fileName[len] = 0;
    prgFileInfo.editFlag = 0;//标记没有编辑过
    prgFileInfo.modulePrg.clear();

    parseProgramElement(root);
    return true;

}

bool RobotPrgFileStruct::parseProgramElement(const QDomElement &element)
{
    QDomNode child = element.firstChild();

    while (!child.isNull()) {
//        qDebug("parseProgramElement start");
        if (child.toElement().tagName() == MODULE_NAME) {
            parseModuleElement(child.toElement());
        }
        child = child.nextSibling();
//        qDebug("parseProgramElement end");
    }
    return true;
}
#if 0
bool RobotPrgFileStruct::parseModuleElement(const QDomElement &element)
{
    QString moduleName = element.text();
    QByteArray tranStr = moduleName.toLatin1();
    char moduleFileName[256];
    char lineStrBuf[1024];
    int len = tranStr.length();
    QFile file;
    char *ptr = NULL;
//    int mainModule = 0;//是main主模块，则要放在module链表开头
//    int modulePos = -1;//当前模块
//    int routinePos = -1;//当前例行程序
//    int moduleLine = 0;
    program_pointer prgPointer = {-1, -1, -1, -1};
    int readCnt;
    int i = 0;
    int result;

//    qDebug()<<moduleName;

    module_prg_property modPrgPro;
    routine_prg_property routinePrgPro;

//    modulePrg.moduleName = new char[len + 1];

//    strcpy(modulePrg.moduleName, tranStr.data());
//    prgFileInfo.modulePrg.push_back(modulePrg);
    strcpy(moduleFileName, prgFileInfo.filePath);
    strcat(moduleFileName, tranStr.data());
    file.setFileName(moduleFileName);
    if (!file.open(QFile::ReadOnly|QFile::Text)) {
        return false;
    }

    while ((readCnt=file.readLine(lineStrBuf, sizeof(lineStrBuf))) != -1) {//读取一行
        ptr = lineStrBuf;
        while (*ptr != 0) {
            if (*ptr != ' ' && *ptr != '\t'){
                break;
            }
            ptr++;
        }
//        qDebug("read %d: %s", prgPointer.curModuleLine, ptr);
        if (decodeStringCompareEqual(ptr, "MODULE", strlen("MODULE")) == 1) {//模块开始
            ptr = &ptr[strlen("MODULE")];
            i = 0;
            while (*ptr != 0) {
                if (*ptr!=' ' || *ptr!='\t' || *ptr!='\r' || *ptr!='\n') {
                    modPrgPro.moduleName[i] = *ptr;
                    i++;
                }
                ptr++;
            }
            modPrgPro.moduleName[i] = 0;
            modPrgPro.moduleType = 0;
            prgPointer.curModule = prgFileInfo.modulePrg.size();
            prgPointer.curModuleLine = 0;
//            qDebug("mod create");
            result = createNewModule(modPrgPro);
//            qDebug("mod end");
//            qDebug("mod:%d", result);
            if (result == false) {
                break;
            }

//            modulePrg.lineStrData[i].str = new char[readCnt];
//            modulePrg.lineStrData[i].type =
//            strcpy(modulePrg.lineStrData[i].str, lineStrBuf);
        } else if (decodeStringCompareEqual(ptr, "PROC", strlen("PROC")) == 1) {//例行程序开始
            ptr = &ptr[strlen("PROC")];
            i = 0;
            while (*ptr != 0) {
                if (*ptr!=' ' && *ptr!='\t' && *ptr!='\r' && *ptr!='\n') {
                    if (*ptr == '(') {
                        break;
                    }
                    routinePrgPro.routineName[i] = *ptr;
                    i++;
                }
                ptr++;
            }
            routinePrgPro.routineName[i] = 0;
            routinePrgPro.routineType = 0;
            ptr = strchr(ptr, '(');
            strcpy(routinePrgPro.parameter, ptr);
//            qDebug("routine create");
            result = createNewRoutine(prgPointer.curModule, routinePrgPro);
//            qDebug("routine end");
//            qDebug("routine:%d", result);
            if (result == false) {
                break;
            }

            prgPointer.curModuleLine++;

        } else if (decodeStringCompareEqual(ptr, "ENDPROC", strlen("ENDPROC")) != NULL){//例行程序结束
            prgPointer.curModuleLine++;
        } else if (decodeStringCompareEqual(ptr, "ENDMODULE", strlen("ENDMODULE")) != NULL) {//模块程序结束
            break;
        } else { //插入一行
            result = insertNewLineStr(prgPointer.curModule, prgPointer.curModuleLine, 0, lineStrBuf);
//            qDebug("insert:%d", prgPointer.curModuleLine);
            prgPointer.curModuleLine++;
        }

    }

    file.close();
//    qDebug("mod parse end: %d", result);
    return (bool)result;

//    qDebug("%s", moduleFileName);


}
#else
bool RobotPrgFileStruct::parseModuleElement(const QDomElement &element)
{
    QString moduleName = element.text();
    QByteArray tranStr = moduleName.toLatin1();
    char moduleFileName[256];
    char lineStrBuf[1024];
    char segmentStr[256];
    int len = tranStr.length();
    QFile file;
    char *ptr = NULL;
//    int mainModule = 0;//是main主模块，则要放在module链表开头
//    int modulePos = -1;//当前模块
//    int routinePos = -1;//当前例行程序
//    int moduleLine = 0;
    program_pointer prgPointer = {-1, -1, -1, -1};
    int readCnt;
    int i = 0;
    int result;
    int scope = 0;//范围

//    qDebug()<<moduleName;

    module_prg_property modPrgPro;
    routine_prg_property routinePrgPro;
    decode_character_property charPro;

//    modulePrg.moduleName = new char[len + 1];

//    strcpy(modulePrg.moduleName, tranStr.data());
//    prgFileInfo.modulePrg.push_back(modulePrg);

    strcpy(moduleFileName, prgFileInfo.filePath);
    strcat(moduleFileName, tranStr.data());
    file.setFileName(moduleFileName);
    if (!file.open(QFile::ReadOnly|QFile::Text)) {
        return false;
    }

    while ((readCnt=file.readLine(lineStrBuf, sizeof(lineStrBuf))) != -1) {//读取一行
        ptr = lineStrBuf;
        while (ptr != NULL) {
           ptr = decodeGetOneSegment(ptr, segmentStr);
           if (ptr != NULL) {
                if (getOneCharacterIndex(0, segmentStr, &charPro) == 1) {//首次找到
                    if (charPro.charType == D_CHAR_TYPE_INSTRUCTION) {//指令
                        if (charPro.selfIndex == INSTR_MODULE) {//创建模块
                            ptr = decodeGetOneSegment(ptr, segmentStr);
                            strncpy(modPrgPro.moduleName, segmentStr, sizeof(modPrgPro.moduleName));
                            modPrgPro.moduleType = 0;//先设为程序0
                            result = createNewModule(modPrgPro);
                            //prgPointer.curModule++;//初始化时是-1，所以此处还是从0开始
                            prgPointer.curModule = prgFileInfo.modulePrg.size() - 1;//初始化时是-1，所以此处还是从0开始
                            prgPointer.curModuleLine += 1;//因为开始是-1，所以加1变为0
                        } else if (charPro.selfIndex == INSTR_PROC) {//注意：此处有待添加功能函数及中断函数
                             //prgPointer.curRoutine++;
                             i = 0;
                             ptr = decodeGetOneSegment(ptr, segmentStr);
                             while (segmentStr[i] != '(') {
                                 routinePrgPro.routineName[i] = segmentStr[i];
                                 i++;
                             }
                             routinePrgPro.routineName[i] = 0;
                             routinePrgPro.routineType = 0;//为例行程序
                             //以下要补充完整属性
                             //routinePrgPro.routineType;//例行程序类型procedure function trap
                             //routinePrgPro.routineFlag;//标志位，参看ABB用来标记local undo error backward
                             if (scope == 1) {//标记为local
                                routinePrgPro.routineFlag |= scope;
                             }
                             routinePrgPro.parentModule = prgPointer.curModule;//所属模块索引
                             strcpy(routinePrgPro.parameter, &segmentStr[i]);
                 //            qDebug("routine create");
                             result = createNewRoutine(prgPointer.curModule, routinePrgPro);
                //                             prgPointer.curModuleLine += 1;//创建一个例行程序需加2行，此处一行，下文还加1行
                             prgPointer.curRoutine++;
                        } else if (charPro.selfIndex == INSTR_ENDMODULE) {
//                            break;
                        } else if (charPro.selfIndex == INSTR_ENDPROC) {
//                            break;
                        } else {//插入指令
                            insertNewLineStr(prgPointer.curModule, prgPointer.curModuleLine, lineStrBuf);
                            prgPointer.curModuleLine++;
                            break;
                        }
                        prgPointer.curModuleLine++;//此处还有加1的
                    } else if (charPro.charType == D_CHAR_TYPE_VARTYPE) {//数据类型
                        if (charPro.selfIndex == D_DATATYPE_LOCAL) {
                            scope = 1;
                        } else {//创建变量
                            createNewVariable(prgPointer.curModule, prgPointer.curRoutine, lineStrBuf);
                            prgPointer.curModuleLine++;
                            break;
                        }

                    } else {//表达式
                        insertNewLineStr(prgPointer.curModule, prgPointer.curModuleLine, lineStrBuf);
                        prgPointer.curModuleLine++;
                        break;
                    }
                } else {
                    insertNewLineStr(prgPointer.curModule, prgPointer.curModuleLine, lineStrBuf);
                    prgPointer.curModuleLine++;
                    break;
                }
           }
        }
        scope = 0;
    }
    file.close();
    return (bool)result;
}
#endif

bool RobotPrgFileStruct::writeRobotPrgFile(const char *filePathName)
{
    QFile file(filePathName);
    int size;
    int i;

    if (!file.open(QFile::WriteOnly|QFile::Text)) {
        return false;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.setCodec("ISO 8859-1");
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement(PROGRAM_NAME);

    size = prgFileInfo.modulePrg.size();

    for (i=0; i<size; i++) {
        writeModuleElement(&xmlWriter, i);
    }
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();
    if (file.error()) {
        return false;
    }

    return true;

}

bool RobotPrgFileStruct::writeModuleElement(QXmlStreamWriter *xmlWriter, int modulePos)
{
    QString moduleName = prgFileInfo.modulePrg[modulePos].modulePrgPro.moduleName;
    QFile file(moduleName);
    int moduleLineCnt;
    int i = 0;
    int j = 0;
    int writeCnt;
    bool errFlag = false;

//    xmlWriter->writeStartElement(MODULE_NAME);
    if (!file.open(QFile::WriteOnly|QFile::Text)) {
        return false;
    }

    xmlWriter->writeTextElement(MODULE_NAME, moduleName);

    moduleLineCnt = prgFileInfo.modulePrg[modulePos].lineStrData.size() - 1;//先减1，最后一行模块结束先不写入

    while (i < moduleLineCnt) {//写入module中程序的size()-1行
//        writeCnt = strlen(prgFileInfo.modulePrg[modulePos].lineStrData[i].str);
//        writeCnt = file.write(prgFileInfo.modulePrg[modulePos].lineStrData[i].str, writeCnt);
        if (writeCnt == -1) {//写入出错
            errFlag = true;
            break;
        }
        i++;
    }

    while (j < prgFileInfo.modulePrg[modulePos].routinePrg.size()) {//写入模块中所有例行程序
        i = 0;
        while (i < prgFileInfo.modulePrg[modulePos].routinePrg[j].lineStrData.size()) {
//            writeCnt = strlen(prgFileInfo.modulePrg[modulePos].routinePrg[j].lineStrData[i].str);
//            writeCnt = file.write(prgFileInfo.modulePrg[modulePos].routinePrg[j].lineStrData[i].str, writeCnt);
            if (writeCnt == -1) {//写入出错
                errFlag = true;
                break;
            }
            i++;
        }
        j++;
    }

//    writeCnt = strlen(prgFileInfo.modulePrg[modulePos].lineStrData[moduleLineCnt].str);//写入模块最后一行结束
//    writeCnt = file.write(prgFileInfo.modulePrg[modulePos].lineStrData[moduleLineCnt].str, writeCnt);
    if (writeCnt == -1) {//写入出错
        errFlag = true;
    }

    file.close();

    return errFlag;

}

#if 0
program_pointer RobotPrgFileStruct::getSelectModuleLinePos(int selMod, int lineNum)
{
    program_pointer prgPointer = {-1, -1, -1, -1};
    int moduleLine;
    int tmpLine;
//    int routinePos = -1;
//    int routineLine = -1;
    int i = 0;

    if (prgFileInfo.modulePrg.size() > selMod) {
        moduleLine = prgFileInfo.modulePrg[selMod].lineStrData.size();
        if (moduleLine >= 2) {
            moduleLine -= 2;
        } else {
            if (moduleLine >= lineNum) {
               goto deal;
            } else {
                return prgPointer;
            }

        }

        if (moduleLine < lineNum) {
            while (i < prgFileInfo.modulePrg[selMod].routinePrg.size()) {
                tmpLine = moduleLine;
                moduleLine += prgFileInfo.modulePrg[selMod].routinePrg[i].lineStrData.size();
                if (moduleLine >= lineNum) {
                    prgPointer.curRoutine = i;
                    prgPointer.curRoutineLine = lineNum - tmpLine;
                    moduleLine = lineNum;
                    goto deal;
                }
                i++;
            }
//            if (i > 0) {
//                prgPointer.curRoutine = i - 1;
//                prgPointer.curRoutineLine = lineNum - tmpLine + 1;
//            }
            if (++moduleLine != lineNum) {
                return prgPointer;// 找完后都不满足行号
            }
        } else {
            if (moduleLine == lineNum) {
                i = prgFileInfo.modulePrg[selMod].routinePrg.size();
                if ((i > 0) && (prgFileInfo.modulePrg[selMod].routinePrg[i-1].lineStrData.size() == 0)) {
                    prgPointer.curRoutine = i - 1;
                    prgPointer.curRoutineLine = 0;
                }

            }
            moduleLine = lineNum;
        }
deal:
        prgPointer.curModule = selMod;
        prgPointer.curModuleLine = moduleLine;

    }

//    if (prgFileInfo.modulePrg.size() > 0) {
//        moduleLine = prgFileInfo.modulePrg[selMod].lineStrData.size() - 2;//先减去模块中最后一行MODULEEND，并且是要从0开始计数
//        if (prgFileInfo.modulePrg[selMod].routinePrg.size() > 0) {
//            while (moduleLine <= lineNum) {
//                if (prgFileInfo.modulePrg[selMod].routinePrg.size() == routinePos) {
//                    break;
//                }
//                routinePos++;
//                moduleLine += prgFileInfo.modulePrg[selMod].routinePrg[routinePos].lineStrData.size();
//            }
//        }

//        if (moduleLine < lineNum) {
//            if ((++moduleLine) != lineNum) {//模块随后一行都不满足，则报错
//                routinePos = -1;
//                lineNum = -1;
//                selMod = -1;
//            }
//        }

//        if (routinePos != -1) {
//            routineLine = moduleLine - lineNum;
//        }
//        prgPointer.curModuleLine = lineNum;
//        prgPointer.curRoutineLine = routineLine;
//        prgPointer.curModule = selMod;
//        prgPointer.curRoutine = routinePos;
//    }
    return prgPointer;
}
#else
program_pointer RobotPrgFileStruct::getSelectModuleLinePos(int selMod, int lineNum)
{
    program_pointer prgPointer = {-1, -1, -1, -1};
    int moduleLine;
    int tmpLine;
    int i = 0;

    if (prgFileInfo.modulePrg.size() > selMod) {
        moduleLine = prgFileInfo.modulePrg[selMod].lineStrData.size();
        if (moduleLine >= 2) {
            moduleLine -= 2;
        }
        if (moduleLine < lineNum) {
            while (i < prgFileInfo.modulePrg[selMod].routinePrg.size()) {
                tmpLine = moduleLine;
                moduleLine += prgFileInfo.modulePrg[selMod].routinePrg[i].lineStrData.size();
                if (moduleLine >= lineNum) {
                    prgPointer.curRoutine = i;
                    prgPointer.curRoutineLine = lineNum - tmpLine - 1;
                    moduleLine = lineNum;
                    goto deal;
                }
                i++;
            }

            if (++moduleLine != lineNum) {
                return prgPointer;// 找完后都不满足行号
            } else {
                moduleLine = prgFileInfo.modulePrg[selMod].lineStrData.size() - 1;//最后一行
            }
        } else {
            moduleLine = lineNum;
        }
deal:
        prgPointer.curModule = selMod;
        prgPointer.curModuleLine = moduleLine;

    }
    return prgPointer;
}
#endif


int RobotPrgFileStruct::getSelectModuleLineStr(int selMod, int lineNum, char *str)
{
    program_pointer prgPointer;
    int errFlag = -1;

    if (lineNum == 79) {
        lineNum = lineNum;
    }

    prgPointer = getSelectModuleLinePos(selMod, lineNum);
//    qDebug("line %d:%d %d %d %d", lineNum, prgPointer.curModule, prgPointer.curModuleLine, prgPointer.curRoutine, prgPointer.curRoutineLine);

    if (prgPointer.curModule != -1) {
        if (prgPointer.curRoutine != -1) {
            strcpy(str, prgFileInfo.modulePrg[selMod].routinePrg[prgPointer.curRoutine].lineStrData[prgPointer.curRoutineLine].str);
            errFlag = strlen(str);
        } else if (prgPointer.curModuleLine != -1){
            if (prgPointer.curModuleLine > prgFileInfo.modulePrg[selMod].lineStrData.size() - 1) {//模块内最后一行的处理
                prgPointer.curModuleLine = prgFileInfo.modulePrg[selMod].lineStrData.size() - 1;
            }
            strcpy(str, prgFileInfo.modulePrg[selMod].lineStrData[prgPointer.curModuleLine].str);
            errFlag = strlen(str);
        }
    }

    return errFlag;
}


bool RobotPrgFileStruct::createNewModule(module_prg_property modPrgPro)
{
    int i = 0;
    module_prg modulePrg;
//    line_str_data lineStrData;
    bool errFlag = FALSE;
    char str[256];
    decode_character_property charPro;
    if (getOneCharacterIndex(0, modPrgPro.moduleName, &charPro) == 1) {//如果已经存在同名，则返回失败
        return errFlag;
    }
    modPrgPro.moduleType = 0;
    modulePrg.modulePrgPro = modPrgPro;
    modulePrg.flag |= 0x1;//标记为有效使用

//    lineStrData.type = LINE_TYPE_MODULE;
//    lineStrData.str[0] = new char[strlen("MODULE")+1];
//    strcpy(lineStrData.str[0], "MODULE");
//    lineStrData.str[1] = new char[strlen(modPrgPro.moduleName)+1];
//    strcpy(lineStrData.str[1], modPrgPro.moduleName);
//    modulePrg.lineStrData.push_back(lineStrData);

//    lineStrData.type = LINE_TYPE_MODULEEND;
//    lineStrData.str[0] = new char[strlen("MODULEEND")+1];
//    strcpy(lineStrData.str[0], "MODULEEND");
//    modulePrg.lineStrData.push_back(lineStrData);

    prgFileInfo.modulePrg.push_back(modulePrg);
    strcpy(str, "MODULE ");
    strcat(str, modPrgPro.moduleName);
    strcat(str, "\n");
//    errFlag = insertNewLineStr(prgFileInfo.modulePrg.size()-1, 0, str);
    errFlag = appendLineStr(prgFileInfo.modulePrg.size()-1, -1, str);

//    lineStrData.str = new char[strlen(str) + 1];
//    lineStrData.type = LINE_TYPE_MODULE;
//    strcpy(lineStrData.str, str);
//    prgFileInfo.modulePrg[prgFileInfo.modulePrg.size()-1].lineStrData.push_back(lineStrData);
    strcpy(str, "ENDMODULE\n");
//    errFlag = insertNewLineStr(prgFileInfo.modulePrg.size()-1, 1, str);
     errFlag = appendLineStr(prgFileInfo.modulePrg.size()-1, -1, str);

    strcpy(charPro.charName, modPrgPro.moduleName);
    charPro.charType = D_CHAR_TYPE_MODNAME;
    charPro.selfIndex = INSTR_MODULE;
    charPro.scope1 = -1;
    charPro.scope2 = modPrgPro.moduleType;
    charPro.modParent = prgFileInfo.modulePrg.size()-1;
    charPro.routineParent = -1;
    createOneCharacterIndex(charPro);
    return errFlag;
}

bool RobotPrgFileStruct::createNewRoutine(int modNum, routine_prg_property routinePrgPro)
{
    routine_prg routinePrg;
    decode_character_property charPro;
    char str[256];//空格是必须的
    int i = 0;
    int moduleLine;
    bool errFlag = FALSE;

    while (getOneCharacterIndex(i, routinePrgPro.routineName, &charPro) == 1) {
        if ((modNum != charPro.modParent) && (charPro.scope2 == 2)//如果是不同模块且是局部的
           && ((charPro.charType == D_CHAR_TYPE_PRCNAME) || (charPro.charType == D_CHAR_TYPE_FUNNAME) || (charPro.charType == D_CHAR_TYPE_TRAPNAME))) {//且为如果是例行、功能、中断名

        } else {
            return errFlag;
        }
        i++;
    }
    //要放在压入新例行程序之前计算
    moduleLine = prgFileInfo.modulePrg[modNum].lineStrData.size() - 1;//要在倒数第一行插入，因此-1
    for (i = 0; i < prgFileInfo.modulePrg[modNum].routinePrg.size(); i++) {
        moduleLine += prgFileInfo.modulePrg[modNum].routinePrg[i].lineStrData.size();
    }

    routinePrg.routinePrgPro = routinePrgPro;
    routinePrg.flag |= 0x1;//标记为有效使用
    prgFileInfo.modulePrg[modNum].routinePrg.push_back(routinePrg);
    if ((routinePrgPro.routineFlag & 0xFF) == 1) {//如果是局部的
        sprintf(str, "%s", "LOCAL ");//空格是需要的
    }
    sprintf(str, "%s", "PROC ");//空格是需要的
    strcat(str, routinePrgPro.routineName);
//    strcat(str, "(");
    strcat(str, routinePrgPro.parameter);//已经包含了括号
    strcat(str, "\n");
//    strcat(str, ")");

//    qDebug("rot insert start");
//    errFlag = appendLineStr(modNum, prgFileInfo.modulePrg[modNum].routinePrg.size()-1, str);
//    qDebug("rot insert end 1");
//    errFlag = insertNewLineStr(modNum, moduleLine, str);
    errFlag = appendLineStr(modNum, prgFileInfo.modulePrg[modNum].routinePrg.size()-1, str);
//    insertNewLineStr(modNum, moduleLine+1, 0, "<SMT>");//暂时由外部插入
//    errFlag = appendLineStr(modNum, prgFileInfo.modulePrg[modNum].routinePrg.size()-1, "ENDPROC\n");
//    errFlag = insertNewLineStr(modNum, moduleLine + 1, "ENDPROC\n");
    errFlag = appendLineStr(modNum, prgFileInfo.modulePrg[modNum].routinePrg.size()-1, "ENDPROC\n");
//    typedef struct {//建立词法分析中所创建的所有字符快速索引表
//        char charName[32];//ABB允许最大是32字节
//        int charType:8;//decode_character_type中的类型
//        int selfIndex:8;//每种类型在自身表中的索引，如：指令和数据类型索引表
//        int scope1:8;//CONST PERS VAR
//        int scope2:8;//0:全局 1:TASK 2:LOCAL
//        int modParent:16;//所属模块索引编号，因为删除只做标记，所以给16位大些
//        int routineParent:16;//所属例行程序
//    }decode_character_property;
    strcpy(charPro.charName, routinePrgPro.routineName);
    charPro.charType = D_CHAR_TYPE_PRCNAME;
    charPro.selfIndex = INSTR_PROC;
    charPro.scope1 = -1;
    if ((routinePrgPro.routineFlag & 0xFF) == 1) {//如果是局部的
        charPro.scope2 = 2;//局部
    } else {
        charPro.scope2 = 0;//全局
    }

    charPro.modParent = modNum;
    charPro.routineParent = prgFileInfo.modulePrg[modNum].routinePrg.size() - 1;
    createOneCharacterIndex(charPro);
    return errFlag;
}

bool RobotPrgFileStruct::modifyOneLineStr(int modNum, int lineNum, const char *str)
{
    int i = 0;
    char *ptr = (char *)str;
    char segmentStr[256];
    line_str_data lineStrData;
    decode_character_property charPro;
    int scope = 0;

    program_pointer prgPointer = getSelectModuleLinePos(modNum, lineNum);


    if (prgPointer.curModule != -1) {
        while (ptr != NULL) {
            ptr = decodeGetOneSegment(ptr, segmentStr);
            if (getOneCharacterIndex(0, segmentStr, &charPro) == 1) {
                if (charPro.charType == D_CHAR_TYPE_INSTRUCTION) {//如果是指令
                    lineStrData.type = LINE_TYPE_INSTRUCTION;

                } else if (charPro.charType == D_CHAR_TYPE_VARTYPE) {//数据类型
                    if (charPro.selfIndex == D_DATATYPE_LOCAL) {
                        scope = 1;
                        continue;
                    } else {
                        lineStrData.type = LINE_TYPE_DATA;
                    }
                }
             } else {//表达式或注释
                lineStrData.type = LINE_TYPE_INSTRUCTION;
            }
            lineStrData.varType = charPro.selfIndex;
            lineStrData.scope = scope;
            lineStrData.tabCnt = 0;//此处tab数目暂时为0，后续会判断修改
            break;
        }
        lineStrData.str = new char[strlen(str) + 1];
        strcpy(lineStrData.str, str);
        if (prgPointer.curRoutine != -1) {
            delete prgFileInfo.modulePrg[modNum].routinePrg[prgPointer.curRoutine].lineStrData[prgPointer.curRoutineLine].str;
            memcpy(&prgFileInfo.modulePrg[modNum].routinePrg[prgPointer.curRoutine].lineStrData[prgPointer.curRoutineLine], &lineStrData, sizeof(line_str_data));
//            prgFileInfo.modulePrg[modNum].routinePrg[prgPointer.curRoutine].lineStrData.insert(prgPointer.curRoutineLine, lineStrData);
        } else {
            delete prgFileInfo.modulePrg[modNum].lineStrData[prgPointer.curModuleLine].str;
            memcpy(&prgFileInfo.modulePrg[modNum].lineStrData[prgPointer.curModuleLine], &lineStrData, sizeof(line_str_data));
//            prgFileInfo.modulePrg[modNum].lineStrData.insert(prgPointer.curModuleLine, lineStrData);
        }
    } else {
        return false;
    }

    return true;
}

bool RobotPrgFileStruct::deleteOneLineStr(int modNum, int lineNum)
{
    program_pointer prgPointer = getSelectModuleLinePos(modNum, lineNum);

    if (prgPointer.curModule != -1) {
        if (prgPointer.curRoutine != -1) {
            delete prgFileInfo.modulePrg[modNum].routinePrg[prgPointer.curRoutine].lineStrData[prgPointer.curRoutineLine].str;
            prgFileInfo.modulePrg[modNum].routinePrg[prgPointer.curRoutine].lineStrData.removeAt(prgPointer.curRoutineLine);
        } else {
            delete prgFileInfo.modulePrg[modNum].lineStrData[prgPointer.curModuleLine].str;
            prgFileInfo.modulePrg[modNum].lineStrData.removeAt(prgPointer.curModuleLine);
        }
    } else {
        return false;
    }

    return true;
}

bool RobotPrgFileStruct::insertNewLineStr(int modNum, int lineNum, const char *str)
{
#if 0
//    char *pStr[20];
    char *storeStr;
    int i = 0;
    int len;
    line_str_data lineStrData;

    program_pointer prgPointer = getSelectModuleLinePos(modNum, lineNum);


    if (prgPointer.curModule != -1) {
        lineStrData.str = new char[strlen(str) + 1];
        strcpy(lineStrData.str, str);
        if (prgPointer.curRoutine != -1) {
            if (direction == 0) {//向下插入
                prgPointer.curRoutineLine += 1;
//                if (prgPointer.curRoutineLine < 0) {
//                    prgPointer.curRoutineLine = 0;
//                }
                prgFileInfo.modulePrg[modNum].routinePrg[prgPointer.curRoutine].lineStrData.insert(prgPointer.curRoutineLine, lineStrData);
            }
        } else {
            if (direction == 0) {//向上插入
                prgPointer.curModuleLine += 1;
//                if (prgPointer.curModuleLine < 0) {
//                    prgPointer.curModuleLine = 0;
//                }
            }
            prgFileInfo.modulePrg[modNum].lineStrData.insert(prgPointer.curModuleLine, lineStrData);
        }
    } else {
        return false;
    }

    return true;
#else
//    char *pStr[20];
    int i = 0;
    char *ptr = (char *)str;
    char segmentStr[256];
    line_str_data lineStrData;
    decode_character_property charPro;
    int scope = 0;

    program_pointer prgPointer = getSelectModuleLinePos(modNum, lineNum);


    if (prgPointer.curModule != -1) {
        while (ptr != NULL) {
            ptr = decodeGetOneSegment(ptr, segmentStr);
            if (getOneCharacterIndex(0, segmentStr, &charPro) == 1) {
                if (charPro.charType == D_CHAR_TYPE_INSTRUCTION) {//如果是指令
                    lineStrData.type = LINE_TYPE_INSTRUCTION;

                } else if (charPro.charType == D_CHAR_TYPE_VARTYPE) {//数据类型
                    if (charPro.selfIndex == D_DATATYPE_LOCAL) {
                        scope = 1;
                        continue;
                    } else {
                        lineStrData.type = LINE_TYPE_DATA;
                    }
                }
             } else {//表达式或注释
                lineStrData.type = LINE_TYPE_INSTRUCTION;
            }
            lineStrData.varType = charPro.selfIndex;
            lineStrData.scope = scope;
            lineStrData.tabCnt = 0;//此处tab数目暂时为0，后续会判断修改
            break;
        }
        lineStrData.str = new char[strlen(str) + 1];
        strcpy(lineStrData.str, str);
        if (prgPointer.curRoutine != -1) {
            prgFileInfo.modulePrg[modNum].routinePrg[prgPointer.curRoutine].lineStrData.insert(prgPointer.curRoutineLine, lineStrData);
        } else {
            prgFileInfo.modulePrg[modNum].lineStrData.insert(prgPointer.curModuleLine, lineStrData);
        }
    } else {
        return false;
    }

    return true;
#endif
}

int RobotPrgFileStruct::getModulePos(char *modName)
{
    int i = 0;
    int result;

    while (i < prgFileInfo.modulePrg.size()) {
        result = decodeStringCompareEqual(prgFileInfo.modulePrg[i].modulePrgPro.moduleName, modName, strlen(modName));
        if (result == 1) {
            return i;
        }
        i++;
    }

    return -1;
}

int RobotPrgFileStruct::getRoutinePos(int modNum, char *routineName)
{
    int i, j;
    int result;

    i = 0;
    j = 0;

    while (i < prgFileInfo.modulePrg.size()) {
       while (j < prgFileInfo.modulePrg[i].routinePrg.size()) {
           result = decodeStringCompareEqual(prgFileInfo.modulePrg[i].routinePrg[j].routinePrgPro.routineName, routineName, strlen(routineName));
           if (result == 1) {
               return j;
           }
           j++;
       }
       i++;
    }
    return -1;
}

bool RobotPrgFileStruct::appendLineStr(int modNum, int routineNum, const char *str)
{
    line_str_data lineStrData;

    if (routineNum != -1) {
        lineStrData.str = new char[strlen(str) + 1];
        lineStrData.type = 0;
        strcpy(lineStrData.str, str);
        prgFileInfo.modulePrg[modNum].routinePrg[routineNum].lineStrData.append(lineStrData);
    } else {
        lineStrData.str = new char[strlen(str) + 1];
        lineStrData.type = 0;
        strcpy(lineStrData.str, str);
        prgFileInfo.modulePrg[modNum].lineStrData.append(lineStrData);
    }
    return true;
}

bool RobotPrgFileStruct::printModuleLineStrTest(int modNum)
{
    int i = 0;
    int j = 0;
    if (modNum < prgFileInfo.modulePrg.size()) {
        while (i < prgFileInfo.modulePrg[modNum].lineStrData.size()-1){
           qDebug("%s", prgFileInfo.modulePrg[modNum].lineStrData[i].str);
           i++;
        }
        i = 0;
        while (i < prgFileInfo.modulePrg[modNum].routinePrg.size()) {
            j = 0;
            while (j < prgFileInfo.modulePrg[modNum].routinePrg[i].lineStrData.size()) {
                qDebug("%s", prgFileInfo.modulePrg[modNum].routinePrg[i].lineStrData[j].str);
                j++;
            }
            i++;
        }
        qDebug("%s", prgFileInfo.modulePrg[modNum].lineStrData[prgFileInfo.modulePrg[modNum].lineStrData.size()-1].str);

    }
}

int RobotPrgFileStruct::createNewVariable(int modNum, int routineNum, char *str)//该函数要完善不同类型错误返回码
{
    decode_type_property typePro;
    decode_character_property charPro;
    robot_variable robotVar;
    decode_type_data typeData;
//    decode_robtarget *robTarget;//测试用
//    decode_tooldata *toolData;//测试用
    decode_wobjdata *wobjData;//测试用
    char testStr[256];//测试用
    char tempStr[256];
    char lineStr[1024];
    char *ptr = str;
    int i = 0;
    int j = 0;
    int pos;
    int modLine;

//    ptr = decodeGetOneSegment(ptr, tempStr);
//    if (getOneCharacterIndex(0, tempStr, &charPro) == 1) {
//        if

//    }
    memset(&typePro, 0, sizeof(decode_type_property));//此处清零有必要
    while (ptr != NULL) {
        ptr = decodeGetOneSegment(ptr, tempStr);
        if (getOneCharacterIndex(0, tempStr, &charPro) == 1) {
            if (charPro.charType == D_CHAR_TYPE_VARTYPE) {
                if ((charPro.selfIndex == D_DATATYPE_LOCAL) || (charPro.selfIndex == D_DATATYPE_TASK)) {
                    typePro.scope = charPro.selfIndex;//存储范围
                } else if ((charPro.selfIndex == D_DATATYPE_VAR) || (charPro.selfIndex == D_DATATYPE_CONST) || (charPro.selfIndex == D_DATATYPE_PERS)) {
                    typePro.storeType = charPro.selfIndex;//存储类型
                } else {//变量类型
                    robotVar.varType = charPro.selfIndex;
                    if (charPro.selfIndex == D_DATATYPE_NUM) {//如果是num类型，要根据是否有小数点区分为double和int
                        if (strchr(str, '.') != NULL) {
                            robotVar.varType = D_DATATYPE_NUM_D;//浮点
                        } else {
                            robotVar.varType = D_DATATYPE_NUM_I;//整型
                        }
                    }

                    strcat(lineStr, tempStr);
                    strcat(lineStr, " ");
                    break;
                }
            } else {
                //此处要报输入字符串非法返回
                return -1;
            }
            sprintf(lineStr, "%s", tempStr);
            strcat(lineStr, " ");
        }
    }

    while (*ptr != ';') {//此处暂时没有处理数组的
        if ((*ptr != ' ') && (*ptr != '\t')) {
            tempStr[i] = *ptr;
            if (*ptr == ':') {//开始获取变量名
                strncpy(typePro.typeName, tempStr, i);                
                //此处要添加变量重名的问题
                while (getOneCharacterIndex(j, tempStr, &charPro) == 1) {
                    if (charPro.charType == D_CHAR_TYPE_VARNAME) {//如果是变量名
//                        if ((charPro.modParent != modNum) && ((typePro.scope != charPro.scope2) || ((typePro.scope == charPro.scope2) && (typePro.scope == D_SCOPE_LOCAL))) ) {

//                        } else if ((charPro.modParent == modNum) && ()) {
                        if (((typePro.scope != charPro.scope2) || ((typePro.scope == charPro.scope2) && (typePro.scope == D_SCOPE_LOCAL))) ) {

                        } else {//重命名
                            return -1;
                        }
                    } else {
                        return -1;
                    }
                    j++;
                }
            } else if (*ptr == '=') { //第一次出现=，表明数据域开始
                pos = i + 1;
            }
            i++;
        }
        ptr++;
    }
    tempStr[i] = ';';
    tempStr[i+1] = '\n';
    tempStr[i+2] = 0;
    strcat(lineStr, tempStr);
    typePro.parentModule = modNum;
    typePro.parentRoutine = routineNum;
    typePro.parentTask = 0;//默认设置为0通道
    robotVar.typePro = typePro;
    robotVar.valIndex = decodeTypestructMallocDataStoreBuf();//申请的下标索引
    ptr = &tempStr[pos];
    qDebug("var:%s", ptr);
    decodeTypestructScanfVal(robotVar.varType, &ptr, &typeData);
//    robTarget = (decode_robtarget *)&typeData;//测试用
//    toolData = (decode_tooldata *)&typeData;//测试用

    transVarValueToStr(robotVar.varType, testStr, &typeData);
    qDebug("val:%s", testStr);
    wobjData = (decode_wobjdata *)&typeData;//测试用
    decodeTypestructSetData(robotVar.valIndex, typeData);
    i = 0;
    modLine = prgFileInfo.modulePrg[modNum].lineStrData.size() - 1;//去掉末尾那行
    if (routineNum != -1) {
         while (i < routineNum) {//在该例行程序之前
            modLine +=  prgFileInfo.modulePrg[modNum].routinePrg[i].lineStrData.size();
            i++;
        }
        modLine += 1;//加1行是例行程序开始PROC那行
        i = 0;
        while (i <= robotVar.varType) {//到该类型的末尾
            modLine += prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[i].robotVar.size();
            i++;
        }
        prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[robotVar.varType].robotVar.push_back(robotVar);
    } else {        
        i = 0;
        modLine = 1;//模块开头的第一行
        while (i <= robotVar.varType) {//到该类型的末尾
            modLine += prgFileInfo.modulePrg[modNum].robotVarArry[i].robotVar.size();
            i++;
        }
        prgFileInfo.modulePrg[modNum].robotVarArry[robotVar.varType].robotVar.push_back(robotVar);
    }
    insertNewLineStr(modNum, modLine, lineStr);
    strcpy(charPro.charName, typePro.typeName);
    charPro.charType = D_CHAR_TYPE_VARNAME;
    charPro.selfIndex = robotVar.varType;// typePro.storeType;
    charPro.scope1 = typePro.storeType;//存储类型
    charPro.scope2 = typePro.scope;//存储范围
    charPro.modParent = modNum;
    charPro.routineParent = routineNum;
    charPro.valPos  = robotVar.valIndex;//记录值下标索引，只有变量才用
    createOneCharacterIndex(charPro);
//    if (robotVar.varType == D_DATATYPE_TOOLDATA) {//测试用，校验值转成字符串
//        decodeTypestructTransValToStr(D_DATATYPE_TOOLDATA, lineStr, (decode_type_data *)toolData);
//    }
    return 1;
}

int RobotPrgFileStruct::modifyOneVariable(int modNum, int routineNum, char *varName, char *str)//该函数要完善不同类型错误返回码
{
    decode_type_property typePro;
    decode_character_property charPro;
    robot_variable robotVar;
    decode_type_data typeData;
//    decode_robtarget *robTarget;//测试用
    decode_tooldata *toolData;//测试用
    char tempStr[256];
    char lineStr[1024];
    char *ptr = str;
    int i = 0;
    int j = 0;
    int pos;
    int modLine;

//    ptr = decodeGetOneSegment(ptr, tempStr);
//    if (getOneCharacterIndex(0, tempStr, &charPro) == 1) {
//        if

//    }
    while (ptr != NULL) {
        ptr = decodeGetOneSegment(ptr, tempStr);
        if (getOneCharacterIndex(0, tempStr, &charPro) == 1) {
            if (charPro.charType == D_CHAR_TYPE_VARTYPE) {
                if ((charPro.selfIndex == D_DATATYPE_LOCAL) || (charPro.selfIndex == D_DATATYPE_TASK)) {
                    typePro.scope = charPro.selfIndex;
                } else if ((charPro.selfIndex == D_DATATYPE_VAR) || (charPro.selfIndex == D_DATATYPE_CONST) || (charPro.selfIndex == D_DATATYPE_PERS)) {
                    typePro.storeType = charPro.selfIndex;
                } else {//变量类型
                    robotVar.varType = charPro.selfIndex;
                    strcat(lineStr, tempStr);
                    strcat(lineStr, " ");
                    break;
                }
            } else {
                //此处要报输入字符串非法返回
                return -1;
            }
            sprintf(lineStr, "%s", tempStr);
            strcat(lineStr, " ");
        }
    }

    while (*ptr != ';') {//此处暂时没有处理数组的
        if ((*ptr != ' ') && (*ptr != '\t')) {
            tempStr[i] = *ptr;
            if (*ptr == ':') {//开始获取变量名
                strncpy(typePro.typeName, tempStr, i);
                //此处要添加变量重名的问题
                while (getOneCharacterIndex(j, tempStr, &charPro) == 1) {
                    if (charPro.charType == D_CHAR_TYPE_VARNAME) {//如果是变量名
//                        if ((charPro.modParent != modNum) && ((typePro.scope != charPro.scope2) || ((typePro.scope == charPro.scope2) && (typePro.scope == D_SCOPE_LOCAL))) ) {

//                        } else if ((charPro.modParent == modNum) && ()) {
                        if (((typePro.scope != charPro.scope2) || ((typePro.scope == charPro.scope2) && (typePro.scope == D_SCOPE_LOCAL))) ) {

                        } else {
                            return -1;
                        }
                    } else {
                        return -1;
                    }
                    j++;
                }
            } else if (*ptr == '=') { //第一次出现=，表明数据域开始
                pos = i + 1;
            }
            i++;
        }
        ptr++;
    }
    tempStr[i] = ';';
    tempStr[i+1] = '\n';
    tempStr[i+2] = 0;
    strcat(lineStr, tempStr);
    typePro.parentModule = modNum;
    typePro.parentRoutine = routineNum;
    typePro.parentTask = 0;//默认设置为0通道
    robotVar.typePro = typePro;
    robotVar.valIndex = decodeTypestructMallocDataStoreBuf();//申请的下标索引
    ptr = &tempStr[pos];
    decodeTypestructScanfVal(robotVar.varType, &ptr, &typeData);
//    robTarget = (decode_robtarget *)&typeData;//测试用
    toolData = (decode_tooldata *)&typeData;//测试用
    decodeTypestructSetData(robotVar.valIndex, typeData);
    i = 0;
    modLine = prgFileInfo.modulePrg[modNum].lineStrData.size() - 1;//去掉末尾那行
    if (routineNum != -1) {
         while (i < routineNum) {//在该例行程序之前
            modLine +=  prgFileInfo.modulePrg[modNum].routinePrg[i].lineStrData.size();
            i++;
        }
        modLine += 1;//加1行是例行程序开始PROC那行
        i = 0;
        while (i <= robotVar.varType) {//到该类型的末尾
            modLine += prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[i].robotVar.size();
            i++;
        }
        prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[robotVar.varType].robotVar.push_back(robotVar);
    } else {
        i = 0;
        modLine = 1;//模块开头的第一行
        while (i <= robotVar.varType) {//到该类型的末尾
            modLine += prgFileInfo.modulePrg[modNum].robotVarArry[i].robotVar.size();
            i++;
        }
        prgFileInfo.modulePrg[modNum].robotVarArry[robotVar.varType].robotVar.push_back(robotVar);
    }
    insertNewLineStr(modNum, modLine, lineStr);
    strcpy(charPro.charName, typePro.typeName);
    charPro.charType = D_CHAR_TYPE_VARNAME;
    charPro.selfIndex = typePro.storeType;
    charPro.scope1 = typePro.scope;
    charPro.scope2 = typePro.storeType;
    charPro.modParent = modNum;
    charPro.routineParent = routineNum;
    createOneCharacterIndex(charPro);
    return 1;
}

int RobotPrgFileStruct::deleteOneVariable(int modNum, int routineNum, char *varName)//该函数要完善不同类型错误返回码
{
    decode_character_property charPro;
    int i = 0;
    int j = 0;

    if (modNum < prgFileInfo.modulePrg.size()) {
        while (getOneCharacterIndex(i, varName, &charPro) == 1) {
            if ((charPro.charType == D_CHAR_TYPE_VARNAME) && (charPro.modParent == modNum) && (charPro.routineParent ==routineNum)) {//如果是变量名
                if (routineNum != -1) {
                    while (j < prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[charPro.selfIndex].robotVar.size()) {//在该类型内
                        if (decodeStringCompare(prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[charPro.selfIndex].robotVar[j].typePro.typeName, varName, 0) == 0) {//如果名称相等
                            decodeTypestructDeleteDataStoreBuf(j);//释放缓冲
                            prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[charPro.selfIndex].robotVar.removeAt(j);//从队列中移除
                            delete [] prgFileInfo.modulePrg[modNum].routinePrg[routineNum].lineStrData[j+1].str;//需要j+1，因为例行程序开始有一行proc
                            prgFileInfo.modulePrg[modNum].routinePrg[routineNum].lineStrData.removeAt(j);//删除该行
                        }
                        j++;
                    }

                } else {
                    while (j < prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar.size()) {//在该类型内
                        if (decodeStringCompare(prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar[j].typePro.typeName, varName, 0) == 0) {//如果名称相等
                            decodeTypestructDeleteDataStoreBuf(j);//释放缓冲
                            prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar.removeAt(j);//从队列中移除
                            delete [] prgFileInfo.modulePrg[modNum].lineStrData[j+1].str;//需要j+1，因为例行程序开始有一行proc
                            prgFileInfo.modulePrg[modNum].lineStrData.removeAt(j);//删除该行
                        }
                        j++;
                    }
                }
                deleteOneCharacterIndex(charPro);//删除一个记录索引
                return 1;
            }
            i++;
        }
    }

    return -1;
}


int RobotPrgFileStruct::getFileModuleCnt(void)
{
    int i = 0;
    int cnt = 0;

    while (i < prgFileInfo.modulePrg.size()) {
        if ((prgFileInfo.modulePrg[i].flag & 0x1) != 0) {//表明标记有效
            cnt++;
        }
        i++;
    }
    return cnt;
}

int RobotPrgFileStruct::getSelModRoutineCnt(int modNum)
{
    int i = 0;
    int cnt = 0;

    if (modNum < prgFileInfo.modulePrg.size()) {
        while (i < prgFileInfo.modulePrg[modNum].routinePrg.size()) {
            if ((prgFileInfo.modulePrg[modNum].routinePrg[i].flag & 0x01) != 0) {
                cnt++;
            }
            i++;
        }
    }
    return cnt;
}

int RobotPrgFileStruct::usePosGetSelVarProperty(int modNum, int routineNum, int pos, int varType, decode_type_property *pro)
{
    int errFlag = -1;
    if (modNum < prgFileInfo.modulePrg.size()) {
        if ((routineNum != -1) && (routineNum < prgFileInfo.modulePrg[modNum].routinePrg.size())) {
            if (pos < prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[varType].robotVar.size()) {
                memcpy(pro, &prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[varType].robotVar[pos].typePro, sizeof(decode_type_property));
                errFlag = 1;
            }
        } else {
            if (pos < prgFileInfo.modulePrg[modNum].robotVarArry[varType].robotVar.size()) {
                memcpy(pro, &prgFileInfo.modulePrg[modNum].robotVarArry[varType].robotVar[pos].typePro, sizeof(decode_type_property));
                errFlag = 1;
            }
        }
    }
    return errFlag;
}

int RobotPrgFileStruct::usePosGetSelVarVal(int modNum, int routineNum, int pos, int varType, decode_type_data *dataVal)
{
    int errFlag = -1;
    int valPos;

    if (modNum < prgFileInfo.modulePrg.size()) {
        if (routineNum != -1) {
            valPos = prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[varType].robotVar[pos].valIndex;
        } else {
            valPos = prgFileInfo.modulePrg[modNum].robotVarArry[varType].robotVar[pos].valIndex;
        }
        errFlag = decodeTypestructGetData(valPos, dataVal);
    }
    return errFlag;
}

int RobotPrgFileStruct::useNameGetSelVarData(int modNum, int routineNum, char *varName, decode_type_data *dataVal)
{
    int errFlag = -1;
    int pos = 0;
    int i = 0;
    int valPos;
    decode_character_property charPro;

    if (modNum < prgFileInfo.modulePrg.size()) {
        while (1) {
            if (getOneCharacterIndex(pos, varName, &charPro) == 1) {
                if (charPro.charType == D_CHAR_TYPE_VARNAME) {//如果是变量名
                    if ((charPro.modParent == modNum) && (charPro.routineParent == routineNum)) {
                        if (routineNum != -1) {
                            while (i < prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar.size()) {
                                if (decodeStringCompare(varName, prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[charPro.selfIndex].robotVar[i].typePro.typeName,0) == 0) {//找到名称相同
                                    break;
                                }
                                i++;
                            }
                            valPos = prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[charPro.selfIndex].robotVar[i].valIndex;
//                            memcpy(&pro, &prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[charPro.selfIndex].robotVar[i].typePro, sizeof(decode_type_property));
                        } else {
                            while (i < prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar.size()) {
                                if (decodeStringCompare(varName, prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar[i].typePro.typeName,0) == 0) {//找到名称相同
                                    break;
                                }
                                i++;
                            }
                            valPos = prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar[i].valIndex;
//                            memcpy(&pro, &prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar[i].typePro, sizeof(decode_type_property));
                        }
                        errFlag = decodeTypestructGetData(valPos, dataVal);
                    }
                }

            } else {
                break;
            }
            pos++;
        }
    }
    return errFlag;
}

int RobotPrgFileStruct::useNameSetSelVarData(int modNum, int routineNum, char *varName, decode_type_data dataVal)
{
    int errFlag = -1;
    int pos = 0;
    int i = 0;
    int valPos;
    decode_character_property charPro;

    if (modNum < prgFileInfo.modulePrg.size()) {
        while (1) {
            if (getOneCharacterIndex(pos, varName, &charPro) == 1) {
                if (charPro.charType == D_CHAR_TYPE_VARNAME) {//如果是变量名
                    if ((charPro.modParent == modNum) && (charPro.routineParent == routineNum)) {
                        if (routineNum != -1) {
                            while (i < prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar.size()) {
                                if (decodeStringCompare(varName, prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[charPro.selfIndex].robotVar[i].typePro.typeName,0) == 0) {//找到名称相同
                                    break;
                                }
                                i++;
                            }
                            valPos = prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[charPro.selfIndex].robotVar[i].valIndex;
//                            memcpy(&pro, &prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[charPro.selfIndex].robotVar[i].typePro, sizeof(decode_type_property));
                        } else {
                            while (i < prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar.size()) {
                                if (decodeStringCompare(varName, prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar[i].typePro.typeName,0) == 0) {//找到名称相同
                                    break;
                                }
                                i++;
                            }
                            valPos = prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar[i].valIndex;
//                            memcpy(&pro, &prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar[i].typePro, sizeof(decode_type_property));
                        }
//                        errFlag = decodeTypestructGetData(valPos, pro);
                        errFlag = decodeTypestructSetData(valPos, dataVal);
                    }
                }

            } else {
                break;
            }
            pos++;
        }
    }
    return errFlag;
}

int RobotPrgFileStruct::useNameGetSelVarProperty(int modNum, int routineNum, char *varName, decode_type_property *pro)
{
    int errFlag = -1;
    int pos = 0;
    int i = 0;
    decode_character_property charPro;

    if (modNum < prgFileInfo.modulePrg.size()) {
        while (1) {
            if (getOneCharacterIndex(pos, varName, &charPro) == 1) {
                if (charPro.charType == D_CHAR_TYPE_VARNAME) {//如果是变量名
                    if ((charPro.modParent == modNum) && (charPro.routineParent == routineNum)) {
                        if (routineNum != -1) {
                            while (i < prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar.size()) {
                                if (decodeStringCompare(varName, prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[charPro.selfIndex].robotVar[i].typePro.typeName,0) == 0) {//找到名称相同
                                    break;
                                }
                                i++;
                            }
                            memcpy(pro, &prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[charPro.selfIndex].robotVar[i].typePro, sizeof(decode_type_property));
                        } else {
                            while (i < prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar.size()) {
                                if (decodeStringCompare(varName, prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar[i].typePro.typeName,0) == 0) {//找到名称相同
                                    break;
                                }
                                i++;
                            }
                            memcpy(pro, &prgFileInfo.modulePrg[modNum].robotVarArry[charPro.selfIndex].robotVar[i].typePro, sizeof(decode_type_property));
                        }
                        errFlag = 1;
                    }
                }

            } else {
                break;
            }
            pos++;
        }
    }
    return errFlag;
}


int RobotPrgFileStruct::deleteSelRoutine(int modNum, int routineNum)
{
    decode_character_property charPro;
    int errFlag = -1;
    int i = 0;
    int j = 0;
    int m = 0;

    if (modNum < prgFileInfo.modulePrg.size()) {
        if (routineNum < prgFileInfo.modulePrg[modNum].routinePrg.size()) {
            prgFileInfo.modulePrg[modNum].routinePrg[routineNum].parameterStruct.clear();
            i = D_DATATYPE_END_CNT - 1;//最后一个元素
            while (i >= 0) {//要倒着删才行，删除变量
                while (j < prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[i].robotVar.size()) {
                    while (getOneCharacterIndex(m, prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[i].robotVar[j].typePro.typeName, &charPro) == 1) {
                        decodeTypestructDeleteDataStoreBuf(prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[i].robotVar[j].valIndex);//释放空间
                        if ((charPro.charType == D_CHAR_TYPE_VARNAME) && (charPro.modParent == modNum) && (charPro.routineParent ==routineNum)) {//如果是变量名
                            deleteOneCharacterIndex(charPro);//删除一个记录索引
                        }
                        m++;
                    }
                    j++;
                    m = 0;
                }
                prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[i].robotVar.clear();
                j = 0;
                i--;
            }
            i = prgFileInfo.modulePrg[modNum].routinePrg[routineNum].lineStrData.size() - 1;
            while (i >= 0) {//删除程序行
                delete []prgFileInfo.modulePrg[modNum].routinePrg[routineNum].lineStrData[i].str;
                prgFileInfo.modulePrg[modNum].routinePrg[routineNum].lineStrData.removeAt(i);
                i--;
            }
            prgFileInfo.modulePrg[modNum].routinePrg[routineNum].flag &= 0xFFFFFFFE;//标记为无效
            prgFileInfo.modulePrg[modNum].routinePrg[routineNum].parameterStruct.clear();//释放形参
//            prgFileInfo.modulePrg[modNum].routinePrg.removeAt(routineNum);//为了保持下标不变，并不移除，需要做标记
            errFlag = 1;
        }

    }
    return errFlag;
}

int RobotPrgFileStruct::deleteSelModule(int modNum)
{
    decode_character_property charPro;
    int errFlag = -1;
    int i = 0;
    int j = 0;
    int m = 0;

    if (modNum < prgFileInfo.modulePrg.size()) {
        i = prgFileInfo.modulePrg[modNum].routinePrg.size() - 1;
        while (i >= 0) {//要倒着删除
            deleteSelRoutine(modNum, i);//因为删除例行程序不去除本身
            prgFileInfo.modulePrg[modNum].routinePrg.removeAt(i);//如果删除模块，则例行程序的空间要释放
            i--;
        }
        i = D_DATATYPE_END_CNT - 1;
        while (i >= 0) {//删除变量
            while (j < prgFileInfo.modulePrg[modNum].robotVarArry[i].robotVar.size()) {//逐个删除
                decodeTypestructDeleteDataStoreBuf(prgFileInfo.modulePrg[modNum].robotVarArry[i].robotVar[j].valIndex);//释放空间
                while (getOneCharacterIndex(m, prgFileInfo.modulePrg[modNum].robotVarArry[i].robotVar[j].typePro.typeName, &charPro) == 1) {
                    if ((charPro.charType == D_CHAR_TYPE_VARNAME) && (charPro.modParent == modNum)) {//如果是变量名，且模块匹配
                        deleteOneCharacterIndex(charPro);//删除一个记录索引
                        break;//删除记录后即刻退出
                    }
                    m++;
                }
                j++;
                m = 0;
            }
            prgFileInfo.modulePrg[modNum].robotVarArry[i].robotVar.clear();
            j = 0;
            i--;
        }
        i = prgFileInfo.modulePrg[modNum].lineStrData.size() - 1;
        while (i >= 0) {//删除程序行
            delete []prgFileInfo.modulePrg[modNum].lineStrData[i].str;
            prgFileInfo.modulePrg[modNum].lineStrData.removeAt(i);
            i--;
        }
        prgFileInfo.modulePrg[modNum].flag &= 0xFFFFFFFE;//标记为无效
//        prgFileInfo.modulePrg.removeAt(modNum);//为了保持下标不变，并不移除，需要做标记
        errFlag = 1;
    }
    return errFlag;
}

int RobotPrgFileStruct::getSelModuleProperty(int modNum, module_prg_property *modPro)
{
    int errFlag = -1;

    if (modNum < prgFileInfo.modulePrg.size()) {
        memcpy(modPro, &prgFileInfo.modulePrg[modNum].modulePrgPro, sizeof(module_prg_property));
        errFlag = 1;
    }
    return errFlag;
}

int RobotPrgFileStruct::useNameGetModuleProperty(char *modName, module_prg_property *modPro)
{
    int errFlag = -1;
    int pos = 0;
    int modNum;


    decode_character_property charPro;

    while (1) {
        if (getOneCharacterIndex(pos, modName, &charPro) == 1) {
            if (charPro.charType == D_CHAR_TYPE_MODNAME) {
                modNum = charPro.modParent;
                memcpy(modPro, &prgFileInfo.modulePrg[modNum].modulePrgPro, sizeof(module_prg_property));
                errFlag = 1;
                break;
            }
        }
        pos++;
    }
    return errFlag;
}

int RobotPrgFileStruct::modifySelModuleProperty(int modNum, module_prg_property modPro)
{
    int errFlag = -1;

    if (modNum < prgFileInfo.modulePrg.size()) {
        memcpy(&prgFileInfo.modulePrg[modNum].modulePrgPro, &modPro, sizeof(module_prg_property));
        errFlag = 1;
    }
    return errFlag;
}


int RobotPrgFileStruct::useNameGetRoutineProperty(char *routineName, routine_prg_property *routinePro)
{
    int errFlag = -1;
    int pos = 0;
    int modNum;
    int routineNum;

    decode_character_property charPro;

    while (1) {
        if (getOneCharacterIndex(pos, routineName, &charPro) == 1) {
            if (charPro.charType == D_CHAR_TYPE_PRCNAME) {
                modNum = charPro.modParent;
                routineNum = charPro.routineParent;
                memcpy(routinePro, &prgFileInfo.modulePrg[modNum].routinePrg[routineNum].routinePrgPro, sizeof(routine_prg_property));
                errFlag = 1;
                break;
            }
        }
        pos++;
    }
    return errFlag;
}


int RobotPrgFileStruct::getSelRoutineProperty(int modNum, int routineNum, routine_prg_property *routinePro)
{
    int errFlag = -1;

    if (modNum < prgFileInfo.modulePrg.size()) {
        if (routineNum < prgFileInfo.modulePrg[modNum].routinePrg.size()) {
            memcpy(routinePro, &prgFileInfo.modulePrg[modNum].routinePrg[routineNum].routinePrgPro, sizeof(routine_prg_property));
            errFlag = 1;
        }
    }
    return errFlag;
}

int RobotPrgFileStruct::modifySelRoutineProperty(int modNum, int routineNum, routine_prg_property routinePro)
{
    int errFlag = -1;

    if (modNum < prgFileInfo.modulePrg.size()) {
        if (routineNum < prgFileInfo.modulePrg[modNum].routinePrg.size()) {
            memcpy(&prgFileInfo.modulePrg[modNum].routinePrg[routineNum].routinePrgPro, &routinePro, sizeof(routine_prg_property));
            errFlag = 1;
        }
    }
    return errFlag;
}

int RobotPrgFileStruct::isCanEditLineStr(int modNum, int lineNum)
{
    int errFlag = -1;
    program_pointer prgPointer;
    line_str_data lineStrData;

    prgPointer = getSelectModuleLinePos(modNum, lineNum);

    if (prgPointer.curModule != -1) {
        if (prgPointer.curRoutine != -1) {
            lineStrData = prgFileInfo.modulePrg[prgPointer.curModule].routinePrg[prgPointer.curRoutine].lineStrData[prgPointer.curRoutineLine];
        } else {
            lineStrData = prgFileInfo.modulePrg[prgPointer.curModule].lineStrData[prgPointer.curModuleLine];
        }
        if (lineStrData.type == LINE_TYPE_INSTRUCTION) {
            if ((lineStrData.varType != INSTR_MODULE) && (lineStrData.varType != INSTR_ENDMODULE) && (lineStrData.varType != INSTR_PROC) && (lineStrData.varType != INSTR_ENDPROC)) {//暂时是这几种，后续要补充
               errFlag = 1;
            }
        }
    }
    return errFlag;
}

int RobotPrgFileStruct::isCanSelLineStr(int modNum, int lineNum)
{
    int errFlag = 1;
    program_pointer prgPointer;
    line_str_data lineStrData;

    prgPointer = getSelectModuleLinePos(modNum, lineNum);

    if (prgPointer.curModule != -1) {
        if (prgPointer.curRoutine != -1) {
            lineStrData = prgFileInfo.modulePrg[prgPointer.curModule].routinePrg[prgPointer.curRoutine].lineStrData[prgPointer.curRoutineLine];
        } else {
            lineStrData = prgFileInfo.modulePrg[prgPointer.curModule].lineStrData[prgPointer.curModuleLine];
        }
        if (lineStrData.type == LINE_TYPE_INSTRUCTION) {
            if ((lineStrData.varType == INSTR_MODULE) || (lineStrData.varType == INSTR_ENDMODULE) || (lineStrData.varType == INSTR_PROC) || (lineStrData.varType == INSTR_ENDPROC)) {//暂时是这几种，后续要补充
               errFlag = 1;
            }
        }
    }
    return errFlag;
}

int RobotPrgFileStruct::getSelRoutineLineStr(int modNum, int routineNum, int routineLineNum, char *str)
{
    program_pointer prgPointer;
    int errFlag = -1;
    int i = 0;
    int routineLine = 0;

    if (modNum < prgFileInfo.modulePrg.size()) {
        if (routineNum < prgFileInfo.modulePrg[modNum].routinePrg.size()) {
            if (routineLineNum < prgFileInfo.modulePrg[modNum].routinePrg[routineNum].lineStrData.size()) {
                strcpy(str, prgFileInfo.modulePrg[modNum].routinePrg[routineNum].lineStrData[routineLineNum].str);
                errFlag = 1;
            }
        }
    }

    return errFlag;
}

int RobotPrgFileStruct::getFirstInstructionLinePos(int modNum, int routineNum)
{
    int errFlag = -1;
    int i = 0;
    int modLine;

    if (modNum < prgFileInfo.modulePrg.size()) {
        modLine = prgFileInfo.modulePrg[modNum].lineStrData.size() - 1;//末尾行去掉
        if (routineNum < prgFileInfo.modulePrg[modNum].routinePrg.size()) {
            while (i < routineNum) {
                modLine += prgFileInfo.modulePrg[modNum].routinePrg[i].lineStrData.size();
                i++;
            }
            i = 0;
            while (i < D_DATATYPE_END_CNT) {//加上所有变量
                modLine += prgFileInfo.modulePrg[modNum].routinePrg[routineNum].robotVarArry[i].robotVar.size();
                i++;
            }
            errFlag = modLine;
        }
    }

    return errFlag;
}

int RobotPrgFileStruct::getSelRoutineFirstLinePos(int modNum, int routineNum)
{
    int errFlag = -1;
    int i = 0;
    int modLine;

    if (modNum < prgFileInfo.modulePrg.size()) {
        modLine = prgFileInfo.modulePrg[modNum].lineStrData.size() - 1;//末尾行去掉
        if (routineNum < prgFileInfo.modulePrg[modNum].routinePrg.size()) {
            while (i < routineNum) {
                modLine += prgFileInfo.modulePrg[modNum].routinePrg[i].lineStrData.size();
                i++;
            }
            errFlag = modLine;
        }
    }

    return errFlag;
}

int RobotPrgFileStruct::getFileLineTotalCnt(void)
{
    int cnt = 0;
    int i = 0;
    int j = 0;

    while (i < prgFileInfo.modulePrg.size()) {
        cnt += prgFileInfo.modulePrg[i].lineStrData.size();
        while (j < prgFileInfo.modulePrg[i].routinePrg.size()) {
            cnt += prgFileInfo.modulePrg[i].routinePrg[j].lineStrData.size();
            j++;
        }
        i++;
    }
    return cnt;
}

int RobotPrgFileStruct::getSelModuleFileLineTotalCnt(int modNum)
{
    int cnt = 0;
    int i = 0;

    if (modNum < prgFileInfo.modulePrg.size()) {
        cnt += prgFileInfo.modulePrg[modNum].lineStrData.size();
        while (i < prgFileInfo.modulePrg[modNum].routinePrg.size()) {
            cnt += prgFileInfo.modulePrg[modNum].routinePrg[i].lineStrData.size();
            i++;
        }
    }
    return cnt;
}

int RobotPrgFileStruct::getSelRoutineFileLineTotalCnt(int modNum, int routineNum)
{
    int cnt = 0;

    if (modNum < prgFileInfo.modulePrg.size()) {
        if (routineNum < prgFileInfo.modulePrg[modNum].routinePrg.size()) {
            cnt = prgFileInfo.modulePrg[modNum].routinePrg[routineNum].lineStrData.size();
        }
    }
    return cnt;
}

int RobotPrgFileStruct::clearFileStructInfo(void)//清除文件，本函数清除所有模块、例行程序、变量，有别于单独删除一个模块或例行程序
{
    decode_character_property charPro;
    int errFlag = -1;
    int i = 0;
    int j = 0;
    int k = 0;
    int m = 0;
    int n = 0;


    i = prgFileInfo.modulePrg.size() - 1;
    while (i >= 0) {//要倒着清除
        j = prgFileInfo.modulePrg[i].routinePrg.size() - 1;
        while (j >= 0) {//删除例行程序
            while (k < D_DATATYPE_END_CNT) {//先删除变量
                while (m < prgFileInfo.modulePrg[i].routinePrg[j].robotVarArry[k].robotVar.size()) {//释放变量值空间
                    decodeTypestructDeleteDataStoreBuf(prgFileInfo.modulePrg[i].routinePrg[j].robotVarArry[k].robotVar[m].valIndex);
                    while (getOneCharacterIndex(n, prgFileInfo.modulePrg[i].routinePrg[j].robotVarArry[k].robotVar[m].typePro.typeName, &charPro) == 1) {
                        if ((charPro.charType == D_CHAR_TYPE_VARNAME) && (charPro.modParent == i) && (charPro.routineParent ==j)) {//如果是变量名
                            deleteOneCharacterIndex(charPro);
                            break;
                        }
                        n++;
                    }
                    n = 0;
                    m++;
                }
                m = 0;
                prgFileInfo.modulePrg[i].routinePrg[j].robotVarArry[k].robotVar.clear();//释放变量列表
                k++;
            }
            prgFileInfo.modulePrg[i].routinePrg[j].parameterStruct.clear();//释放形参列表
            k = 0;
            while (k < prgFileInfo.modulePrg[i].routinePrg[j].lineStrData.size()) {//删除程序行
                delete []prgFileInfo.modulePrg[i].routinePrg[j].lineStrData[k].str;
                k++;
            }
            prgFileInfo.modulePrg[i].routinePrg[j].lineStrData.clear();
            j--;
        }
        prgFileInfo.modulePrg[i].routinePrg.clear();//清除例行程序存储
        k = 0;
        m = 0;
        n = 0;
        while (k < D_DATATYPE_END_CNT) {//删除模块中的变量
            while (m < prgFileInfo.modulePrg[i].robotVarArry[k].robotVar.size()) {//释放变量值空间
                decodeTypestructDeleteDataStoreBuf(prgFileInfo.modulePrg[i].robotVarArry[k].robotVar[m].valIndex);
                while (getOneCharacterIndex(n, prgFileInfo.modulePrg[i].robotVarArry[k].robotVar[m].typePro.typeName, &charPro) == 1) {
                    if ((charPro.charType == D_CHAR_TYPE_VARNAME) && (charPro.modParent == i)) {//如果是变量名
                        deleteOneCharacterIndex(charPro);
                        break;
                    }
                    n++;
                }
                n = 0;
                m++;
            }
            prgFileInfo.modulePrg[i].robotVarArry[k].robotVar.clear();//释放变量列表
            m = 0;
            k++;
        }
        k = 0;
        while (k < prgFileInfo.modulePrg[i].lineStrData.size()) {//删除程序行
            delete []prgFileInfo.modulePrg[i].lineStrData[k].str;
            k++;
        }
        j = 0;
        k = 0;
        m = 0;
        n = 0;
        i--;
        errFlag = 1;
    }
    prgFileInfo.modulePrg.clear();//释放所有模块空间
    return errFlag;
}

int RobotPrgFileStruct::parseModLineToRelation(int *modNum, int *modLine, int *routine, int *routineLine)
{
    program_pointer prgPointer;
    prgPointer = getSelectModuleLinePos(*modNum, *modLine);
    *modNum = prgPointer.curModule;
    *routine = prgPointer.curRoutine;
    *modLine = prgPointer.curModuleLine;
    *routineLine = prgPointer.curRoutineLine;

    return 1;
}



























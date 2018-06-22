/***************************************************************************
* Module Operation Explanation
* Replace &name& with the name of the file
* Fills the include, flaged with +include+
* Fills the definition flaged with =====
*
****************************************************************************/

/***************************************************************************
****************************************************************************
*Copyright(c),2008-2028,广州数控设备有限公司
*File name: decode_proc_task.c
*First Author: 
*Date:06/17/2009  
*文件功能描述：
****************************************************************************
****************************************************************************/

#define  DECODE_PROC_TASK_GLOBALS

/**********************************************************************/
/*Add all #includes here*/
#include "decode_proc_task.h"
#include "decode_lexical_analysis.h"
#include "robotdecodepublic.h"
#include "ITP_Interface.h"
#include "decode_shmem.h"

/****** luhuadong (begin) ******/
#include "GRS_SharedMemory.h"
/******* luhuadong (end) *******/

/**********************************************************************/

/**********************************************************************/
/*Add all #defines here*/
//#define

/****** luhuadong (begin) ******/
/*
#if Windows
extern int DataManage_InitShareMem_InputData(int flagNew,void **pShareMem);//输入共享内存申请
extern void DataManage_CloseShareMem_InputData(void *pShareMem);			//输入共享内存释放

extern int DataManage_InitShareMem_OutputData(int flagNew, void **pShareMem);
extern void DataManage_CloseShareMem_OutputData(void *pShareMem);

extern int DataManage_InitShareMem_MotSetData(int flagNew,void **pShareMem);
extern void DataManage_CloseShareMem_MotSetData(void *pShareMem);
#elif Linux
extern void *DataManage_ShareMem_Linux(int index);
#endif
*/
/******* luhuadong (end) *******/

volatile int DecodeTaskRunFlag = 2;//译码任务执行启动标志，0：停止并退出，1：运行，2：暂停
volatile int DecodeTaskRunMod = -1;//译码正在运行的模块
volatile int DecodeTaskRunRoutine = -1;//译码正在运行的例行程序
volatile int DecodeTaskRunModLine = -1;//译码正在运行的模块行
volatile int DecodeTaskRunRoutineLine = -1;//译码正在运行的例行程序行

volatile program_pointer InterprolateSendPtr = {-1, -1, -1, -1};//记录发给插补的程序指针
volatile program_pointer InterprolateRecvPtr = {-1, -1, -1, -1};//记录插补执行完成的程序指针
volatile program_pointer ParserPtr = {-1, -1, -1, -1};//记录译码取字符串指令指针位置

volatile int InterprolateBufCmdCnt = 0;//插补发送指令缓冲数目，根据ABB现象及结合插补模块需求，暂定连续发送不超过2条
volatile int InterprolateSendCmdCnt = 0;//发送给插补的指令数目
volatile int InterprolateExceCmdCnt = 0;//插补已完成执行的指令数目，最终InterprolateSendCmdCnt要相等

#define PRG_POINTER_BUF_LEN     (1000)//队列缓冲大小

typedef struct {//插补模块程序指针队列
    program_pointer prgPtrBuf[PRG_POINTER_BUF_LEN];//缓冲
    int readPos;//读位置
    int writePos;//写位置
    int cnt;//缓冲内数据数目
}inter_pc_ptr_queue;

volatile ITP_InputData *pITP_InputDataPtr;
volatile ITP_OutputData *pITP_OutputDataPtr;

decode_shmem_input *pDecodeShmemInputPtr;
decode_shmem_output *pDecodeShmemOutputPtr;
inter_pc_ptr_queue InterpolationPtrQueGlobal;
/**********************************************************************/

/**********************************************************************/
/*以下开始函数定义*/
/**********************************************************************/ 
/*函数功能：*/ 
/*输入参数： 参数1：注明输入参数意义及有效性范围*/			
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/
int decodeTaskInit(void)
{
    decodeInitLexicalInfo();

    /****** luhuadong (begin) ******/
/*
#if Windows
    DataManage_InitShareMem_InputData(0, &pITP_InputDataPtr);
    DataManage_InitShareMem_OutputData(0, &pITP_OutputDataPtr);
#elif Linux
    pITP_InputDataPtr = (ITP_InputData *)DataManage_ShareMem_Linux(0);
    pITP_OutputDataPtr = (ITP_OutputData *)DataManage_ShareMem_Linux(1);
#endif
*/
    pITP_InputDataPtr = pInputData;
    pITP_OutputDataPtr = pOutputData;
    /******* luhuadong (end) *******/

    initDecodeShmem();
    pDecodeShmemInputPtr = getDecodeShmemInputPtr();
    pDecodeShmemOutputPtr = getDecodeShmemOutputPtr();
    InterpolationPtrQueGlobal.cnt = 0;
    InterpolationPtrQueGlobal.readPos = 0;
    InterpolationPtrQueGlobal.writePos = 0;
	return (1);
}
/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/
int popInterpolationPtrQue(program_pointer *prgPtr)//本函数要考虑互斥访问
{
    int errFlag = -1;
    int pos;

    if (InterpolationPtrQueGlobal.cnt > 0) {
        pos = InterpolationPtrQueGlobal.readPos;
        memcpy(prgPtr, &InterpolationPtrQueGlobal.prgPtrBuf[pos], sizeof(program_pointer));
        InterpolationPtrQueGlobal.readPos = (pos + 1) % PRG_POINTER_BUF_LEN;
        InterpolationPtrQueGlobal.cnt--;
        errFlag = 1;
    }
    return errFlag;
}
/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/
int pushInterpolationPtrQue(program_pointer prgPtr)//本函数要考虑互斥访问
{
    int errFlag = -1;
    int pos;

    if (InterpolationPtrQueGlobal.cnt < PRG_POINTER_BUF_LEN) {
        pos = InterpolationPtrQueGlobal.writePos;
        memcpy(&InterpolationPtrQueGlobal.prgPtrBuf[pos], &prgPtr, sizeof(program_pointer));
        InterpolationPtrQueGlobal.writePos = (pos + 1) % PRG_POINTER_BUF_LEN;
        InterpolationPtrQueGlobal.cnt++;
        errFlag = 1;
    }
    return errFlag;
}
/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/
int getInterpolationPtrQueCnt(void)
{
    return InterpolationPtrQueGlobal.cnt;
}

/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/
int checkCurInterpolationPtrQue(program_pointer *prgPtr)//本函数要考虑互斥访问
{
    int errFlag = -1;
    int pos;

    if (InterpolationPtrQueGlobal.cnt > 0) {
        pos = InterpolationPtrQueGlobal.readPos;
        memcpy(prgPtr, &InterpolationPtrQueGlobal.prgPtrBuf[pos], sizeof(program_pointer));//获取当前的，并不改变位置，方便用于检测
        errFlag = 1;
    }
    return errFlag;
}
/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/

/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/
void decodeTaskRunningProc(int channel)//译码任务执行函数
{
#if 1
//    static int runFlag = FALSE;
    int pos = 0;
//    static int modNum = -1;
//    static int modLine = -1;
//    static int routineNum = -1;
//    static int routineLine = -1;
    char str[256];
    decode_character_property charPro;
    decode_instr_parse_info parse_info;
    decode_cmd_data cmdData;
    program_pointer tmpPtr;

    if (DecodeTaskRunFlag == DECODE_TASK_START) {//开始执行
        if (decodeCheckParserBufFull() == 0) {//译码缓冲没满
            if (getSelectModuleLineStr(channel, ParserPtr.curModule, ParserPtr.curModuleLine, str) > 0) {//获取有效的行，如果译码缓冲没填满，则插入完成
                if (decodeStringCompare(str, "ENDPROC", strlen("ENDPROC")) != 0) {//此处要增加遇到跳转等译码指针要先变化，并且要考虑判断条件成立的情况
                    parse_info.modNum = ParserPtr.curModule;
                    parse_info.modLine = ParserPtr.curModuleLine;
                    parse_info.routineNum = ParserPtr.curRoutine;
                    parse_info.routineLine = ParserPtr.curRoutineLine;
                    decodeInstructionLexicalAnalysis(str, &parse_info);
                    if (decodePushParserBuf(parse_info) == 1) {//插入成功
                        ParserPtr.curModuleLine++;
                    }
    //                if (decodePopParserBuf(&parse_info) == 1) {//出列成功
    //                    decodeInstructionExecute(&parse_info);
    //                }

    //                if (decodePopCmdBuf(&cmdData) == 1) {
    //                    memcpy(&pITP_InputDataPtr->cmd, &cmdData, sizeof(decode_cmd_data));
    //                }
                }
            }
        }

//        if (getInterpolationPtrQueCnt() > 0) {//如果有插补执行，光标行号要等待插补模块执行行号一致
//            if (checkCurInterpolationPtrQue(&tmpPtr) == 1) {//探测队列中数据
//                if ((tmpPtr.curModule == pITP_OutputDataPtr->pc_exe.module) && (tmpPtr.curModuleLine == pITP_OutputDataPtr->pc_exe.progNo)) {
//                    popInterpolationPtrQue(&tmpPtr);
//                    pDecodeShmemOutputPtr->execPtr.curModule = tmpPtr.curModule;
//                    pDecodeShmemOutputPtr->execPtr.curModuleLine = tmpPtr.curModuleLine;
//                }
//            }
//        } else {//否则与译码模块读取执行的行号一致
//            pDecodeShmemOutputPtr->execPtr.curModule = pDecodeShmemOutputPtr->readPtr.curModule;
//            pDecodeShmemOutputPtr->execPtr.curModuleLine = pDecodeShmemOutputPtr->readPtr.curModuleLine;
//        }
        //ABB中高亮光标与插补执行指令一致，执行过程中页面的跳转以译码预读执行的位置一致

        if ((InterprolateRecvPtr.curModule != pITP_OutputDataPtr->pc_exe.module) || (InterprolateRecvPtr.curModuleLine != pITP_OutputDataPtr->pc_exe.progNo)) {//如果记录不一致，表明插补执行完成新的一行
            pDecodeShmemOutputPtr->execPtr.curModule = pITP_OutputDataPtr->pc_exe.module;
            pDecodeShmemOutputPtr->execPtr.curModuleLine = pITP_OutputDataPtr->pc_exe.progNo;
            InterprolateRecvPtr.curModule = pITP_OutputDataPtr->pc_exe.module;
            InterprolateRecvPtr.curModuleLine = pITP_OutputDataPtr->pc_exe.progNo;
//            InterprolateBufCmdCnt--;//插补处理完一条要递减
//            if (InterprolateBufCmdCnt < 0) {
//                InterprolateBufCmdCnt = 0;
//            }
//            InterprolateExceCmdCnt++;//执行数目加1
        }


        if (decodeGetParserBufCnt() > 0) {//如果译码缓冲中有指令
            if ((decodeCheckParserBufCurCmdType() == D_INSTR_OWN_INTERPRATION)) {// && (InterprolateBufCmdCnt < 2)) {//如果当前是插补运动指令，并且发给插补的数目小于2
                if ((pITP_OutputDataPtr->pc_read.module == InterprolateSendPtr.curModule) && (pITP_OutputDataPtr->pc_read.progNo == InterprolateSendPtr.curModuleLine)) {
                    if (decodePopParserBuf(&parse_info) == 1) {//出列成功
                        decodeInstructionExecute(&parse_info);
                        pDecodeShmemOutputPtr->readPtr.curModule = parse_info.modNum;
                        pDecodeShmemOutputPtr->readPtr.curModuleLine = parse_info.modLine;


                        //此处暂时将译码读的指针与插补执行指针一致

                        if (decodePopCmdBuf(&cmdData) == 1) {//发送给插补
                            memcpy(&pITP_InputDataPtr->cmd, &cmdData, sizeof(decode_cmd_data));
                            InterprolateSendPtr.curModule = cmdData.prgPointer.modNum;
                            InterprolateSendPtr.curModuleLine = cmdData.prgPointer.modLineNum;
                             if (InterprolateSendPtr.curModuleLine == 0) {
                                 InterprolateSendPtr.curModuleLine = InterprolateSendPtr.curModuleLine;
                             }
//                            InterprolateBufCmdCnt++;
//                            InterprolateSendCmdCnt++;//发送数目加1
//                            pushInterpolationPtrQue(InterprolatePtr);
                        }
                    }
                }
            } else if (decodeCheckParserBufCurCmdType() == D_INSTR_OWN_DECODE) {//如果是译码指令
                if (decodePopParserBuf(&parse_info) == 1) {//出列成功
                    decodeInstructionExecute(&parse_info);
                    pDecodeShmemOutputPtr->readPtr.curModule = parse_info.modNum;
                    pDecodeShmemOutputPtr->readPtr.curModuleLine = parse_info.modLine;

                }
            }
        } else {//否则译码结束
//            if ((pDecodeShmemOutputPtr->execPtr.curModule == pDecodeShmemOutputPtr->readPtr.curModule) && (pDecodeShmemOutputPtr->execPtr.curModuleLine == pDecodeShmemOutputPtr->readPtr.curModuleLine)) {
//            if ((pDecodeShmemOutputPtr->execPtr.curModule == pITP_OutputDataPtr->pc_exe.module) && (pDecodeShmemOutputPtr->execPtr.curModuleLine == pITP_OutputDataPtr->pc_exe.progNo)
//                && (ParserPtr.curModule == pDecodeShmemOutputPtr->readPtr.curModule) && ((ParserPtr.curModuleLine - 1) == pDecodeShmemOutputPtr->readPtr.curModuleLine)  ) {

//                DecodeTaskRunFlag = 0;
//            }
//            if (InterprolateExceCmdCnt == InterprolateSendCmdCnt ) {//如果发送的数目与执行数目一致，则结束，因为译码执行的指令随着从缓冲中取走并执行，此处只需判断插补的即可
//                DecodeTaskRunFlag = 0;
//            }
            if ((InterprolateSendPtr.curModule == pITP_OutputDataPtr->pc_exe.module) && (InterprolateSendPtr.curModuleLine == pITP_OutputDataPtr->pc_exe.progNo)) {//如果插补发送最后一条指令与插补完成的指令标号一致，证明执行完成
                pDecodeShmemOutputPtr->execPtr.curModule = pITP_OutputDataPtr->pc_exe.module;
                pDecodeShmemOutputPtr->execPtr.curModuleLine = pITP_OutputDataPtr->pc_exe.progNo;
                InterprolateRecvPtr.curModule = pITP_OutputDataPtr->pc_exe.module;
                InterprolateRecvPtr.curModuleLine = pITP_OutputDataPtr->pc_exe.progNo;
                DecodeTaskRunFlag = 0;
            }


        }

//        else {
//            DecodeTaskRunFlag = 0;//程序结束
//            pITP_InputDataPtr->mode = 0;
//            pITP_InputDataPtr->start = 1;
//        }
    } else {//停止或暂停

    }

//    if (runFlag == FALSE) {
//        while (1) {
//            if (getOneCharacterIndex(pos, "main", &charPro) == 1) {
//                if (charPro.charType == D_CHAR_TYPE_PRCNAME) {
//                    modNum = charPro.modParent;
//                    routineNum = charPro.routineParent;
//                    modLine = getSelRoutineFirstLinePos(channel, modNum, routineNum) + 1;//从程序行开始，注意此处要实际从指令行开始，需要调整
//                    parse_info.modNum = modNum;
//                    parse_info.routineNum = routineNum;
//                    break;
//                }
//                pos++;
//            }

//        }
//        runFlag = TRUE;
//    } else {//获取程序行
//        if (getSelectModuleLineStr(channel, modNum, modLine, str) > 0) {//获取有效的行
//            decodeInstructionLexicalAnalysis(str, &parse_info);

//        }

//    }

#else
    decodeTaskInit();//初始化放这里
    while (DecodeTaskRunFlag > 0) {
        if (DecodeTaskRunFlag == 1) {//运行

        } else {//暂停，此处添加挂起代码

        }

    }
#endif
}

/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/
int setDecodeTaskState(int state)
{
    int oldState = DecodeTaskRunFlag;
    switch (state) {
    case DECODE_TASK_STOP:
    case DECODE_TASK_PAUSE://暂停或停止
        pITP_InputDataPtr->start = 0;
        break;
    case DECODE_TASK_START://启动
        if ((DecodeTaskRunFlag == DECODE_TASK_PAUSE) || (DecodeTaskRunFlag == DECODE_TASK_STOP)) {
            pITP_InputDataPtr->mode = ITP_Mode_CoordinateOp_PosMode;
            pITP_InputDataPtr->start = 1;
//            InterprolateSendPtr.curModule = pITP_OutputDataPtr->pc_read.module;//初值与插补模块的位置一致
//            InterprolateSendPtr.curModuleLine = pITP_OutputDataPtr->pc_read.progNo;
//            InterprolateRecvPtr.curModule = pITP_OutputDataPtr->pc_exe.module;//初值与插补模块已经执行完成的位置一致
//            InterprolateRecvPtr.curModuleLine = pITP_OutputDataPtr->pc_exe.progNo;
        }

        break;
    default:
        break;
    }

    DecodeTaskRunFlag = state;
    return oldState;
}

/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/
int getDecodeTaskState(void)
{
    return DecodeTaskRunFlag;
}

/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/
int setDecodeTaskFirstLinePos(int modNum, int modLine)//界面中pptomain调用
{
//    DecodeTaskRunMod = modNum;
//    DecodeTaskRunModLine = modLine;
    if (pITP_InputDataPtr->reset == 0) {//复位插补
        pITP_InputDataPtr->reset = 1;
    } else {
        pITP_InputDataPtr->reset = 0;
    }
    decodeResetParserBuf();//清空译码预读缓冲
    decodeResetCmdBuf();//清空插补指令缓冲
    ParserPtr.curModule = modNum;
    ParserPtr.curModuleLine = modLine;
    pDecodeShmemInputPtr->startPtr.curModule = modNum;
    pDecodeShmemInputPtr->startPtr.curModuleLine = modLine;
    pDecodeShmemOutputPtr->execPtr.curModule = modNum;
    pDecodeShmemOutputPtr->execPtr.curModuleLine = modLine;
    pDecodeShmemOutputPtr->readPtr.curModule = modNum;
    pDecodeShmemOutputPtr->readPtr.curModuleLine = modLine;
    InterprolateBufCmdCnt = 0;
    InterprolateSendCmdCnt = 0;
    InterprolateExceCmdCnt = 0;
//    pITP_OutputDataPtr->pc_read.module = modNum;
//    pITP_OutputDataPtr->pc_read.progNo = modLine;
    InterprolateSendPtr.curModule = pITP_OutputDataPtr->pc_read.module;//初值与插补模块的位置一致
    InterprolateSendPtr.curModuleLine = pITP_OutputDataPtr->pc_read.progNo;
    InterprolateRecvPtr.curModule = pITP_OutputDataPtr->pc_exe.module;//初值与插补模块已经执行完成的位置一致
    InterprolateRecvPtr.curModuleLine = pITP_OutputDataPtr->pc_exe.progNo;
    return 1;
}
/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/
int getDecodeTaskRunLinePos(int *modNum, int *modLine)
{
//    *modNum = DecodeTaskRunMod;
//    *modLine = DecodeTaskRunModLine;
    *modNum = pDecodeShmemOutputPtr->execPtr.curModule;
    *modLine = pDecodeShmemOutputPtr->execPtr.curModuleLine;
    return 1;
}

/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/

/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/

/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/

/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/

/**********************************************************************/

/**********************************************************************/
/*函数功能：*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***********************************************************************/
/**********************************************************************/
/*函数实现原理：请注明获取输出参数的方法*/
/**********************************************************************/

/**********************************************************************/

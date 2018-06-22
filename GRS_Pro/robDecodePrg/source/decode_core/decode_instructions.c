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
*File name: decode_instructions.c
*First Author: 
*Date:06/17/2009  
*文件功能描述：
****************************************************************************
****************************************************************************/

#define  DECODE_INSTRUCTIONS_GLOBALS

/**********************************************************************/
/*Add all #includes here*/
#include "decode_parse_file.h"
#include "decode_instructions.h"
#include "decode_public.h"
#include "robotdecodepublic.h"
#include "ITP_Interface.h"
/**********************************************************************/

/**********************************************************************/
/*Add all #defines here*/
decode_instruction_index_info DecodeInstrIndexInfoGlobalTable;
//以下中数据类型应当与界面提供的数据类型存储索引一致
//类型成员解释：低16位表示参数类型，bit16~bit23位表示所在的参数顺序(从0开始编号)，可选互斥的顺序相同，高8位是否必选参数，-1表示无效
decode_instruction_info DecodeInstrInfoGlobalTable[INSTR_END_CNT+1] = {
    {{{" ", -1}}, NULL},//预留一个空，与decode_instruction_enum类型顺序保持一致
    {{{"!", 0x01000000}, {" ", -1}}, NULL},//为注释
    {{{"<SMT>", 0x01000000}, {" ", -1}}, NULL},//预留一个空，与decode_instruction_enum类型顺序保持一致
    {{{"MODULE", 0x01000000}, {" ", -1}}, NULL},
    {{{"ENDMODULE", 0x01000000}, {" ", -1}}, NULL},
    {{{"PROC", 0x01000000}, {" ", -1}}, NULL},
    {{{"ENDPROC", 0x01000000}, {" ", -1}}, NULL},
//速度和加速度相关指令
    {{{"AccSet", 0x01000000}, {" ", -1}}, NULL},
    {{{"WorldAccLim", 0x01000000}, {" ", -1}}, NULL},
    {{{"PathAccLim", 0x01000000}, {" ", -1}}, NULL},
    {{{"SpeedLimAxis", 0x01000000}, {" ", -1}}, NULL},
    {{{"SpeedLimCheckPoint", 0x01000000}, {" ", -1}}, NULL},
    {{{"SpeedRefresh", 0x01000000}, {" ", -1}}, NULL},
    {{{"VelSet", 0x01000000}, {" ", -1}}, NULL},
//运动相关配置
    {{{"ConfJ", 0x01000000}, {" ", -1}}, NULL},
    {{{"ConfL", 0x01000000}, {" ", -1}}, NULL},
    {{{"ActUnit", 0x01000000}, {" ", -1}}, NULL},
    {{{"DeactUnit", 0x01000000}, {" ", -1}}, NULL},
    {{{"MechUnitLoad", 0x01000000}, {" ", -1}}, NULL},
    {{{"GripLoad", 0x01000000}, {" ", -1}}, NULL},
    {{{"MotionSup", 0x01000000}, {" ", -1}}, NULL},
    {{{"SingArea", 0x01000000}, {" ", -1}}, NULL},
    {{{"CirPathMode", 0x01000000}, {" ", -1}}, NULL},
//触发操作相关指令
    {{{"EOffsOn", 0x01000000}, {" ", -1}}, NULL},
    {{{"EOffsOff", 0x01000000}, {" ", -1}}, NULL},
    {{{"EOffsSet", 0x01000000}, {" ", -1}}, NULL},
    {{{"PDispSet", 0x01000000}, {" ", -1}}, NULL},
    {{{"PDispOn", 0x01000000}, {" ", -1}}, NULL},
    {{{"PDispOff", 0x01000000}, {" ", -1}}, NULL},
    {{{"TriggCheckIO", 0x01000000}, {" ", -1}}, NULL},
    {{{"TriggIO", 0x01000000}, {" ", -1}}, NULL},
    {{{"TriggEquip", 0x01000000}, {" ", -1}}, NULL},
    {{{"TriggInt", 0x01000000}, {" ", -1}}, NULL},
    {{{"TriggRampAO", 0x01000000}, {" ", -1}}, NULL},
    {{{"TriggSpeed", 0x01000000}, {" ", -1}}, NULL},
//运动指令
    {{{"MoveAbsJ", 0x01000000}, {" ", -1}}, NULL},
    {{{"MoveExtJ", 0x01000000}, {" ", -1}}, NULL},
    //以下要注意MOVE中传递*的情况，还没添加
    {{{"MoveJ", 0x01000200}, {"\\Conc", 0x00010007}, {"ToPoint", 0x01020006}, {"\\ID", 0x00030008}, {"Speed", 0x01040009}, {"\\V", 0x00050019}, {"\\T", 0x00050019}, {"Zone", 0x0106000B}, {"\\Z", 0x00070019}, {"\\Inpos", 0x0008001A}, {"Tool", 0x0109000D}, {"\\WObj", 0x000A000E}, {"\\TLOAD", 0x000B000F}, {" ", -1}}, decodeInstructMoveJExecute},
    {{{"MoveL", 0x01000200}, {"\\Conc", 0x00010007}, {"ToPoint", 0x01020006}, {"\\ID", 0x00030008}, {"Speed", 0x01040009}, {"\\V", 0x00050009}, {"\\T", 0x00050019}, {"Zone", 0x0106000B}, {"\\Z", 0x00070019}, {"\\Inpos", 0x0008001A}, {"Tool", 0x0109000D}, {"\\WObj", 0x000A000E}, {"\\Corr", 0x000B0007}, {"\\TLOAD", 0x000C000F}, {" ", -1}}, decodeInstructMoveLExecute},
    {{{"MoveC", 0x01000200}, {"\\Conc", 0x00010007}, {"CirPoint", 0x01020006}, {"ToPoint", 0x01030006}, {"\\ID", 0x00040008}, {"Speed", 0x01050009}, {"\\V", 0x00060009}, {"\\T", 0x00060019}, {"Zone", 0x0107000B}, {"\\Z", 0x00080019}, {"\\Inpos", 0x0009001A}, {"Tool", 0x010A000D}, {"\\WObj", 0x000B000E}, {"\\Corr", 0x000C0007}, {"\\TLOAD", 0x000D000F}, {" ", -1}}, decodeInstructMoveCExecute},
    {{{"SearchC", 0x01000000}, {" ", -1}}, NULL},
    {{{"SearchExtJ", 0x01000000}, {" ", -1}}, NULL},
    {{{"SearchL", 0x01000000}, {" ", -1}}, NULL},
    {{{"TriggJ", 0x01000000}, {" ", -1}}, NULL},
    {{{"TriggL", 0x01000000}, {" ", -1}}, NULL},
    {{{"TriggC", 0x01000000}, {" ", -1}}, NULL},
    {{{"TriggLIOs", 0x01000000}, {" ", -1}}, NULL},
    {{{"ClearPath", 0x01000000}, {" ", -1}}, NULL},
    {{{"StorePath", 0x01000000}, {" ", -1}}, NULL},
    {{{"RestoPath", 0x01000000}, {" ", -1}}, NULL},
    {{{"StartMove", 0x01000000}, {" ", -1}}, NULL},
    {{{"StartMoveRetry", 0x01000000}, {" ", -1}}, NULL},
    {{{"StopMove", 0x01000000}, {" ", -1}}, NULL},
    {{{"StopMoveReset", 0x01000000}, {" ", -1}}, NULL},
};


//decode_instruction_info d_instr_info_global_table[INSTR_END_CNT+1] = {//支持的指令索引表，该表顺序要与decode_instruction_enum一致
//第一条指令为空
//	{"NULL", D_INSTR_TYPE_NULL, 0, 0, NULL},
/*
//速度和加速度相关指令
	{"ACCSET", D_INSTR_TYPE_VELACC, 2, 1, decode_instruct_accset_proc},
	{"WORLDACCLIM", D_INSTR_TYPE_VELACC, 0, 1, decode_instruct_worldacclim_proc},
	{"PATHACCLIM", D_INSTR_TYPE_VELACC, 2, 2, decode_instruct_pathacclim_proc},
	{"SPEEDLIMAXIS", D_INSTR_TYPE_VELACC, 3, 0, decode_instruct_speedlimaxis_proc},
	{"SPEEDLIMCHECKPOINT", D_INSTR_TYPE_VELACC, 1, 0, decode_instruct_speedlimcheckpoint_proc},
	{"SPEEDREFRESH", D_INSTR_TYPE_VELACC, 1, 0, decode_instruct_speedrefresh_proc},
	{"VELSET", D_INSTR_TYPE_VELACC, 2, 0, decode_instruct_velset_proc},
//运动相关配置指令
	{"CONFJ", D_INSTR_TYPE_MOTIONCFG, 0, 1, decode_instruct_confj_proc},
	{"CONFL", D_INSTR_TYPE_MOTIONCFG, 0, 1, decode_instruct_confl_proc},
	{"ACTUNIT", D_INSTR_TYPE_MOTIONCFG, 1, 0, decode_instruct_actunit_proc},
	{"DEACTUNIT", D_INSTR_TYPE_MOTIONCFG, 1, 0, decode_instruct_deactunit_proc},
	{"MECHUNITLOAD", D_INSTR_TYPE_MOTIONCFG, 3, 0, decode_instruct_mechunitload_proc},
	{"GRIPLOAD", D_INSTR_TYPE_MOTIONCFG, 1, 0, decode_instruct_gripload_proc},
	{"MOTIONSUP", D_INSTR_TYPE_MOTIONCFG, 0, 2, decode_instruct_motionsup_proc},
	{"SINGAREA", D_INSTR_TYPE_MOTIONCFG, 0, 1, decode_instruct_singarea_proc},
	{"CIRPATHMODE", D_INSTR_TYPE_MOTIONCFG, 0, 1, decode_instruct_cirpathmode_proc},
	{"EOFFSON", D_INSTR_TYPE_MOTIONCFG, 1, 1, decode_instruct_eoffson_proc},
	{"EOFFSOFF", D_INSTR_TYPE_MOTIONCFG, 0, 0, decode_instruct_eoffsoff_proc},
	{"EOFFSSET", D_INSTR_TYPE_MOTIONCFG, 1, 0, decode_instruct_eoffsset_proc},
	{"PDISPSET", D_INSTR_TYPE_MOTIONCFG, 1, 0, decode_instruct_pdispset_proc},
	{"PDISPON", D_INSTR_TYPE_MOTIONCFG, 2, 3, decode_instruct_pdispon_proc},
	{"PDISPOFF", D_INSTR_TYPE_MOTIONCFG, 0, 0, decode_instruct_pdispoff_proc},
	{"TRIGGCHECKIO", D_INSTR_TYPE_MOTIONCFG, 6, 2, decode_instruct_triggcheckio_proc},
	{"TRIGGIO", D_INSTR_TYPE_MOTIONCFG, 3, 3, decode_instruct_triggio_proc},
	{"TRIGGEQUIP", D_INSTR_TYPE_MOTIONCFG, 4, 3, decode_instruct_triggequip_proc},
	{"TRIGGINT", D_INSTR_TYPE_MOTIONCFG, 3, 1, decode_instruct_triggint_proc},
	{"TRIGGRAMPAO", D_INSTR_TYPE_MOTIONCFG, 6, 2, decode_instruct_triggrampao_proc},
	{"TRIGGSPEED", D_INSTR_TYPE_MOTIONCFG, 5, 4, decode_instruct_triggspeed_proc},
//运动指令
	{"MOVEABSJ", D_INSTR_TYPE_MOTION, 4, 9, decode_instruct_moveabsj_proc},
	{"MOVEEXTJ", D_INSTR_TYPE_MOTION, 3, 5, decode_instruct_moveextj_proc},
*/
//	{"MOVEJ", D_INSTR_TYPE_MOTION, 4, 8, decode_instruct_movej_analysis, decode_instruct_movej_execute},
/*	{"MOVEL", D_INSTR_TYPE_MOTION, 4, 8, decode_instruct_movel_proc},
	{"MOVEC", D_INSTR_TYPE_MOTION, 5, 8, decode_instruct_movec_proc},
	{"SEARCHC", D_INSTR_TYPE_MOTION, 6, 7, decode_instruct_searchc_proc},
	{"SEARCHEXTJ", D_INSTR_TYPE_MOTION, 3, 5, decode_instruct_searchextj_proc},
	{"SEARCHL", D_INSTR_TYPE_MOTION, 5, 8, decode_instruct_searchl_proc},
	{"TRIGGJ", D_INSTR_TYPE_MOTION, 5, 13, decode_instruct_triggj_proc},
	{"TRIGGL", D_INSTR_TYPE_MOTION, 5, 14, decode_instruct_triggl_proc},
	{"TRIGGC", D_INSTR_TYPE_MOTION, 6, 14, decode_instruct_triggc_proc},
	{"TRIGGLIOS", D_INSTR_TYPE_MOTION, 4, 10, decode_instruct_trigglios_proc},
	{"CLEARPATH", D_INSTR_TYPE_MOTION, 0, 0, decode_instruct_clearpath_proc},
	{"STOREPATH", D_INSTR_TYPE_MOTION, 0, 1, decode_instruct_storepath_proc},
	{"RESTOPATH", D_INSTR_TYPE_MOTION, 0, 0, decode_instruct_restopath_proc},
	{"STARTMOVE", D_INSTR_TYPE_MOTION, 0, 1, decode_instruct_startmove_proc},
	{"STARTMOVERETRY", D_INSTR_TYPE_MOTION, 0, 0, decode_instruct_startmoveretry_proc},
	{"STOPMOVE", D_INSTR_TYPE_MOTION, 0, 2, decode_instruct_stopmove_proc},
	{"STOPMOVERESET", D_INSTR_TYPE_MOTION, 0, 1, decode_instruct_stopmovereset_proc},
*/
//};
#define DECODE_PARSER_BUF_LEN   (100)//译码中间数据缓冲数目
typedef struct {
    decode_instr_parse_info parserDataBuf[DECODE_PARSER_BUF_LEN];
    int readPos;
    int writePos;
    int cnt;
}decode_parse_queue;

#define DECODE_CMD_BUF_LEN  (100)//译码指令缓冲数目
typedef struct {
    decode_cmd_data cmdDataBuf[DECODE_CMD_BUF_LEN];
    int readPos;
    int writePos;
    int cnt;
}decode_cmd_queue;
decode_parse_queue DecodeParserQueGlobal;//译码中间数据缓冲
decode_cmd_queue DecodeCmdQueGlobal;//译码与插补交互数据缓冲
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
int decodeInitInstrIndexInfo(void)
{
#if 0
    int flag = 0;
    int i, j;
    int m = 0;

    for(i=1; i<=DECODE_STR_NAME_LEN; i++){
        for(j=0; j<INSTR_END_CNT; j++){
            if((int)strlen(DecodeInstrInfoGlobalTable[j].instrOrg[0].name) == i){
                if(flag == 0){
                    flag = 1;
                    DecodeInstrIndexInfoGlobalTable.beginPos[i] = m;
                }
                DecodeInstrIndexInfoGlobalTable.instructionIndex[m].index = j;
                strcpy(DecodeInstrIndexInfoGlobalTable.instructionIndex[m].instrName, DecodeInstrInfoGlobalTable[j].instrOrg[0].name);
                m++;
            }
        }
        if(flag == 1){
            DecodeInstrIndexInfoGlobalTable.endPos[i] = m;
            flag = 0;
        }
    }
#else
    //    int flag = 0;
        int i, j;
        decode_character_property charPro;
        memset(&DecodeCmdQueGlobal, 0, sizeof(decode_cmd_queue));//清空缓冲
        for (i=1; i<INSTR_END_CNT; i++) {//首个保留，所以从1开始
            //strcpy(charPro.charName, DecodeTypeInfoGlobalTable[i].typeElement[0].elementName);
            memset (&charPro, 0, sizeof(decode_character_property));
            strcpy(charPro.charName, DecodeInstrInfoGlobalTable[i].instrOrg[0].name);
            charPro.charType = D_CHAR_TYPE_INSTRUCTION;
            charPro.selfIndex = i;
            if (createOneCharacterIndex(charPro) == -1) {
                return -1;
            }
        }
#endif
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
int decodeGetInstructionIndex(char *str)
{
    int pos = strlen(str);
    int i;
    int index = -1;

    i = DecodeInstrIndexInfoGlobalTable.beginPos[pos];
    for( ; i<DecodeInstrIndexInfoGlobalTable.endPos[pos]; i++){
        if(strcmp(str, DecodeInstrIndexInfoGlobalTable.instructionIndex[i].instrName) == 0){
            index = DecodeInstrIndexInfoGlobalTable.instructionIndex[i].index;
            break;
        }
    }
    return index;
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
int decodeInstructionLexicalAnalysis(char *str, decode_instr_parse_info *parse_info)
{//暂时没处理表达式
#if 0
    char *pStr = str;
    char tmpStr[256];
    int i = 0;

    while (*pStr != 0) {
        if ((*pStr == ' ') || (*pStr == '\t')) {//去掉空格
            pStr++;
        }
    }

    while (*pStr != 0) {
        if (*pStr == ',') {//放在前面提高效率
            if (i != 0) {

            }
            i = 0;
        } else if ((*pStr == ' ') || (*pStr == '\t')) {//主要用于获取指令
            if (i != 0) {

            }

            i = 0;
        } else if (*pStr == '\\') {//识别可选参数
            while (*pStr != ':') {//冒号

            }
            pStr++;//过滤掉"="
            i = 0;
        } else {//有效字符
            tmpStr[i] = *pStr;
            i++;
        }
        pStr++;
    }
#else
    int errFlag = -1;
    int i = 1;//除去自身，从1开始
    int j = 0;
    char tmpStr[256];
    char *pStr = str;
    decode_character_property charPro;
    decode_character_property instrCharPro;//记录找到变量的索引
    decode_instruction_info *p_instruction_info;
    int last_grp = -1;//记录上一次的参数所在的组
    int opt_mask, grp_mask, type_mask;
//    int flag = 1;
    int findOK = 1;//首次进来默认找到字符串
    int globalFlag = -1;//找到全局变量的标志，记录全局变量所在索引位置
    int *valPtr;
    int offset;//可选参数变量值偏移起始

    pStr = decodeGetOneSegment(pStr, tmpStr);

    if (pStr != NULL) {//指令
        if (getOneCharacterIndex(0, tmpStr, &charPro) == 1) {//首次找到
            if (charPro.charType == D_CHAR_TYPE_INSTRUCTION) {//指令，此处需要继续完善，现仅作为测试
                parse_info->index = charPro.selfIndex;//找到指令索引
                p_instruction_info = &DecodeInstrInfoGlobalTable[charPro.selfIndex];
                valPtr = (int *)(&parse_info->instrData);
                while (p_instruction_info->instrOrg[i].mask != -1) {
                    if (findOK == 1) {//匹配后才进行下一字符串段的获取
                        pStr = decodeGetOneSegment(pStr, tmpStr);//此处要进行判断
                        findOK = 0;
                    }
                    type_mask = p_instruction_info->instrOrg[i].mask & 0xFF;//变量类型
                    grp_mask = (p_instruction_info->instrOrg[i].mask >> 16) & 0xF;//所在的组
                    opt_mask = (p_instruction_info->instrOrg[i].mask >> 24) & 0xF;//可选还是必选

                    if(opt_mask > 0){//为必选参数
                        while (getOneCharacterIndex(j, tmpStr, &charPro) == 1) {//直接索引界面，察看该类型的变量是否存在
                            if (charPro.charType == D_CHAR_TYPE_VARNAME) {//如果是变量名
                                if ((charPro.modParent == parse_info->modNum ) && (charPro.routineParent == parse_info->routineNum)) {//如果是全局的或者模块及例行程序号相同
//                                    memcpy(&instrCharPro, &charPro, sizeof(decode_character_property));//涉及联合体，要用内存拷贝
                                    findOK = 1;
                                    break;
                                }

                                if ((charPro.scope2 == 0) && (findOK == 0)) {//如果是全局的，并且没找到的
//                                    memcpy(&instrCharPro, &charPro, sizeof(decode_character_property));//涉及联合体，要用内存拷贝
                                    globalFlag = j;//找到全局变量，并记录
                                }
                            }
                            j++;
                        }
                        //如果参数存在，设置指针指向，并设置使用的掩码位
                        //flag = 1;
                        if (findOK == 1) {//如果找到
                            //parse_info->mark.mask |= 1<<i;//设置为1，表明使用该参数
                            valPtr[i - 1] = charPro.valPos;//instrCharPro.valPos;
                        } else if (globalFlag != -1) {//如果是全局的
                            getOneCharacterIndex(globalFlag, tmpStr, &charPro);//找到全局的位置
                            valPtr[i - 1] = charPro.valPos;
                            findOK = 1;
                        } else {//如果没有找到则报警退出

                        }
                        last_grp = grp_mask;
                        //parse_info->m_mark.m_parg[i] = ;//指针
                        //j++;
                    } else {//为可选参数，此处暂时没实现，先按常规必选参数测试
                        if(last_grp == grp_mask){//可选参数同组，互斥型不能同时出现，因此要报错
                            //添加互斥型可选参数不能同时出现，需要报警
                            break;
                        }
                        if(strncmp(tmpStr, p_instruction_info->instrOrg[i].name, strlen(p_instruction_info->instrOrg[i].name)) == 0){
                            //如果参数存在，可选参数要从:=后开始，获取的tmpStr中已经有变量了
                            offset = strlen(p_instruction_info->instrOrg[i].name) + 2;//加2的目的是跳过:=两个字符
                            while (getOneCharacterIndex(j, &tmpStr[offset], &charPro) == 1) {//直接索引界面，察看该类型的变量是否存在
                                if (charPro.charType == D_CHAR_TYPE_VARNAME) {//如果是变量名
                                    if ((charPro.modParent == parse_info->modNum ) && (charPro.routineParent == parse_info->routineNum)) {//如果是全局的或者模块及例行程序号相同
    //                                    memcpy(&instrCharPro, &charPro, sizeof(decode_character_property));//涉及联合体，要用内存拷贝
                                        findOK = 1;
                                        break;
                                    }

                                    if ((charPro.scope2 == 0) && (findOK == 0)) {//如果是全局的，并且没找到的
    //                                    memcpy(&instrCharPro, &charPro, sizeof(decode_character_property));//涉及联合体，要用内存拷贝
                                        globalFlag = j;//找到全局变量，并记录
                                    }
                                }
                                j++;
                            }
                            if (findOK == 1) {//如果找到
                                //parse_info->mark.mask |= 1<<i;//设置为1，表明使用该参数
                                valPtr[i - 1] = charPro.valPos;//instrCharPro.valPos;
                            } else if (globalFlag != -1) {//如果是全局的
                                if (getOneCharacterIndex(globalFlag, &tmpStr[offset], &charPro) == 1) {//找到全局的位置
                                    valPtr[i - 1] = charPro.valPos;
                                    findOK = 1;
                                }
                            } else {//如果没有找到则报警退出

                            }
                            last_grp = grp_mask;
                        } else {
                           valPtr[i - 1] = -1;
                        }
                    }

                    i++;
                    j = 0;
                    globalFlag = -1;
                }

            } else if (charPro.charType == D_CHAR_TYPE_PRCNAME) {//调用子程序

            }
        }
    } else {//表达式

    }
//    int i = 1;//除去自身，从1开始
//    int j = 0;
//    int pos;
//    int last_grp = -1;//记录上一次的参数所在的组
//    int opt_mask, grp_mask, type_mask;

//    decode_instruction_info *p_instruction_info;


//    pos = decodeGetInstructionIndex(str[j++]);

//    if(pos >= 0){
//        p_instruction_info = &DecodeInstrInfoGlobalTable[pos];
//        while(p_instruction_info->instrOrg[i].mask != -1){
//            type_mask = p_instruction_info->instrOrg[i].mask & 0xFF;//变量类型
//            grp_mask = (p_instruction_info->instrOrg[i].mask >> 16) & 0xF;//所在的组
//            opt_mask = (p_instruction_info->instrOrg[i].mask >> 24) & 0xF;//

//            if(opt_mask > 0){//为必选参数
//                if(type_mask){//直接索引界面，察看该类型的变量是否存在
//                    //该类型变量不存在，报警退出
//                    break;
//                }
//                //如果参数存在，设置指针指向，并设置使用的掩码位
//                parse_info->mark.mask |= 1<<i;//设置为1，表明使用该参数
//                //parse_info->m_mark.m_parg[i] = ;//指针
//                j++;
//            }else{//为可选参数
//                if(last_grp == grp_mask){//可选参数同组，互斥型不能同时出现，因此要报错
//                    //添加互斥型可选参数不能同时出现，需要报警
//                    break;
//                }
//                if(strncmp(str[j], p_instruction_info->instrOrg[i].name, strlen(p_instruction_info->instrOrg[i].name)) == 0){
//                    //如果参数存在，设置指针指向，并设置使用的掩码位，可选参数要从:=后开始
//                    parse_info->mark.mask |= 1<<i;//设置为1，表明使用该参数
//                    //parse_info->m_mark.m_parg[i] = ;//指针
//                    j++;
//                }
//            }
//            last_grp = grp_mask;
//            i++;
//        }
//    }
#endif
    return 0;
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
int decodeInstructionExecute(decode_instr_parse_info *parse_info)
{
    int errFlag = -1;
    decode_instruction_info *p_instruction_info;

    p_instruction_info = &DecodeInstrInfoGlobalTable[parse_info->index];
    errFlag = p_instruction_info->execute(parse_info);
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
int decode_instruct_accset_proc(char *str[])
{
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
int decode_instruct_worldacclim_proc(char *str[])
{
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
int decode_instruct_pathacclim_proc(char *str[])
{
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
int decode_instruct_speedlimaxis_proc(char *str[])
{
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
int decode_instruct_speedlimcheckpoint_proc(char *str[])
{
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
int decode_instruct_speedrefresh_proc(char *str[])
{
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
int decode_instruct_velset_proc(char *str[])
{
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
int decode_instruct_confj_proc(char *str[])
{
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
int decode_instruct_confl_proc(char *str[])
{
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
int decode_instruct_actunit_proc(char *str[])
{
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
int decode_instruct_deactunit_proc(char *str[])
{
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
int decode_instruct_mechunitload_proc(char *str[])
{
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
int decode_instruct_gripload_proc(char *str[])
{
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
int decode_instruct_motionsup_proc(char *str[])
{
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
int decode_instruct_singarea_proc(char *str[])
{
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
int decode_instruct_cirpathmode_proc(char *str[])
{
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
int decode_instruct_eoffson_proc(char *str[])
{
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
int decode_instruct_eoffsoff_proc(char *str[])
{
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
int decode_instruct_eoffsset_proc(char *str[])
{
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
int decode_instruct_pdispset_proc(char *str[])
{
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
int decode_instruct_pdispon_proc(char *str[])
{
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
int decode_instruct_pdispoff_proc(char *str[])
{
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
int decode_instruct_triggcheckio_proc(char *str[])
{
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
int decode_instruct_triggio_proc(char *str[])
{
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
int decode_instruct_triggequip_proc(char *str[])
{
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
int decode_instruct_triggint_proc(char *str[])
{
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
int decode_instruct_triggrampao_proc(char *str[])
{
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
int decode_instruct_triggspeed_proc(char *str[])
{
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
int decode_instruct_moveabsj_proc(char *str[])
{
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
int decode_instruct_moveextj_proc(char *str[])
{
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
int decodeInstructMovejAnalysis(char *str[], decode_instr_parse_info *pinfo)
{
	int mutex_flag = 0;//互斥型可选参数互斥标志
	int str_pos = 0;//字符串索引下标，找到匹配后才前进
//	instr_movej_data *p_movej_data = &pinfo->m_instr_data.m_movej_data;
//以下按命令参数出现的顺序排列，全部用大写比较
	if(strncmp(str[str_pos], "\\CONC", 5) == 0){
		str_pos++;
	}

	if(1){//索引ToPoint(robtarget类型)正确
		str_pos++;
	}

	if(strncmp(str[str_pos], "\\ID", 3) == 0){
		str_pos++;
	}

	if(1){//索引Speed(robtarget类型)正确
		str_pos++;
	}

	if(strncmp(str[str_pos], "\\V", 2) == 0){
		mutex_flag = 1;
		str_pos++;
	}

	if(mutex_flag == 0 && strncmp(str[str_pos], "\\T", 2) == 0){
		str_pos++;
	}
	mutex_flag = 0;

	if(1){//Zone
		str_pos++;
	}

	if(strncmp(str[str_pos], "\\Z", 2) == 0){
		str_pos++;
	}

	if(strncmp(str[str_pos], "\\INPOS", 6) == 0){
		str_pos++;
	}

	if(1){//Tool
		str_pos++;
	}

	if(strncmp(str[str_pos], "\\WOBJ", 5) == 0){
		str_pos++;
	}

	if(strncmp(str[str_pos], "\\TLOAD", 6) == 0){

	}



	return 1;
}

int decodeInstructMoveJExecute(decode_instr_parse_info *pinfo)
{
//	int i = 1;//自身的字符串需要忽略
//	int mask = pinfo->m_mark.m_mask;
    int errFlag = -1;
    decode_cmd_data cmdData;
    instr_movej_data *p_movej_data = (instr_movej_data *)&pinfo->instrData.movejData;
//    decode_character_property charPro;//测试用
    decode_type_data varData;

    cmdData.cmd = ITP_CMD_MoveJ;
    cmdData.prgPointer.modNum = pinfo->modNum;
    cmdData.prgPointer.modLineNum = pinfo->modLine;
//    if (getOneCharacterIndex(0, "Workobject_1", &charPro) == 1) {//测试用
//        if (charPro.charType == D_CHAR_TYPE_VARNAME) {
//            p_movej_data->wobj = charPro.valPos;
//        }
//    }

    if (p_movej_data->toPoint != -1) {
        decodeTypestructGetData(p_movej_data->toPoint, &varData);
        memcpy(&cmdData.P, &varData, sizeof(cmdData.P));
    }

    if (p_movej_data->speed != -1) {
        decodeTypestructGetData(p_movej_data->speed, &varData);
        memcpy(&cmdData.V, &varData, sizeof(cmdData.V));
    }

    if (p_movej_data->zoneData != -1) {
        decodeTypestructGetData(p_movej_data->zoneData, &varData);
        memcpy(&cmdData.Z, &varData, sizeof(cmdData.Z));
    }

    if (p_movej_data->tool != -1) {
        decodeTypestructGetData(p_movej_data->tool, &varData);
        memcpy(&cmdData.T, &varData, sizeof(cmdData.T));
    }

    if (p_movej_data->wobj != -1) {
        decodeTypestructGetData(p_movej_data->wobj, &varData);
        memcpy(&cmdData.W, &varData, sizeof(cmdData.W));
    }

    if (p_movej_data->time != -1) {
        decodeTypestructGetData(p_movej_data->time, &varData);
        memcpy(&cmdData.time, &varData, sizeof(cmdData.time));
//        cmdData.time = 1;
    } else {
        cmdData.time = 0;
    }

    if (decodePushCmdBuf(cmdData) == 1) {
        errFlag = D_INSTR_OWN_INTERPRATION;//是插补类型指令
    }
    //memcpy(p_movej_data, pinfo->mark.m_parg, sizeof(instr_movej_data));

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
int decode_instruct_movel_proc(char *str[])
{
	return 1;
}

int decodeInstructMoveLExecute(decode_instr_parse_info *pinfo)
{
    //	int i = 1;//自身的字符串需要忽略
    //	int mask = pinfo->m_mark.m_mask;
        int errFlag = -1;
        decode_cmd_data cmdData;
        instr_movel_data *p_movel_data = (instr_movel_data *)&pinfo->instrData.movelData;
//        decode_character_property charPro;//测试用
        decode_type_data varData;

        cmdData.cmd = ITP_CMD_MoveL;
        cmdData.prgPointer.modNum = pinfo->modNum;
        cmdData.prgPointer.modLineNum = pinfo->modLine;
//        if (getOneCharacterIndex(0, "Workobject_1", &charPro) == 1) {//测试用
//            if (charPro.charType == D_CHAR_TYPE_VARNAME) {
//                p_movel_data->wobj = charPro.valPos;
//            }
//        }


        if (p_movel_data->toPoint != -1) {
            decodeTypestructGetData(p_movel_data->toPoint, &varData);
            memcpy(&cmdData.P, &varData, sizeof(cmdData.P));
        }

        if (p_movel_data->speed != -1) {
            decodeTypestructGetData(p_movel_data->speed, &varData);
            memcpy(&cmdData.V, &varData, sizeof(cmdData.V));
        }

        if (p_movel_data->zoneData != -1) {
            decodeTypestructGetData(p_movel_data->zoneData, &varData);
            memcpy(&cmdData.Z, &varData, sizeof(cmdData.Z));
        }

        if (p_movel_data->tool != -1) {
            decodeTypestructGetData(p_movel_data->tool, &varData);
            memcpy(&cmdData.T, &varData, sizeof(cmdData.T));
        }

        if (p_movel_data->wobj != -1) {
            decodeTypestructGetData(p_movel_data->wobj, &varData);
            memcpy(&cmdData.W, &varData, sizeof(cmdData.W));
        }

        if (p_movel_data->time != -1) {
            decodeTypestructGetData(p_movel_data->time, &varData);
            memcpy(&cmdData.time, &varData, sizeof(cmdData.time));
        } else {
            cmdData.time = 0;
        }

        if (decodePushCmdBuf(cmdData) == 1) {
            errFlag = D_INSTR_OWN_INTERPRATION;//是插补类型指令
        }
        //memcpy(p_movej_data, pinfo->mark.m_parg, sizeof(instr_movej_data));

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
int decode_instruct_movec_proc(char *str[])
{
	return 1;
}

int decodeInstructMoveCExecute(decode_instr_parse_info *pinfo)
{
    //	int i = 1;//自身的字符串需要忽略
    //	int mask = pinfo->m_mark.m_mask;
        int errFlag = -1;
        decode_cmd_data cmdData;
        instr_movec_data *p_movec_data = (instr_movec_data *)&pinfo->instrData.movecData;
//        decode_character_property charPro;//测试用
        decode_type_data varData;

        cmdData.cmd = ITP_CMD_MoveC;
        cmdData.prgPointer.modNum = pinfo->modNum;
        cmdData.prgPointer.modLineNum = pinfo->modLine;
//        if (getOneCharacterIndex(0, "Workobject_1", &charPro) == 1) {//测试用
//            if (charPro.charType == D_CHAR_TYPE_VARNAME) {
//                p_movec_data->wobj = charPro.valPos;
//            }
//        }



        if (p_movec_data->toPoint != -1) {
            decodeTypestructGetData(p_movec_data->toPoint, &varData);
            memcpy(&cmdData.P, &varData, sizeof(cmdData.P));
        }
        if (p_movec_data->cirPoint != -1) {
            decodeTypestructGetData(p_movec_data->cirPoint, &varData);
            memcpy(&cmdData.P_Circle, &varData, sizeof(cmdData.P_Circle));
        }

        if (p_movec_data->speed != -1) {
            decodeTypestructGetData(p_movec_data->speed, &varData);
            memcpy(&cmdData.V, &varData, sizeof(cmdData.V));
        }

        if (p_movec_data->zoneData != -1) {
            decodeTypestructGetData(p_movec_data->zoneData, &varData);
            memcpy(&cmdData.Z, &varData, sizeof(cmdData.Z));
        }

        if (p_movec_data->tool != -1) {
            decodeTypestructGetData(p_movec_data->tool, &varData);
            memcpy(&cmdData.T, &varData, sizeof(cmdData.T));
        }

        if (p_movec_data->wobj != -1) {
            decodeTypestructGetData(p_movec_data->wobj, &varData);
            memcpy(&cmdData.W, &varData, sizeof(cmdData.W));
        }

        if (p_movec_data->time != -1) {
            decodeTypestructGetData(p_movec_data->time, &varData);
            memcpy(&cmdData.time, &varData, sizeof(cmdData.time));
        } else {
            cmdData.time = 0;
        }

        if (decodePushCmdBuf(cmdData) == 1) {
            errFlag = D_INSTR_OWN_INTERPRATION;//是插补类型指令
        }
        //memcpy(p_movej_data, pinfo->mark.m_parg, sizeof(instr_movej_data));

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
int decode_instruct_searchc_proc(char *str[])
{
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
int decode_instruct_searchextj_proc(char *str[])
{
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
int decode_instruct_searchl_proc(char *str[])
{
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
int decode_instruct_triggj_proc(char *str[])
{
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
int decode_instruct_triggl_proc(char *str[])
{
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
int decode_instruct_triggc_proc(char *str[])
{
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
int decode_instruct_trigglios_proc(char *str[])
{
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
int decode_instruct_clearpath_proc(char *str[])
{
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
int decode_instruct_storepath_proc(char *str[])
{
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
int decode_instruct_restopath_proc(char *str[])
{
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
int decode_instruct_startmove_proc(char *str[])
{
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
int decode_instruct_startmoveretry_proc(char *str[])
{
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
int decode_instruct_stopmove_proc(char *str[])
{
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
int decode_instruct_stopmovereset_proc(char *str[])
{
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
decode_instruction_info *decodeGetInstructionInfo(int instrType)
{
    if (instrType < INSTR_END_CNT) {
        return &DecodeInstrInfoGlobalTable[instrType];
    }
    return NULL;
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
int decodePushCmdBuf(decode_cmd_data cmdData)//此处需考虑添加互斥访问
{
    int errFlag = -1;
    int pos;
    if (DecodeCmdQueGlobal.cnt < DECODE_CMD_BUF_LEN) {
        pos = DecodeCmdQueGlobal.writePos;
        memcpy(&DecodeCmdQueGlobal.cmdDataBuf[pos], &cmdData, sizeof(decode_cmd_data));
        DecodeCmdQueGlobal.writePos = (pos + 1) % DECODE_CMD_BUF_LEN;
        DecodeCmdQueGlobal.cnt++;
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
int decodePopCmdBuf(decode_cmd_data *cmdData)//此处需考虑添加互斥访问
{
    int errFlag = -1;
    int pos;
    if (DecodeCmdQueGlobal.cnt > 0) {
        pos = DecodeCmdQueGlobal.readPos;
        memcpy(cmdData, &DecodeCmdQueGlobal.cmdDataBuf[pos], sizeof(decode_cmd_data));
        DecodeCmdQueGlobal.readPos = (pos + 1) % DECODE_CMD_BUF_LEN;
        DecodeCmdQueGlobal.cnt--;
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
int decodeResetCmdBuf(void)
{
    DecodeCmdQueGlobal.cnt = 0;
    DecodeCmdQueGlobal.readPos = 0;
    DecodeCmdQueGlobal.writePos = 0;
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
int decodePushParserBuf(decode_instr_parse_info parserData)//此处需考虑添加互斥访问
{
    int errFlag = -1;
    int pos;
    if (DecodeParserQueGlobal.cnt < DECODE_PARSER_BUF_LEN) {
        pos = DecodeParserQueGlobal.writePos;
        memcpy(&DecodeParserQueGlobal.parserDataBuf[pos], &parserData, sizeof(decode_instr_parse_info));
        DecodeParserQueGlobal.writePos = (pos + 1) % DECODE_PARSER_BUF_LEN;
        DecodeParserQueGlobal.cnt++;
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
int decodePopParserBuf(decode_instr_parse_info *parserData)//此处需考虑添加互斥访问
{
    int errFlag = -1;
    int pos;
    if (DecodeParserQueGlobal.cnt > 0) {
        pos = DecodeParserQueGlobal.readPos;
        memcpy(parserData, &DecodeParserQueGlobal.parserDataBuf[pos], sizeof(decode_instr_parse_info));
        DecodeParserQueGlobal.readPos = (pos + 1) % DECODE_PARSER_BUF_LEN;
        DecodeParserQueGlobal.cnt--;
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
int decodeGetParserBufCnt(void)
{
    return DecodeParserQueGlobal.cnt;
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
int decodeCheckParserBufFull(void)
{
    if (DecodeParserQueGlobal.cnt < DECODE_PARSER_BUF_LEN) {
        return 0;
    }
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
int decodeCheckParserBufCurCmdType(void)
{
    int errFlag = -1;
    int pos;
    int index;

    if (DecodeParserQueGlobal.cnt > 0) {
        pos = DecodeParserQueGlobal.readPos;
        index = DecodeParserQueGlobal.parserDataBuf[pos].index;
        errFlag = (DecodeInstrInfoGlobalTable[index].instrOrg[0].mask >> 8) & 0xFF;
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
int decodeResetParserBuf(void)
{
   DecodeParserQueGlobal.cnt = 0;
   DecodeParserQueGlobal.readPos = 0;
   DecodeParserQueGlobal.writePos = 0;
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

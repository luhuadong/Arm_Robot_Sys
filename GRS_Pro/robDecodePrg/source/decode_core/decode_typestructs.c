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
*File name: decode_typestructs.c
*First Author: 
*Date:06/17/2009  
*文件功能描述：
****************************************************************************
****************************************************************************/

#define  DECODE_TYPESTRUCTS_GLOBALS

/**********************************************************************/
/*Add all #includes here*/
#include "decode_typestructs.h"
#include "decode_public.h"
#include "robotdecodepublic.h"
#include "sharemempublic.h"
/**********************************************************************/

/**********************************************************************/
/*Add all #defines here*/
decode_typestruct_index_info DecodeTypeIndexInfoGlobalTable;
decode_type_data DecodeTypeDataArryGlobal[D_TYPEDATA_ARRY_CNT];//存储变量数据
//decode_type_data *DecodeTypeDataArryGlobal;//改为指针，指向共享内存
char DecodeTypeDataUseFlag[D_TYPEDATA_ARRY_CNT];//存储变量空间被使用标记，1为使用
int DecodeTypeDataArryPos = 0;//当前可用空间的位置
int DecodeTypeDataShmemId;//记录变量共享内存的ID

decode_typestruct_info DecodeTypeInfoGlobalTable[D_DATATYPE_END_CNT+1] = {
//    {{{{" ", 0xFFFFFFFF},}, 0xFFFFFFFF}, decodeTypestructRobtargetExecute}, //第一行为无效，预留
//    {{{{"robtarget", 0x00000000}, {"trans", 0x0000000D}, {"rot", 0x0000000E}, {"robconf", 0x00000011}, {"extax", 0x00000012}}, 0xFFFFFFFF}, decodeTypestructRobtargetExecute}, //robotarget
    {{{" ", -1}, }, -1, NULL}, //第一行为无效，预留
    {{{"VAR", 0x1}, {" ", -1}}, 0x0, NULL},
    {{{"CONST", 0x2}, {" ", -1}}, 0x0, NULL},
    {{{"PERS", 0x3}, {" ", -1}}, 0x0, NULL},
    {{{"LOCAL", 0x4}, {" ", -1}}, 0x0, NULL},
    {{{"TASK", 0x5}, {" ", -1}}, 0x0, NULL},
    {{{"robtarget", 0x6}, {"trans", 0x12}, {"rot", 0x13}, {"robconf", 0x16}, {"extax", 0x17}, {" ", -1}}, 0x0, decodeTypestructRobtargetExecute}, //robotarget
    {{{"switch", 0x7}, {"switch", 0x7}, {" ", -1}}, 0x44, NULL},//基本数据类型[1]也要为自身，方便编程
    {{{"identno", 0x8}, {"identno", 0x8}, {" ", -1}}, 0x44, NULL},//基本数据类型[1]也要为自身，方便编程
    {{{"speeddata", 0x9}, {"v_tcp", 0x19}, {"v_ori", 0x19}, {"v_leax", 0x19}, {"v_reax", 0x19}, {" ", -1}}, 0x0, NULL},
    {{{"num", 0xA}, {"num", 0xA}, {" ", -1}}, 0x44, NULL},//基本数据类型[1]也要为自身，方便编程
    {{{"zonedata", 0xB}, {"finep", 0x10}, {"pzone_tcp", 0x19}, {"pzone_ori", 0x19}, {"pzone_eax", 0x19}, {"zone_ori", 0x19}, {"zone_leax", 0x19}, {"zone_reax", 0x19}, {" ", -1}}, 0x0, NULL},
    {{{"stoppointdata", 0xC}, {"type", 0x18}, {"progsynch", 0x10}, {"inpos", 0x1A}, {"stoptime", 0x19}, {"followtime", 0x19}, {"signal", 0x15}, {"relation", 0x1B}, {"checkvalue", 0x19}, {" ", -1}}, 0x0, NULL},
    {{{"tooldata", 0xD}, {"robhold", 0x10}, {"tframe", 0x14}, {"tload", 0xF}, {" ", -1}}, 0x0, NULL},
    {{{"wobjdata", 0xE}, {"robhold", 0x10}, {"ufprog", 0x10}, {"ufmec", 0x15}, {"uframe", 0x14}, {"oframe", 0x14}, {" ", -1}}, 0x0, NULL},
    {{{"loaddata", 0xF}, {"mass", 0x19}, {"cog", 0x12}, {"aom", 0x13}, {"ix", 0x19}, {"iy", 0x19}, {"iz", 0x19}, {" ", -1}}, 0x0, NULL},
    {{{"bool", 0x10}, {"bool", 0x10}, {" ", -1}}, 0x11, NULL},//基本数据类型[1]也要为自身，方便编程
    {{{"dnum", 0x11}, {"dnum", 0x11}, {" ", -1}}, 0x88, NULL},//基本数据类型[1]也要为自身，方便编程
    {{{"pos", 0x12}, {"x", 0x19}, {"y", 0x19}, {"z", 0x19}, {" ", -1}}, 0x0, NULL},
    {{{"orient", 0x13}, {"q1", 0x19}, {"q2", 0x19}, {"q3", 0x19}, {"q4", 0x19}, {" ", -1}}, 0x0, NULL},
    {{{"pose", 0x14}, {"trans", 0x12}, {"rot", 0x13}, {" ", -1}}, 0x0, NULL},
    {{{"string", 0x15}, {"string", 0x15}, {" ", -1}}, 0x501, NULL},//0x50 80个字符
    {{{"confdata", 0x16}, {"cf1", 0x18}, {"cf4", 0x18}, {"cf6", 0x18}, {"cfx", 0x18}, {" ", -1}}, 0x0, NULL},
    {{{"extjoint", 0x17}, {"eax_a", 0x19}, {"eax_b", 0x19}, {"eax_c", 0x19}, {"eax_d", 0x19}, {"eax_e", 0x19}, {"eax_f", 0x19}, {" ", -1}}, 0x0, NULL},
    {{{"num_i", 0x18}, {"num_i", 0x18}, {" ", -1}}, 0x44, NULL},//基本数据类型[1]也要为自身，方便编程
    {{{"num_d", 0x19}, {"num_d", 0x19}, {" ", -1}}, 0x88, NULL},//基本数据类型[1]也要为自身，方便编程
//    {{{"stoppoint", 0x1A}, {"inpos", 0x18}, {"stoptime", 0x19}, {"followtime", 0x19}, {" ", -1}}, 0x0, NULL},
    {{{"inposdata", 0x1A}, {"position", 0x19}, {"speed", 0x19}, {"mintime", 0x19}, {"maxtime", 0x19}, {" ", -1}}, 0x0, NULL},
    {{{"opnum", 0x1B}, {"opnum", 0x1B}, {" ", -1}}, 0x44, NULL},//基本数据类型[1]也要为自身，方便编程
    {{{"robjoint", 0x1C}, {"rax_1", 0x19}, {"rax_2", 0x19}, {"rax_3", 0x19}, {"rax_4", 0x19}, {"rax_5", 0x19}, {"rax_6", 0x19}, {" ", -1}}, 0x0, NULL},
    {{{"jointtarget", 0x1D}, {"robax", 0x1C}, {"extax", 0x17}, {" ", -1}}, 0x0, NULL},

};
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
#if 1// 4zi jie dui qi
int decodeTypestructCalTypeSizeof(int type)
{
    int mask;
    int maxByte = 0;
    int minByte = 0;
    int tmpMax;
    int tmpMin;
    int i = 1;
    int tmpType;

    if ((DecodeTypeInfoGlobalTable[type].mask & 0xFFFF) == 0) {
        while (DecodeTypeInfoGlobalTable[type].typeElement[i].elementMask != -1) {
            tmpType = DecodeTypeInfoGlobalTable[type].typeElement[i].elementMask & 0xFF;
            if ((DecodeTypeInfoGlobalTable[tmpType].mask & 0xFFFF) == 0) {
                mask = decodeTypestructCalTypeSizeof(tmpType);
//                DecodeTypeInfoGlobalTable[tmpType].mask |= mask;
            } else {
                mask = DecodeTypeInfoGlobalTable[tmpType].mask;
            }
            {
                if ((maxByte == 0) && (minByte == 0)) {
                    maxByte = (mask >> 4) & 0xFFF;
                    minByte = mask & 0xF;
                    if (minByte >= STRUCT_ALIGN) {
                        minByte = STRUCT_ALIGN;
                    }
                } else {
                    tmpMax = (mask >> 4) & 0xFFF;
                    tmpMin = mask & 0xF;
                    if (tmpMin >= STRUCT_ALIGN) {
                        if ((maxByte % STRUCT_ALIGN) != 0) {
                            maxByte += (STRUCT_ALIGN - maxByte % STRUCT_ALIGN);
                        }
                        minByte = STRUCT_ALIGN;
                    } else {
                        if ((maxByte % tmpMin) != 0) {
                            maxByte += (tmpMin - maxByte % tmpMin);
                        }
//                        maxByte += tmpMax;
                    }
                    maxByte += tmpMax;
//                    if (tmpMin > minByte) {
//                        if ((maxByte % tmpMin) != 0) {
//                            maxByte += (tmpMin - maxByte % tmpMin) + tmpMax;
//                        }
//                        minByte = tmpMin;
//                    } else {
//                        if ((maxByte % tmpMin) != 0) {
//                            maxByte += (tmpMin - maxByte % tmpMin);
//                        }
//                        maxByte += tmpMax;
//                    }
                }
            }
            i++;
        }
        if ((maxByte != 0) && (minByte != 0)) {
            if ((maxByte % minByte) != 0) {
                maxByte += (minByte - maxByte % minByte);// % STRUCT_ALIGN;
            }
            DecodeTypeInfoGlobalTable[type].mask = (maxByte << 4) | minByte;
        }
    }
    return DecodeTypeInfoGlobalTable[type].mask;
}
#else// 8zi jie dui qi
int decodeTypestructCalTypeSizeof(int type)
{
    int mask;
    int maxByte = 0;
    int minByte = 0;
    int tmpMax;
    int tmpMin;
    int i = 1;
    int tmpType;

    if ((DecodeTypeInfoGlobalTable[type].mask & 0xFFFF) == 0) {
        while (DecodeTypeInfoGlobalTable[type].typeElement[i].elementMask != -1) {
            tmpType = DecodeTypeInfoGlobalTable[type].typeElement[i].elementMask & 0xFF;
            if ((DecodeTypeInfoGlobalTable[tmpType].mask & 0xFFFF) == 0) {
                mask = decodeTypestructCalTypeSizeof(tmpType);
//                DecodeTypeInfoGlobalTable[tmpType].mask |= mask;
            } else {
                mask = DecodeTypeInfoGlobalTable[tmpType].mask;
            }
            {
                if ((maxByte == 0) && (minByte == 0)) {
                    maxByte = (mask >> 4) & 0xFFF;
                    minByte = mask & 0xF;
                } else {
                    tmpMax = (mask >> 4) & 0xFFF;
                    tmpMin = mask & 0xF;
                    if (tmpMin > minByte) {
                        if ((maxByte % tmpMin) != 0) {
                            maxByte += (tmpMin - maxByte % tmpMin) + tmpMax;
                        }
                        minByte = tmpMin;
                    } else {
                        if ((maxByte % tmpMin) != 0) {
                            maxByte += (tmpMin - maxByte % tmpMin);
                        }
                        maxByte += tmpMax;
                    }
                }
            }
            i++;
        }
        if ((maxByte != 0) && (minByte != 0)) {
            if ((maxByte % minByte) != 0) {
                maxByte += minByte - maxByte % minByte;
            }
            DecodeTypeInfoGlobalTable[type].mask = (maxByte << 4) | minByte;
        }
    }
    return DecodeTypeInfoGlobalTable[type].mask;
}
#endif
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
void decodeTypestructMaskTest(void);
int decodeInitTypestructIndexInfo(void)
{
#if 0
    int flag = 0;
    int i, j;
    int m = 0;

    for(i=1; i<=D_TYPENAME_LEN; i++){
        for(j=0; j<D_DATATYPE_END_CNT; j++){
            if((int)strlen(DecodeTypeInfoGlobalTable[j].typeElement[0].elementName) == i){
                if(flag == 0){
                    flag = 1;
                    DecodeTypeIndexInfoGlobalTable.beginPos[i] = m;
                }
                DecodeTypeIndexInfoGlobalTable.typestructIndex[m].index = j;
                strcpy(DecodeTypeIndexInfoGlobalTable.typestructIndex[m].typeName, DecodeTypeInfoGlobalTable[j].typeElement[0].elementName);
                m++;
            }
        }
        if(flag == 1){
            DecodeTypeIndexInfoGlobalTable.endPos[i] = m;
            flag = 0;
        }
    }
#else
//    int flag = 0;
    int i, j;
    decode_character_property charPro;

//    D_TYPEDATA_SHMEM_NAME
//    DecodeTypeDataShmemId = createUserSharemem(D_TYPEDATA_SHMEM_NAME, sizeof(decode_type_data)*D_TYPEDATA_ARRY_CNT);
//    if (DecodeTypeDataShmemId != -1) {
//        DecodeTypeDataArryGlobal = mmapUserSharemem(DecodeTypeDataShmemId, sizeof(decode_type_data)*D_TYPEDATA_ARRY_CNT, 1);//要先清零
//    }
    for (i=1; i<D_DATATYPE_END_CNT; i++) {//首个保留，所以从1开始
        strcpy(charPro.charName, DecodeTypeInfoGlobalTable[i].typeElement[0].elementName);
        charPro.charType = D_CHAR_TYPE_VARTYPE;
        charPro.selfIndex = i;
        if ((DecodeTypeInfoGlobalTable[i].mask & 0xFFFF) == 0) {
            DecodeTypeInfoGlobalTable[i].mask |= decodeTypestructCalTypeSizeof(i);
        }
        if (createOneCharacterIndex(charPro) == -1) {
            return -1;
        }
    }
    decodeTypestructMaskTest();
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
int decodeGetTypestructIndex(char *str)
{
    int pos = strlen(str);
    int i;
    int index = -1;

    i = DecodeTypeIndexInfoGlobalTable.beginPos[pos];
    for( ; i<DecodeTypeIndexInfoGlobalTable.endPos[pos]; i++){
        if(strcmp(str, DecodeTypeIndexInfoGlobalTable.typestructIndex[i].typeName) == 0){
            index = DecodeTypeIndexInfoGlobalTable.typestructIndex[i].index;
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
int decodeTypestructRobtargetExecute(decode_type_data *pTypeData)
{
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
int decodeTypestructMallocDataStoreBuf(void)
{
    int i = 0;

    while (DecodeTypeDataUseFlag[DecodeTypeDataArryPos] == 1) {
        if (i == D_TYPEDATA_ARRY_CNT) {//存储空间满
            return -1;
        }
        DecodeTypeDataArryPos++;
        if (DecodeTypeDataArryPos == D_TYPEDATA_ARRY_CNT) {//缓冲回环
            DecodeTypeDataArryPos = 0;
        }
        i++;
    }
    DecodeTypeDataUseFlag[DecodeTypeDataArryPos] = 1;//标记被使用
    return DecodeTypeDataArryPos;
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
void decodeTypestructDeleteDataStoreBuf(int pos)
{
    if (pos < D_TYPEDATA_ARRY_CNT) {
        DecodeTypeDataUseFlag[pos] = 0;//标记被使用
    }
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
int decodeTypestructGetData(int pos, decode_type_data *typeData)
{
    if (pos < D_TYPEDATA_ARRY_CNT) {
//        *typeData = DecodeTypeDataArryGlobal[pos];//联合体一定要拷贝内存，不能直接赋值
        memcpy(typeData, &DecodeTypeDataArryGlobal[pos], sizeof(decode_type_data));
        return 1;
    }
    return -1;
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
int decodeTypestructSetData(int pos, decode_type_data typeData)
{
    if (pos < D_TYPEDATA_ARRY_CNT) {
//        DecodeTypeDataArryGlobal[pos] = typeData;//联合体一定要拷贝内存，不能直接赋值
        memcpy(&DecodeTypeDataArryGlobal[pos], &typeData, sizeof(decode_type_data));
        return 1;
    }
    return -1;
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
#pragma pack(8)
typedef struct {
    char a;
    double b;
    double c;
    char d;
}tmp;

typedef struct {//__attribute((aligned(8)))
//    double b;
//    char a;
//    double b;
//    double c;
//    int a;
//    int b;
//    int c;
//    tmp d;
    char a[11];
}test;
//#pragma pack()
void decodeTypestructMaskTest(void)
{
    char buf[100];
    int i = 6;
    char test3;
    double test1, test2;
    test a;
//    a.a = 'm';
//    a.b = 4.5;
//    a.c = 5.6;

//    test3 = *((int *)((int)&a + 0));
//    test1 = *((double *)((int)&a + 4));
//    test2 = *((double *)((int)&a + 12));
//    test1 = -1.96661E-07;
//    sprintf(buf, "%.9f %-.9f", test1, 1.2345);
//    sprintf(buf, "%e %e", test1, 1.234556);


    sprintf(buf, "%d %d", sizeof(tmp), sizeof(test));

    sprintf(buf, "%d %d %d", sizeof(decode_robtarget), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_switch), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_identno), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_speeddata), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_num), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_zonedata), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_stoppointdata), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_tooldata), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_wobjdata), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_loaddata), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_bool), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_dnum), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_pos), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_orient), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_pose), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_string), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_confdata), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_extjoint), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_num_i), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_num_d), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
//    sprintf(buf, "%d %d %d", sizeof(decode_stoppoint), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
//    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_inposdata), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_opnum), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_robjoint), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
    i++;
    sprintf(buf, "%d %d %d", sizeof(decode_jointtarget), (DecodeTypeInfoGlobalTable[i].mask>>4)&0xFFF, DecodeTypeInfoGlobalTable[i].mask&0xF);
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
int decodeTypestructScanfVal(int type, char **str, void *valOft)
{
    int mask;
    int maxByte = 0;
    int minByte = 0;
    int tmpMax;
    int tmpMin;
    int addrOft = 0;//地址偏移起始位置
    int i = 1;
//    int i = 0;//change by wuheng 2015.12.2改为0，解决首个是基础类型情况
    int j = 0;
    int tmpType;
    char *pStr = *str;
    int flag = 0;
    char tmpStr[50];
    U32 addr;
    decode_typestruct_info *pInfo = &DecodeTypeInfoGlobalTable[type];
    double tmpDoubleVal;

    if (*pStr == '[') {//为了起始配对
        flag++;
        pStr++;
    }
    addr = (U32)valOft;
    while (pInfo->typeElement[i].elementMask != -1) {
//        if (i != 0) {//change by wuheng 2015.12.2改为0，解决首个是基础类型情况
            tmpType = pInfo->typeElement[i].elementMask & 0xFF;
            mask = DecodeTypeInfoGlobalTable[tmpType].mask;
    //        maxByte = (DecodeTypeInfoGlobalTable[tmpType].mask >> 4) & 0xFFF;
    //        minByte = DecodeTypeInfoGlobalTable[tmpType].mask & 0xF;
            if ((maxByte == 0) && (minByte == 0)) {
                maxByte = (mask >> 4) & 0xFFF;
                minByte = mask & 0xF;
                if (minByte >= STRUCT_ALIGN) {
                    minByte = STRUCT_ALIGN;
                }
            } else {
                tmpMax = (mask >> 4) & 0xFFF;
                tmpMin = mask & 0xF;
                if (tmpMin >= STRUCT_ALIGN) {
                    if ((maxByte % STRUCT_ALIGN) != 0) {
                        maxByte += (STRUCT_ALIGN - maxByte % STRUCT_ALIGN);
//                        addrOft = maxByte;
//                        maxByte += tmpMax;
                    }
                    minByte = STRUCT_ALIGN;
                } else {
                    if ((maxByte % tmpMin) != 0) {
                        maxByte += (tmpMin - maxByte % tmpMin);
                    }
//                    addrOft = maxByte;
//                    maxByte += tmpMax;
                }
                addrOft = maxByte;
                maxByte += tmpMax;
            }
//        }
//        while ((*pStr != '[') || (*pStr != ']') || (*pStr != ',') || (*pStr != ';')) {
//            tmpStr[j] = *pStr;
//            j++;
//            pStr++;
//        }

        if (tmpType == D_DATATYPE_BOOL) {
            while ((*pStr != '[') && (*pStr != ']') && (*pStr != ',') && (*pStr != ';')) {
                tmpStr[j] = *pStr;
                j++;
                pStr++;
            }
            tmpStr[j] = 0;
            if (decodeStringCompare(tmpStr, "TRUE", 0) == 0) {
                *((char *)(addr + addrOft)) = 1;
            } else if (decodeStringCompare(tmpStr, "FALSE", 0) == 0) {
                *((char *)(addr + addrOft)) = 0;
            }

        } else if (tmpType == D_DATATYPE_NUM_I) {
            while ((*pStr != '[') && (*pStr != ']') && (*pStr != ',') && (*pStr != ';')) {
                tmpStr[j] = *pStr;
                j++;
                pStr++;
            }
            tmpStr[j] = 0;
            *((int *)(addr + addrOft)) = atoi(tmpStr);
        } else if (tmpType == D_DATATYPE_NUM_D) {
            while ((*pStr != '[') && (*pStr != ']') && (*pStr != ',') && (*pStr != ';')) {
                tmpStr[j] = *pStr;
                j++;
                pStr++;
            }
            tmpStr[j] = 0;
            *((double *)(addr + addrOft)) = atof(tmpStr);
//            sscanf(tmpStr, "%g", &tmpDoubleVal);
//            *((double *)(addr + addrOft)) = tmpDoubleVal;
        } else if (tmpType == D_DATATYPE_NUM) {
            while ((*pStr != '[') && (*pStr != ']') && (*pStr != ',') && (*pStr != ';')) {
                tmpStr[j] = *pStr;
                j++;
                pStr++;
            }
            tmpStr[j] = 0;
            *((int *)(addr + addrOft)) = atoi(tmpStr);
        } else if (tmpType == D_DATATYPE_DNUM) {
            while ((*pStr != '[') && (*pStr != ']') && (*pStr != ',') && (*pStr != ';')) {
                tmpStr[j] = *pStr;
                j++;
                pStr++;
            }
            tmpStr[j] = 0;
            *((int *)(addr + addrOft)) = atoi(tmpStr);
        } else if (tmpType == D_DATATYPE_OPNUM) {
            while ((*pStr != '[') && (*pStr != ']') && (*pStr != ',') && (*pStr != ';')) {
                tmpStr[j] = *pStr;
                j++;
                pStr++;
            }
            tmpStr[j] = 0;
            *((int *)(addr + addrOft)) = atoi(tmpStr);
        } else if (tmpType == D_DATATYPE_STRING) {
            while ((*pStr != '[') && (*pStr != ']') && (*pStr != ',') && (*pStr != ';')) {
                tmpStr[j] = *pStr;
                j++;
                pStr++;
            }
            tmpStr[j] = 0;
            if (strlen(tmpStr) > 2) {
                memcpy((void *)(addr + addrOft), &tmpStr[1], strlen(tmpStr) - 2);//去掉双引号
            } else {//只有双引号
                *((char *)(addr + addrOft)) = 0;
            }

        } else if (tmpType == D_DATATYPE_SWITCH) {
            while ((*pStr != '[') && (*pStr != ']') && (*pStr != ',') && (*pStr != ';')) {
                tmpStr[j] = *pStr;
                j++;
                pStr++;
            }
            tmpStr[j] = 0;
            *((int *)(addr + addrOft)) = atoi(tmpStr);
        } else if (tmpType == D_DATATYPE_IDENTNO) {
            while ((*pStr != '[') && (*pStr != ']') && (*pStr != ',') && (*pStr != ';')) {
                tmpStr[j] = *pStr;
                j++;
                pStr++;
            }
            tmpStr[j] = 0;
            *((int *)(addr + addrOft)) = atoi(tmpStr);
        } else {
//            if (i != 0) {
                decodeTypestructScanfVal(tmpType, &pStr, (void *)(addr + addrOft));
                pStr++;
//            }
        }
        if (*pStr == ',') {
            pStr++;
        }
        j = 0;
        i++;
    }

    if (*pStr == ']') {//一组变量结束
        flag--;
    }
    *str = pStr;

    if (flag != 0) {//不匹配报错
        return -1;
    }
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
#if 1
int decodeTypestructTransValToStr(int type, char *str, decode_type_data *varVal)
{
    int addrOft = 0;//地址偏移起始位置
    int mask;
    int maxByte = 0;
    int minByte = 0;
    int tmpMax;
    int tmpMin;
    U32 addr;
    int tmpType;
    char *pStr = str;
    int i = 1;
//    int i = 0;//change by wuheng 2015.12.2改为0，解决首个是基础类型情况
    int flag = 0;
    double temVal;

    decode_typestruct_info *pInfo = &DecodeTypeInfoGlobalTable[type];

    if ((type != D_DATATYPE_BOOL) && (type != D_DATATYPE_NUM_I) && (type != D_DATATYPE_NUM_D) && (type != D_DATATYPE_NUM)
            && (type != D_DATATYPE_DNUM) && (type != D_DATATYPE_OPNUM) && (type != D_DATATYPE_STRING) && (type != D_DATATYPE_SWITCH) && (type != D_DATATYPE_IDENTNO)) {
        sprintf(pStr, "%s", "[");
        pStr++;
        flag = 1;
    } else {
        *pStr = 0;
    }

    addr = (U32)varVal;
    while (pInfo->typeElement[i].elementMask != -1) {
//        if (i != 0) {//change by wuheng 2015.12.2改为0，解决首个是基础类型情况
            tmpType = pInfo->typeElement[i].elementMask & 0xFF;
            mask = DecodeTypeInfoGlobalTable[tmpType].mask;
            if ((maxByte == 0) && (minByte == 0)) {
                maxByte = (mask >> 4) & 0xFFF;
                minByte = mask & 0xF;
                if (minByte >= STRUCT_ALIGN) {
                    minByte = STRUCT_ALIGN;
                }
            } else {
                tmpMax = (mask >> 4) & 0xFFF;
                tmpMin = mask & 0xF;
                if (tmpMin >= STRUCT_ALIGN) {
                    if ((maxByte % STRUCT_ALIGN) != 0) {
                        maxByte += (STRUCT_ALIGN - maxByte % STRUCT_ALIGN);
//                        addrOft = maxByte;
//                        maxByte += tmpMax;
                    }
                    minByte = STRUCT_ALIGN;
                } else {
                    if ((maxByte % tmpMin) != 0) {
                        maxByte += (tmpMin - maxByte % tmpMin);
                    }
//                    addrOft = maxByte;
//                    maxByte += tmpMax;
                }
                addrOft = maxByte;
                maxByte += tmpMax;
            }
//        }

        if (tmpType == D_DATATYPE_BOOL) {
            if (*((char *)(addr + addrOft)) == 1) {
                sprintf(pStr, "%s", "TRUE");
            } else {
                sprintf(pStr, "%s", "FALSE");
            }
        } else if (tmpType == D_DATATYPE_NUM_I) {
            sprintf(pStr, "%d", *((int *)(addr + addrOft)));
        } else if (tmpType == D_DATATYPE_NUM_D) {
//            if ((*((double *)(addr + addrOft)) > -0.000001) && (*((double *)(addr + addrOft)) < 0.000001)) {
//            temVal = *((double *)(addr + addrOft)) - (int)(*((double *)(addr + addrOft)));
//            if ((temVal > -0.000001) && (temVal < 0.000001)) {
//               sprintf(pStr, "%0.0f", *((double *)(addr + addrOft)));
//            } else {
//               sprintf(pStr, "%0.9f", *((double *)(addr + addrOft)));
//            }
            sprintf(pStr, "%G", *((double *)(addr + addrOft)));
        } else if (tmpType == D_DATATYPE_NUM) {
            sprintf(pStr, "%d", *((int *)(addr + addrOft)));
        } else if (tmpType == D_DATATYPE_DNUM) {
            sprintf(pStr, "%d", *((int *)(addr + addrOft)));
        } else if (tmpType == D_DATATYPE_OPNUM) {
            sprintf(pStr, "%d", *((int *)(addr + addrOft)));
        } else if (tmpType == D_DATATYPE_STRING) {
            sprintf(pStr, "\"%s\"", ((char *)(addr + addrOft)));
        } else if (tmpType == D_DATATYPE_SWITCH) {
            sprintf(pStr, "%d", *((int *)(addr + addrOft)));
        } else if (tmpType == D_DATATYPE_IDENTNO) {
            sprintf(pStr, "%d", *((int *)(addr + addrOft)));
        } else {//此处递归调用
//            if (i != 0) {
    //            sprintf(pStr, "%s", "[");
                decodeTypestructTransValToStr(tmpType, pStr, (decode_type_data *)(addr + addrOft));
    //            strcat(pStr, "]");
//            }
        }
//        if (i != 0) {
            if (pInfo->typeElement[i+1].elementMask != -1) {
                strcat(pStr, ",");
            }
//        }
        pStr = (char *)(pStr + strlen(pStr));
        i++;
    }



    *pStr = 0;
    if (flag == 1) {
        strcat(pStr, "]");
    }

    return 1;
}
#else
int decodeTypestructTransValToStr(int type, char *str, decode_type_data *varVal)
{
    int addrOft = 0;//地址偏移起始位置
    int mask;
    int maxByte = 0;
    int minByte = 0;
    int tmpMax;
    int tmpMin;
    U32 addr;
    int tmpType;
    char *pStr = str;
    int i = 1;
//    int i = 0;//change by wuheng 2015.12.2改为0，解决首个是基础类型情况
    int flag = 0;
    double temVal;

    decode_typestruct_info *pInfo = &DecodeTypeInfoGlobalTable[type];

    if ((type != D_DATATYPE_BOOL) && (type != D_DATATYPE_NUM_I) && (type != D_DATATYPE_NUM_D) && (type != D_DATATYPE_NUM)
            && (type != D_DATATYPE_DNUM) && (type != D_DATATYPE_OPNUM) && (type != D_DATATYPE_STRING) && (type != D_DATATYPE_SWITCH) && (type != D_DATATYPE_IDENTNO)) {
        sprintf(pStr, "%s", "[");
        pStr++;
        flag = 1;
    } else {
        *pStr = 0;
    }

    addr = (U32)varVal;
    while (pInfo->typeElement[i].elementMask != -1) {
//        if (i != 0) {//change by wuheng 2015.12.2改为0，解决首个是基础类型情况
            tmpType = pInfo->typeElement[i].elementMask & 0xFF;
            mask = DecodeTypeInfoGlobalTable[tmpType].mask;
            if ((maxByte == 0) && (minByte == 0)) {
                maxByte = (mask >> 4) & 0xFFF;
                minByte = mask & 0xF;
            } else {
                tmpMax = (mask >> 4) & 0xFFF;
                tmpMin = mask & 0xF;
                if (tmpMin > minByte) {
                    if ((maxByte % tmpMin) != 0) {
                        maxByte += (tmpMin - maxByte % tmpMin);
                        addrOft = maxByte;
                        maxByte += tmpMax;
                    }
                    minByte = tmpMin;
                } else {
                    if ((maxByte % tmpMin) != 0) {
                        maxByte += (tmpMin - maxByte % tmpMin);
                    }
                    addrOft = maxByte;
                    maxByte += tmpMax;
                }
            }
//        }

        if (tmpType == D_DATATYPE_BOOL) {
            if (*((char *)(addr + addrOft)) == 1) {
                sprintf(pStr, "%s", "TRUE");
            } else {
                sprintf(pStr, "%s", "FALSE");
            }
        } else if (tmpType == D_DATATYPE_NUM_I) {
            sprintf(pStr, "%d", *((int *)(addr + addrOft)));
        } else if (tmpType == D_DATATYPE_NUM_D) {
//            if ((*((double *)(addr + addrOft)) > -0.000001) && (*((double *)(addr + addrOft)) < 0.000001)) {
            temVal = *((double *)(addr + addrOft)) - (int)(*((double *)(addr + addrOft)));
            if ((temVal > -0.000001) && (temVal < 0.000001)) {
               sprintf(pStr, "%0.0f", *((double *)(addr + addrOft)));
            } else {
               sprintf(pStr, "%0.9f", *((double *)(addr + addrOft)));
            }
        } else if (tmpType == D_DATATYPE_NUM) {
            sprintf(pStr, "%d", *((int *)(addr + addrOft)));
        } else if (tmpType == D_DATATYPE_DNUM) {
            sprintf(pStr, "%d", *((int *)(addr + addrOft)));
        } else if (tmpType == D_DATATYPE_OPNUM) {
            sprintf(pStr, "%d", *((int *)(addr + addrOft)));
        } else if (tmpType == D_DATATYPE_STRING) {
            sprintf(pStr, "\"%s\"", ((char *)(addr += addrOft)));
        } else if (tmpType == D_DATATYPE_SWITCH) {
            sprintf(pStr, "%d", *((int *)(addr += addrOft)));
        } else if (tmpType == D_DATATYPE_IDENTNO) {
            sprintf(pStr, "%d", *((int *)(addr += addrOft)));
        } else {//此处递归调用
//            if (i != 0) {
    //            sprintf(pStr, "%s", "[");
                decodeTypestructTransValToStr(tmpType, pStr, (decode_type_data *)(addr + addrOft));
    //            strcat(pStr, "]");
//            }
        }
//        if (i != 0) {
            if (pInfo->typeElement[i+1].elementMask != -1) {
                strcat(pStr, ",");
            }
//        }
        pStr = (char *)(pStr + strlen(pStr));
        i++;
    }    



    *pStr = 0;
    if (flag == 1) {
        strcat(pStr, "]");
    }

    return 1;
}
#endif
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
void *getVarDataBufAddress(void)
{
    return DecodeTypeDataArryGlobal;
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
decode_typestruct_info *decodeGetVarTypeInfo(int varType)
{
    if (varType < D_DATATYPE_END_CNT) {
        return &DecodeTypeInfoGlobalTable[varType];
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


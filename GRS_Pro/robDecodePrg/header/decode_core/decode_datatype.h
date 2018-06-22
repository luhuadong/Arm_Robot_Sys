/***************************************************************************
* Module Operation Explanation
* Replace &name& with the name of the file
****************************************************************************/

/***************************************************************************
****************************************************************************
*Copyright(c),2008-2028,广州数控设备有限公司
*File name: decode_datatype.h
*First Author: 
*Date:06/17/2009  
*接口文件功能描述：
****************************************************************************
****************************************************************************/

#ifndef DECODE_DATATYPE_H
#define DECODE_DATATYPE_H

#ifdef DECODE_DATATYPE_GLOBALS
#define DECODE_DATATYPE_EXT
#else
#define DECODE_DATATYPE_EXT extern
#endif

/***************************************************************************/
/*Add all #includes here*/
//#include "basic_data_type.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/***************************************************************************/

#if defined(__cplusplus)
extern "C"
{
#endif

/***************************************************************************/
/*Add all #defines here*/
typedef char decode_bool; //只能为TRUE或者FALSE
typedef int decode_num;//此处把num和dnum都为一样，与ABB存在差异
typedef double decode_dnum;

typedef int decode_num_i;//自定义的num可以识别为num_i或num_d，根据小数点定义
typedef double decode_num_d;//浮点型

typedef struct {//__attribute((aligned(8)))
    decode_num_d x;
    decode_num_d y;
    decode_num_d z;
}decode_pos;

typedef struct {//__attribute((aligned(8)))
    decode_num_d q1;
    decode_num_d q2;
    decode_num_d q3;
    decode_num_d q4;
}decode_orient;

typedef struct {//__attribute((aligned(8)))
	decode_pos trans;
	decode_orient rot;
}decode_pose;

typedef struct {//__attribute((aligned(8))) //与骆不一致
    decode_num_d v_tcp;
    decode_num_d v_ori;
    decode_num_d v_leax;
    decode_num_d v_reax;
}decode_speeddata;

typedef struct {//__attribute((aligned(8))) //与骆不一致
	decode_bool finep;
    decode_num_d pZoneTcp;
    decode_num_d pZoneOri;
    decode_num_d pZoneEax;
    decode_num_d zoneOri;
    decode_num_d zoneLeax;
    decode_num_d zoneReax;
}decode_zonedata;

typedef struct {
    char data[80];//字符串类型，与ABB一致，只保存80个字符
}decode_string;

typedef int decode_switch;//用于开关选择，可以理解0或者1

typedef struct {//__attribute((aligned(8)))
    decode_num_i cf1;
    decode_num_i cf4;
    decode_num_i cf6;
    decode_num_i cfx;
}decode_confdata;

typedef struct {//__attribute((aligned(8)))
    decode_num_d eax_a;
    decode_num_d eax_b;
    decode_num_d eax_c;
    decode_num_d eax_d;
    decode_num_d eax_e;
    decode_num_d eax_f;
}decode_extjoint;

typedef struct {//__attribute((aligned(8)))
    decode_num_d rax_1;
    decode_num_d rax_2;
    decode_num_d rax_3;
    decode_num_d rax_4;
    decode_num_d rax_5;
    decode_num_d rax_6;
}decode_robjoint;

typedef struct {//__attribute((aligned(8)))//与骆不一致
    decode_pos trans;
    decode_orient rot;
    decode_confdata robconf;
    decode_extjoint extax;
}decode_robtarget;

typedef struct {//__attribute((aligned(8)))
    decode_robjoint robax;
    decode_extjoint extax;
}decode_jointtarget;

typedef decode_num_i decode_identno;

//typedef struct {
//    decode_num_i inpos;
//    decode_num_d stoptime;
//    decode_num_d followtime;
//}decode_stoppoint;

typedef struct {//__attribute((aligned(8)))
    decode_num_d position;
    decode_num_d speed;
    decode_num_d mintime;
    decode_num_d maxtime;
}decode_inposdata;

typedef decode_num_i decode_opnum;//相当于枚举类型

typedef struct {//__attribute((aligned(8)))
    decode_num_i type;
    decode_bool progsynch;
    decode_inposdata inpos;
    decode_num_d stoptime;
    decode_num_d followtime;
    decode_string signal;//保留，骆没定义
    decode_opnum relation;//保留
    decode_num_d checkvalue;//保留
}decode_stoppointdata;

typedef struct {//__attribute((aligned(8)))
    decode_num_d mass;
    decode_pos cog;
    decode_orient aom;
    decode_num_d ix;
    decode_num_d iy;
    decode_num_d iz;
}decode_loaddata;

typedef struct {//__attribute((aligned(8)))
    decode_bool robhold;
    decode_pose tframe;
    decode_loaddata tload;
}decode_tooldata;

typedef struct {//__attribute((aligned(8)))
    decode_bool robhold;
    decode_bool ufprog;
    decode_string ufmec;//此处与骆定义有区别
    decode_pose uframe;
    decode_pose oframe;
}decode_wobjdata;
//typedef int decode_wobjdata;

#define debug_printf printf

//#define D_DATATYPE_INSTRUCT			(0) //是指令类型
//#define D_DATATYPE_ROBTARGET 		(0x1)
//#define D_DATATYPE_SWITCH			(0x2)
//#define D_DATATYPE_IDENTNO			(0x3)
//#define D_DATATYPE_SPEEDDATA		(0x4)
//#define D_DATATYPE_NUM				(0x5)
//#define D_DATATYPE_ZONEDATA			(0x6)
//#define D_DATATYPE_STOPPOINTDATA	(0x7)
//#define D_DATATYPE_TOOLDATA			(0x8)
//#define D_DATATYPE_WOBJDATA			(0x9)
//#define D_DATATYPE_LOADDATA			(0xA)

typedef enum {//注意这个顺序要与decode_typestruct_info定义的全局表顺序一致，最好是只能往后加
    D_DATATYPE_INSTRUCT = 0,//0
    D_DATATYPE_VAR,//1变量
    D_DATATYPE_CONST,//2常量
    D_DATATYPE_PERS,//3静态变量
    D_DATATYPE_LOCAL,//4局部变量
    D_DATATYPE_TASK,//5任务
    D_DATATYPE_ROBTARGET,//6
    D_DATATYPE_SWITCH,//7
    D_DATATYPE_IDENTNO,//8
    D_DATATYPE_SPEEDDATA,//9
    D_DATATYPE_NUM,//A要转成D_DATATYPE_NUM_I或D_DATATYPE_NUM_D，通过判断是否有小数点决定
    D_DATATYPE_ZONEDATA,//B
    D_DATATYPE_STOPPOINTDATA,//C
    D_DATATYPE_TOOLDATA,//D
    D_DATATYPE_WOBJDATA,//E
    D_DATATYPE_LOADDATA,//F
    D_DATATYPE_BOOL,//10
    D_DATATYPE_DNUM,//11要转成D_DATATYPE_NUM_I或D_DATATYPE_NUM_D
    D_DATATYPE_POS,//12
    D_DATATYPE_ORIENT,//13
    D_DATATYPE_POSE,//14
    D_DATATYPE_STRING,//15
    D_DATATYPE_CONFDATA,//16
    D_DATATYPE_EXTJOINT,//17
    D_DATATYPE_NUM_I,//18自定义的整型
    D_DATATYPE_NUM_D,//19自定义的浮点型
    D_DATATYPE_INPOSDATA,//1A
    D_DATATYPE_OPNUM,//1B
    D_DATATYPE_ROBJOINT,//1C
    D_DATATYPE_JOINTTARGET,//1D
    D_DATATYPE_END_CNT//作为最后，表示目前支持的类型总数
}decode_typestruct;


/***************************************************************************/

/***************************************************************************/
/*以下开始函数声明*/
/***************************************************************************/ 
/*函数功能：若该函数有特定客户，请注明*/ 
/*输入参数： 参数1：注明输入参数意义及有效性范围*/			
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/*函数功能：若该函数有特定客户，请注明*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/*函数功能：若该函数有特定客户，请注明*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/*函数功能：若该函数有特定客户，请注明*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/*函数功能：若该函数有特定客户，请注明*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/*函数功能：若该函数有特定客户，请注明*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/*函数功能：若该函数有特定客户，请注明*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/*函数功能：若该函数有特定客户，请注明*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/*函数功能：若该函数有特定客户，请注明*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/*函数功能：若该函数有特定客户，请注明*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/*函数功能：若该函数有特定客户，请注明*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/*函数功能：若该函数有特定客户，请注明*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/

/***************************************************************************/
/*函数功能：若该函数有特定客户，请注明*/
/*输入参数： 参数1：注明输入参数意义及有效性范围*/
/*输出参数: 	  参数1：注明输出参数意义及有效性范围*/
/*返回值：	  返回值范围及意义*/
/***************************************************************************/

/***************************************************************************/


#if defined(__cplusplus)
}
#endif 

#endif

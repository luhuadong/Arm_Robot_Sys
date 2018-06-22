#ifndef GRS_SHAREDMEMORY_H
#define GRS_SHAREDMEMORY_H

#include "ITP_Interface.h"

#if defined(__cplusplus)
extern "C"
{
#endif


/*
 * 共享内存
 */
#ifdef SHAREDMEMORY_GLOBALS
#define SHAREDMEMORY_EXT
#else
#define SHAREDMEMORY_EXT extern
#endif

SHAREDMEMORY_EXT ITP_InputData *pInputData;		// 参数输入共享内存
SHAREDMEMORY_EXT ITP_OutputData *pOutputData;	// 数据输出共享内存（只读）
SHAREDMEMORY_EXT ITP_MotSetData *pMotSetData;   // 运动参数共享内存


/*
 * @brief   共享内存初始化函数和清除函数完成
 * @author  luhuadong
 * @date    20160119
 */
int G_SharedMemoryInit();
void G_SharedMemoryCleanup();


#if defined(__cplusplus)
}
#endif

#endif // GRS_SHAREDMEMORY_H

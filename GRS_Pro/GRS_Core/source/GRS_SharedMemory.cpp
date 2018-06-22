#define SHAREDMEMORY_GLOBALS
#include "GRS_SharedMemory.h"
#include <QtGlobal>

#ifdef Q_OS_LINUX
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#elif defined(Q_OS_WIN)
#include <Windows.h>
#define pwstr_size 32

//存放文件映射对象的句柄，因为释放时需要用到，所以做成全局变量。
static HANDLE hFileMapping_InputData;
static HANDLE hFileMapping_OutputData;
static HANDLE hFileMapping_MotSetData;

#endif

typedef enum{
    SHM_InputData,
    SHM_OutputData,
    SHM_MotSetData
}SharedMemoryTypes;


#ifdef Q_OS_LINUX
void getSharedMemory_linux(SharedMemoryTypes type, void **pSharedMemory)
{
    int shmid = 0;
    int shmflg = 0;

    // Open a shared memory, not create.
    switch (type) {
    case SHM_InputData:
        shmid = shmget(ITP_Key_InputShareMem, sizeof(ITP_InputData), 0666);
        break;
    case SHM_OutputData:
    {   // read only
        shmid = shmget(ITP_Key_OutputShareMem, sizeof(ITP_OutputData), 0444);
        shmflg = SHM_RDONLY;
    }
        break;
    case SHM_MotSetData:
        shmid = shmget(ITP_Key_ParaShareMem, sizeof(ITP_MotSetData), 0666);
        break;
    default:
        shmid = -1;
        break;
    }

    // Attache the shared memory segment to the address space of the calling process.
    if(shmid > 0) {*pSharedMemory = shmat(shmid, NULL, shmflg);}
    else {*pSharedMemory = NULL;}
}
#elif defined(Q_OS_WIN)
void getSharedMemory_win(SharedMemoryTypes type, void **pSharedMemory)
{
    char *cstr;
    DWORD dwDesiredAccess = FILE_MAP_READ | FILE_MAP_WRITE;
    HANDLE *handle;

    switch (type) {
    case SHM_InputData:
    {
        cstr = "ITP_ShareMem_InputData";
        handle = &hFileMapping_InputData;
    }
        break;
    case SHM_OutputData:
    {
        cstr = "ITP_ShareMem_OutputData";
        dwDesiredAccess = FILE_MAP_READ;
        handle = &hFileMapping_OutputData;
    }
        break;
    case SHM_MotSetData:
    {
        cstr = "ITP_ShareMem_MotSetData";
        handle = &hFileMapping_MotSetData;
    }
        break;
    default:
        *pSharedMemory = NULL;
        return;
    }

    /* ***** Type (char *) switch to type (wchar_t *) ***** */
    size_t cstr_len = strlen(cstr);
    size_t n = (size_t)MultiByteToWideChar(CP_ACP,0,(const char*)cstr,(int)cstr_len,NULL,0);
    wchar_t pwstr[pwstr_size];
    if(n>=pwstr_size) n = pwstr_size-1;

    MultiByteToWideChar(CP_ACP,0,(const char*)cstr,(int)cstr_len,pwstr,(int)n);
    pwstr[n]=0;
    /* ***** switch over ***** */

    //尝试打开指定命名的共享内存，获取相应对象的句柄
    *handle = OpenFileMapping(dwDesiredAccess, 0, pwstr);

    if(NULL != *handle)//判断句柄是否指向有效的命名对象，否则初始化失败
    {
        // 将文件映射内核对象handle映射到当前应用程序的进程地址
        *pSharedMemory = MapViewOfFile(*handle, dwDesiredAccess, 0, 0, 0);
    }
    else {*pSharedMemory = NULL;}
}
#endif

/*
 * @brief 共享内存初始化函数
 *
 * @param 无
 *
 * @return 成功返回0，失败返回1
 *
 * @description 平台无关的共享内存初始化函数，该版本只支持LINUX和WINDOWS两类操作系统，
 *  用于打开插补模块提供的共享内存，并连接到本地全局变量。
 */
int G_SharedMemoryInit()
{
#ifdef Q_OS_LINUX
    getSharedMemory_linux(SHM_InputData, (void **)&pInputData);
    getSharedMemory_linux(SHM_OutputData, (void **)&pOutputData);
    getSharedMemory_linux(SHM_MotSetData, (void **)&pMotSetData);

#elif defined(Q_OS_WIN)
    getSharedMemory_win(SHM_InputData, (void **)&pInputData);
    getSharedMemory_win(SHM_OutputData, (void **)&pOutputData);
    getSharedMemory_win(SHM_MotSetData, (void **)&pMotSetData);

#else
    // This version only support LINUX and WINDOWS operating system.
    qDebug("Sorry, the application don't suit for your OS.");
    return 1;
#endif

    // As long as one of above shared memory attached unsuccessfully, it would be return 1,
    // i.e, those shared memorys initialize failed. You have to recall this function if you don't want to exit.
    if(pInputData == NULL || pOutputData == NULL || pMotSetData == NULL)
    {
        G_SharedMemoryCleanup();
        qDebug("Shared Memory Initialization Failed!");
        return 1;
    }
    return 0;
}

void G_SharedMemoryCleanup()
{
#ifdef Q_QS_LINUX
    if(NULL != pInputData) {shmdt(pInputData);}
    if(NULL != pOutputData) {shmdt(pOutputData);}
    if(NULL != pMotSetData) {shmdt(pMotSetData);}

    return ;

#elif defined(Q_OS_WIN)
    if(NULL != pInputData) {UnmapViewOfFile(pInputData);}
    if(NULL != pOutputData) {UnmapViewOfFile(pOutputData);}
    if(NULL != pMotSetData) {UnmapViewOfFile(pMotSetData);}

    if(NULL != hFileMapping_InputData)
    {
        CloseHandle(hFileMapping_InputData);
        hFileMapping_InputData = NULL;
    }
    if(NULL != hFileMapping_OutputData)
    {
        CloseHandle(hFileMapping_OutputData);
        hFileMapping_OutputData = NULL;
    }
    if(NULL != hFileMapping_MotSetData)
    {
        CloseHandle(hFileMapping_MotSetData);
        hFileMapping_MotSetData = NULL;
    }
    return ;
#endif
}

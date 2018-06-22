#include "sharemempublic.h"

#ifdef Q_OS_LINUX
#elif defined (Q_OS_WIN32)
#include <windows.h>//本文件要补充完整，兼容windows和linux共享内存申请

#include <QList>

#define SHMEM_TYPE_CREATE   0//创建
#define SHMEM_TYPE_OPEN     1//打开

typedef struct {
    HANDLE shareMemHandle;//要兼容linux
    void *shareMemPtr;//共享内存数据指针
    int type;//创建的还是打开的
    int size;//共享内存大小字节
}sharemem_struct;

QList<sharemem_struct> ShareMemInfoGlobal;

int createUserSharemem(char *memName, int size)
{
    int errFlag = -1;
    sharemem_struct shmemStruct;

    char *cstr = memName;
    size_t cstr_len = strlen(cstr);
    size_t n = (size_t)MultiByteToWideChar(CP_ACP, 0, (const char *)cstr, (int)cstr_len, NULL, 0);
    wchar_t pwstr[100];
    MultiByteToWideChar(CP_ACP, 0, (const char *)cstr, (int)cstr_len, pwstr, (int)n);
    pwstr[n] = 0;
    shmemStruct.shareMemHandle = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE | SEC_COMMIT, 0,size, pwstr);
    shmemStruct.type = SHMEM_TYPE_CREATE;
    shmemStruct.size = size;
    if (shmemStruct.shareMemHandle != NULL) {
        ShareMemInfoGlobal.push_back(shmemStruct);
        errFlag = ShareMemInfoGlobal.size() - 1;//返回索引下标
    }

    return errFlag;
}

int openUserSharemem(char *memName, int size)
{
    int errFlag = -1;
    sharemem_struct shmemStruct;
    char *cstr = memName;
    size_t cstr_len = strlen(cstr);
    size_t n = (size_t)MultiByteToWideChar(CP_ACP, 0, (const char *)cstr, (int)cstr_len, NULL, 0);
    wchar_t pwstr[100];
    MultiByteToWideChar(CP_ACP, 0, (const char *)cstr, (int)cstr_len, pwstr, (int)n);
    pwstr[n] = 0;
    shmemStruct.shareMemHandle = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, pwstr);
    shmemStruct.type = SHMEM_TYPE_OPEN;
    shmemStruct.size = size;
    if (shmemStruct.shareMemHandle != NULL) {
        ShareMemInfoGlobal.push_back(shmemStruct);
        errFlag = ShareMemInfoGlobal.size() - 1;//返回下标索引
    }
    return errFlag;
}

int closeUserSharemem(int id)
{
    int errFlag = -1;
    if (id < ShareMemInfoGlobal.size()) {
        if (ShareMemInfoGlobal[id].shareMemPtr != NULL) {//将指针和句柄都关闭
            UnmapViewOfFile(ShareMemInfoGlobal[id].shareMemPtr);
        }
        if (ShareMemInfoGlobal[id].shareMemHandle != NULL) {
            CloseHandle(ShareMemInfoGlobal[id].shareMemHandle);
        }
        ShareMemInfoGlobal.removeAt(id);
        errFlag = 1;
    }
    return errFlag;
}

void* mmapUserSharemem(int id, int size, int initFlag)
{
    if (id < ShareMemInfoGlobal.size()) {
        ShareMemInfoGlobal[id].shareMemPtr = MapViewOfFile(ShareMemInfoGlobal[id].shareMemHandle, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0);
        if (initFlag == 1) {
            memset(ShareMemInfoGlobal[id].shareMemPtr, 0, size);
        }
        return ShareMemInfoGlobal[id].shareMemPtr;
    }
    return NULL;
}

#endif































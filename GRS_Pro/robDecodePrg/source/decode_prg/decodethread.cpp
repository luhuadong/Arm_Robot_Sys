#include "decodethread.h"
#include "decode_proc_task.h"
#include <QDebug>


DecodeThread::DecodeThread()
{

}

DecodeThread::~DecodeThread()
{

}

void DecodeThread::run()
{
//    int i = 0;
    decodeTaskInit();
    while (1) {
        decodeTaskRunningProc(0);
        msleep(1);//此处用来测试1ms一次
    }
}




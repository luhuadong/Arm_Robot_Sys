#include "mainwidget.h"
#include "hmiapp.h"

#include "GRS_Common.h"
#include "GRS_SharedMemory.h"
#include "decode_public.h"
#include "robotdecodepublic.h"
#include "decode_proc_task.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    HmiApp a(argc, argv);

    int ret1 = G_CommonInit();
    int ret2 = G_SharedMemoryInit();

    if(ret1 || ret2)
    {
        G_SharedMemoryCleanup();
        a.exit(1);
        return 1;
    }

    if(1 == decodeTaskInit())
    {
        readRobotPrgFile(0, "E:/workspace/Qt_Project/GSK_Robot_Sys/GRS_Pro/GR_HMI/system/example/example.pgf");
    }
    else qDebug("Decode module initialize failed.");

    QFile file(":/qss/GR-HMI.qss");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());
    file.close();

    MainWidget w;
    w.show();

    int retcode;
    retcode = a.exec();

    return retcode;
    //return a.exec();
}

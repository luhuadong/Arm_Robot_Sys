#include "hmiapp.h"
#include <QDebug>

//HmiApp::HmiApp(int &argc, char **argv)
//{

//}

bool HmiApp::event(QEvent *e)
{
    if(e->type() == myCustomEvent)
    {
        qDebug() << "I'm HmiApp, myCustomEvent is comming.";

        // 修改参数
        // 通知相关界面参数被修改了

        return true;
    }

    return QApplication::event(e);
}

/*
 * 子类化QApplication，为了获取PanelWidget产生的按钮事件。
*/
#ifndef HMIAPP_H
#define HMIAPP_H

#include <QApplication>
#include "GRS_Common.h"

class HmiApp : public QApplication
{
public:
    HmiApp(int & argc, char ** argv):QApplication(argc, argv){}

protected:
    bool event(QEvent *);
};

#endif // HMIAPP_H

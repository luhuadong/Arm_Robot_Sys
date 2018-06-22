#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>

#include "GRS_Common.h"
#include "panelwidget.h"
#include "screenwidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void switchBtnClicked();

private:
    QHBoxLayout *mainLayout;

    PanelWidget *panelWidget;       // 模拟示教器操作面板
    ScreenWidget *screenWidget;     // 模拟示教器屏幕

};

#endif // MAINWIDGET_H

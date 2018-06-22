#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QList>
#include <QTableWidgetItem>
#include <QStackedLayout>

#include "GRS_Common.h"
#include "GRS_SharedMemory.h"

#include "mainmenuwidget.h"
#include "quicksetbar.h"
#include "CustomWidget/simplepage.h"
#include "Page_Jogging/joggingpage.h"
#include "Page_ProgramData/programdatapage.h"


class ScreenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenWidget(QWidget *parent = 0);

    QWidget *topBar;
    QPushButton *menuBtn;
    MainMenuWidget *menuWidget;
    QPushButton *operatorBtn;

    QPushButton *eventLogBtn;
    QLabel *operatingModeLabel;     // 机器人操作模式
    QLabel *controllerOSInfoLabel;  // 机器人本体型号 + 控制器系统版本信息
    QLabel *controllerStatusLabel;  // 机器人和控制器状态
    QLabel *productionStatusLabel;  // 程序执行状态 + 当前速度倍率
    QLabel *taskInfoLabel;          // 系统当前任务提示信息

    QPushButton *closeBtn;

    QStackedWidget *centralWidget;
    // 只能打开一个的界面
    QWidget *homePage;
    JoggingPage *joggingPage;
    ProgramDataPage *programDataPage;

    QWidget *bottomBar;
    QButtonGroup *bottomBarBtnGroup;
    QPushButton *quickSetBtn;
    QuickSetBar *quickSetBar;


signals:

public slots:
    void eventLogBtnUpdate();

private slots:
    void quickSetBtnClicked(bool);
    void menuBtnClicked(bool);
    void selectMenuItem(QTableWidgetItem *);
    void closeBtnClicked();

private:
    QFrame *mainFrame;

};

#endif // SCREENWIDGET_H

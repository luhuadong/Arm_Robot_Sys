#ifndef PANELWIDGET_H
#define PANELWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QTimer>

#include "GRS_Common.h"
#include "GRS_SharedMemory.h"
#include "systembutton.h"
#include "panelbutton.h"
#include "virtualbutton.h"


class PanelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PanelWidget(QWidget *parent = 0);

    int switchFlag;

protected:
    bool event(QEvent *);

signals:
    void showMin();
    void closeApp();
    void switchBtnClicked();
    void statusChanged();   // 相关状态改变，需要修改主界面的状态显示（事件按钮）

public slots:

private slots:
    void modeBtnResponded();
    void operatingModeChanged(const int);
    void keyLongPressListenerRespond(); // 按钮长按监听
    void incrementTimerRespond();       // 增量模式下连续运动处理函数
    void robotJogging(int id);    // 机器人动
    void robotStanding(int id);   // 机器人不动

private:
    QFrame *mainFrame;
    SystemButton *minBtn;   // minimize button of qApp
    SystemButton *closeBtn; // close button of qApp
    QPushButton *switchBtn; // switch button of qApp

    QPushButton *modeBtn;
    QPushButton *emergencyStopBtn;  // emergency stop button of GRS

    void createPanelBtns();

    PanelButton *btnA;
    PanelButton *btnB;
    PanelButton *btnC;
    PanelButton *btnD;

    PanelButton *btnE;
    PanelButton *btnF;
    PanelButton *btnG;
    PanelButton *btnH;

    PanelButton *btnI;
    PanelButton *btnJ;
    PanelButton *btnK;
    PanelButton *btnL;

    typedef enum moveBtnFlag{
        Btn_Pos_J1, Btn_Neg_J1,
        Btn_Pos_J2, Btn_Neg_J2,
        Btn_Pos_J3, Btn_Neg_J3,
        Btn_Pos_J4, Btn_Neg_J4,
        Btn_Pos_J5, Btn_Neg_J5,
        Btn_Pos_J6, Btn_Neg_J6
    }moveBtnTpyes;

    PanelButton *moveBtnPosJ1;
    PanelButton *moveBtnNegJ1;
    PanelButton *moveBtnPosJ2;
    PanelButton *moveBtnNegJ2;
    PanelButton *moveBtnPosJ3;
    PanelButton *moveBtnNegJ3;
    PanelButton *moveBtnPosJ4;
    PanelButton *moveBtnNegJ4;
    PanelButton *moveBtnPosJ5;
    PanelButton *moveBtnNegJ5;
    PanelButton *moveBtnPosJ6;
    PanelButton *moveBtnNegJ6;
    QButtonGroup *moveBtnGroup;
    QTimer *keyLongPressListener;   // 用于监听增量模式下相关动作按钮的长按状态
    QTimer *incrementTimer; // 增量模式持续运动时的定时器，如果控制杆偏转持续一秒钟或数秒钟，机器人就会持续移动（速率为每秒10步）。

    VirtualButton *enableBtn;
    VirtualButton *holdBtn;

    void createVirtualWidget();
    QWidget *virtualWidget;
    QPushButton *motorSwitchBtn;
    QButtonGroup *operatingModeBtnGroup;

};

#endif // PANELWIDGET_H

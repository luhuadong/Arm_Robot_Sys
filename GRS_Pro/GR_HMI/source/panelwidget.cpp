#include "panelwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDebug>

PanelWidget::PanelWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(UI_PANEL_WIDTH, UI_PANEL_HEIGHT);
    mainFrame = new QFrame;

    minBtn = new SystemButton();
    minBtn->setPicName(QString(":/images/systemButton/min"));
    minBtn->setCursor(Qt::PointingHandCursor);
    closeBtn = new SystemButton();
    closeBtn->setPicName(QString(":/images/systemButton/close"));
    closeBtn->setCursor(Qt::PointingHandCursor);

    switchBtn = new QPushButton(tr("Switch"));
    switchBtn->setCursor(Qt::PointingHandCursor);
    switchFlag = 0;

    QHBoxLayout *sysBtnBarLayout = new QHBoxLayout;
    sysBtnBarLayout->addWidget(switchBtn);
    sysBtnBarLayout->addWidget(minBtn);
    sysBtnBarLayout->addWidget(closeBtn);

    QLabel *hostNameLabel = new QLabel(G_SysName);
    QLabel *controllerLabel = new QLabel(G_ControllerName);
    hostNameLabel->setStyleSheet("color: white");
    controllerLabel->setStyleSheet("color: white");

    modeBtn = new QPushButton(tr("mode"), this);
    modeBtn->setFixedSize(40, 40);
    modeBtn->setCheckable(true);
    emergencyStopBtn = new QPushButton(tr("Emergency"), this);
    emergencyStopBtn->setFixedSize(100, 40);
    emergencyStopBtn->setObjectName("emergencyStopBtn");
    emergencyStopBtn->setCheckable(true);
    enableBtn = new VirtualButton(tr("Enable"));
    holdBtn = new VirtualButton(tr("Hold to Run"));

    createPanelBtns();

    QHBoxLayout *emergencyLayout = new QHBoxLayout;
    emergencyLayout->setContentsMargins(0, 10, 0, 0);
    emergencyLayout->setSpacing(40);
    emergencyLayout->addWidget(modeBtn);
    emergencyLayout->addWidget(emergencyStopBtn);
    QHBoxLayout *customBtnsLayout = new QHBoxLayout;
    customBtnsLayout->setContentsMargins(0, 20, 0, 0);
    customBtnsLayout->addWidget(btnA);
    customBtnsLayout->addWidget(btnB);
    customBtnsLayout->addWidget(btnC);
    customBtnsLayout->addWidget(btnD);
    QGridLayout *moveBtnsLayout = new QGridLayout;
    moveBtnsLayout->setContentsMargins(0, 30, 0, 30);
    moveBtnsLayout->addWidget(enableBtn, 0, 0, 1, 2, Qt::AlignCenter);
    moveBtnsLayout->addWidget(moveBtnPosJ1, 0, 2, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(moveBtnNegJ1, 0, 3, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(moveBtnPosJ2, 1, 2, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(moveBtnNegJ2, 1, 3, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(moveBtnPosJ3, 2, 2, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(moveBtnNegJ3, 2, 3, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(moveBtnPosJ4, 3, 2, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(moveBtnNegJ4, 3, 3, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(moveBtnPosJ5, 4, 2, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(moveBtnNegJ5, 4, 3, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(moveBtnPosJ6, 5, 2, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(moveBtnNegJ6, 5, 3, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(btnE, 1, 0, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(btnF, 2, 0, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(btnG, 3, 0, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(btnH, 4, 0, 1, 1, Qt::AlignCenter);
    moveBtnsLayout->addWidget(holdBtn, 5, 0, 1, 2, Qt::AlignCenter);
    QHBoxLayout *operateBtnsLayout = new QHBoxLayout;
    operateBtnsLayout->addWidget(btnI);
    operateBtnsLayout->addWidget(btnJ);
    operateBtnsLayout->addWidget(btnK);
    operateBtnsLayout->addWidget(btnL);


    QVBoxLayout *mainFrameLayout = new QVBoxLayout(mainFrame);
    mainFrameLayout->addLayout(sysBtnBarLayout);
    mainFrameLayout->addLayout(emergencyLayout);
    mainFrameLayout->addLayout(customBtnsLayout);
    mainFrameLayout->addLayout(moveBtnsLayout);
    mainFrameLayout->addLayout(operateBtnsLayout);
    mainFrameLayout->addStretch();
    mainFrameLayout->addWidget(hostNameLabel);
    mainFrameLayout->addWidget(controllerLabel);

    connect(minBtn, SIGNAL(clicked()), this, SIGNAL(showMin()));
    connect(closeBtn, SIGNAL(clicked()), this, SIGNAL(closeApp()));
    connect(switchBtn, SIGNAL(clicked()), this, SIGNAL(switchBtnClicked()));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget(mainFrame);

    createVirtualWidget();
    connect(modeBtn, SIGNAL(clicked()), this, SLOT(modeBtnResponded()));

    keyLongPressListener = new QTimer;
    keyLongPressListener->setSingleShot(true);  // 只做一次定时操作
    connect(keyLongPressListener, SIGNAL(timeout()), this, SLOT(keyLongPressListenerRespond()));

    incrementTimer = new QTimer;
    connect(incrementTimer, SIGNAL(timeout()), this, SLOT(incrementTimerRespond()));
}


bool PanelWidget::event(QEvent *e)
{
    if(e->type() == myCustomEvent)
    {
        qDebug() << "myCustomEvent is coming.";
        return true;
    }
    return QWidget::event(e);
}

void PanelWidget::createPanelBtns()
{
    btnA = new PanelButton(tr("A"), this);
    btnB = new PanelButton(tr("B"), this);
    btnC = new PanelButton(tr("C"), this);
    btnD = new PanelButton(tr("D"), this);

    btnE = new PanelButton(tr("E"), this);
    btnF = new PanelButton(tr("F"), this);
    btnG = new PanelButton(tr("G"), this);
    btnH = new PanelButton(tr("H"), this);

    btnI = new PanelButton(tr("I"), this);
    btnJ = new PanelButton(tr("J"), this);
    btnK = new PanelButton(tr("K"), this);
    btnL = new PanelButton(tr("L"), this);

    moveBtnPosJ1 = new PanelButton(tr("J1/X+"), this);
    moveBtnNegJ1 = new PanelButton(tr("J1/X-"), this);
    moveBtnPosJ2 = new PanelButton(tr("J2/Y+"), this);
    moveBtnNegJ2 = new PanelButton(tr("J2/Y-"), this);
    moveBtnPosJ3 = new PanelButton(tr("J3/Z+"), this);
    moveBtnNegJ3 = new PanelButton(tr("J3/Z-"), this);
    moveBtnPosJ4 = new PanelButton(tr("J4/A+"), this);
    moveBtnNegJ4 = new PanelButton(tr("J4/A-"), this);
    moveBtnPosJ5 = new PanelButton(tr("J5/B+"), this);
    moveBtnNegJ5 = new PanelButton(tr("J5/B-"), this);
    moveBtnPosJ6 = new PanelButton(tr("J6/C+"), this);
    moveBtnNegJ6 = new PanelButton(tr("J6/C-"), this);

    moveBtnGroup = new QButtonGroup;
    moveBtnGroup->addButton(moveBtnPosJ1, Btn_Pos_J1);
    moveBtnGroup->addButton(moveBtnNegJ1, Btn_Neg_J1);
    moveBtnGroup->addButton(moveBtnPosJ2, Btn_Pos_J2);
    moveBtnGroup->addButton(moveBtnNegJ2, Btn_Neg_J2);
    moveBtnGroup->addButton(moveBtnPosJ3, Btn_Pos_J3);
    moveBtnGroup->addButton(moveBtnNegJ3, Btn_Neg_J3);
    moveBtnGroup->addButton(moveBtnPosJ4, Btn_Pos_J4);
    moveBtnGroup->addButton(moveBtnNegJ4, Btn_Neg_J4);
    moveBtnGroup->addButton(moveBtnPosJ5, Btn_Pos_J5);
    moveBtnGroup->addButton(moveBtnNegJ5, Btn_Neg_J5);
    moveBtnGroup->addButton(moveBtnPosJ6, Btn_Pos_J6);
    moveBtnGroup->addButton(moveBtnNegJ6, Btn_Neg_J6);

    connect(moveBtnGroup, SIGNAL(buttonPressed(int)), this, SLOT(robotJogging(int)));
    connect(moveBtnGroup, SIGNAL(buttonReleased(int)), this, SLOT(robotStanding(int)));
}

void PanelWidget::createVirtualWidget()
{
    virtualWidget = new QWidget(this);
    virtualWidget->setObjectName("virtualWidget");  // 20160112
    virtualWidget->setFixedSize(UI_PANEL_WIDTH / 2, UI_PANEL_HEIGHT / 3);

    motorSwitchBtn = new QPushButton(tr("motor"), this);
    motorSwitchBtn->setFixedSize(80, 80);
    motorSwitchBtn->setObjectName("motorSwitchBtn");

    operatingModeBtnGroup = new QButtonGroup(this);

    QRadioButton *autoRadioBtn = new QRadioButton(tr("Auto"), this);
    QRadioButton *manualRadioBtn = new QRadioButton(tr("Manual"), this);
    QRadioButton *manualFullSpeedRadioBtn = new QRadioButton(tr("Manual Full Speed"), this);

    operatingModeBtnGroup->addButton(autoRadioBtn, OM_Auto);
    operatingModeBtnGroup->addButton(manualRadioBtn, OM_Manual);
    operatingModeBtnGroup->addButton(manualFullSpeedRadioBtn, OM_ManualFullSpeed);

    operatingModeBtnGroup->button(OM_Auto)->setChecked(true);

    // Layout of virtualWidget
    QVBoxLayout *virtualWidgetLayout = new QVBoxLayout;
    virtualWidgetLayout->addWidget(motorSwitchBtn);
    virtualWidgetLayout->addWidget(autoRadioBtn);
    virtualWidgetLayout->addWidget(manualRadioBtn);
    virtualWidgetLayout->addWidget(manualFullSpeedRadioBtn);
    virtualWidget->setLayout(virtualWidgetLayout);

    virtualWidget->setGeometry((UI_PANEL_WIDTH - virtualWidget->width()) / 2,
                               (UI_PANEL_HEIGHT - virtualWidget->height()) / 2,
                               virtualWidget->width(), virtualWidget->height());
    virtualWidget->hide();
    connect(operatingModeBtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(operatingModeChanged(int)));
}

void PanelWidget::modeBtnResponded()
{
    if(modeBtn->isChecked()) {virtualWidget->show();}
    else virtualWidget->hide();
}

void PanelWidget::operatingModeChanged(const int id)
{
    G_OperatingMode = (operatingModeTypes)id;
    emit statusChanged();
}

void PanelWidget::keyLongPressListenerRespond()
{
    //keyLongPressListener->stop();   // 本来就是 SingleShot 模式，不用 stop 也行;
    incrementTimer->start(100);     // 启动增量模式下的持续运动
}

void PanelWidget::incrementTimerRespond()
{
    // 机器人持续移动（速率为每秒10步），这里规定当机器人正在运动的时候修改速度倍率和增量大小无效
    pInputData->flagIncUpdate = !(pInputData->flagIncUpdate);   // 更新增量标识
}

// 机器人动
void PanelWidget::robotJogging(int id)
{
    // 当操作模式为手动或手动全速，且使能键按下，急停按钮没有按下的情况下才能运动
    if(emergencyStopBtn->isChecked() || !enableBtn->isChecked() || operatingModeBtnGroup->checkedId() == OM_Auto)
    {
        qDebug("Robot can not moving, please check robot's status.");
        return ;
    }
    /* 2015.11.11
     * 增量运动模式下，若用户操作按钮太频繁机器人系统上一步增量运动还未到位，则忽略此次请求；
     * 另一种做法是我们要记录下用户请求的增量运动次数，然后在适当的时候进行移动。
     * 这里采用第一种处理方式，即直接忽略。
    */
    if(pOutputData->moving == 1) return;

    /*
     * 清除上一次操作留下的数据
    */
    pInputData->jogMoveVal = 0.0;
    for(int i=0; i<ITP_RobotJointNum; i++) {pInputData->robotMove[i] = 0;}
    pInputData->moveSpeed = 0;

    if(SW_Increment == SW_OFF)
    {
        // 增量开关关闭时 2015.11.09
        //pInputData->jogMoveVal = 0.0; // 插补模块通过判断 jogMoveVal 的值来确定当前是否处于增量模式 ？
        if(joggingPar.motionMode == Jogging::PAR_AXIS1_6)
        {
            // 每个关节不一样（单位：弧度/插补周期）
            joggingPar.curSpeed = (MaxSpeed_Joint[id/2] * interpolatePeriod / 1000) * JoggingSpeedRate * SysSpeedRate;
            // 设置速度和关节运动轴（通过 robotMove[x] ）
            pInputData->robotMove[id/2] = (id%2 == 0)?(joggingPar.curSpeed):-(joggingPar.curSpeed);
        }
        else if(joggingPar.motionMode == Jogging::PAR_LINEAR)
        {
            if(id < 6)
                // 笛卡尔坐标系下的轴运动速度 = 最大速度 × 速度倍率。注意单位转换（mm/min --> mm/插补周期）
                joggingPar.curSpeed = (MaxSpeed_Linear * interpolatePeriod / 1000) * JoggingSpeedRate * SysSpeedRate;
            else
                joggingPar.curSpeed = (MaxSpeed_Reorient * interpolatePeriod / 1000) * JoggingSpeedRate * SysSpeedRate;

            // 设置速度和线性运动轴（通过 moveSpeed 和 flagMoveItem ）
            pInputData->moveSpeed = (id%2 == 0)?(joggingPar.curSpeed):-(joggingPar.curSpeed);
            pInputData->flagMoveItem = id/2;
        }
    }
    else if(SW_Increment == SW_ON)
    {
        keyLongPressListener->start(900);   // 如果按钮长按一秒（900+100）以上，就进入增量模式下的持续运动状态
        //pInputData->jogMoveVal = 1.0;   // 插补模块通过判断 jogMoveVal 的值来确定当前是否处于增量模式，那么我需要先对其赋一个非零的值吗？
        // 增量开关开启时 2015.11.09
        if(joggingPar.motionMode == Jogging::PAR_AXIS1_6)
        {
            // 每个关节不一样（单位：弧度/插补周期）
            joggingPar.curSpeed = (MaxSpeed_Joint[id/2] * interpolatePeriod / 1000) * JoggingSpeedRate * SysSpeedRate;
            // 设置速度和关节运动轴（通过 robotMove[x] ）
            pInputData->robotMove[id/2] = joggingPar.curSpeed;
            // 设置增量大小和正反方向（只与 jogMoveVal 有关）
            pInputData->jogMoveVal = (id%2 == 0)?(incVal[joggingPar.increment].axis):-(incVal[joggingPar.increment].axis);
            //pInputData->jogMoveVal = 0;
        }
        else if(joggingPar.motionMode == Jogging::PAR_LINEAR)
        {
            if(id < 6)
            {
                // 笛卡尔坐标系下的轴运动速度 = 最大速度 × 速度倍率。注意单位转换（mm/min --> mm/插补周期）
                joggingPar.curSpeed = (MaxSpeed_Linear * interpolatePeriod / 1000) * JoggingSpeedRate * SysSpeedRate;
                // 设置速度和线性运动轴（通过 moveSpeed 和 flagMoveItem ）
                pInputData->moveSpeed = joggingPar.curSpeed;
                pInputData->flagMoveItem = id/2;
                // 设置增量大小和正反方向（只与 jogMoveVal 有关）
                pInputData->jogMoveVal = (id%2 == 0)?(incVal[joggingPar.increment].linear):-(incVal[joggingPar.increment].linear);
            }
            else
            {
                joggingPar.curSpeed = (MaxSpeed_Reorient * interpolatePeriod / 1000) * JoggingSpeedRate * SysSpeedRate;
                // 设置速度和重定向运动轴（通过 moveSpeed 和 flagMoveItem ）
                pInputData->moveSpeed = joggingPar.curSpeed;
                pInputData->flagMoveItem = id/2;
                // 设置增量大小和正反方向（只与 jogMoveVal 有关）
                pInputData->jogMoveVal = (id%2 == 0)?(incVal[joggingPar.increment].reorient):-(incVal[joggingPar.increment].reorient);
            }
        }
        pInputData->flagIncUpdate = !(pInputData->flagIncUpdate);   // 最后，记得要更新增量标识！
    }

    /* ********************************** Debug **********************************
     * 插补周期为 1ms，Windows系统达不到要求，所以动起来会慢14倍左右
     * **********************************       ********************************** */
    if(joggingPar.motionMode == Jogging::PAR_AXIS1_6)
    {
        qDebug("<Button_%d pressed> speed: %.4lf(rad/period) %.4lf(°/period) %.2lf(rad/s) %.2lf(°/s)", id,
               joggingPar.curSpeed, ITP_radian2angle(joggingPar.curSpeed),
               joggingPar.curSpeed * 1000, ITP_radian2angle(joggingPar.curSpeed) * 1000);
    }
    else if(joggingPar.motionMode == Jogging::PAR_LINEAR)
    {
        qDebug("<Button_%d pressed> speed: %.4lf(mm/period) %.2lf(mm/min) %.2lf(mm/s)", id, joggingPar.curSpeed,
               ITP_PeriodToSpeedUnit(joggingPar.curSpeed), ITP_PeriodToSpeedUnit(joggingPar.curSpeed)/60);
    }
    else if(joggingPar.motionMode == Jogging::PAR_REORIENT)
    {
        qDebug("<Button_%d pressed> speed: %.4lf", id, joggingPar.curSpeed);
    }
    /* ********************************** Debug ********************************** */
}


// 机器人不动
void PanelWidget::robotStanding(int id)
{
    if(SW_Increment == SW_OFF)
    {
        pInputData->jogMoveVal = 0.0;
        pInputData->robotMove[id/2] = 0;
        pInputData->moveSpeed = 0;    // 设置速度
    }
    else if(SW_Increment == SW_ON)
    {
        keyLongPressListener->stop();
        // 如果松开按钮，则停止计时
        if(incrementTimer->isActive()) {incrementTimer->stop();}

        //pInputData->jogMoveVal = 0.0;
        //pInputData->robotMove[id/2] = 0;

    }

    qDebug("<Button_%d released> X:%.4lf, Y:%.4lf, Z:%.4lf", id, pOutputData->curRob.pose.trans.x,
           pOutputData->curRob.pose.trans.y, pOutputData->curRob.pose.trans.z);
}

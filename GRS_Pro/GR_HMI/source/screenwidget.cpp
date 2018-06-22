#include "screenwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QSize>
#include <QPixmap>
//#include <QtWidgets>
#include <QPalette>
#include <QHostInfo>
#include <QDebug>
#include <QIcon>

ScreenWidget::ScreenWidget(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(UI_SCREEN_WIDTH, UI_SCREEN_HEIGHT);
    setObjectName("screenWidget");  // **

    topBar = new QWidget(this);
    topBar->setObjectName("topBar");
    topBar->setFixedHeight(54);
//    topBar->setStyleSheet("background-color: LightSkyBlue"); // **
//    topBar->setProperty("mandatoryField", "true");    // **
//    topBar->setProperty("class", "mandatory QWidget");    // **
    menuBtn = new QPushButton(topBar);
    menuBtn->setObjectName("mainScreenMenuBtn");
    menuBtn->setFixedSize(108, 54);
    menuBtn->setGeometry(0, 0, menuBtn->width(), menuBtn->height());
    menuBtn->setCheckable(true);
    connect(menuBtn, SIGNAL(toggled(bool)), this, SLOT(menuBtnClicked(bool)));

    operatorBtn = new QPushButton(topBar);
    operatorBtn->setObjectName("mainScreenAdminBtn");
    operatorBtn->setFixedSize(54, 54);
    operatorBtn->setGeometry(108, 0, operatorBtn->width(), operatorBtn->height());
    operatorBtn->setCheckable(true);

    eventLogBtn = new QPushButton(topBar);
    eventLogBtn->setObjectName("eventLogBtn");
    eventLogBtn->setFixedSize(584, 54);
    eventLogBtn->setGeometry(162, 0, eventLogBtn->width(), eventLogBtn->height());
    eventLogBtn->setCheckable(true);

    operatingModeLabel = new QLabel;     // 机器人操作模式
    controllerOSInfoLabel = new QLabel;  // 机器人本体型号 + 控制器系统版本信息
    controllerStatusLabel = new QLabel;  // 机器人和控制器状态
    productionStatusLabel = new QLabel;  // 程序执行状态 + 当前速度倍率
    taskInfoLabel = new QLabel;

    eventLogBtnUpdate();

    QPixmap robotPixmap(":/images/robotIcon_16.png");

    taskInfoLabel->setPixmap(robotPixmap);

    QGridLayout *eventLogLayout = new QGridLayout;
    eventLogLayout->setContentsMargins(20, 5, 20, 7);
    eventLogLayout->setSpacing(0);
    eventLogLayout->addWidget(operatingModeLabel, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    eventLogLayout->addWidget(controllerOSInfoLabel, 1, 0, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    eventLogLayout->addWidget(controllerStatusLabel, 0, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    eventLogLayout->addWidget(productionStatusLabel, 1, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    eventLogLayout->addWidget(taskInfoLabel, 0, 2, 1, 1, Qt::AlignRight | Qt::AlignVCenter);
    eventLogBtn->setLayout(eventLogLayout);


    closeBtn = new QPushButton(topBar);
    closeBtn->setObjectName("mainScreenCloseBtn");
    closeBtn->setFixedSize(54, 54);
    closeBtn->setGeometry(746, 0, closeBtn->width(), closeBtn->height());
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(closeBtnClicked()));


    centralWidget = new QStackedWidget(this);
    centralWidget->setObjectName("centralWidget");
    centralWidget->setFixedHeight(UI_SCREEN_HEIGHT - 54 - 54);
//    centralWidget->setStyleSheet("background-color: LightSlateGray; border-style: inset;");   // **


    // 各级界面 20160126
    homePage = new QWidget; // home page
    QPixmap pixmap(":/images/GRS_home.jpg");
    QLabel *homePicLabel = new QLabel(homePage);
    homePicLabel->setPixmap(pixmap);
    QVBoxLayout *homePageLayout = new QVBoxLayout;
    homePageLayout->setMargin(0);
    homePageLayout->addWidget(homePicLabel, Qt::AlignCenter);
    homePage->setLayout(homePageLayout);

    joggingPage = new JoggingPage;  // jogging page
    programDataPage = new ProgramDataPage; // program data page


    centralWidget->addWidget(homePage);
    centralWidget->addWidget(joggingPage);
    centralWidget->addWidget(programDataPage);
    centralWidget->setCurrentWidget(homePage);

    // 各级界面 20160126


    bottomBar = new QWidget(this);
    bottomBar->setObjectName("bottomBar");
    bottomBar->setFixedHeight(54);
//    bottomBar->setStyleSheet("background-color: LightSteelBlue"); // **
    bottomBarBtnGroup = new QButtonGroup;
    for(int i=0; i<TaskBarMaxNum; i++)
    {
        QPushButton *taskBtn = new QPushButton(QString::number(i+1), bottomBar);
        taskBtn->setObjectName("mainScreenTaskBtn");
        taskBtn->setFixedSize(108, 54);
        taskBtn->setGeometry(i * taskBtn->width(), 0, taskBtn->width(), taskBtn->height());
        taskBtn->setCheckable(true);
        bottomBarBtnGroup->addButton(taskBtn);
    }

    quickSetBtn = new QPushButton(tr("QuickSet"), bottomBar);
    quickSetBtn->setObjectName("mainScreenQuickSetBtn");
    quickSetBtn->setFixedSize(108 + 40, 54);
    quickSetBtn->setGeometry(UI_SCREEN_WIDTH - quickSetBtn->width(), 0, quickSetBtn->width(), quickSetBtn->height());
    quickSetBtn->setCheckable(true);
    connect(quickSetBtn, SIGNAL(toggled(bool)), this, SLOT(quickSetBtnClicked(bool)));

    quickSetBar = new QuickSetBar(this);
    quickSetBar->setGeometry(UI_SCREEN_WIDTH - quickSetBar->width(), topBar->height(), quickSetBar->width(), quickSetBar->height());
//    quickSetBar->raise();
    quickSetBar->hide();

    menuWidget = new MainMenuWidget(this);
    menuWidget->setGeometry(0, topBar->height() - 2, menuWidget->width(), menuWidget->height());
    menuWidget->hide();
    connect(menuWidget->menuTable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(selectMenuItem(QTableWidgetItem*)));


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(topBar);
    mainLayout->addWidget(centralWidget);
    mainLayout->addWidget(bottomBar);
    this->setLayout(mainLayout);
}

void ScreenWidget::quickSetBtnClicked(bool)
{
    if(quickSetBtn->isChecked())
    {
        if(menuBtn->isChecked()) {menuBtn->setChecked(false);}
        quickSetBar->show();
    }
    else {quickSetBar->hide();}
}

void ScreenWidget::menuBtnClicked(bool)
{
    if(menuBtn->isChecked())
    {
        if(quickSetBtn->isChecked()) {quickSetBtn->setChecked(false);}
        menuWidget->menuTableUpdate();
        menuWidget->show();
    }
    else {menuWidget->hide();}
}

void ScreenWidget::selectMenuItem(QTableWidgetItem *item)
{
    if(Menu_none == item->data(Qt::UserRole)) {return ;}
    // 加一个update来取消选中 20160120
    menuWidget->menuTableUpdate();
    menuBtn->setChecked(false); // 如果点击成功，则将自己关闭

    qDebug() << item->text();

    // QVariant 类型的数据使用 qvariant_cast<> 进行转换
    switch (qvariant_cast<int>(item->data(Qt::UserRole))) {
    case Menu_HotEdit:
    {
    }
        break;
    case Menu_IO:
    {
    }
        break;
    case Menu_Jogging:
    {
        centralWidget->setCurrentWidget(joggingPage);
    }
        break;
    case Menu_AutoProduction:
    {
    }
        break;
    case Menu_ProgramEditor:
    {
    }
        break;
    case Menu_ProgramData:
    {
        centralWidget->setCurrentWidget(programDataPage);
    }
        break;
    case Menu_Arc:
    {
    }
        break;
    case Menu_Backup:
    {
    }
        break;
    case Menu_Calibration:
    {
    }
        break;
    case Menu_ControlPanel:
    {
    }
        break;
    case Menu_EventLog:
    {
    }
        break;
    case Menu_Explorer:
    {
    }
        break;
    case Menu_SystemInfo:
    {
    }
        break;
    case Menu_LogOff:
    {
    }
        break;
    case Menu_Restart:
    {
    }
        break;
    default:
        break;
    }
}

void ScreenWidget::closeBtnClicked()
{
    if(menuBtn->isChecked()) {menuBtn->setChecked(false);}
    if(quickSetBtn->isChecked()) {quickSetBtn->setChecked(false);}
    centralWidget->setCurrentWidget(homePage);
}

void ScreenWidget::eventLogBtnUpdate()
{
    if(OM_Auto == G_OperatingMode) {operatingModeLabel->setText(tr("Auto"));}
    else if(OM_Manual == G_OperatingMode) {operatingModeLabel->setText(tr("Manual"));}
    else if(OM_ManualFullSpeed == G_OperatingMode) {operatingModeLabel->setText(tr("Manual Full Speed"));}
    else {operatingModeLabel->setText(tr("error"));}

    // 出错
//    G_SysName = QHostInfo::localHostName();
//    controllerOSInfoLabel->setText(G_ControllerName + "(" + G_SysName + ")");

    if(CS_MotorOff == G_ControllerStatus) {controllerStatusLabel->setText(tr("Motors Off"));}
    else if(CS_MotorOn == G_ControllerStatus) {controllerStatusLabel->setText(tr("Motors On"));}
    else {controllerStatusLabel->setText(tr("error"));}

    int speedScaleFactor = G_SpeedScaleFactor * 100;
    QString speedScaleFactorStr = " (Speed " + QString::number(speedScaleFactor) + "%)";
    if(PS_Running == G_ProductionStatus) {productionStatusLabel->setText(tr("Running") + speedScaleFactorStr);}
    else if(PS_Ready == G_ProductionStatus) {productionStatusLabel->setText(tr("Ready") + speedScaleFactorStr);}
    else if(PS_Stopped == G_ProductionStatus) {productionStatusLabel->setText(tr("Stopped") + speedScaleFactorStr);}
    else {productionStatusLabel->setText(tr("error"));}
}

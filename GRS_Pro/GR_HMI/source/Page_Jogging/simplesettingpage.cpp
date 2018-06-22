#include "header/Page_Jogging/simplesettingpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

SimpleSettingPage::SimpleSettingPage(Jogging::MANUALSUBWIDGET flag, QWidget *parent) :
    SimplePage(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    wFlag = flag;
    QPixmap pixmap(":/images/menu/menu_Jogging.png");
    setIcon(QIcon(pixmap));

    createPage();

    iconSize.setWidth(70);
    iconSize.setHeight(70);

    switch (flag) {
    case Jogging::MOTION_MODE:
    {
        setText(tr("Jogging - Motion Mode"));
        tipsLabel->setText(tr("Select motion mode."));
        initMotionMode();
    }
        break;
    case Jogging::COORDINATE_SYSTEM:
    {
        setText(tr("Jogging - Coordinate System"));
        tipsLabel->setText(tr("Select coordinate system."));
        initCoordSystem();
    }
        break;
    case Jogging::INCREMENT:
    {
        setText(tr("Jogging - Increment"));
        tipsLabel->setText(tr("Select incremental mode."));
        initIncrementalMode();
    }
        break;
    default:
        break;
    }

    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(contentWidgetUpdate(int)));
    connect(operatingBar->fourthButton(), SIGNAL(clicked()), this, SLOT(okBtnResponded()));
    connect(operatingBar->fifthButton(), SIGNAL(clicked()), this, SLOT(cancelBtnResponded()));
}

void SimpleSettingPage::createPage()
{
    tipsWidget = new QWidget;
    tipsWidget->setObjectName("tipsWidgetOfSimpleSettingPage");
    tipsWidget->setFixedHeight(132);
    currentKeyLabel = new QLabel(tipsWidget);
    currentKeyLabel->setProperty("h", 0);
    currentValLabel = new QLabel(tipsWidget);
    currentValLabel->setProperty("h", 0);
    tipsLabel = new QLabel(tipsWidget);
    tipsLabel->setProperty("h", 1);

    currentKeyLabel->setText(tr("Current selection:\t\t"));
    currentKeyLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    currentValLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    tipsLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    QHBoxLayout *currentKeyValLayout = new QHBoxLayout;
    currentKeyValLayout->setMargin(0);
    currentKeyValLayout->setSpacing(0);
    currentKeyValLayout->addWidget(currentKeyLabel);
    currentKeyValLayout->addWidget(currentValLabel);
    currentKeyValLayout->addStretch();
    QVBoxLayout *tipsLayout = new QVBoxLayout;
    tipsLayout->setMargin(10);
    tipsLayout->setSpacing(20);
    tipsLayout->addLayout(currentKeyValLayout);
    tipsLayout->addWidget(tipsLabel);
    tipsWidget->setLayout(tipsLayout);

    contentWidget = new QWidget;
    contentWidget->setFixedHeight(274);
    btnGroup = new QButtonGroup;

    operatingBar = new OperatingBar;
    operatingBar->firstButton()->hide();
    operatingBar->secondButton()->hide();
    operatingBar->thirdButton()->hide();
    operatingBar->fourthButton()->setText(tr("OK"));
    operatingBar->fifthButton()->setText(tr("Cancel"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(tipsWidget);
    mainLayout->addWidget(contentWidget);
    mainLayout->addWidget(operatingBar);
    this->setMainLayout(mainLayout);
}

void SimpleSettingPage::initMotionMode()
{
    iconStrList<<"Axis"<<"Linear";
    textStrList<<tr("Axis 1 - 6")<<tr("Linear");

    QHBoxLayout *mainLayout = new QHBoxLayout;

    for(int i=0; i<iconStrList.size(); i++)
    {
        QToolButton *toolBtn = new QToolButton;
        toolBtn->setObjectName("toolBtnOfSimpleSettingPage");
        toolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        QPixmap pixmap(QString(":/images/Jogging/MotionMode_%1.png").arg(iconStrList.at(i)));
        toolBtn->setIcon(pixmap);   // 为按钮设置图标
        toolBtn->setIconSize(iconSize);
        toolBtn->setFixedSize(iconSize.width()+30, iconSize.height()+30);
        toolBtn->setText(textStrList.at(i)); // 设置提示文字
        toolBtn->setCheckable(true);
        toolBtn->setChecked(false);

        btnGroup->addButton(toolBtn, Jogging::PAR_AXIS1_6+i);
        mainLayout->addWidget(toolBtn);
    }
    contentWidget->setLayout(mainLayout);

    // 设置初选项
    contentWidgetUpdate(joggingPar.motionMode);
}

void SimpleSettingPage::initCoordSystem()
{
    iconStrList<<"World"<<"Base"<<"Tool"<<"Wobj";
    textStrList<<tr("大地坐标")<<tr("基坐标")<<tr("工具坐标")<<tr("工件坐标");

    QHBoxLayout *mainLayout = new QHBoxLayout;

    for(int i=0; i<iconStrList.size(); i++)
    {
        QToolButton *toolBtn = new QToolButton;
        toolBtn->setObjectName("toolBtnOfSimpleSettingPage");
        toolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        QPixmap pixmap(QString(":/images/Jogging/Coordinate_%1.png").arg(iconStrList.at(i)));
        toolBtn->setIcon(pixmap);   // 为按钮设置图标
        toolBtn->setIconSize(iconSize);
        toolBtn->setFixedSize(iconSize.width()+30, iconSize.height()+30);
        toolBtn->setText(textStrList.at(i)); // 设置提示文字
        toolBtn->setCheckable(true);
        toolBtn->setChecked(false);

        btnGroup->addButton(toolBtn, Jogging::PAR_WORLD+i);
        mainLayout->addWidget(toolBtn);
    }
    contentWidget->setLayout(mainLayout);

    // 设置初选项
    contentWidgetUpdate(joggingPar.coordinateSystem);
}

void SimpleSettingPage::initIncrementalMode()
{
    iconStrList<<"None"<<"Small"<<"Medium"<<"Large"<<"User";
    textStrList<<tr("无")<<tr("小")<<tr("中")<<tr("大")<<tr("用户");

    QHBoxLayout *mainLayout = new QHBoxLayout;

    for(int i=0; i<iconStrList.size(); i++)
    {
        QToolButton *toolBtn = new QToolButton;
        toolBtn->setObjectName("toolBtnOfSimpleSettingPage");
        toolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        QPixmap pixmap(QString(":/images/Jogging/Increment_%1.png").arg(iconStrList.at(i)));
        toolBtn->setIcon(pixmap);   // 为按钮设置图标
        toolBtn->setIconSize(iconSize);
        toolBtn->setFixedSize(iconSize.width()+30, iconSize.height()+30);
        toolBtn->setText(textStrList.at(i)); // 设置提示文字
        toolBtn->setCheckable(true);
        toolBtn->setChecked(false);

        btnGroup->addButton(toolBtn, Jogging::PAR_NONE+i);
        mainLayout->addWidget(toolBtn);
    }
    contentWidget->setLayout(mainLayout);

    // 设置初选项
    contentWidgetUpdate(joggingPar.increment);
}

void SimpleSettingPage::contentWidgetUpdate(int id)
{
    btnGroup->button(id)->setChecked(true);
//    currentValLabel->setText(textStrList.at(id));
    currentValLabel->setText(btnGroup->checkedButton()->text());
}

void SimpleSettingPage::okBtnResponded()
{
    switch (wFlag) {
    case Jogging::MOTION_MODE:
    {
        joggingPar.motionMode = (Jogging::TypeMotionMode)btnGroup->checkedId();
        // 更新到插补模块
        if(Jogging::PAR_AXIS1_6 == joggingPar.motionMode) {pInputData->mode = ITP_Mode_JointOp_SpeedAndIncMode;}  // 关节模式
        else if(Jogging::PAR_LINEAR == joggingPar.motionMode) {pInputData->mode = ITP_Mode_CoordinateOp_SpeedAndIncMode;} // 坐标模式
    }
        break;
    case Jogging::COORDINATE_SYSTEM:
    {
        joggingPar.coordinateSystem = (Jogging::TypeCoordinateSystem)btnGroup->checkedId();
        // 更新到插补模块
        if(Jogging::PAR_WORLD == joggingPar.coordinateSystem) {pInputData->flagCoordinate = -1;}  // 未定义
        else if(Jogging::PAR_BASE == joggingPar.coordinateSystem) {pInputData->flagCoordinate = 0;}   // 0表示基坐标系
        else if(Jogging::PAR_TOOL == joggingPar.coordinateSystem) {pInputData->flagCoordinate = 1;}   // 1表示工具坐标系
        else if(Jogging::PAR_WORKOBJECT == joggingPar.coordinateSystem) {pInputData->flagCoordinate = 2;} // 2表示用户自定义坐标系(基准坐标系为基坐标系)
    }
        break;
    case Jogging::INCREMENT:
    {
        joggingPar.increment = (Jogging::TypeIncrement)btnGroup->checkedId();
        if(Jogging::PAR_NONE == joggingPar.increment) {SW_Increment = SW_OFF;} // 关闭或打开增量开关
        else {SW_Increment = SW_ON;}
        // 不需要更新到插补模块，仅由界面控制
    }
        break;
    default:
        break;
    }

    emit okBtnClicked();
    this->close();
}

void SimpleSettingPage::cancelBtnResponded()
{
    this->close();
}

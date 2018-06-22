#include "header/Page_Jogging/gotopage.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

GoToPage::GoToPage(QWidget *parent) :
    SimplePage(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    QPixmap pixmap(":/images/menu/menu_Jogging.png");
    setIcon(QIcon(pixmap));
    setText(tr("Jogging - Go to Position"));

    mode = pInputData->mode;
    memset(&tarRob, 0, sizeof(ITP_RobTarget));
    createPage();
    listDataUpdate();

    connect(operatingBar->fifthButton(), SIGNAL(clicked()), this, SLOT(closeBtnResponded()));
    connect(robtargetListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(listWidgetClicked()));
    connect(goToPosBtn, SIGNAL(pressed()), this, SLOT(goToPosBtnPressed()));
    connect(goToPosBtn, SIGNAL(released()), this, SLOT(goToPosBtnReleased()));
}

void GoToPage::createPage()
{
    /* 提示栏 */
    tipsWidget = new QWidget;
    tipsWidget->setObjectName("tipsWidgetOfSimpleSettingPage");
    tipsWidget->setFixedHeight(132);

    mechanicalUnitKeyLabel = new QLabel(tipsWidget);  // 机械单元
    mechanicalUnitValLabel = new QLabel(tipsWidget);
    activeToolKeyLabel = new QLabel(tipsWidget);      // 活动工具
    activeToolValLabel = new QLabel(tipsWidget);
    activeWobjKeyLabel = new QLabel(tipsWidget);      // 活动工件
    activeWobjValLabel = new QLabel(tipsWidget);
    mechanicalUnitKeyLabel->setProperty("h", 0);
    mechanicalUnitValLabel->setProperty("h", 0);
    activeToolKeyLabel->setProperty("h", 0);
    activeToolValLabel->setProperty("h", 0);
    activeWobjKeyLabel->setProperty("h", 0);
    activeWobjValLabel->setProperty("h", 0);

    mechanicalUnitKeyLabel->setText(tr("Mechanical unit:"));
    mechanicalUnitValLabel->setText(tr("ROB_1"));
    activeToolKeyLabel->setText(tr("Active tool:"));
    activeToolValLabel->setText(QString(QLatin1String(G_CurToolData.dataProperty.typeName)));
    activeWobjKeyLabel->setText(tr("active work object:"));
    activeWobjValLabel->setText(QString(QLatin1String(G_CurWobjData.dataProperty.typeName)));

    QGridLayout *tipsLayout = new QGridLayout;
    tipsLayout->setContentsMargins(20, 20, 300, 30);
    tipsLayout->setSpacing(20);
    tipsLayout->addWidget(mechanicalUnitKeyLabel, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    tipsLayout->addWidget(mechanicalUnitValLabel, 0, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    tipsLayout->addWidget(activeToolKeyLabel, 1, 0, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    tipsLayout->addWidget(activeToolValLabel, 1, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    tipsLayout->addWidget(activeWobjKeyLabel, 2, 0, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    tipsLayout->addWidget(activeWobjValLabel, 2, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    tipsWidget->setLayout(tipsLayout);

    /* 内容栏 */
    contentWidget = new QWidget;
    contentWidget->setFixedSize(UI_SCREEN_WIDTH, 274);
    robtargetListWidget = new QListWidget(contentWidget);   // 显示当前任务中的机器人示教点列表

    activeFilterKeyLabel = new QLabel(contentWidget); // 活动过滤器： XXX
    activeFilterKeyLabel->setProperty("h", 5);
    activeFilterKeyLabel->setText(tr("Active filter:\t"));
    activeFilterValLabel = new QLabel(contentWidget);
    activeFilterValLabel->setProperty("h", 5);
    activeFilterValLabel->setText(tr("None"));
    step1Label = new QLabel(contentWidget);
    step1Label->setText(tr("1. Press and hold Enabling Device."));
    step2Label = new QLabel(contentWidget);
    step2Label->setText(tr("2. Press and hold Go To button to go to position."));
    goToPosBtn = new QPushButton(contentWidget);    // “转到”按钮
    goToPosBtn->setProperty("type", "normal");
    goToPosBtn->setFixedHeight(40);
    goToPosBtn->setText(tr("Go to"));
    goToPosBtn->setEnabled(false);

    QHBoxLayout *contentRightTopLayout = new QHBoxLayout;
    contentRightTopLayout->setContentsMargins(0, 0, 0, 10);
    contentRightTopLayout->setSpacing(0);
    contentRightTopLayout->addWidget(activeFilterKeyLabel);
    contentRightTopLayout->addWidget(activeFilterValLabel);
    contentRightTopLayout->addStretch();

    QVBoxLayout *contentRightLayout = new QVBoxLayout;
    contentRightLayout->setMargin(20);
    contentRightLayout->setSpacing(20);
    contentRightLayout->addLayout(contentRightTopLayout);
    contentRightLayout->addWidget(step1Label);
    contentRightLayout->addWidget(step2Label);
    contentRightLayout->addStretch();
    contentRightLayout->addWidget(goToPosBtn, Qt::AlignCenter);

    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->setMargin(0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(robtargetListWidget, 6);
    contentLayout->addLayout(contentRightLayout, 4);
    contentWidget->setLayout(contentLayout);


    // bottom
    operatingBar = new OperatingBar;
    operatingBar->firstButton()->setText(tr("Filter"));
    operatingBar->secondButton()->hide();
    operatingBar->thirdButton()->hide();
    operatingBar->fourthButton()->hide();
    operatingBar->fifthButton()->setText(tr("Close"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(tipsWidget);
    mainLayout->addWidget(contentWidget);
    mainLayout->addWidget(operatingBar);
    this->setMainLayout(mainLayout);
}

void GoToPage::listDataUpdate()
{
    // ==================== 表格内容显示
    int moduleNum = getFileModuleCnt(0);
    decode_type_property tmpProperty;
    module_prg_property tmpModule;
    decode_type_data tmpData;
    int i = 0, j = 0, rowCnt = 0;

    for(i=0; i<moduleNum; i++)
    {
        j = 0;
        getSelModuleProperty(0, i, &tmpModule);
        while(1 == usePosGetSelVarProperty(0, i, -1, j, D_DATATYPE_ROBTARGET, &tmpProperty))
        {
            // 手动操纵可用的TOOLDATA一定是PERS类型的，且不能是LOCAL的
            //if(tmpProperty.storeType != D_DATATYPE_PERS || tmpProperty.scope == D_DATATYPE_LOCAL) continue;

            QString name = QString(QLatin1String(tmpProperty.typeName));
            qDebug("typeName = %s", tmpProperty.typeName);
            QListWidgetItem *item = new QListWidgetItem(name);
            item->setSizeHint(QSize(300, 40));
            robtargetListWidget->addItem(item);

            usePosGetSelVarVal(0, i, -1, j, D_DATATYPE_ROBTARGET, &tmpData);
            // 插补模块和译码模块定义的数据类型不一样，所以要这样子处理
            tarRob.pose.trans.x = tmpData.robtargetData.trans.x;
            tarRob.pose.trans.y = tmpData.robtargetData.trans.y;
            tarRob.pose.trans.z = tmpData.robtargetData.trans.z;
            tarRob.pose.rot.s = tmpData.robtargetData.rot.q1;
            tarRob.pose.rot.x = tmpData.robtargetData.rot.q2;
            tarRob.pose.rot.y = tmpData.robtargetData.rot.q3;
            tarRob.pose.rot.z = tmpData.robtargetData.rot.q4;

            map.insert(name, tarRob);

            qDebug("x=%f, y=%f, z=%f, q1=%f, q2=%f, q3=%f, q4=%f", tmpData.robtargetData.trans.x, tmpData.robtargetData.trans.y,
                   tmpData.robtargetData.trans.z, tmpData.robtargetData.rot.q1, tmpData.robtargetData.rot.q2,
                   tmpData.robtargetData.rot.q3, tmpData.robtargetData.rot.q4);

            rowCnt++;
            j++;
        }
    }
}

void GoToPage::listWidgetClicked()
{
    if(!robtargetListWidget->currentItem()->text().isEmpty()) {goToPosBtn->setEnabled(true);}

    tarRob = map.value(robtargetListWidget->currentItem()->text());
}

void GoToPage::filterBtnResponded()
{

}

void GoToPage::closeBtnResponded()
{
    pInputData->mode = mode;
    this->close();
}

void GoToPage::goToPosBtnPressed()
{
    // 是否有一些点被插补算法认为无法到达，所以按键动作失效？——2015.11.13
    // 问题描述：有时候按键失灵，但是一旦按键进入转到模式后就不再失灵。。。

    if(ITP_Mode_JointOp_PosMode != pInputData->mode) {pInputData->mode = ITP_Mode_JointOp_PosMode;}

    pInputData->tarRob = tarRob;
    // robotPosSpeed 表示工具轨迹速度,工具方向速度。当两个值有一个不为0时，以该值为速度运动
    pInputData->robotPosSpeed.v_ort = (MaxSpeed_Reorient * interpolatePeriod / 1000) * JoggingSpeedRate * SysSpeedRate;
    pInputData->robotPosSpeed.v_tcp = (MaxSpeed_Linear * interpolatePeriod / 1000) * JoggingSpeedRate * SysSpeedRate;

    qDebug("x=%f, y=%f, z=%f, q1=%f, q2=%f, q3=%f, q4=%f", pInputData->tarRob.pose.trans.x, pInputData->tarRob.pose.trans.y,
           pInputData->tarRob.pose.trans.z, pInputData->tarRob.pose.rot.s, pInputData->tarRob.pose.rot.x,
           pInputData->tarRob.pose.rot.y, pInputData->tarRob.pose.rot.z);
    qDebug("<GoToBtn pressed> v_ort=%f, v_tcp=%f, mode=%d", pInputData->robotPosSpeed.v_ort, pInputData->robotPosSpeed.v_tcp, pInputData->mode);
}

void GoToPage::goToPosBtnReleased()
{
    pInputData->robotPosSpeed.v_ort = 0;    // 工具方向速度 radian/插补周期
    pInputData->robotPosSpeed.v_tcp = 0;    // 工具轨迹速度 mm/插补周期

    qDebug("<GoToBtn released>");
}


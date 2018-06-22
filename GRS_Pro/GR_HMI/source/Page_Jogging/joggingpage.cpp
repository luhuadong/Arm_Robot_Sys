#include "header/Page_Jogging/joggingpage.h"

#include <QPixmap>
#include <QIcon>
#include <QHBoxLayout>
#include <QHeaderView>

JoggingPage::JoggingPage(QWidget *parent) :
    SimplePage(parent)
{
    QPixmap pixmap(":/images/menu/menu_Jogging.png");
    setIcon(QIcon(pixmap));
    setText(tr("Jogging"));

    joggingParInit();

    // 中间
    centralWidgetInit();

    operatingBar = new OperatingBar(this);
    operatingBar->firstButton()->setText(tr("Align"));
    operatingBar->secondButton()->setText(tr("Go To"));
    operatingBar->thirdButton()->setText(tr("Activate Units"));
    operatingBar->thirdButton()->setEnabled(false);
    operatingBar->fourthButton()->hide();
    operatingBar->fifthButton()->hide();
    operatingBar->setGeometry(0, this->height() - operatingBar->height(), operatingBar->width(), operatingBar->height());
    connect(operatingBar->firstButton(), SIGNAL(clicked()), this, SLOT(alignBtnClicked()));
    connect(operatingBar->secondButton(), SIGNAL(clicked()), this, SLOT(goToBtnClicked()));

    // 最后做的事情
    updatePoseDataThread = new UpdatePoseDataThread;
    connect(updatePoseDataThread, SIGNAL(poseDataUpdated(QString*)), this, SLOT(updatePoseData(QString*)));
    updatePoseDataThread->start();
}

void JoggingPage::joggingParInit()
{
    // ITP_Mode_JointOp_SpeedMode为关节模式，ITP_Mode_CoordinateOp_SpeedMode为坐标模式，-1为未定义
    if(Jogging::PAR_AXIS1_6 == joggingPar.motionMode) {pInputData->mode = ITP_Mode_JointOp_SpeedAndIncMode;}
    else if(Jogging::PAR_LINEAR == joggingPar.motionMode) {pInputData->mode = ITP_Mode_CoordinateOp_SpeedAndIncMode;}

    // -1表示未定义，0表示基坐标系，1表示工具坐标系，2表示用户自定义坐标系（基准坐标系为基坐标系）
    if(Jogging::PAR_WORLD == joggingPar.coordinateSystem) {pInputData->flagCoordinate = -1;}
    else if(Jogging::PAR_BASE == joggingPar.coordinateSystem) {pInputData->flagCoordinate = 0;}
    else if(Jogging::PAR_TOOL == joggingPar.coordinateSystem) {pInputData->flagCoordinate = 1;}
    else if(Jogging::PAR_WORKOBJECT == joggingPar.coordinateSystem) {pInputData->flagCoordinate = 2;}

    // 速度倍率设为0.2
    //pInputData->rate = 0.2;

    for(int i=0; i<ITP_RobotJointNum; i++) {pInputData->robotMove[i] = 0;}  // 机器人停止
    for(int i=0; i<ITP_ExtaxAxisNum; i++) {pInputData->extaxRotate[i] = 0;} // 外部轴停止

    pInputData->flagIncUpdate = 0;  // 增量标识
    pInputData->jogMoveVal = 0.0;   // 增量距离，初始值设为任意值即可，因为操纵杆动作时会对其重新赋值

//    sysInitRobTarget = pOutputData->curRob; // 定义一个机器人位置点，用于测试【转到】功能


    /* ************* 以下是手动操纵界面中用到的全局字符串或字符串链表（初始化） ************* */
//    joggingPropertyStrList << tr("机械单元：") << tr("绝对精度：") << tr("动作模式：")
//                           << tr("坐标系：") << tr("工具坐标：") << tr("工件坐标：")
//                           << tr("有效载荷：") << tr("操纵杆锁定：") << tr("增量：");
    joggingPropertyStrList << tr("Mechanical unit:") << tr("Absolute accuracy:") << tr("Motion mode:")
                           << tr("Coordinate system:") << tr("Tool:") << tr("Work object:")
                           << tr("Payload:") << tr("Joystick lock:") << tr("Increment:");
    mechanicalUnitStrList<<tr("ROB_1");      // 机械单元选项文字
    absoluteAccuracyStrList<<tr("Off")<<tr("On");    // 绝对精度选项文字
    motionModeStrList<<tr("轴 1 - 6")<<tr("线性")<<tr("重定位");          // 动作模式选项文字
    coordinateSystemStrList<<tr("大地坐标")<<tr("基坐标")<<tr("工具坐标")<<tr("工件坐标");    // 坐标系选项文字
    //toolStrList;                // 工具坐标选项文字
    //workObjectStrList;          // 工件坐标选项文字
    //payloadStrList;             // 有效载荷选项文字
    //joystickLockStrList;        // 操纵杆锁定选项文字
    incrementStrList<<tr("无")<<tr("小")<<tr("中")<<tr("大")<<tr("用户");           // 增量选项文字
    propertyTipsStrList<<tr("选择要更改其状态的机械单元。")<<("选择是否开启绝对精度。")<<tr("选择动作模式。")
                      <<tr("选择坐标系。")<<tr("从列表中选择一个工具坐标。")<<tr("从列表中选择一个工件坐标。")
                     <<tr("从列表中选择一个有效载荷。")<<tr("选择需要锁定的操纵杆方向。")<<tr("选择增量模式。");

    jointOpKeyStrList<<tr("1:")<<tr("2:")<<tr("3:")<<tr("4:")<<tr("5:")<<tr("6:")<<tr(" ");
    cooOpKeyStrList_Qua<<tr("X:")<<tr("Y:")<<tr("Z:")<<tr("q1:")<<tr("q2:")<<tr("q3")<<tr("q4");
    cooOpKeyStrList_Eul<<tr("X:")<<tr("Y:")<<tr("Z:")<<tr("EZ:")<<tr("EY:")<<tr("EX:")<<tr(" ");
    jointOpUnitStrList_Deg<<tr("°")<<tr("°")<<tr("°")<<tr("°")<<tr("°")<<tr("°")<<tr(" ");
    jointOpUnitStrList_Rad<<tr("rad")<<tr("rad")<<tr("rad")<<tr("rad")<<tr("rad")<<tr("rad")<<tr(" ");
    cooOpUnitStrList_Qua<<tr("mm")<<tr("mm")<<tr("mm")<<(" ")<<(" ")<<(" ")<<(" ");
    cooOpUnitStrList_Eul_Deg<<tr("mm")<<tr("mm")<<tr("mm")<<tr("°")<<tr("°")<<tr("°")<<tr(" ");
    cooOpUnitStrList_Eul_Rad<<tr("mm")<<tr("mm")<<tr("mm")<<tr("rad")<<tr("rad")<<tr("rad")<<tr(" ");

    jointMoveBtnStrList<<tr("J1+")<<tr("J1-")<<tr("J2+")<<tr("J2-")<<tr("J3+")<<tr("J3-")
                         <<tr("J4+")<<tr("J4-")<<tr("J5+")<<tr("J5-")<<tr("J6+")<<tr("J6-");
    coordMoveBtnStrList<<tr("X+")<<tr("X-")<<tr("Y+")<<tr("Y-")<<tr("Z+")<<tr("Z-")
                         <<tr("A+")<<tr("A-")<<tr("B+")<<tr("B-")<<tr("C+")<<tr("C-");

}

void JoggingPage::centralWidgetInit()
{
    int sideMargin=10, topPole=30+16, boxHeight=380;
    int spacing = 5;

    // 属性设置框
    settingBox = new QGroupBox(tr("Tap a property to change it"), this);
    settingBox->setFixedSize(475, boxHeight);

    settingTable = new QTableWidget(9, 2);
    settingTable->setFixedSize(settingBox->width()-20, settingBox->height()-30);
    settingTable->horizontalHeader()->hide();
    settingTable->verticalHeader()->hide();
    settingTable->setShowGrid(false);
    settingTable->setFrameShape(QFrame::NoFrame);
    settingTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    settingTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    settingTable->setEditTriggers(false);
    settingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    settingTable->setSelectionMode(QAbstractItemView::SingleSelection);
    for(int i=0; i<settingTable->rowCount(); i++)
    {
        settingTable->setRowHeight(i, settingTable->height()/9);
    }
    settingTable->verticalHeader()->setStretchLastSection(true);
    for(int i=0; i<settingTable->columnCount(); i++)
    {
        settingTable->setColumnWidth(i, settingTable->width()/2);
    }
    settingTable->horizontalHeader()->setStretchLastSection(true);

    createSettingTableItem();

    QHBoxLayout *settingBoxLayout = new QHBoxLayout;
    settingBoxLayout->setContentsMargins(10, 20, 10, 10);
    settingBoxLayout->addWidget(settingTable);
    settingBox->setLayout(settingBoxLayout);
    settingBox->setGeometry(sideMargin, topPole, settingBox->width(), settingBox->height());
    connect(settingTable, SIGNAL(cellClicked(int,int)), this, SLOT(settingTableClicked(int,int)));

    // 位置显示框
    displayBox = new QGroupBox(tr("Position"), this);
    displayBox->setFixedSize(300, boxHeight);   // UI_SCREEN_WIDTH - sideMargin*2 - spacing - settingBox->width()
    displayBox->setGeometry(sideMargin+settingBox->width()+spacing, topPole, displayBox->width(), displayBox->height());

    displayLabel = new QLabel(tr("Positions in coord: "), displayBox);
    displayLabel->setObjectName("blueLabel");
    displayLabel->setGeometry(10, 10, displayBox->width(), 30);

    displayTable = new QTableWidget(POSITION_ROWS, POSITION_COLUMNS, displayBox);
    displayTable->setFixedSize(260, 245);
    displayTable->setGeometry(20, 40, displayBox->width(), displayBox->height());
    displayTable->horizontalHeader()->hide();
    displayTable->verticalHeader()->hide();
    displayTable->setShowGrid(false);
    displayTable->setFrameShape(QFrame::NoFrame);
    displayTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    displayTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    displayTable->setSelectionMode(QAbstractItemView::NoSelection);
    for(int i=0; i<displayTable->rowCount(); i++)
    {
        displayTable->setRowHeight(i, displayTable->height()/POSITION_ROWS);
    }
    displayTable->verticalHeader()->setStretchLastSection(true);
    displayTable->setColumnWidth(0, displayTable->width()/4);
    displayTable->setColumnWidth(1, displayTable->width()/2);
    displayTable->setColumnWidth(2, displayTable->width()/4);
    displayTable->horizontalHeader()->setStretchLastSection(true);


    int btnSpacing = 30, btnWidth = 60, btnHeight = 60;
    positionShowInBtn = new QPushButton(displayBox);
    positionShowInBtn->setObjectName("displayBoxBtn");
    positionShowInBtn->setFixedSize(btnWidth, btnHeight);
    positionShowInBtn->setGeometry(btnSpacing, 300, btnWidth, btnHeight);
    connect(positionShowInBtn, SIGNAL(clicked()), this, SLOT(positionShowInBtnClicked()));

    orientationFormatBtn = new QPushButton(displayBox);
    orientationFormatBtn->setObjectName("displayBoxBtn");
    orientationFormatBtn->setFixedSize(btnWidth, btnHeight);
    orientationFormatBtn->setGeometry(btnSpacing*2 + btnWidth, 300, btnWidth, btnHeight);
    connect(orientationFormatBtn, SIGNAL(clicked()), this, SLOT(orientationFormatBtnClicked()));

    angleUnitBtn = new QPushButton(displayBox);
    angleUnitBtn->setObjectName("displayBoxBtn");
    angleUnitBtn->setFixedSize(btnWidth, btnHeight);
    angleUnitBtn->setGeometry(btnSpacing*3 + btnWidth*2, 300, btnWidth, btnHeight);
    connect(angleUnitBtn, SIGNAL(clicked()), this, SLOT(angleUnitBtnClicked()));

    createDisplayTableItem();
}

void JoggingPage::createSettingTableItem()
{
    /* ***********************************************************************
     * 属性表的Key的text在这里设置好，而Value的text则要根据手动操纵模块的参数来设置。 20160122
    *********************************************************************** */
    for(int i=0; i<settingTable->rowCount(); i++)
    {
        QTableWidgetItem *keyItem = new QTableWidgetItem(joggingPropertyStrList.at(i));
        settingTable->setItem(i, 0, keyItem);

        QTableWidgetItem *valItem = new QTableWidgetItem(tr("Hello World"));
        settingTable->setItem(i, 1, valItem);
    }

    // 未开放的功能
    settingTable->item(1, 0)->setFlags(settingTable->item(1, 0)->flags() & ~(Qt::ItemIsEnabled));
    settingTable->item(1, 1)->setFlags(settingTable->item(1, 1)->flags() & ~(Qt::ItemIsEnabled));
    settingTable->item(1, 1)->setText(absoluteAccuracyStrList.at(0));
    settingTable->item(6, 0)->setFlags(settingTable->item(6, 0)->flags() & ~(Qt::ItemIsEnabled));
    settingTable->item(6, 1)->setFlags(settingTable->item(6, 1)->flags() & ~(Qt::ItemIsEnabled));
    settingTable->item(6, 1)->setText("load0");
    settingTable->item(7, 0)->setFlags(settingTable->item(7, 0)->flags() & ~(Qt::ItemIsEnabled));
    settingTable->item(7, 1)->setFlags(settingTable->item(7, 1)->flags() & ~(Qt::ItemIsEnabled));
    settingTable->item(7, 1)->setText("None");

    settingBoxUpdate();
}

void JoggingPage::createDisplayTableItem()
{
    for(int i=0; i<POSITION_ROWS; i++)
    {
        // 第一列
        QTableWidgetItem *keyItem = new QTableWidgetItem();
        keyItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        poseTabKeyItemList.append(keyItem);
        displayTable->setItem(i, 0, keyItem);

        // 第二列
        QTableWidgetItem *valItem = new QTableWidgetItem();
        valItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        poseTabValItemList.append(valItem);
        displayTable->setItem(i, 1, valItem);

        // 第三列
        QTableWidgetItem *unitItem = new QTableWidgetItem();
        unitItem->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        poseTabUnitItemList.append(unitItem);
        displayTable->setItem(i, 2, unitItem);
    }

    displayFmt.curCoord = Jogging::PAR_SHOW_BASE;
    displayFmt.curOrientFmt = Jogging::PAR_SHOW_QUATERNION;
    displayFmt.curUnit = Jogging::PAR_SHOW_RADIANS;
    positionShowInBtn->setText(tr("Base"));
    orientationFormatBtn->setText(tr("Quate"));
    angleUnitBtn->setText(tr("Rad"));
    angleUnitBtn->setEnabled(false);

    displayBoxUpdate();
}

//void JoggingPage::subPageIsOk()
//{
//    if(motionModePage != NULL)
//    {
//        settingBoxUpdate();
//        displayBoxUpdate();
//        disconnect(motionModePage, SIGNAL(okBtnClicked()), this, SLOT(subPageIsOk()));
//        delete motionModePage;
//        motionModePage = NULL;
//    }
//}

void JoggingPage::settingTableClicked(const int row, const int)
{
    if(row == 0)
    {
        settingBoxUpdate();
    }
    else if(row == 2)
    {
        motionModePage = new SimpleSettingPage(Jogging::MOTION_MODE, this);
        connect(motionModePage, SIGNAL(okBtnClicked()), this, SLOT(settingBoxUpdate()));
        connect(motionModePage, SIGNAL(okBtnClicked()), this, SLOT(displayBoxUpdate()));
//        connect(motionModePage, SIGNAL(okBtnClicked()), this, SLOT(subPageIsOk()));
        motionModePage->show();
    }
    else if(row == 3)
    {
        coordSystemPage = new SimpleSettingPage(Jogging::COORDINATE_SYSTEM, this);
        connect(coordSystemPage, SIGNAL(okBtnClicked()), this, SLOT(settingBoxUpdate()));
        coordSystemPage->show();
    }
    else if(row == 4)
    {
        toolPage = new TableSettingPage(Jogging::TOOL, this);
        connect(toolPage, SIGNAL(okBtnClicked()), this, SLOT(settingBoxUpdate()));
        toolPage->show();
    }
    else if(row == 5)
    {
        wobjPage = new TableSettingPage(Jogging::WORK_OBJECT, this);
        connect(wobjPage, SIGNAL(okBtnClicked()), this, SLOT(settingBoxUpdate()));
        wobjPage->show();
    }
    else if(row == 8)
    {
        incrementPage = new SimpleSettingPage(Jogging::INCREMENT, this);
        connect(incrementPage, SIGNAL(okBtnClicked()), this, SLOT(settingBoxUpdate()));
        incrementPage->show();
    }
}

void JoggingPage::positionShowInBtnClicked()
{
    if(Jogging::PAR_SHOW_WORLD == displayFmt.curCoord)
    {
        displayFmt.curCoord = Jogging::PAR_SHOW_BASE;
        positionShowInBtn->setText(tr("Base"));
    }
    else if(Jogging::PAR_SHOW_BASE == displayFmt.curCoord)
    {
        displayFmt.curCoord = Jogging::PAR_SHOW_WOBJ;
        positionShowInBtn->setText(tr("Wobj"));
    }
    else if(Jogging::PAR_SHOW_WOBJ == displayFmt.curCoord)
    {
        displayFmt.curCoord = Jogging::PAR_SHOW_WORLD;
        positionShowInBtn->setText(tr("World"));
    }
    displayBoxUpdate();
}

void JoggingPage::orientationFormatBtnClicked()
{
    if(Jogging::PAR_SHOW_QUATERNION == displayFmt.curOrientFmt)
    {
        displayFmt.curOrientFmt = Jogging::PAR_SHOW_EULERANGLES;
        orientationFormatBtn->setText(tr("Euler"));
        angleUnitBtn->setEnabled(true);
    }
    else if(Jogging::PAR_SHOW_EULERANGLES == displayFmt.curOrientFmt)
    {
        displayFmt.curOrientFmt = Jogging::PAR_SHOW_QUATERNION;
        orientationFormatBtn->setText(tr("Quate"));
        angleUnitBtn->setEnabled(false);
    }
    displayBoxUpdate();
}

void JoggingPage::angleUnitBtnClicked()
{
    if(Jogging::PAR_SHOW_DEGREES == displayFmt.curUnit)
    {
        displayFmt.curUnit = Jogging::PAR_SHOW_RADIANS;
        angleUnitBtn->setText(tr("Rad"));
    }
    else if(Jogging::PAR_SHOW_RADIANS == displayFmt.curUnit)
    {
        displayFmt.curUnit = Jogging::PAR_SHOW_DEGREES;
        angleUnitBtn->setText(tr("Deg"));
    }
    displayBoxUpdate();
}

void JoggingPage::displayBoxUpdate()
{
    // 根据当前对关节、坐标操作，四元数、欧拉角的选择来确定【位置】表中第一列的显示格式
    if(joggingPar.motionMode == Jogging::PAR_AXIS1_6)   // 关节操作
    {
        displayLabel->setText(tr(" "));    // 关节操作没有坐标概念，故隐藏坐标提示
        poseKeyItemStrList = &jointOpKeyStrList;
        if(displayFmt.curUnit == Jogging::PAR_SHOW_DEGREES) {poseUnitItemStrList = &jointOpUnitStrList_Deg;}// 度
        else if(displayFmt.curUnit == Jogging::PAR_SHOW_RADIANS) {poseUnitItemStrList = &jointOpUnitStrList_Rad;}// 弧度
    }
    else    // 坐标操作
    {
        // 坐标操作有必要显示位置数据是在哪个坐标系中表示的
        displayLabel->setText(tr("Positions in Coord: ") + positionShowInBtn->text());
        if(displayFmt.curOrientFmt == Jogging::PAR_SHOW_QUATERNION)  // 四元数
        {
            poseKeyItemStrList = &cooOpKeyStrList_Qua;
            poseUnitItemStrList = &cooOpUnitStrList_Qua;
        }
        else if(displayFmt.curOrientFmt == Jogging::PAR_SHOW_EULERANGLES)    // 欧拉角
        {
            poseKeyItemStrList = &cooOpKeyStrList_Eul;
            if(displayFmt.curUnit == Jogging::PAR_SHOW_DEGREES) {poseUnitItemStrList = &cooOpUnitStrList_Eul_Deg;}// 度
            else if(displayFmt.curUnit == Jogging::PAR_SHOW_RADIANS) {poseUnitItemStrList = &cooOpUnitStrList_Eul_Rad;}// 弧度
        }
    }

    for(int i=0; i<POSITION_ROWS; i++)
    {
        poseTabKeyItemList.at(i)->setText(poseKeyItemStrList->at(i));   // 第一列
        poseTabUnitItemList.at(i)->setText(poseUnitItemStrList->at(i)); // 第三列
    }
}

// 实时更新位置数据
void JoggingPage::updatePoseData(QString *poseData)
{
    for(int i=0; i<POSITION_ROWS; i++) {poseTabValItemList.at(i)->setText(poseData[i]);}
}

void JoggingPage::settingBoxUpdate()
{
    settingTable->item(0, 1)->setText(mechanicalUnitStrList.at(0));
    settingTable->item(2, 1)->setText(motionModeStrList.at(joggingPar.motionMode));
    settingTable->item(3, 1)->setText(coordinateSystemStrList.at(joggingPar.coordinateSystem));
    settingTable->item(4, 1)->setText(QString(QLatin1String(G_CurToolData.dataProperty.typeName)));
    settingTable->item(5, 1)->setText(QString(QLatin1String(G_CurWobjData.dataProperty.typeName)));
    settingTable->item(8, 1)->setText(incrementStrList.at(joggingPar.increment));

    for(int i=0; i<PROPERTY_ROWS; i++)
    {
        settingTable->item(i, 0)->setSelected(false);
        settingTable->item(i, 1)->setSelected(false);
    }
}

void JoggingPage::alignBtnClicked()
{
    alignPage = new AlignPage(this);
    alignPage->show();
}

void JoggingPage::goToBtnClicked()
{
    goToPage = new GoToPage(this);
    goToPage->show();
}

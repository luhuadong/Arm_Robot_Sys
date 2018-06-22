#include "header/Page_Jogging/tablesettingpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

TableSettingPage::TableSettingPage(Jogging::MANUALSUBWIDGET flag, QWidget *parent) :
    SimplePage(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    wFlag = flag;
    QPixmap pixmap(":/images/menu/menu_Jogging.png");
    setIcon(QIcon(pixmap));

    createPage();

    switch (flag) {
    case Jogging::TOOL:
    {
        setText(tr("Jogging - Tool"));
        tabModel->setHeaderData(0, Qt::Horizontal, tr("Tool Name"));
        dataType = D_DATATYPE_TOOLDATA;
    }
        break;
    case Jogging::WORK_OBJECT:
    {
        setText(tr("Jogging - Work Object"));
        tabModel->setHeaderData(0, Qt::Horizontal, tr("Work Object Name"));
        dataType = D_DATATYPE_WOBJDATA;
    }
        break;
    default:
        break;
    }
    thisData.dataType = dataType;
    tableDataUpdate();

    connect(operatingBar->fourthButton(), SIGNAL(clicked()), this, SLOT(okBtnResponded()));
    connect(operatingBar->fifthButton(), SIGNAL(clicked()), this, SLOT(cancelBtnResponded()));
    connect(tabView, SIGNAL(clicked(QModelIndex)), this, SLOT(tabViewClicked(QModelIndex)));
}

void TableSettingPage::createPage()
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
    tipsLabel->setText(tr("Select an item from the list."));

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
    contentWidget->setFixedSize(UI_SCREEN_WIDTH, 274);
    tabView = new QTableView(contentWidget);
    tabView->setFixedSize(UI_SCREEN_WIDTH - 10, 274);
    tabView->setGeometry(5, 0, tabView->width(), tabView->height());
    tabModel = new QStandardItemModel;
    itemCntLabel = new QLabel(contentWidget);
    itemCntLabel->setProperty("h", 6);
    itemCntLabel->setFixedSize(80, 30);
    itemCntLabel->setAlignment(Qt::AlignCenter);
    itemCntLabel->setGeometry(contentWidget->width() - itemCntLabel->width(), 0, itemCntLabel->width(), itemCntLabel->height());

    tabModel->setColumnCount(3);
    tabModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    tabModel->setHeaderData(1, Qt::Horizontal, tr("Module"));
    tabModel->setHeaderData(2, Qt::Horizontal, tr("Scope"));

    tabView->setModel(tabModel);
    tabView->verticalHeader()->hide();   // 隐藏垂直表头
    tabView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 不显示垂直滚动条
    tabView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 不显示水平滚动条
    tabView->horizontalHeader()->setSectionsClickable(false);    // 设置水平表头不可点击
    tabView->setSelectionBehavior(QAbstractItemView::SelectRows);    // 设置每次点击选中一整行
    tabView->setSelectionMode(QAbstractItemView::SingleSelection);   // 设置每次只能选中一行
    tabView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 设置为不可编辑
    tabView->setShowGrid(false); // 设置不显示网格线
//    tabView->setFrameShape(QFrame::Box);
    QFont font = tabView->horizontalHeader()->font();    // 设置水平表头字体加粗
    font.setBold(true);
    tabView->horizontalHeader()->setFont(font);
    tabView->horizontalHeader()->setFixedHeight(34);
    tabView->verticalHeader()->setDefaultSectionSize(40);   // 改变默认行高
    //tabView->horizontalHeader()->setTextElideMode(Qt::ElideLeft);    // 超出显示区域时省略号在左边
    tabView->setColumnWidth(0, 200); // 设置第一列宽度为200
    tabView->setColumnWidth(1, 400); // 设置第二列宽度为400
    tabView->horizontalHeader()->setStretchLastSection(true);    // 设置最后一列充满表宽度


    // bottom
    operatingBar = new OperatingBar;
    operatingBar->firstButton()->setText(tr("New"));
    operatingBar->secondButton()->setText(tr("Edit"));
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

void TableSettingPage::tableDataUpdate()
{
    int rowCnt = tabModel->rowCount();
    if(rowCnt > 0)  // 先清除原来的数据
    {
        for(int i=0; i<rowCnt; i++)
        {
            delete(tabModel->item(i, 0));
            delete(tabModel->item(i, 1));
            delete(tabModel->item(i, 2));
        }
    }
    // ==================== 表格内容显示
    int moduleNum = getFileModuleCnt(0);
    qDebug("moduleNum = %d", moduleNum);
    decode_type_property tmpProperty;
    module_prg_property tmpModule;
    int i = 0, j = 0, selectedRow = 0;
    rowCnt = 0;

    for(i=0; i<moduleNum; i++)
    {
        j = 0;
        getSelModuleProperty(0, i, &tmpModule);
        while(1 == usePosGetSelVarProperty(0, i, -1, j, dataType, &tmpProperty))
        {
            // 手动操纵可用的TOOLDATA一定是PERS类型的，且不能是LOCAL的
            if(tmpProperty.storeType != D_DATATYPE_PERS || tmpProperty.scope == D_DATATYPE_LOCAL) continue;

            qDebug("typeName = %s", tmpProperty.typeName);
            QStandardItem *itemName = new QStandardItem(tmpProperty.typeName);
            tabModel->setItem(rowCnt, 0, itemName);
            //QStandardItem *itemModule = new QStandardItem(QString::number(tmpProperty.parentModule));
            QStandardItem *itemModule = new QStandardItem(tmpModule.moduleName);
            tabModel->setItem(rowCnt, 1, itemModule);
            QStandardItem *itemScope = new QStandardItem(tmpProperty.scope == D_DATATYPE_TASK ? tr("Task") : tr("Global"));
            tabModel->setItem(rowCnt, 2, itemScope);
            rowCnt++;
            j++;
        }
    }
    // 紫色字体的可选数量提示
    if(rowCnt > 0)
    {
        itemCntLabel->setText(tr("1 to ") + QString::number(rowCnt) + tr(" of ") + QString::number(rowCnt));

        if(D_DATATYPE_TOOLDATA == dataType)
        {
            for(i=0; i<rowCnt; i++)
            {
                if(tabModel->item(i, 0)->text() == QString(QLatin1String(G_CurToolData.dataProperty.typeName)))
                    selectedRow = i;
            }
        }
        else if(D_DATATYPE_WOBJDATA == dataType)
        {
            for(i=0; i<rowCnt; i++)
            {
                if(tabModel->item(i, 0)->text() == QString(QLatin1String(G_CurWobjData.dataProperty.typeName)))
                    selectedRow = i;
            }
        }
        else if(D_DATATYPE_LOADDATA == dataType)
        {
            for(i=0; i<rowCnt; i++)
            {
                if(tabModel->item(i, 0)->text() == QString(QLatin1String(G_CurLoadData.dataProperty.typeName)))
                    selectedRow = i;
            }
        }
        // 根据不同程序数据类型的当前选中参数，设置表框中的选中项
        tabView->setCurrentIndex(tabModel->index(selectedRow, 0));
        // 被选中的某行的数据显示在提示栏
        currentValLabel->setText(tabView->currentIndex().data().toString());
    }
}

void TableSettingPage::okBtnResponded()
{
    updateThisData();

    if(D_DATATYPE_TOOLDATA == dataType)
    {
        G_CurToolData = thisData;
        memcpy(&(pInputData->toolPose), &(G_CurToolData.dataValue.tooldataData.tframe), sizeof(ITP_Pose));
    }
    else if(D_DATATYPE_WOBJDATA == dataType)
    {
        G_CurWobjData = thisData;
        memcpy(&(pInputData->userCoor), &(G_CurWobjData.dataValue.wobjdataData.uframe), sizeof(ITP_Pose));
    }
    else if(D_DATATYPE_LOADDATA == dataType)
    {
        G_CurLoadData = thisData;
    }

    emit okBtnClicked();
    this->close();
}

void TableSettingPage::cancelBtnResponded()
{
    this->close();
}

void TableSettingPage::tabViewClicked(QModelIndex modelIndex)
{
    tabView->setCurrentIndex(tabModel->index(modelIndex.row(), 0));
    currentValLabel->setText(tabView->currentIndex().data().toString());
}

void TableSettingPage::updateThisData()
{
    thisData.dataType = dataType;   // 不用加这行也行，因为界面一旦生成，thisData.dataType就不会改变了
    QString name = tabView->currentIndex().data().toString();
    QString moduleInfo = tabModel->index(tabView->currentIndex().row(), 1).data().toString();

    QByteArray moduleInfoByteArray = moduleInfo.toLocal8Bit();
    int moduleNum = getModulePos(0, moduleInfoByteArray.data());
    QByteArray nameByteArray = name.toLocal8Bit();
    useNameGetSelVarProperty(0, moduleNum, -1, nameByteArray.data(), &thisData.dataProperty);
    useNameGetSelVarData(0, moduleNum, -1, thisData.dataProperty.typeName, &thisData.dataValue);
}

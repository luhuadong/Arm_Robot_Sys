#include "mainwidget.h"
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRect>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("appMainWidget");
//    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/images/GR-HMI-icon.png"));
    //setMinimumSize(878, 504);
    setFixedSize(UI_SCREEN_WIDTH + UI_PANEL_WIDTH, UI_SCREEN_HEIGHT);

    //mainFrame = new QFrame;
    panelWidget = new PanelWidget;
    screenWidget = new ScreenWidget;


    //mainFrame->setCursor(Qt::SizeAllCursor);

    //QHBoxLayout *mainFrameLayout = new QHBoxLayout(mainFrame);
    //mainFrameLayout->setMargin(0);
    //mainFrameLayout->setSpacing(0);
    //mainFrameLayout->addWidget(screenWidget, 3);
    //mainFrameLayout->addWidget(panelWidget, 1);


    mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(screenWidget);
    mainLayout->addWidget(panelWidget);
    this->setLayout(mainLayout);

    connect(panelWidget, SIGNAL(showMin()), this, SLOT(showMinimized()));
    connect(panelWidget, SIGNAL(closeApp()), this, SLOT(close()));
    connect(panelWidget, SIGNAL(switchBtnClicked()), this, SLOT(switchBtnClicked()));
    // 当操作面板的相关操作改变了系统的一些模式和状态，会在屏幕中中更新显示
    connect(panelWidget, SIGNAL(statusChanged()), screenWidget, SLOT(eventLogBtnUpdate()));
}

MainWidget::~MainWidget()
{

}

void MainWidget::switchBtnClicked()
{
    if(panelWidget->switchFlag == 0)
    {
        panelWidget->switchFlag = 1;
        QRect thisRect = this->geometry();
        QRect panelRect =  panelWidget->geometry();

        mainLayout->removeWidget(screenWidget);
        screenWidget->hide();
        this->setFixedSize(panelWidget->size());
        this->setGeometry(thisRect.x() + panelRect.x(), thisRect.y() + panelRect.y(), panelRect.width(), panelRect.height());


    }
    else if(panelWidget->switchFlag == 1)
    {
        panelWidget->switchFlag = 0;
        QRect thisRect = this->geometry();

        mainLayout->insertWidget(0, screenWidget);
        screenWidget->show();
        this->setFixedSize(UI_SCREEN_WIDTH + UI_PANEL_WIDTH, UI_SCREEN_HEIGHT);
        this->setGeometry(thisRect.x() - UI_SCREEN_WIDTH, thisRect.y(), thisRect.width() + UI_SCREEN_WIDTH, thisRect.height());
    }
}

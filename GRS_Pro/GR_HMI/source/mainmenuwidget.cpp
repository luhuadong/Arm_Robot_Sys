#include "header/mainmenuwidget.h"
#include <QHBoxLayout>
#include <QFrame>
#include <QTableWidgetItem>
#include <QHeaderView>

MainMenuWidget::MainMenuWidget(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(UI_SCREEN_WIDTH * 0.9, UI_SCREEN_HEIGHT - 54*2);
    setObjectName("mainScreenMenuWidget");

    QFrame *frame = new QFrame;

    menuTable = new QTableWidget(9, 2);
    menuTable->setFixedSize(600, 450);
    menuTable->horizontalHeader()->hide();
    menuTable->verticalHeader()->hide();
    menuTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    menuTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    menuTable->setShowGrid(false);
    menuTable->setFrameShape(QFrame::NoFrame);
    menuTable->setEditTriggers(false);
    menuTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    menuTable->setSelectionMode(QAbstractItemView::SingleSelection);

    for(int i=0; i<menuTable->rowCount(); i++)
    {
        menuTable->setRowHeight(i, menuTable->height()/9);
    }
    menuTable->verticalHeader()->setStretchLastSection(true);
    for(int i=0; i<menuTable->columnCount(); i++)
    {
        menuTable->setColumnWidth(i, menuTable->width()/2);
    }
    menuTable->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout *frameLayout = new QHBoxLayout;
    frameLayout->addWidget(menuTable, Qt::AlignCenter);
    frame->setLayout(frameLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(frame);
    setLayout(mainLayout);

    createTableItem();
}

void MainMenuWidget::createTableItem()
{
    menuItemNameList << tr("HotEdit") << tr("Inputs and Outputs") << tr("Jogging")
                     << tr("Production Window") << tr("Program Editor") << tr("Program Data")
                     << tr("RobotWare Arc") << tr("") << tr("Log Off Default User")
                     << tr("Backup and Restore") << tr("Calibration") << tr("Control Panel")
                     << tr("Event Log") << tr("FlexPendand Explorer") << tr("System Info")
                     << tr("") << tr("") << tr("Restart");

    int totalCount = menuTable->rowCount() * menuTable->columnCount();

    QPixmap pixmap;
//    QPixmap pixmap(":/images/robotIcon_16.png");
//    QIcon icon(pixmap);

    for(int i=0; i<totalCount; i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;

        if(menuItemNameList.at(i).isEmpty())
        {
            item->setFlags(item->flags() & ~(Qt::ItemIsEnabled));
//            item->setFlags(item->flags() & ~(Qt::ItemIsSelectable));
            item->setData(Qt::UserRole, Menu_none);
        }else
        {
            pixmap = QPixmap(QString(":/images/menu/menu_%1.png").arg(i));
            item->setIcon(QIcon(pixmap));
            item->setText(menuItemNameList.at(i));
            item->setData(Qt::UserRole, (menuItemTypes)i);
        }

        if(i < totalCount/2) {menuTable->setItem(i, 0, item);}
        else {menuTable->setItem(i - totalCount/2, 1, item);}
    }

}

void MainMenuWidget::menuTableUpdate()
{
    int totalCount = menuTable->rowCount() * menuTable->columnCount();
    for(int i=0; i<totalCount; i++)
    {
        if(i < totalCount/2) {menuTable->item(i, 0)->setSelected(false);}
        else {menuTable->item(i - totalCount/2, 1)->setSelected(false);}
    }
}


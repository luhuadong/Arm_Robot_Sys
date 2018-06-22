#include "header/Page_ProgramData/programdatapage.h"

#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>

ProgramDataPage::ProgramDataPage(QWidget *parent) :
    SimplePage(parent)
{
    QPixmap pixmap(":/images/menu/menu_ProgramData.png");
    setIcon(QIcon(pixmap));
    setText(tr("Program Data"));

    createPage();
}

// 20160126 突然感觉好累
void ProgramDataPage::createPage()
{
    tipsLabel = new QLabel(tr("Select a data type from the list."));
    tipsLabel->setFixedSize(UI_SCREEN_WIDTH, 48);
    tipsLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    tipsLabel->setMargin(5);
    tipsLabel->setStyleSheet("border-bottom: 1px solid Gray");

    scopeKeyLabel = new QLabel(tr("Scope:"));
    scopeKeyLabel->setFixedSize(80, 32);
    scopeKeyLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    scopeKeyLabel->setMargin(5);
    scopeValLabel = new QLabel(tr("RAPID/T_ROB1"));
    scopeValLabel->setFixedSize(560, 32);
    scopeValLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    scopeValLabel->setMargin(5);
    scopeBtn = new QPushButton(tr("Change Scope"));
    scopeBtn->setFixedSize(160, 32);

    QHBoxLayout *scopeLayout = new QHBoxLayout;
    scopeLayout->setMargin(0);
    scopeLayout->setSpacing(0);
    scopeLayout->addWidget(scopeKeyLabel);
    scopeLayout->addWidget(scopeValLabel);
    scopeLayout->addWidget(scopeBtn);

    contentTable = new QTableWidget;
    contentTable->setFixedSize(UI_SCREEN_WIDTH, 326);
    contentTable->setColumnCount(3);
    contentTable->setRowCount(5);
    contentTable->verticalHeader()->hide();
    contentTable->horizontalHeader()->hide();
    contentTable->setShowGrid(false);
    contentTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    contentTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    contentTable->setEditTriggers(false);
    contentTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    contentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    contentTable->horizontalHeader()->setFixedHeight(34);
    contentTable->verticalHeader()->setDefaultSectionSize(40);
    contentTable->setColumnWidth(0, contentTable->width()/3);
    contentTable->setColumnWidth(1, contentTable->width()/3);
    contentTable->horizontalHeader()->setStretchLastSection(true);


    // bottom
    operatingBar = new OperatingBar;
    operatingBar->firstButton()->hide();
    operatingBar->secondButton()->hide();
    operatingBar->thirdButton()->hide();
    operatingBar->fourthButton()->setText(tr("Show Data"));
    operatingBar->fifthButton()->setText(tr("View"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(tipsLabel);
    mainLayout->addLayout(scopeLayout);
    mainLayout->addWidget(contentTable);
    mainLayout->addWidget(operatingBar);
    this->setMainLayout(mainLayout);

}

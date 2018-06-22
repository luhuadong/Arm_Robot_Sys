#include "header/quicksetbar.h"
#include <QPalette>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>

QuickSetBar::QuickSetBar(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(96, UI_SCREEN_HEIGHT - 108);
    setObjectName("mainScreenQuickSetBar");

    QFrame *w = new QFrame;
//    w->setStyleSheet("background-color: FloralWhite");

//    QLabel *label = new QLabel(tr("hello"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(w);
//    mainLayout->addWidget(label);
    setLayout(mainLayout);
}

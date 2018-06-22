#include "header/CustomWidget/operatingbar.h"
#include <QHBoxLayout>

OperatingBar::OperatingBar(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(UI_SCREEN_WIDTH, 54);

    mainWidget = new QWidget;
    mainWidget->setObjectName("operatingBar");

    buttonGroup = new QButtonGroup;
    createButtons();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(mainWidget);
    setLayout(mainLayout);
}

void OperatingBar::createButtons()
{
    int btnWidth = 160;


    // 为了界面简洁和操作方便，规定操作栏最多五个按钮，可以将其中一个按钮设为“更多”来包含额外的操作。
    for(int i=0; i<5; i++)
    {
        QPushButton *btn = new QPushButton(mainWidget);
        btn->setFixedSize(btnWidth, this->height());
        btn->setObjectName("operatingBarBtn");
        btn->setGeometry(btnWidth * i, 0, btn->width(), btn->height());
        buttonGroup->addButton(btn, i+1);
    }


}

QAbstractButton *OperatingBar::firstButton() const
{
    return buttonGroup->button(1);
}

QAbstractButton *OperatingBar::secondButton() const
{
    return buttonGroup->button(2);
}

QAbstractButton *OperatingBar::thirdButton() const
{
    return buttonGroup->button(3);
}

QAbstractButton *OperatingBar::fourthButton() const
{
    return buttonGroup->button(4);
}

QAbstractButton *OperatingBar::fifthButton() const
{
    return buttonGroup->button(5);
}

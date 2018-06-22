#include "virtualbutton.h"

VirtualButton::VirtualButton(const QString & text, QWidget *parent) :
    QPushButton(parent)
{
    this->setObjectName("virtualBtn");
    this->setText(text);
    this->setFixedSize(80, 30);
    this->setCheckable(true);
}

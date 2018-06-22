#include "panelbutton.h"
#include <QMouseEvent>
#include <QCoreApplication>

#include "GRS_Common.h"

PanelButton::PanelButton(const QString & text, QWidget *parent) :
    QPushButton(parent)
{
    this->setObjectName("panelBtn");
    this->setText(text);
    this->setFixedSize(40, 40);
    parentWindow = parent;
    mousePressedFlag = false;
}

void PanelButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        mousePressedFlag = true;
    }
    else if(e->button() == Qt::RightButton)
    {
//        QEvent event = QEvent(myCustomEvent);
        QEvent myEvent(myCustomEvent);
        QWidget *receiver = this->parentWidget();
        if(receiver == 0)
            return;
        receiver = parentWindow;
        QCoreApplication::sendEvent(receiver, &myEvent);
        QCoreApplication::sendEvent(qApp, &myEvent);
        qApp->children();
    }
    QPushButton::mousePressEvent(e);
}

void PanelButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(mousePressedFlag && this->rect().contains(e->pos()))
    {
        mousePressedFlag = false;

        QEvent myEvent = QEvent(myCustomEvent);
        QCoreApplication::sendEvent(this->parentWidget(), &myEvent);
    }
    QPushButton::mouseReleaseEvent(e);
}

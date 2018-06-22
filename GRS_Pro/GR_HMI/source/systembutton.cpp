#include "systembutton.h"

SystemButton::SystemButton(QWidget *parent) :
    QPushButton(parent)
{
    status = NORMAL;
    mouse_press = false;
}

void SystemButton::setPicName(QString pic_name)
{
    this->pic_name = pic_name;
    setFixedSize(QPixmap(pic_name).size());
}

void SystemButton::enterEvent(QEvent *)
{
    status = ENTER;
    update();
}

void SystemButton::mousePressEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
        status = PRESS;
        update();
    }
}

void SystemButton::mouseReleaseEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if(mouse_press  && this->rect().contains(event->pos()))
    {
        mouse_press = false;
        status = ENTER;
        update();
        emit clicked();
    }
}

void SystemButton::leaveEvent(QEvent *)
{
    status = NORMAL;
    update();
}

void SystemButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap;
    switch(status)
    {
    case NORMAL:
        {
            pixmap.load(pic_name);
            break;
        }
    case ENTER:
        {
            pixmap.load(pic_name + QString("_hover"));
            break;
        }
    case PRESS:
        {
            pixmap.load(pic_name + QString("_pressed"));
            break;
        }
    case NOSTATUS:
        {
            pixmap.load(pic_name);
            break;
        }
    default:
        pixmap.load(pic_name);
    }

    painter.drawPixmap(rect(), pixmap);
}

#ifndef PANELBUTTON_H
#define PANELBUTTON_H

#include <QPushButton>

class PanelButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PanelButton(const QString & text, QWidget *parent = 0);

protected:
    // 原来的想法是用事件过滤和重写事件的方法，模拟实际按钮，这样看起来对以后改成嵌入式版本更适合。
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

signals:

public slots:

private:
    bool mousePressedFlag;
    QWidget *parentWindow;

};

#endif // PANELBUTTON_H

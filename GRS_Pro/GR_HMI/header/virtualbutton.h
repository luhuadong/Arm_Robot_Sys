#ifndef VIRTUALBUTTON_H
#define VIRTUALBUTTON_H

#include <QPushButton>

class VirtualButton : public QPushButton
{
    Q_OBJECT
public:
    explicit VirtualButton(const QString & text, QWidget *parent = 0);

signals:

public slots:

};

#endif // VIRTUALBUTTON_H

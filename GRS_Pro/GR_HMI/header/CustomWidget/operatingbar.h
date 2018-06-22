#ifndef OPERATINGBAR_H
#define OPERATINGBAR_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>

#include "GRS_Common.h"

class OperatingBar : public QWidget
{
    Q_OBJECT
public:
    explicit OperatingBar(QWidget *parent = 0);

    QButtonGroup *buttonGroup;

    QAbstractButton *firstButton() const;
    QAbstractButton *secondButton() const;
    QAbstractButton *thirdButton() const;
    QAbstractButton *fourthButton() const;
    QAbstractButton *fifthButton() const;

signals:

public slots:

private:
    void createButtons();
    QWidget *mainWidget;

};

#endif // OPERATINGBAR_H

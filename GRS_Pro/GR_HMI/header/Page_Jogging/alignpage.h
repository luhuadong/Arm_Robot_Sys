#ifndef ALIGNPAGE_H
#define ALIGNPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

#include "GRS_Common.h"
#include "CustomWidget/simplepage.h"
#include "CustomWidget/operatingbar.h"

class AlignPage : public SimplePage
{
    Q_OBJECT
public:
    explicit AlignPage(QWidget *parent = 0);

    OperatingBar *operatingBar;

signals:

public slots:
    void closeBtnResponded();

private:
    void createPage();

    QWidget *tipsWidget;
    QLabel *currentKeyLabel;
    QLabel *currentValLabel;
    QLabel *tipsLabel;

    QWidget *contentWidget;
    QLabel *step1Label;
    QLabel *coordLabel;
    QComboBox *coordComboBox;
    QLabel *step2Label;
    QPushButton *startBtn;
    QLabel *step3Label;

};

#endif // ALIGNPAGE_H

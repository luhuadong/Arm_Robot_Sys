#ifndef PROGRAMDATAPAGE_H
#define PROGRAMDATAPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

#include "GRS_Common.h"
#include "CustomWidget/simplepage.h"
#include "CustomWidget/operatingbar.h"

class ProgramDataPage : public SimplePage
{
    Q_OBJECT
public:
    explicit ProgramDataPage(QWidget *parent = 0);

    OperatingBar *operatingBar;

signals:

public slots:

private:
    void createPage();

//    QWidget *tipsWidget;
    QLabel *tipsLabel;

//    QWidget *scopeWidget;
    QLabel *scopeKeyLabel;
    QLabel *scopeValLabel;
    QPushButton *scopeBtn;

//    QWidget *contentWidget;
    QTableWidget *contentTable;

};

#endif // PROGRAMDATAPAGE_H

#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>

#include "GRS_Common.h"

class MainMenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenuWidget(QWidget *parent = 0);

    QTableWidget *menuTable;

signals:

public slots:
    void menuTableUpdate();

private slots:
//    void dontSelectMenuItem(QTableWidgetItem *);

private:
    void createTableItem();


    QStringList menuItemNameList;

};

#endif // MAINMENUWIDGET_H

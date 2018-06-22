#ifndef TABLESETTINGPAGE_H
#define TABLESETTINGPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>

#include "GRS_Common.h"
#include "GRS_SharedMemory.h"
#include "CustomWidget/simplepage.h"
#include "CustomWidget/operatingbar.h"
#include "decode_datatype.h"

class TableSettingPage : public SimplePage
{
    Q_OBJECT
public:
    explicit TableSettingPage(Jogging::MANUALSUBWIDGET flag, QWidget *parent = 0);

    OperatingBar *operatingBar;

    ProgramData::proData thisData;  // 用于保存本页面内的操作，可作为参数传递到子界面
    decode_typestruct dataType;

signals:
    void okBtnClicked();

public slots:
    void tableDataUpdate();

private slots:
    void okBtnResponded();
    void cancelBtnResponded();
    void tabViewClicked(QModelIndex modelIndex);

private:
    void createPage();
    void updateThisData();

    Jogging::MANUALSUBWIDGET wFlag;

    QWidget *tipsWidget;
    QLabel *currentKeyLabel;
    QLabel *currentValLabel;
    QLabel *tipsLabel;

    QWidget *contentWidget;
    QTableView *tabView;
    QStandardItemModel *tabModel;
    QLabel *itemCntLabel;
};

#endif // TABLESETTINGPAGE_H

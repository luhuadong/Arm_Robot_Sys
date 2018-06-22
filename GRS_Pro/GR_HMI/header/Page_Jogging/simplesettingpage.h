#ifndef SIMPLESETTINGPAGE_H
#define SIMPLESETTINGPAGE_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QButtonGroup>

#include "GRS_Common.h"
#include "GRS_SharedMemory.h"

#include "CustomWidget/simplepage.h"
#include "CustomWidget/operatingbar.h"

class SimpleSettingPage : public SimplePage
{
    Q_OBJECT
public:
    explicit SimpleSettingPage(Jogging::MANUALSUBWIDGET flag, QWidget *parent = 0);

    OperatingBar *operatingBar;

signals:
    void okBtnClicked();

public slots:
    void contentWidgetUpdate(int);  // 处理该界面的主项目选址区事务，更新显示

private slots:
    void okBtnResponded();
    void cancelBtnResponded();

private:
    void createPage();
    void initMotionMode();
    void initCoordSystem();
    void initIncrementalMode();

    Jogging::MANUALSUBWIDGET wFlag;

    QWidget *tipsWidget;
    QLabel *currentKeyLabel;
    QLabel *currentValLabel;
    QLabel *tipsLabel;

    QWidget *contentWidget;
    QButtonGroup *btnGroup;
    QSize iconSize;
    QStringList iconStrList, textStrList;

};

#endif // SIMPLESETTINGPAGE_H

#ifndef GOTOPAGE_H
#define GOTOPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>

#include "GRS_Common.h"
#include "GRS_SharedMemory.h"
#include "CustomWidget/simplepage.h"
#include "CustomWidget/operatingbar.h"

class GoToPage : public SimplePage
{
    Q_OBJECT
public:
    explicit GoToPage(QWidget *parent = 0);

    OperatingBar *operatingBar;

signals:

public slots:
    void filterBtnResponded();
    void closeBtnResponded();

private slots:
    void listWidgetClicked();
    void goToPosBtnPressed();
    void goToPosBtnReleased();

private:
    void createPage();
    void listDataUpdate();

    QWidget *tipsWidget;
    QLabel *mechanicalUnitKeyLabel;   // 机械单元
    QLabel *mechanicalUnitValLabel;
    QLabel *activeToolKeyLabel;       // 活动工具
    QLabel *activeToolValLabel;
    QLabel *activeWobjKeyLabel;       // 活动工件
    QLabel *activeWobjValLabel;

    QWidget *contentWidget;
    QListWidget *robtargetListWidget;   // 显示当前任务中的机器人示教点列表
    QLabel *activeFilterKeyLabel; // 活动过滤器： XXX
    QLabel *activeFilterValLabel;
    // 按住“启动装置”(1.Press and hold Enabling Device.)
    QLabel *step1Label;
    // 按住“转到”按钮，转到位置Px(2.Press and hold Go To button to go to position Px)
    QLabel *step2Label;
    QPushButton *goToPosBtn;    // “转到”按钮

    char mode;  // 记录进行“转到”操作前系统的模式
    ITP_RobTarget tarRob;   // 目标点
    QMap<QString, ITP_RobTarget> map;

};

#endif // GOTOPAGE_H

#ifndef JOGGINGPAGE_H
#define JOGGINGPAGE_H

#include <QWidget>
#include <QGroupBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLabel>
#include <QPushButton>
#include <QList>
#include <QStringList>

#include "GRS_Common.h"
#include "GRS_SharedMemory.h"
#include "CustomWidget/simplepage.h"
#include "CustomWidget/operatingbar.h"
#include "CustomThread/updateposedatathread.h"
#include "Page_Jogging/simplesettingpage.h"
#include "Page_Jogging/tablesettingpage.h"
#include "Page_Jogging/alignpage.h"
#include "Page_Jogging/gotopage.h"

class JoggingPage : public SimplePage
{
    Q_OBJECT
public:
    explicit JoggingPage(QWidget *parent = 0);

    OperatingBar *operatingBar;

public slots:
    void settingBoxUpdate();
    void updatePoseData(QString *poseData); // 接收线程updatePoseDataThread的信号来更新数据
    void displayBoxUpdate();    // 主要用于属性设置子界面设定参数后的更新  20160122

private slots:
    void settingTableClicked(const int, const int);
    void positionShowInBtnClicked();
    void orientationFormatBtnClicked();
    void angleUnitBtnClicked();
//    void subPageIsOk();
    void alignBtnClicked();
    void goToBtnClicked();

private:
    void joggingParInit();
    void centralWidgetInit();
    void createSettingTableItem();
    void createDisplayTableItem();

    QGroupBox *settingBox;
    QTableWidget *settingTable;

    QGroupBox *displayBox;
    QTableWidget *displayTable;
    QLabel *displayLabel;
    QPushButton *positionShowInBtn;
    QPushButton *orientationFormatBtn;
    QPushButton *angleUnitBtn;

    QStringList *poseKeyItemStrList;    // 指向当前位置表第一列（key）的字符串链表
    QStringList *poseUnitItemStrList;   // 指向当前位置表第三列（unit）的字符串链表
    QList<QTableWidgetItem *> poseTabKeyItemList;   // 第一列
    QList<QTableWidgetItem *> poseTabValItemList;   // 第二列
    QList<QTableWidgetItem *> poseTabUnitItemList;  // 第三列

    UpdatePoseDataThread *updatePoseDataThread; // 实时更新机器人位姿数据的线程

    // 子界面
    SimpleSettingPage *motionModePage;
    SimpleSettingPage *coordSystemPage;
    SimpleSettingPage *incrementPage;
    TableSettingPage *toolPage;
    TableSettingPage *wobjPage;
    AlignPage *alignPage;
    GoToPage *goToPage;
};

#endif // JOGGINGPAGE_H

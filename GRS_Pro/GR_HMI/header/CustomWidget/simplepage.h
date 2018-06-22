#ifndef SIMPLEPAGE_H
#define SIMPLEPAGE_H

#include <QWidget>
#include <QLabel>

#include "GRS_Common.h"

class SimplePage : public QWidget
{
    Q_OBJECT
public:
    explicit SimplePage(QWidget *parent = 0);
    explicit SimplePage(const QString &text, QWidget *parent = 0);
    explicit SimplePage(const QIcon &icon, const QString &text, QWidget *parent = 0);

    void setIcon(const QIcon & icon);
    void setText(const QString & text);
    void setMainLayout(QLayout * layout);   // set layout for mainWidget

    int navBarHeight() const;
    int mainWidgetHeight() const;

signals:

public slots:

private:
    void createWidget();
    void simplePageInit();

    QWidget *navBar;
    QLabel *navIcon;
    QLabel *navText;

    QWidget *mainWidget;


};

#endif // SIMPLEPAGE_H

#include "header/CustomWidget/simplepage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QSize>

SimplePage::SimplePage(QWidget *parent) :
    QWidget(parent)
{
    simplePageInit();
    setText(tr("Simple Page"));
}

SimplePage::SimplePage(const QString &text, QWidget *parent) :
    QWidget(parent)
{
    simplePageInit();
    setText(text);
}

SimplePage::SimplePage(const QIcon &icon, const QString &text, QWidget *parent) :
    QWidget(parent)
{
    simplePageInit();
    setIcon(icon);
    setText(text);
}

void SimplePage::setIcon(const QIcon &icon)
{
    navIcon->setPixmap(icon.pixmap(QSize(24, 24)));
}

void SimplePage::setText(const QString &text)
{
    navText->setText(text);
}

void SimplePage::setMainLayout(QLayout *layout)
{
    mainWidget->setLayout(layout);
}

int SimplePage::navBarHeight() const
{
    return navBar->height();
}

int SimplePage::mainWidgetHeight() const
{
    return mainWidget->height();
}

void SimplePage::simplePageInit()
{
    setFixedSize(UI_SCREEN_WIDTH, UI_SCREEN_HEIGHT - 54*2);
//    setFixedSize(parent->width(), parent->height());
//    setObjectName("SimplePage");

    navBar = new QWidget;
    navBar->setObjectName("SimplePageNavBar");
    navBar->setFixedHeight(32);
    navIcon = new QLabel;
    navIcon->setFixedSize(32, 32);
    navText = new QLabel;

    QHBoxLayout *navBarLayout = new QHBoxLayout;
    navBarLayout->setContentsMargins(5, 0, 0, 0);
    navBarLayout->setSpacing(0);
    navBarLayout->addWidget(navIcon, Qt::AlignLeft | Qt::AlignVCenter);
    navBarLayout->addWidget(navText, Qt::AlignLeft | Qt::AlignVCenter);
    navBar->setLayout(navBarLayout);

    mainWidget = new QWidget;
    mainWidget->setFixedSize(UI_SCREEN_WIDTH, this->height() - navBar->height());
    mainWidget->setObjectName("SimplePageMainWidget");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(navBar);
    mainLayout->addWidget(mainWidget);
    this->setLayout(mainLayout);
}

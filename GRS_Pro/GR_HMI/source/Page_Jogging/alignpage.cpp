#include "header/Page_Jogging/alignpage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

AlignPage::AlignPage(QWidget *parent) :
    SimplePage(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    QPixmap pixmap(":/images/menu/menu_Jogging.png");
    setIcon(QIcon(pixmap));
    setText(tr("Jogging - Align"));

    createPage();

    connect(operatingBar->fifthButton(), SIGNAL(clicked()), this, SLOT(closeBtnResponded()));
}

void AlignPage::createPage()
{
    tipsWidget = new QWidget;
    tipsWidget->setObjectName("tipsWidgetOfSimpleSettingPage");
    tipsWidget->setFixedHeight(132);
    currentKeyLabel = new QLabel(tipsWidget);
    currentKeyLabel->setProperty("h", 0);
    currentValLabel = new QLabel(tipsWidget);
    currentValLabel->setProperty("h", 0);
    tipsLabel = new QLabel(tipsWidget);
    tipsLabel->setProperty("h", 1);

    currentKeyLabel->setText(tr("Current tool:\t\t"));
    currentValLabel->setText(QString(QLatin1String(G_CurToolData.dataProperty.typeName)));
    currentKeyLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    currentValLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    tipsLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    QHBoxLayout *currentKeyValLayout = new QHBoxLayout;
    currentKeyValLayout->setMargin(0);
    currentKeyValLayout->setSpacing(0);
    currentKeyValLayout->addWidget(currentKeyLabel);
    currentKeyValLayout->addWidget(currentValLabel);
    currentKeyValLayout->addStretch();
    QVBoxLayout *tipsLayout = new QVBoxLayout;
    tipsLayout->setMargin(10);
    tipsLayout->setSpacing(20);
    tipsLayout->addLayout(currentKeyValLayout);
    tipsLayout->addWidget(tipsLabel);
    tipsWidget->setLayout(tipsLayout);

    contentWidget = new QWidget;
    contentWidget->setFixedSize(UI_SCREEN_WIDTH, 274);

    step1Label = new QLabel(tr("1. Select coordinate system to align the currently selected tool to:"));
    coordLabel = new QLabel(tr("Coord:"));
    coordComboBox = new QComboBox;
    coordComboBox->setFixedSize(300, 30);
    coordComboBox->addItem(tr("World"));
    coordComboBox->addItem(tr("Base"));
    coordComboBox->addItem(tr("Work object"));
    step2Label = new QLabel(tr("2. Press the enabling device the tap and hold 'Start Align'."));
    startBtn = new QPushButton(tr("Start Align"));
    startBtn->setProperty("type", "normal");
    startBtn->setFixedSize(300, 30);
    step3Label = new QLabel(tr("3. When ready tap 'Close'."));

    QGridLayout *contentLayout = new QGridLayout;
    contentLayout->setContentsMargins(60, 10, 60, 10);
    contentLayout->setSpacing(20);
    contentLayout->addWidget(step1Label, 0, 0, 1, 2, Qt::AlignLeft | Qt::AlignVCenter);
    contentLayout->addWidget(coordLabel, 1, 0, 1, 1, Qt::AlignCenter);
    contentLayout->addWidget(coordComboBox, 1, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    contentLayout->addWidget(step2Label, 2, 0, 1, 2, Qt::AlignLeft | Qt::AlignVCenter);
    contentLayout->addWidget(startBtn, 3, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    contentLayout->addWidget(step3Label, 4, 0, 1, 2, Qt::AlignLeft | Qt::AlignVCenter);
    contentWidget->setLayout(contentLayout);


    // bottom
    operatingBar = new OperatingBar;
    operatingBar->firstButton()->hide();
    operatingBar->secondButton()->hide();
    operatingBar->thirdButton()->hide();
    operatingBar->fourthButton()->hide();
    operatingBar->fifthButton()->setText(tr("Close"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(tipsWidget);
    mainLayout->addWidget(contentWidget);
    mainLayout->addWidget(operatingBar);
    this->setMainLayout(mainLayout);
}

void AlignPage::closeBtnResponded()
{
    this->close();
}

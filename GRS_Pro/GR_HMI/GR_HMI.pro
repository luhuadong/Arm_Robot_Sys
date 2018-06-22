#-------------------------------------------------
#
# Project created by QtCreator 2016-01-15T14:05:14
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GR_HMI
TEMPLATE = app

ROOTDIR = $${PWD}/../

INCLUDEPATH += \
    $${ROOTDIR}/GRS_Core/header \
    $${ROOTDIR}/GR_HMI/header \
    $${ROOTDIR}/robDecodePrg/header/decode_core \
    $${ROOTDIR}/robDecodePrg/header/decode_prg

LIBS += -L $${ROOTDIR}/GR_HMI/lib -lrobDecodePrg -lGRS_Core

OBJECTS_DIR = obj/

DESTDIR = bin/

SOURCES += \
    source/main.cpp \
    source/hmiapp.cpp \
    source/mainwidget.cpp \
    source/panelbutton.cpp \
    source/panelwidget.cpp \
    source/screenwidget.cpp \
    source/systembutton.cpp \
    source/virtualbutton.cpp \
    source/quicksetbar.cpp \
    source/mainmenuwidget.cpp \
    source/CustomWidget/simplepage.cpp \
    source/Page_Jogging/joggingpage.cpp \
    source/CustomWidget/operatingbar.cpp \
    source/CustomThread/updateposedatathread.cpp \
    source/Page_Jogging/simplesettingpage.cpp \
    source/Page_Jogging/tablesettingpage.cpp \
    source/Page_Jogging/alignpage.cpp \
    source/Page_Jogging/gotopage.cpp \
    source/Page_ProgramData/programdatapage.cpp

HEADERS  += \
    header/hmiapp.h \
    header/mainwidget.h \
    header/panelbutton.h \
    header/panelwidget.h \
    header/screenwidget.h \
    header/systembutton.h \
    header/virtualbutton.h \
    header/quicksetbar.h \
    header/mainmenuwidget.h \
    header/CustomWidget/simplepage.h \
    header/Page_Jogging/joggingpage.h \
    header/CustomWidget/operatingbar.h \
    header/CustomThread/updateposedatathread.h \
    header/Page_Jogging/simplesettingpage.h \
    header/Page_Jogging/tablesettingpage.h \
    header/Page_Jogging/alignpage.h \
    header/Page_Jogging/gotopage.h \
    header/Page_ProgramData/programdatapage.h

RESOURCES += \
    resource/styleSheets.qrc \
    resource/images.qrc

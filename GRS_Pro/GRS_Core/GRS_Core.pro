#-------------------------------------------------
#
# Project created by QtCreator 2016-01-15T13:38:55
#
#-------------------------------------------------

QT       += core gui
QT += widgets
QT += network

TARGET = GRS_Core
TEMPLATE = lib
CONFIG += staticlib

ROOTDIR = $${PWD}/../

DESTDIR = $${ROOTDIR}/GR_HMI/lib/

INCLUDEPATH += \
    $${ROOTDIR}/GRS_Core/header \
    $${ROOTDIR}/robDecodePrg/header/decode_core \
    $${ROOTDIR}/robDecodePrg/header/decode_prg

SOURCES += \
    source/ProData_CoorOp.c \
    source/ProData_Kinematic.c \
    source/ProData_Quaternion.c \
    source/GRS_Common.cpp \
    source/GRS_SharedMemory.cpp

HEADERS += \
    header/ProData_CoorOp.h \
    header/ProData_Kinematic.h \
    header/ProData_Quaternion.h \
    header/GRS_Common.h \
    header/GRS_SharedMemory.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

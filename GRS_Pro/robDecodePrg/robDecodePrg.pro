#-------------------------------------------------
#
# Project created by QtCreator 2016-01-15T14:22:22
#
#-------------------------------------------------

QT       += core gui xml

TARGET = robDecodePrg
TEMPLATE = lib
CONFIG += staticlib

DEFINES += ROBDECODEPRG_LIBRARY

ROOTDIR = $${PWD}/../

DESTDIR = $${ROOTDIR}/GR_HMI/lib/

INCLUDEPATH += \
    $${ROOTDIR}/GRS_Core/header \
    header/decode_core \
    header/decode_prg \

SOURCES += \
    source/decode_core/decode_instructions.c \
    source/decode_core/decode_lexical_analysis.c \
    source/decode_core/decode_parse_file.c \
    source/decode_core/decode_proc_task.c \
    source/decode_core/decode_typestructs.c \
    source/decode_prg/decode_public.cpp \
    source/decode_prg/robotdecodepublic.cpp \
    source/decode_prg/robotpgfxmlparser.cpp \
    source/decode_prg/robotprgfilestruct.cpp \
    source/decode_prg/robotvariablestruct.cpp \
    source/decode_prg/decodethread.cpp \
    source/decode_prg/sharemempublic.cpp \
    source/decode_core/decode_shmem.c

HEADERS += \
    header/decode_core/basic_data_type.h \
    header/decode_core/decode_datatype.h \
    header/decode_core/decode_instructions.h \
    header/decode_core/decode_lexical_analysis.h \
    header/decode_core/decode_parse_file.h \
    header/decode_core/decode_proc_task.h \
    header/decode_core/decode_shmem.h \
    header/decode_core/decode_typestructs.h \
    header/decode_core/ITP_Interface.h \
    header/decode_prg/decode_public.h \
    header/decode_prg/robotdecodepublic.h \
    header/decode_prg/robotpgfxmlparser.h \
    header/decode_prg/robotprgfilestruct.h \
    header/decode_prg/robotvariablestruct.h \
    header/decode_prg/decodethread.h \
    header/decode_core/sharemempublic.h


#robdecodeprg.h\
#        robdecodeprg_global.h


win32{

}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#-------------------------------------------------
#
# Project created by QtCreator 2017-09-17T13:05:22
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Updater
TEMPLATE = app


SOURCES += main.cpp\
        dlgmain.cpp \
    ../Base/preferences.cpp \
    ../Base/database.cpp \
    ../Base/defines.cpp

HEADERS  += dlgmain.h \
    ../Base/preferences.h \
    ../Base/database.h \
    ../Base/defines.h \
    rc.h \
    res.rc

FORMS    += dlgmain.ui

INCLUDEPATH += $$PWD/../Base

DISTFILES +=

RC_FILE = res.rc

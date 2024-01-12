#-------------------------------------------------
#
# Project created by QtCreator 2017-07-31T21:22:54
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app

RC_FILE = res.rc

SOURCES += main.cpp\
        dlgmain.cpp \
    ../Base/defines.cpp \
    ../Base/command.cpp \
    ../Base/databaseresult.cpp \
    ../Base/preferences.cpp \
    ../Base/database.cpp

HEADERS  += dlgmain.h \
    ../Base/defines.h \
    ../Base/command.h \
    ../Base/databaseresult.h \
    ../Base/preferences.h \
    ../Base/database.h

FORMS    += dlgmain.ui

RESOURCES += \
    res.qrc

INCLUDEPATH += $$PWD/../Base

DEFINES += _DBDRIVER_=\\\"QMARIADB\\\"

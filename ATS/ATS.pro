#-------------------------------------------------
#
# Project created by QtCreator 2017-09-07T09:35:44
#
#-------------------------------------------------

QT       += core gui serialport serialbus sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ATS
TEMPLATE = app
RC_FILE = res.rc

INCLUDEPATH += $$PWD/../Base
INCLUDEPATH += $$PWD/../Controls

SOURCES += main.cpp\
        dlgmain.cpp \
    ../Base/utils.cpp \
    ../Base/defines.cpp \
    ../Base/database.cpp \
    ../Base/preferences.cpp \
    ../Base/baseuid.cpp

HEADERS  += dlgmain.h \
    ../Base/utils.h \
    ../Base/defines.h \
    ../Base/database.h \
    ../Base/preferences.h \
    rc.h \
    res.rc \
    ../Base/baseuid.h

FORMS    += dlgmain.ui

LIBS += -lVersion

RESOURCES += \
    images.qrc

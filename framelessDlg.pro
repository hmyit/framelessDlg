#-------------------------------------------------
#
# Project created by QtCreator 2017-03-17T21:52:14
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = framelessDlg
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    NcFramelessHelper.cpp

HEADERS  += dialog.h \
    NcFramelessHelper.h

RESOURCES += \
    pic.qrc

CONFIG += c++11

#-------------------------------------------------
#
# Project created by QtCreator 2016-02-02T11:06:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uitemplate
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    windowtitlebar.cpp \
    windowbutton.cpp

HEADERS  += dialog.h \
    windowtitlebar.h \
    windowbutton.h \
    myprop.h

FORMS    += dialog.ui

CONFIG +=c++11

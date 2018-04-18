#-------------------------------------------------
#
# Project created by QtCreator 2018-04-18T09:13:35
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app-qrcode
TEMPLATE = app

RC_FILE += qrc/appsource.rc

RESOURCES += \
    qrc/appsource.qrc

HEADERS += \
    app/appqrcode.h

SOURCES += \
    app/appqrcode.cpp \
    app/main.cpp



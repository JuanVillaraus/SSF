#-------------------------------------------------
#
# Project created by QtCreator 2016-09-29T11:43:13
#
#-------------------------------------------------

QT       += core gui sql
QT       += network
QT       += serialport
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SSF
TEMPLATE = app


SOURCES += main.cpp\
        siviso.cpp \
    blanco.cpp \
    ppi.cpp \
    signal.cpp \
    dbasepostgresql.cpp \
    wconfig.cpp

HEADERS  += siviso.h \
    blanco.h \
    ppi.h \
    signal.h \
    dbasepostgresql.h \
    wconfig.h

FORMS    += siviso.ui \
    wconfig.ui

RESOURCES += \
    resource.qrc

DISTFILES +=

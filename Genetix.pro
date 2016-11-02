#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T00:08:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Genetix
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    partita.cpp \
    tavolo.cpp \
    umano.cpp \
    ai.cpp

HEADERS  += mainwindow.h \
    giocatore.h \
    partita.h \
    tavolo.h \
    umano.h \
    ai.h

FORMS    += mainwindow.ui

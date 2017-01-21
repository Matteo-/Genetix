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
    umano.cpp \
    ai.cpp \
    game.cpp \
    table.cpp \
    engine.cpp \
    brain.cpp

HEADERS  += mainwindow.h \
    umano.h \
    ai.h \
    game.h \
    player.h \
    table.h \
    engine.h \
    brain.h

FORMS    += mainwindow.ui

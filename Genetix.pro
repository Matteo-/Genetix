#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T00:08:36
#
#-------------------------------------------------

QT       += core gui
QT       += network

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
    brain.cpp \
    sleeper.cpp \
    tester.cpp \
    style.cpp \
    tree.cpp \
    random.cpp \
    worker.cpp \
    distributednetwork.cpp \
    client.cpp \
    clientwindow.cpp \
    initwindow.cpp \
    qcustomplot.cpp \
    genetixwindow.cpp

HEADERS  += mainwindow.h \
    umano.h \
    ai.h \
    game.h \
    player.h \
    table.h \
    engine.h \
    brain.h \
    gameabstract.h \
    sleeper.h \
    tester.h \
    style.h \
    tree.h \
    random.h \
    worker.h \
    distributednetwork.h \
    client.h \
    clientwindow.h \
    initwindow.h \
    qcustomplot.h \
    genetixwindow.h

FORMS    += mainwindow.ui \
    initwindow.ui \
    genetixwindow.ui

DISTFILES +=

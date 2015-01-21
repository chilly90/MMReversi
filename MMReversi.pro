#-------------------------------------------------
#
# Project created by QtCreator 2014-12-18T01:06:44
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MMReversi
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    boardwidget.cpp \
    game.cpp \
    board.cpp \
    minimax.cpp \
    gamedialog.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    boardwidget.h \
    boardwidget.h \
    game.h \
    board.h \
    minimax.h \
    gamedialog.h \
    settings.h

FORMS    += mainwindow.ui \
    gamedialog.ui \
    settings.ui

RESOURCES += \
    resources.qrc

#-------------------------------------------------
#
# Project created by QtCreator 2015-07-08T16:32:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphShell

TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp

include(core/GraphShellCore.pri)
include(gui/GraphShellGui.pri)

HEADERS += \
    main.h

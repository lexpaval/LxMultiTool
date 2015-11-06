#-------------------------------------------------
#
# Project created by QtCreator 2015-10-26T22:59:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AndroidMultiTool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogrecovery.cpp \
    dialogsideload.cpp \
    dialogkernel.cpp \
    aboutdialog.cpp \
    dialoglogging.cpp \
    dialogstock.cpp \
    dialogscreenshot.cpp \
    dialogerase.cpp

HEADERS  += mainwindow.h \
    dialogrecovery.h \
    dialogsideload.h \
    dialogkernel.h \
    aboutdialog.h \
    dialoglogging.h \
    dialogstock.h \
    version.h \
    dialogscreenshot.h \
    dialogerase.h

FORMS    += mainwindow.ui \
    dialogrecovery.ui \
    dialogsideload.ui \
    dialogkernel.ui \
    aboutdialog.ui \
    dialoglogging.ui \
    dialogstock.ui \
    dialogscreenshot.ui \
    dialogerase.ui

RESOURCES += \
    icons.qrc

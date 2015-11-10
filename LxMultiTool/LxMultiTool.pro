#-------------------------------------------------
#
# Project created by QtCreator 2015-10-26T22:59:29
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "Lx Multi Tool"
TEMPLATE = app

#ifdef Q_OS_MACX
ICON = ../Icons/android.icns
#endif
#ifdef Q_OS_WIN
RC_ICONS = ../Icons/android.ico
#endif

SOURCES += main.cpp\
        mainwindow.cpp \
    dialogrecovery.cpp \
    dialogsideload.cpp \
    dialogkernel.cpp \
    aboutdialog.cpp \
    dialoglogging.cpp \
    dialogstock.cpp \
    dialogscreenshot.cpp \
    dialogerase.cpp \
    filedownloader.cpp

HEADERS  += mainwindow.h \
    dialogrecovery.h \
    dialogsideload.h \
    dialogkernel.h \
    aboutdialog.h \
    dialoglogging.h \
    dialogstock.h \
    version.h \
    dialogscreenshot.h \
    dialogerase.h \
    filedownloader.h

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

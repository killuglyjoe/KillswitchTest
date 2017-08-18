#-------------------------------------------------
#
# Project created by QtCreator 2017-08-18T19:45:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KillswitchTest
#RC_FILE = ico.rc
TEMPLATE = app

BUILDDIR = Compiled

DESTDIR += $$PWD/Distr

win32:!wince*: BUILDDIR = Compiled/Win32
win64:!wince*: BUILDDIR = Compiled/Win64

UI_DIR      = $$BUILDDIR/Ui
MOC_DIR     = $$BUILDDIR/MOC
OBJECTS_DIR = $$BUILDDIR/OBJ

RCC_DIR = $$PWD/Compiled

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Win manifest file add
#<requestedPrivileges>
#        <requestedExecutionLevel level='requireAdministrator' uiAccess='false'/>
#</requestedPrivileges>
win32 {
    CONFIG += embed_manifest_exe
    QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
#    QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:level=\'requireAdministrator\'
}

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui
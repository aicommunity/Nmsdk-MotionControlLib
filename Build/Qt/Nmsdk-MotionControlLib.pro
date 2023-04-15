#-------------------------------------------------
#
# Project created by QtCreator 2012-11-11T15:17:55
#
#-------------------------------------------------

QT       -= gui

TARGET = Nmsdk-MotionControlLib.qt
TEMPLATE = lib
CONFIG += staticlib

include($$PWD/../../../../Rdk/Build/Lib/Qt/RdkDefines.pri)

INCLUDEPATH += $$PWD/../../../../Rdk/Deploy/Include

VERSION = $$system(hg parents --template '{rev}')
DEFINES += RDK_LIB_CVS_REVISION=$$VERSION

unix {
    target.path = /usr/lib
    INSTALLS += target
    DESTDIR = $$PWD/../../../../Bin/Platform/Linux/Lib.Qt
}

windows {
    DESTDIR = $$PWD/../../../../Bin/Platform/Win/Lib.Qt
}

HEADERS += \
#    ../../Core/NEngineMotionControl.h \
    ../../Core/NMotionControlLibrary.h \
    ../../Core/NMotionElement.h \
#    ../../Core/NMultiPositionControl.h \
    ../../Core/NNewPositionControlElement.h \
    ../../Core/NPositionControlElement.h #\
#    ../../Core/NPCNElement.h

SOURCES += \
 #   ../../Core/NEngineMotionControl.cpp \
    ../../Core/NMotionControlLibrary.cpp \
    ../../Core/NMotionElement.cpp \
#    ../../Core/NMultiPositionControl.cpp \
    ../../Core/NNewPositionControlElement.cpp \
    ../../Core/NPositionControlElement.cpp \
#    ../../Core/NPCNElement.cpp

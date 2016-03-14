#-------------------------------------------------
#
# Project created by QtCreator 2012-11-11T15:17:55
#
#-------------------------------------------------

QT       -= gui

TARGET = Nmsdk-MotionControlLib.qt
TEMPLATE = lib
CONFIG += staticlib

DEFINES += LIBRDK_LIBRARY_EXPORT
DEFINES += RDK_UNICODE_RUN
DEFINES += RDK_QT
INCLUDEPATH += $$PWD/../../../../Rdk/Deploy/Include

unix {
    target.path = /usr/lib
    INSTALLS += target
    DESTDIR = $$PWD/../../../../Bin/Platform/Unix/Lib.Qt
}

windows {
    DESTDIR = $$PWD/../../../../Bin/Platform/Win/Lib.Qt
}

HEADERS += \
    ../../Core/NEngineMotionControl.h \
    ../../Core/NMotionControlLibrary.h \
    ../../Core/NMotionElement.h \
    ../../Core/NMultiPositionControl.h \
    ../../Core/NNewPositionControlElement.h \
    ../../Core/NPositionControlElement.h

SOURCES += \
    ../../Core/NEngineMotionControl.cpp \
    ../../Core/NMotionControlLibrary.cpp \
    ../../Core/NMotionElement.cpp \
    ../../Core/NMultiPositionControl.cpp \
    ../../Core/NNewPositionControlElement.cpp \
    ../../Core/NPositionControlElement.cpp

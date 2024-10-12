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

VERSION = 0 # $$system(hg parents --template '{rev}')
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
    ../../Core/NActuatorSignals.h \
    ../../Core/NAstaticGyro.h \
    ../../Core/NControlObjectSource.h \
    ../../Core/NCounterNeuron.h \
    ../../Core/NDCEngine.h \
    ../../Core/NEngineMotionControl.h \
    ../../Core/NEyeRetina.h \
    ../../Core/NEyeRetinaCore/DiffObjectSelector.h \
    ../../Core/NEyeRetinaCore/NEyeRetinaBWCore.h \
    ../../Core/NEyeRetinaCore/NEyeRetinaMuscleCore.h \
    ../../Core/NFrequencyReceiver.h \
    ../../Core/NIntervalSeparator.h \
    ../../Core/NManipulator.h \
    ../../Core/NManipulatorAndGyro.h \
    ../../Core/NManipulatorInput.h \
    ../../Core/NManipulatorInputEmulator.h \
    ../../Core/NManipulatorSource.h \
    ../../Core/NManipulatorSourceEmulator.h \
    ../../Core/NMotionControlLibrary.h \
    ../../Core/NMotionElement.h \
    ../../Core/NMultiPositionControl.h \
    ../../Core/NNavMousePrimitive.h \
    ../../Core/NNewPositionControlElement.h \
    ../../Core/NObjInArea.h \
    ../../Core/NPendulumAndCart.h \
    ../../Core/NPositionControlElement.h \
    ../../Core/NPCNElement.h \
    ../../Core/NPulseReceiver.h \
    ../../Core/NSeqComparison.h \
    ../../Core/NSignalEstimation.h \
    ../../Core/NSignumSeparator.h \
    ../../Core/NSimpleStatistic.h \
    ../../Core/NSuppressionUnit.h

SOURCES += \
    ../../Core/NActuatorSignals.cpp \
    ../../Core/NAstaticGyro.cpp \
    ../../Core/NControlObjectSource.cpp \
    ../../Core/NCounterNeuron.cpp \
    ../../Core/NDCEngine.cpp \
    ../../Core/NEngineMotionControl.cpp \
    ../../Core/NEyeRetina.cpp \
    ../../Core/NEyeRetinaCore/DiffObjectSelector.cpp \
    ../../Core/NEyeRetinaCore/NEyeRetinaBWCore.cpp \
    ../../Core/NEyeRetinaCore/NEyeRetinaMuscleCore.cpp \
    ../../Core/NFrequencyReceiver.cpp \
    ../../Core/NIntervalSeparator.cpp \
    ../../Core/NManipulator.cpp \
    ../../Core/NManipulatorAndGyro.cpp \
    ../../Core/NManipulatorInput.cpp \
    ../../Core/NManipulatorInputEmulator.cpp \
    ../../Core/NManipulatorSource.cpp \
    ../../Core/NManipulatorSourceEmulator.cpp \
    ../../Core/NMotionControlLibrary.cpp \
    ../../Core/NMotionElement.cpp \
    ../../Core/NMultiPositionControl.cpp \
    ../../Core/NNavMousePrimitive.cpp \
    ../../Core/NNewPositionControlElement.cpp \
    ../../Core/NObjInArea.cpp \
    ../../Core/NPendulumAndCart.cpp \
    ../../Core/NPositionControlElement.cpp \
    ../../Core/NPCNElement.cpp \
    ../../Core/NPulseReceiver.cpp \
    ../../Core/NSeqComparison.cpp \
    ../../Core/NSignalEstimation.cpp \
    ../../Core/NSignumSeparator.cpp \
    ../../Core/NSimpleStatistic.cpp \
    ../../Core/NSuppressionUnit.cpp

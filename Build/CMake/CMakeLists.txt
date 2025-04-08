cmake_minimum_required(VERSION 3.14)
project(Nmsdk-MotionControlLib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(
    ${CMAKE_SOURCE_DIR}/../../../../Rdk/Deploy/Include
    ${CMAKE_SOURCE_DIR}/../../Core
    ${CMAKE_SOURCE_DIR}/../../Core/NEyeRetinaCore
)

add_definitions(-DRDK_LIB_CVS_REVISION=0)

set(HEADERS
    ${CMAKE_SOURCE_DIR}/../../Core/NActuatorSignals.h
    ${CMAKE_SOURCE_DIR}/../../Core/NAstaticGyro.h
    ${CMAKE_SOURCE_DIR}/../../Core/NControlObjectSource.h
    ${CMAKE_SOURCE_DIR}/../../Core/NCounterNeuron.h
    ${CMAKE_SOURCE_DIR}/../../Core/NDCEngine.h
    ${CMAKE_SOURCE_DIR}/../../Core/NEngineMotionControl.h
    ${CMAKE_SOURCE_DIR}/../../Core/NEyeRetina.h
    ${CMAKE_SOURCE_DIR}/../../Core/NEyeRetinaCore/DiffObjectSelector.h
    ${CMAKE_SOURCE_DIR}/../../Core/NEyeRetinaCore/NEyeRetinaBWCore.h
    ${CMAKE_SOURCE_DIR}/../../Core/NEyeRetinaCore/NEyeRetinaMuscleCore.h
    ${CMAKE_SOURCE_DIR}/../../Core/NFrequencyReceiver.h
    ${CMAKE_SOURCE_DIR}/../../Core/NIntervalSeparator.h
    ${CMAKE_SOURCE_DIR}/../../Core/NManipulator.h
    ${CMAKE_SOURCE_DIR}/../../Core/NManipulatorAndGyro.h
    ${CMAKE_SOURCE_DIR}/../../Core/NManipulatorInput.h
    ${CMAKE_SOURCE_DIR}/../../Core/NManipulatorInputEmulator.h
    ${CMAKE_SOURCE_DIR}/../../Core/NManipulatorSource.h
    ${CMAKE_SOURCE_DIR}/../../Core/NManipulatorSourceEmulator.h
    ${CMAKE_SOURCE_DIR}/../../Core/NMazeMemory.h
    ${CMAKE_SOURCE_DIR}/../../Core/NMotionControlLibrary.h
    ${CMAKE_SOURCE_DIR}/../../Core/NMotionElement.h
    ${CMAKE_SOURCE_DIR}/../../Core/NMultiPositionControl.h
    ${CMAKE_SOURCE_DIR}/../../Core/NNavMousePrimitive.h
    ${CMAKE_SOURCE_DIR}/../../Core/NNewPositionControlElement.h
    ${CMAKE_SOURCE_DIR}/../../Core/NObjInArea.h
    ${CMAKE_SOURCE_DIR}/../../Core/NPendulumAndCart.h
    ${CMAKE_SOURCE_DIR}/../../Core/NPositionControlElement.h
    ${CMAKE_SOURCE_DIR}/../../Core/NPCNElement.h
    ${CMAKE_SOURCE_DIR}/../../Core/NPulseReceiver.h
    ${CMAKE_SOURCE_DIR}/../../Core/NSeqComparison.h
    ${CMAKE_SOURCE_DIR}/../../Core/NSignalEstimation.h
    ${CMAKE_SOURCE_DIR}/../../Core/NSignumSeparator.h
    ${CMAKE_SOURCE_DIR}/../../Core/NSimpleStatistic.h
    ${CMAKE_SOURCE_DIR}/../../Core/NSuppressionUnit.h
    ${CMAKE_SOURCE_DIR}/../../Core/NTrajectoryElement.h
)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/../../Core/NActuatorSignals.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NAstaticGyro.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NControlObjectSource.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NCounterNeuron.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NDCEngine.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NEngineMotionControl.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NEyeRetina.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NEyeRetinaCore/DiffObjectSelector.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NEyeRetinaCore/NEyeRetinaBWCore.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NEyeRetinaCore/NEyeRetinaMuscleCore.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NFrequencyReceiver.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NIntervalSeparator.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NManipulator.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NManipulatorAndGyro.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NManipulatorInput.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NManipulatorInputEmulator.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NManipulatorSource.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NManipulatorSourceEmulator.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NMazeMemory.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NMotionControlLibrary.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NMotionElement.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NMultiPositionControl.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NTrajectoryElement.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NNavMousePrimitive.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NNewPositionControlElement.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NObjInArea.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NPendulumAndCart.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NPositionControlElement.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NPCNElement.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NPulseReceiver.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NSeqComparison.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NSignalEstimation.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NSignumSeparator.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NSimpleStatistic.cpp
    ${CMAKE_SOURCE_DIR}/../../Core/NSuppressionUnit.cpp
)

add_library(Nmsdk-MotionControlLib STATIC)

if(UNIX)
    set_target_properties(Nmsdk-MotionControlLib PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../../../../Bin/Platform/Linux/Lib.Qt
    )
elseif(WIN32)
    set_target_properties(Nmsdk-MotionControlLib PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../../../../Bin/Platform/Win/Lib.Qt
    )
endif()

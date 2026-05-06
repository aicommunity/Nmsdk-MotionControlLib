cmake_minimum_required(VERSION 3.16)
project(Nmsdk_MotionControlLib LANGUAGES CXX)

option(NMSDK_MOTIONCONTROLLIB_BUILD_CORE_ONLY
  "Build MotionControl core without Qt-dependent GUI helpers"
  OFF)

set(TARGET_CORE_NAME Nmsdk-MotionControlLib.core)
set(TARGET_QT_NAME Nmsdk-MotionControlLib.qt)

add_library(${TARGET_CORE_NAME} STATIC)
add_library(${TARGET_QT_NAME} INTERFACE)

target_include_directories(${TARGET_CORE_NAME}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/Rdk/Deploy/Include
    ${CMAKE_SOURCE_DIR}/Libraries/Nmsdk-MotionControlLib/Core
)

file(GLOB MOTION_SOURCES
  ${CMAKE_SOURCE_DIR}/Libraries/Nmsdk-MotionControlLib/Core/*.cpp
)

# Exclude files with known compilation errors
list(FILTER MOTION_SOURCES EXCLUDE REGEX "/Libraries/Nmsdk-MotionControlLib/Core/NNetworkLinksStatistic\\.cpp$")

# Add Retina core
list(APPEND MOTION_SOURCES
  ${CMAKE_SOURCE_DIR}/Libraries/Nmsdk-MotionControlLib/Core/NEyeRetinaCore/NEyeRetinaBWCore.cpp
)

target_sources(${TARGET_CORE_NAME} PRIVATE ${MOTION_SOURCES})

if(NMSDK_MOTIONCONTROLLIB_BUILD_CORE_ONLY)
  target_compile_definitions(${TARGET_CORE_NAME} PUBLIC NMSDK_MOTIONCONTROLLIB_CORE_ONLY)
endif()

target_compile_definitions(${TARGET_CORE_NAME} PUBLIC RDK_QT RDK_LIB_CVS_REVISION=0)

include(${CMAKE_SOURCE_DIR}/cmake/SelectQt.cmake)

target_link_libraries(${TARGET_CORE_NAME} PUBLIC
  Threads::Threads
  rdk.static.qt
  Rdk-BasicLib.qt
  Rdk-CvBasicLib.qt
  Rdk-HardwareLib.qt
)

target_link_libraries(${TARGET_QT_NAME} INTERFACE
  ${TARGET_CORE_NAME}
)

if(NOT NMSDK_MOTIONCONTROLLIB_BUILD_CORE_ONLY)
  target_link_libraries(${TARGET_QT_NAME} INTERFACE
    Nmsdk::Qt::Core
  )
endif()



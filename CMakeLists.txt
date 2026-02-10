cmake_minimum_required(VERSION 3.16)
project(Nmsdk_MotionControlLib LANGUAGES CXX)

set(TARGET_NAME Nmsdk-MotionControlLib.qt)

add_library(${TARGET_NAME} STATIC)

target_include_directories(${TARGET_NAME}
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

target_sources(${TARGET_NAME} PRIVATE ${MOTION_SOURCES})

target_compile_definitions(${TARGET_NAME} PUBLIC RDK_QT RDK_LIB_CVS_REVISION=0)

include(${CMAKE_SOURCE_DIR}/cmake/SelectQt.cmake)
target_link_libraries(${TARGET_NAME} PUBLIC
  Nmsdk::Qt::Core
  Threads::Threads
  rdk.static.qt
  Rdk-BasicLib.qt
  Rdk-CvBasicLib.qt
  Rdk-HardwareLib.qt
)



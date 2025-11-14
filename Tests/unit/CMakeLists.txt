# Nmsdk-MotionControlLib Unit Tests CMake Configuration
cmake_minimum_required(VERSION 3.16)

# Find Google Test
find_package(GTest REQUIRED)

# Create test executable for MotionControlLib tests
add_executable(NmsdkMotionControlLibTests
    TestMotionComponents.cpp
    TestMotionComponentsWithoutCreateSamples.cpp
    TestMotionControlLibRegistration.cpp
    TestMotionControlNeurons.cpp
    TestMotionControlActuators.cpp
    ${CMAKE_SOURCE_DIR}/Libraries/Libraries_con.cpp
)

target_include_directories(NmsdkMotionControlLibTests PRIVATE
    ${CMAKE_SOURCE_DIR}/Rdk/Core
    ${CMAKE_SOURCE_DIR}/Rdk/Deploy/Include
    ${CMAKE_SOURCE_DIR}/Libraries/Nmsdk-MotionControlLib/Core
    ${CMAKE_SOURCE_DIR}/Tests
)

target_link_libraries(NmsdkMotionControlLibTests PRIVATE
    rdk.static.qt
    Nmsdk-MotionControlLib.qt
    Rdk-CvBasicLib.qt
    Rdk-HardwareLib.qt
    Nmsdk-PulseLib.qt
    TestHelpers
    GTest::gtest
    GTest::gtest_main
    glog::glog
    Threads::Threads
)

target_compile_features(NmsdkMotionControlLibTests PRIVATE cxx_std_20)
target_compile_definitions(NmsdkMotionControlLibTests PRIVATE RDK_QT RDK_LIB_CVS_REVISION=0)

# Register tests
add_test(NAME NmsdkMotionControlLibTests COMMAND NmsdkMotionControlLibTests)

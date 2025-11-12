# Nmsdk-MotionControlLib Integration Tests CMake Configuration
cmake_minimum_required(VERSION 3.16)

# Find Google Test
find_package(GTest REQUIRED)

# Create test executable for MotionControlLib integration tests
add_executable(NmsdkMotionControlLibIntegrationTests
    TestMotionControlLibIntegration.cpp
    ${CMAKE_SOURCE_DIR}/Libraries/Libraries_con.cpp
)

target_include_directories(NmsdkMotionControlLibIntegrationTests PRIVATE
    ${CMAKE_SOURCE_DIR}/Rdk/Core
    ${CMAKE_SOURCE_DIR}/Rdk/Deploy/Include
    ${CMAKE_SOURCE_DIR}/Libraries/Nmsdk-MotionControlLib/Core
    ${CMAKE_SOURCE_DIR}/Tests
)

target_link_libraries(NmsdkMotionControlLibIntegrationTests PRIVATE
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

target_compile_features(NmsdkMotionControlLibIntegrationTests PRIVATE cxx_std_20)
target_compile_definitions(NmsdkMotionControlLibIntegrationTests PRIVATE RDK_QT RDK_LIB_CVS_REVISION=0)

# Register tests
add_test(NAME NmsdkMotionControlLibIntegrationTests COMMAND NmsdkMotionControlLibIntegrationTests)

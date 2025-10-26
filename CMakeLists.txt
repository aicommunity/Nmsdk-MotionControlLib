cmake_minimum_required(VERSION 3.16)
project(Nmsdk_MotionControlLib LANGUAGES CXX)

set(TARGET_NAME Nmsdk-MotionControlLib.qt)

add_library(${TARGET_NAME} STATIC)

target_include_directories(${TARGET_NAME}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/Rdk/Deploy/Include
    ${CMAKE_SOURCE_DIR}/Libraries/Nmsdk-MotionControlLib/Core
    ${CMAKE_SOURCE_DIR}/Rdk/Core/Engine
    ${CMAKE_SOURCE_DIR}/Rdk/Core/System
)

file(GLOB MOTION_SOURCES
  ${CMAKE_SOURCE_DIR}/Libraries/Nmsdk-MotionControlLib/Core/*.cpp
)

# Исключаем файлы с ошибками компиляции
list(FILTER MOTION_SOURCES EXCLUDE REGEX "/Libraries/Nmsdk-MotionControlLib/Core/NNetworkLinksStatistic\\.cpp$")

# Добавляем ядро Retina
list(APPEND MOTION_SOURCES
  ${CMAKE_SOURCE_DIR}/Libraries/Nmsdk-MotionControlLib/Core/NEyeRetinaCore/NEyeRetinaBWCore.cpp
)

target_sources(${TARGET_NAME} PRIVATE ${MOTION_SOURCES})

target_compile_definitions(${TARGET_NAME} PUBLIC RDK_QT RDK_LIB_CVS_REVISION=0)

find_package(Qt5 REQUIRED COMPONENTS Core)
target_link_libraries(${TARGET_NAME} PUBLIC Qt5::Core Threads::Threads rdk.static.qt Rdk-BasicLib.qt Rdk-CvBasicLib.qt Rdk-HardwareLib.qt)

# Расширенные настройки отладки
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${TARGET_NAME} PRIVATE
        -g3                    # Максимальная отладочная информация
        -O0                    # Отключение оптимизации
        -fno-omit-frame-pointer # Сохранение указателей фреймов
        -fno-inline           # Отключение инлайнинга для лучшей отладки
        -fno-optimize-sibling-calls # Отключение оптимизации хвостовых вызовов
        -rdynamic              # Экспорт всех символов для отладки
        -DDEBUG                # Определение DEBUG
        -D_DEBUG               # Определение _DEBUG
    )
    
    # Дополнительные флаги для детальной отладки
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${TARGET_NAME} PRIVATE
            -ggdb3              # Расширенная отладочная информация для GDB
            -fno-eliminate-unused-debug-symbols # Сохранение неиспользуемых символов
            -fno-merge-debug-strings # Отключение слияния строк отладки
        )
    endif()
    
    # Настройки для лучшей работы с отладчиком
    target_compile_definitions(${TARGET_NAME} PRIVATE
        RDK_DEBUG=1
        RDK_VERBOSE_DEBUG=1
        RDK_ENABLE_ASSERTIONS=1
        MOTIONCONTROLLIB_DEBUG=1
        NEUROMODELER_DEBUG=1
    )
endif()

# Настройки для RelWithDebInfo
if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_options(${TARGET_NAME} PRIVATE
        -g                    # Отладочная информация
        -O2                   # Умеренная оптимизация
        -fno-omit-frame-pointer
        -rdynamic
    )
    
    target_compile_definitions(${TARGET_NAME} PRIVATE
        RDK_DEBUG=1
        MOTIONCONTROLLIB_DEBUG=1
    )
endif()



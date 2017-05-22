cmake_minimum_required(VERSION 3.7)
project(Logger)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_VERBOSE_MAKEFILE FALSE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if(WIN32)
    foreach(FLAG CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE)
        if(${FLAG} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${FLAG} "${${FLAG}}")
        endif()
    endforeach()
    add_definitions(-D_CRT_NONSTDC_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
endif()

find_package(Qt4 REQUIRED QtCore QtGui QtNetwork)
include(${QT_USE_FILE})
set(CMAKE_AUTOMOC true)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES
        application.h
        application.cpp
        mainwindow.h
        mainwindow.cpp
        settings.h
        settings.cpp)

file(GLOB_RECURSE UI_FILES *.ui)
qt4_wrap_ui(UI ${UI_FILES})

file(GLOB_RECURSE RESOURCE_FILES *.qrc)
qt4_add_resources(RESOURCE ${RESOURCE_FILES})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} ${UI} ${RESOURCE})
target_link_libraries(${CMAKE_PROJECT_NAME} ${QT_LIBRARIES})
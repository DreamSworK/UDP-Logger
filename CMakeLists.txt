cmake_minimum_required(VERSION 3.5)
project(Logger)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_VERBOSE_MAKEFILE FALSE)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCHITECTURE "x86")
else()
    message(FATAL_ERROR "Unknown platform architecture")
endif()

message(STATUS "Detected platform architecture: ${ARCHITECTURE}")

# Path to the Qt5 root directory
set(QTDIR "C:/Qt/Qt-5.15.3-${ARCHITECTURE}")

string(TIMESTAMP YEAR %Y)
file(READ application.h CONFIG)
string(REGEX REPLACE "\n" ";" CONFIG "${CONFIG}")
foreach(LINE IN LISTS CONFIG)
    string(REGEX MATCH "^(#define)[ \t]+([^ \t]+)[ \t]+\"(.+)\"$" LINE "${LINE}")
        if(${CMAKE_MATCH_COUNT} EQUAL 3)
            set(${CMAKE_MATCH_2} ${CMAKE_MATCH_3})
        endif()
endforeach()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin-${ARCHITECTURE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(EXISTS ${QTDIR})
    message(STATUS "Qt5 directory exists: ${QTDIR}")
else()
    message(FATAL_ERROR "Please specify the correct path to Qt5 root directory using QTDIR")
endif()

set(CMAKE_PREFIX_PATH ${QTDIR})
find_package(Qt5 REQUIRED COMPONENTS Core Gui Network Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCE_FILES
        application.h
        application.cpp
        mainwindow.h
        mainwindow.cpp
        settings.h
        settings.cpp
        TextEdit.cpp
        TextEdit.h)

file(GLOB_RECURSE UI_FILES *.ui)
qt5_wrap_ui(UI ${UI_FILES})

file(GLOB_RECURSE RESOURCE_FILES *.qrc)
qt5_add_resources(RESOURCES ${RESOURCE_FILES})

if(WIN32)
    foreach(FLAG CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE)
        if(${FLAG} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${FLAG} "${${FLAG}}")
        endif()
    endforeach()
    add_definitions(-D_CRT_NONSTDC_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)
    string(TOLOWER "images/${CMAKE_PROJECT_NAME}.rc" ICON_FILE)
elseif(APPLE)
    set(EXECUTABLE_TYPE MACOSX_BUNDLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
    set(MACOSX_BUNDLE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.app")

    set(MACOSX_BUNDLE_BUNDLE_NAME ${APPLICATION})
    string(TOLOWER "${ORGANIZATION}.${APPLICATION}" MACOSX_BUNDLE_GUI_IDENTIFIER)
    string(REPLACE " " "." MACOSX_BUNDLE_GUI_IDENTIFIER "${MACOSX_BUNDLE_GUI_IDENTIFIER}")

    set(MACOSX_BUNDLE_INFO_STRING "${VERSION}, Copyright ${YEAR} ${ORGANIZATION}")
    set(MACOSX_BUNDLE_COPYRIGHT "(C) ${YEAR} ${ORGANIZATION}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION})
    set(MACOSX_BUNDLE_LONG_VERSION_STRING ${VERSION})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${VERSION})

    string(TOLOWER "images/${CMAKE_PROJECT_NAME}.icns" ICON_FILE)
    get_filename_component(MACOSX_BUNDLE_ICON_FILE ${ICON_FILE} NAME)
    set_source_files_properties(${ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif()

add_executable(${CMAKE_PROJECT_NAME} ${EXECUTABLE_TYPE} ${SOURCE_FILES} ${UI} ${RESOURCES} ${ICON_FILE})
target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Network Qt5::Widgets)
get_target_property(LINKED_LIBRARIES ${CMAKE_PROJECT_NAME} INTERFACE_LINK_LIBRARIES)

if(WIN32)
    if(LINKED_LIBRARIES)
        foreach(LIBRARY ${LINKED_LIBRARIES})
            get_target_property(LIBRARY_PATH ${LIBRARY} LOCATION)
            get_target_property(LIBRARY_NAME ${LIBRARY} NAME)
            if(EXISTS ${LIBRARY_PATH})
                add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY_PATH} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                        COMMENT "Copying ${LIBRARY_NAME} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
                )
            endif()
        endforeach()
    endif()

    set(PLUGIN "plugins/platforms/qwindows.dll")
    if(EXISTS ${QTDIR}/${PLUGIN})
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${QTDIR}/${PLUGIN} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PLUGIN}
                COMMENT "Copying ${QTDIR}/${PLUGIN} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PLUGIN}"
        )
    endif()
elseif(APPLE)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_SOURCE_DIR}/tools/macdeployqt ${MACOSX_BUNDLE_PATH}
            COMMENT "Run Mac deployment tool: ${MACOSX_BUNDLE_PATH}")
endif()
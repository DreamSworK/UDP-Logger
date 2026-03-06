cmake_minimum_required(VERSION 3.16)
project(Logger VERSION 0.0.7 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCHITECTURE "x86")
else()
    message(FATAL_ERROR "Unknown platform architecture")
endif()

message(STATUS "Detected platform architecture: ${ARCHITECTURE}")

string(TIMESTAMP YEAR %Y)
file(READ application.h CONFIG)
string(REGEX REPLACE "\n" ";" CONFIG "${CONFIG}")
foreach(LINE IN LISTS CONFIG)
    string(REGEX MATCH "^(#define)[ \t]+([^ \t]+)[ \t]+\"(.+)\"$" LINE "${LINE}")
        if(${CMAKE_MATCH_COUNT} EQUAL 3)
            set(${CMAKE_MATCH_2} ${CMAKE_MATCH_3})
        endif()
endforeach()

set(QT_ROOT "C:/Qt/6.8.3/msvc2022_64" CACHE PATH "Qt installation prefix")
if(QT_ROOT)
    list(PREPEND CMAKE_PREFIX_PATH "${QT_ROOT}")
endif()

message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

qt_standard_project_setup()

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
qt_wrap_ui(UI ${UI_FILES})

file(GLOB_RECURSE RESOURCE_FILES *.qrc)
qt_add_resources(RESOURCES ${RESOURCE_FILES})

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

qt_add_executable(${PROJECT_NAME} ${EXECUTABLE_TYPE} ${SOURCE_FILES} ${UI} ${RESOURCES} ${ICON_FILE})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin-${ARCHITECTURE}
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
project(libkgapi)

set(KGAPI_VERSION_MAJOR 2)
set(KGAPI_VERSION_MINOR 1)
set(KGAPI_VERSION_RELEASE 80)
set(KGAPI_VERSION "${KGAPI_VERSION_MAJOR}.${KGAPI_VERSION_MINOR}.${KGAPI_VERSION_RELEASE}")

cmake_minimum_required(VERSION 2.8)

include(GenerateExportHeader)

find_package(Qt4 4.7.0 REQUIRED QtCore QtWebKit QtNetwork QtXML QtTest)
find_package(KDE4 4.12.0 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

find_package(KdepimLibs 4.13.80 REQUIRED)

find_package(QJSON REQUIRED)
set_package_properties(QJSON PROPERTIES DESCRIPTION "Qt library for handling JSON data" URL "http://qjson.sourceforge.net/" TYPE REQUIRED PURPOSE "JSON response/request support")

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DQT_NO_KEYWORDS -DQT_NO_CAST_FROM_ASCII)
add_definitions(-DQT_USE_QSTRINGBUILDER)
add_definitions(-DQT_NO_CAST_TO_ASCII)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${QT_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}
    ${KDEPIMLIBS_INCLUDE_DIRS}
    ${KDE4_INCLUDES}
    ${QJSON_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)


add_subdirectory(libkgapi2)

option(KGAPI_BUILD_EXAMPLES "Build example applications" FALSE)
if (KGAPI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(KGAPI_BUILD_EXAMPLES)

# FIXME: Port tests to LibKGAPI2
#add_subdirectory(tests)

project(libkgapi)
set(KGAPI_VERSION_MAJOR 2)
set(KGAPI_VERSION_MINOR 0)
set(KGAPI_VERSION_RELEASE 1)
set(KGAPI_VERSION "${KGAPI_VERSION_MAJOR}.${KGAPI_VERSION_MINOR}.${KGAPI_VERSION_RELEASE}")

cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0017)
  cmake_policy(SET CMP0017 NEW)
endif()

find_package(Qt4 4.7.0 REQUIRED QtCore QtWebKit QtNetwork QtXML QtTest)
include(${QT_USE_FILE})

find_package(KDE4 4.6.0 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

find_package(KdepimLibs 4.5.63 REQUIRED)

find_package(QJSON REQUIRED)
macro_log_feature(QJSON_FOUND "QJSON" "Qt library for handling JSON data" "http://qjson.sourceforge.net/" TRUE)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS}
    -DQT_NO_KEYWORDS -DQT_NO_CAST_FROM_ASCII
    -DKGAPI_DEPRECATED_NOWARN # Don't show deprecated warnings when compiling the library
)
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

option(KGAPI_DISABLE_DEPRECATED "Disable deprecated API" FALSE)
if (NOT KGAPI_DISABLE_DEPRECATED)
    add_subdirectory(libkgapi)
endif(NOT KGAPI_DISABLE_DEPRECATED)

option(KGAPI_BUILD_EXAMPLES "Build example applications" FALSE)
if (KGAPI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(KGAPI_BUILD_EXAMPLES)

add_subdirectory(libkgapi2)

# FIXME: Port tests to LibKGAPI2
if (NOT KGAPI_DISABLE_DEPRECATED)
    add_subdirectory(tests)
endif (NOT KGAPI_DISABLE_DEPRECATED)

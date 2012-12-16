project(libkgapi)
set(KGAPI_VERSION_MAJOR 0)
set(KGAPI_VERSION_MINOR 4)
set(KGAPI_VERSION_RELEASE 4)
set(KGAPI_VERSION 0.4.4)

cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0017)
  cmake_policy(SET CMP0017 NEW)
endif()

find_package(Qt4 4.7.0 REQUIRED QtCore QtWebKit QtNetwork QtXML)
include(${QT_USE_FILE})
option(DEBUG_RAWDATA "Log raw data send and received from Google service (enables CMAKE_BUILD_TYPE=Debug)" FALSE)
if (DEBUG_RAWDATA)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
  add_definitions(-DDEBUG_RAWDATA)
else (DEBUG_RAWDATA)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel.")
endif (DEBUG_RAWDATA)

find_package(KDE4 4.6.0 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

find_package(KdepimLibs 4.5.63 REQUIRED)

find_package(QJSON REQUIRED)
macro_log_feature(QJSON_FOUND "QJSON" "Qt library for handling JSON data" "http://qjson.sourceforge.net/" TRUE)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DQT_NO_KEYWORDS -DLIBKGAPI_LIBRARY)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

option(USE_KCAL "Use deprecated KCal API instead of KCalCore API. Provides backwards compatibility for KDE PIM 4.4" FALSE)
if (USE_KCAL)
  set(KCALENDAR_LIBRARIES ${KDEPIMLIBS_KCAL_LIBS})
  add_definitions(-DWITH_KCAL)

  find_package(Boost REQUIRED)
  macro_log_feature(Boost_FOUND "Boost" "Free peer-reviewed portable C++ source libraries" TRUE)
else (USE_KCAL)
  set(KCALENDAR_LIBRARIES ${KDEPIMLIBS_KCALCORE_LIBS})
endif (USE_KCAL)

include_directories(
	  ${QT_INCLUDE_DIR}
	  ${CMAKE_SOURCE_DIR}
	  ${CMAKE_SOURCE_DIR}/libkgapi
	  ${CMAKE_BINARY_DIR}
	  ${KDEPIMLIBS_INCLUDE_DIRS}
	  ${KDE4_INCLUDES}
	  ${QJSON_INCLUDE_DIR}
	  ${Boost_INCLUDE_DIR}
)

add_subdirectory(libkgapi)

configure_file(${libkgapi_SOURCE_DIR}/LibKGAPIConfig.cmake.in
               ${libkgapi_BINARY_DIR}/LibKGAPIConfig.cmake
               @ONLY
)
# this file is used by to check if the installed version can be used.
macro_write_basic_cmake_version_file(${libkgapi_BINARY_DIR}/LibKGAPIConfigVersion.cmake
                                     ${KGAPI_VERSION_MAJOR} ${KGAPI_VERSION_MINOR} ${KGAPI_VERSION_RELEASE})

install(FILES ${libkgapi_BINARY_DIR}/LibKGAPIConfig.cmake ${libkgapi_BINARY_DIR}/LibKGAPIConfigVersion.cmake
        DESTINATION ${LIB_INSTALL_DIR}/cmake/LibKGAPI)

# Install the file with the exported targets
install(EXPORT kgapiLibraryTargets
        DESTINATION ${LIB_INSTALL_DIR}/cmake/LibKGAPI
        FILE LibKGAPITargetsWithPrefix.cmake)

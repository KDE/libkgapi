project(libkgoogle)
set(KGOOGLE_VERSION_MAJOR 0)
set(KGOOGLE_VERSION_MINOR 3)
set(KGOOGLE_VERSION_RELEASE 1)
set(KGOOGLE_VERSION 0.3.1)

cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0017)
  cmake_policy(SET CMP0017 NEW)
endif()

find_package(Qt4 4.7.0 REQUIRED) 
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

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DQT_NO_KEYWORDS -DLIBKGOOGLE_LIBRARY)
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
	  ${CMAKE_SOURCE_DIR}/libkgoogle
	  ${CMAKE_BINARY_DIR}
	  ${KDEPIMLIBS_INCLUDE_DIRS}
	  ${KDE4_INCLUDES}
	  ${QJSON_INCLUDE_DIR}
	  ${Boost_INCLUDE_DIR}
)

add_subdirectory(libkgoogle)

configure_file(${libkgoogle_SOURCE_DIR}/LibKGoogleConfig.cmake.in
               ${libkgoogle_BINARY_DIR}/LibKGoogleConfig.cmake
               @ONLY
)
# this file is used by to check if the installed version can be used.
macro_write_basic_cmake_version_file(${libkgoogle_BINARY_DIR}/LibKGoogleConfigVersion.cmake
                                     ${KGOOGLE_VERSION_MAJOR} ${KGOOGLE_VERSION_MINOR} ${KGOOGLE_VERSION_RELEASE})

install(FILES ${libkgoogle_BINARY_DIR}/LibKGoogleConfig.cmake ${libkgoogle_BINARY_DIR}/LibKGoogleConfigVersion.cmake
        DESTINATION ${LIB_INSTALL_DIR}/cmake/LibKGoogle)

# Install the file with the exported targets
install(EXPORT kgoogleLibraryTargets
        DESTINATION ${LIB_INSTALL_DIR}/cmake/LibKGoogle
        FILE LibKGoogleTargetsWithPrefix.cmake)

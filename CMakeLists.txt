project(libkgapi)
set(KGAPI_VERSION_MAJOR 1)
set(KGAPI_VERSION_MINOR 9)
set(KGAPI_VERSION_RELEASE 80)
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

option(BUILD_EXAMPLES "Build example applications" FALSE)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)

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
add_subdirectory(libkgapi)
add_subdirectory(tests)

if(NOT WIN32)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libkgapi.pc.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/libkgapi.pc
  )
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/libkgapi.pc
    DESTINATION ${LIB_INSTALL_DIR}/pkgconfig
  )
endif(NOT WIN32)

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

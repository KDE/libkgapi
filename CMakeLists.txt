cmake_minimum_required(VERSION  2.8)

if(POLICY CMP0017)
  cmake_policy(SET CMP0017 NEW)
endif()

find_package(Qt4 4.7.0 REQUIRED) 
find_package(KDE4 4.6.40 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)
find_package(KdepimLibs 4.5.63 REQUIRED)
find_program(XSLTPROC_EXECUTABLE xsltproc)
macro_log_feature(XSLTPROC_EXECUTABLE "xsltproc" "The command line XSLT processor from libxslt" "http://xmlsoft.org/XSLT/" TRUE)
find_package(QJSON)
macro_log_feature(QJSON_FOUND "QJSON" "Qt library for handling JSON data" "http://qjson.sourceforge.net/" TRUE)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
	  ${QT_INCLUDE_DIR}
	  ${CMAKE_SOURCE_DIR}
	  ${CMAKE_SOURCE_DIR}/libkgoogle
	  ${CMAKE_BINARY_DIR}
	  ${KDEPIMLIBS_INCLUDE_DIRS}
	  ${KDE4_INCLUDES}
	  ${QJSON_INCLUDE_DIR}
)

add_subdirectory(libkgoogle)

if (NOT DEFINED WITHOUT_CALENDAR)
  add_subdirectory(calendar)
endif (NOT DEFINED WITHOUT_CALENDAR)

if (NOT DEFINED WITHOUT_CONTACTS)
  add_subdirectory(contacts)
endif (NOT DEFINED WITHOUT_CONTACTS)

if (DEFINED WITH_TESTS)
  add_subdirectory(tests)
endif (DEFINED WITH_TESTS)
cmake_minimum_required(VERSION  2.8)

if(POLICY CMP0017)
  cmake_policy(SET CMP0017 NEW)
endif()

find_package(Qt4 4.7.0 REQUIRED) 
find_package(KDE4 4.7.0 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

find_package(KdepimLibs 4.5.63 REQUIRED)

find_program(XSLTPROC_EXECUTABLE xsltproc)
macro_log_feature(XSLTPROC_EXECUTABLE "xsltproc" "The command line XSLT processor from libxslt" "http://xmlsoft.org/XSLT/" TRUE)

find_package(QJSON REQUIRED)
macro_log_feature(QJSON_FOUND "QJSON" "Qt library for handling JSON data" "http://qjson.sourceforge.net/" TRUE)

macro_display_feature_log()

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})


option(USE_KCAL "Use deprecated KCal API instead of KCalCore API. Provides backwards compatibility for KDE PIM 4.4" TRUE)
if (USE_KCAL)
  set(KCALENDAR_LIBRARIES ${KDEPIMLIBS_KCAL_LIBS})
  add_definitions(-DWITH_KCAL)
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
)

add_subdirectory(libkgoogle)

macro_optional_add_subdirectory(calendar)
macro_optional_add_subdirectory(contacts)

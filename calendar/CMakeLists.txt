project(googlecalendarresource)

cmake_minimum_required(VERSION 2.8)

set(COMMONS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../common )
set(COMMONS_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/../common )

include_directories (${CMAKE_CURRENT_SOURCE_DIR}
		     ${COMMONS_SOURCE_DIR}
)

add_definitions(-DRESOURCE_VERSION=\\"${RESOURCE_VERSION}\\")
if (XSLTPROC_EXECUTABLE)
  macro(kcfg_generate_dbus_interface _kcfg _name)
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_name}.xml
      COMMAND ${XSLTPROC_EXECUTABLE} --stringparam interfaceName ${_name}
	      ${KDEPIMLIBS_DATA_DIR}/akonadi-kde/kcfg2dbus.xsl
	      ${_kcfg}
	      > ${CMAKE_CURRENT_BINARY_DIR}/${_name}.xml
      DEPENDS ${KDEPIMLIBS_DATA_DIR}/akonadi-kde/kcfg2dbus.xsl
	      ${_kcfg}
  )
  endmacro(kcfg_generate_dbus_interface)
endif (XSLTPROC_EXECUTABLE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

set(calendarresource_SRCS
    calendarresource.cpp
    calendarlistjob.cpp
    settings.cpp
    settingsdialog.cpp 
)

install(FILES googlecalendarresource.desktop DESTINATION "${CMAKE_INSTALL_PREFIX}/share/akonadi/agents" )
kde4_add_ui_files(calendarresource_SRCS settingsdialog.ui)
kde4_add_kcfg_files(calendarresource_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/settingsbase.kcfgc)
kcfg_generate_dbus_interface(${CMAKE_CURRENT_SOURCE_DIR}/settingsbase.kcfg org.kde.Akonadi.GoogleCalendar.Settings)
qt4_add_dbus_adaptor(calendarresource_SRCS
		     ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Akonadi.GoogleCalendar.Settings.xml 
		     ${CMAKE_CURRENT_SOURCE_DIR}/settings.h Settings)
kde4_add_executable(akonadi_googlecalendar_resource RUN_UNINSTALLED ${calendarresource_SRCS})

target_link_libraries(akonadi_googlecalendar_resource
		      ${KDEPIMLIBS_AKONADI_LIBS}
		      ${KDEPIMLIBS_KABC_LIBS}
		      ${KDEPIMLIBS_KMIME_LIBS}
		      ${QT_QTCORE_LIBRARY}
		      ${QT_QTGUI_LIBRARY}
		      ${QT_QTDBUS_LIBRARY}
		      ${KDE4_KDECORE_LIBS}
		      ${KDE4_KDEWEBKIT_LIBS}
		      ${CMAKE_CURRENT_BINARY_DIR}/../lib/libkgoogle.so
)

install(TARGETS akonadi_googlecalendar_resource ${INSTALL_TARGETS_DEFAULT_ARGS})

set(kgapicalendar_SRCS
    calendar.cpp
    calendarcreatejob.cpp
    calendardeletejob.cpp
    calendarfetchjob.cpp
    calendarmodifyjob.cpp
    calendarservice.cpp
    event.cpp
    eventcreatejob.cpp
    eventdeletejob.cpp
    eventfetchjob.cpp
    eventmodifyjob.cpp
    eventmovejob.cpp
    reminder.cpp
    freebusyqueryjob.cpp
    ../debug.cpp
    enums.h
)

ecm_generate_headers(kgapicalendar_CamelCase_HEADERS
    HEADER_NAMES
    Calendar
    CalendarCreateJob
    CalendarDeleteJob
    CalendarFetchJob
    CalendarModifyJob
    Enums
    Event
    EventCreateJob
    EventDeleteJob
    EventFetchJob
    EventModifyJob
    EventMoveJob
    Reminder
    FreeBusyQueryJob
    PREFIX KGAPI/Calendar
    REQUIRED_HEADERS kgapicalendar_HEADERS
)

add_library(KPimGAPICalendar
    ${kgapicalendar_SRCS}
)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPimGAPICalendar PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KPimGAPICalendar BASE_NAME kgapicalendar)

add_library(KPim::GAPICalendar ALIAS KPimGAPICalendar)

# TODO: Backwards compatibility, remove in next major release
add_library(KF5::GAPICalendar ALIAS KPimGAPICalendar)
target_include_directories(KPimGAPICalendar INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI>")

target_link_libraries(KPimGAPICalendar
PUBLIC
    KPim::GAPICore
    KF5::CalendarCore
    Qt5::Gui
PRIVATE
    Qt5::Network
)

set_target_properties(KPimGAPICalendar PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPICalendar
)

install(TARGETS
    KPimGAPICalendar
    EXPORT KPimGAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapicalendar_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/KGAPI/Calendar"
    COMPONENT Devel
)

install(FILES
    ${kgapicalendar_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapicalendar_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/kgapi/calendar"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPICalendar
    LIB_NAME KPimGAPICalendar
    DEPS "GAPICore CalendarCore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

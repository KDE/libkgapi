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
)

ecm_generate_headers(kgapicalendar_CamelCase_HEADERS
    HEADER_NAMES
    Calendar
    CalendarCreateJob
    CalendarDeleteJob
    CalendarFetchJob
    CalendarModifyJob
    Event
    EventCreateJob
    EventDeleteJob
    EventFetchJob
    EventModifyJob
    EventMoveJob
    Reminder
    PREFIX KGAPI/Calendar
    REQUIRED_HEADERS kgapicalendar_HEADERS
)

add_library(KF5GAPICalendar
    ${kgapicalendar_SRCS}
)

generate_export_header(KF5GAPICalendar BASE_NAME kgapicalendar)

add_library(KF5::GAPICalendar ALIAS KF5GAPICalendar)
target_include_directories(KF5GAPICalendar INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KGAPI>")
target_include_directories(KF5GAPICalendar PUBLIC "$<BUILD_INTERFACE:${KGAPI_SOURCE_DIR}/src;${KAGAPI_BINARY_DIR}/src>")

target_link_libraries(KF5GAPICalendar
PUBLIC
    KF5::GAPICore
    KF5::CalendarCore
)

set_target_properties(KF5GAPICalendar PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPICalendar
)

install(TARGETS
    KF5GAPICalendar
    EXPORT KF5GAPITargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapicalendar_CamelCase_HEADERS}
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}/KGAPI/KGAPI/Calendar"
    COMPONENT Devel
)

install(FILES
    ${kgapicalendar_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapicalendar_export.h"
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}/KGAPI/kgapi/calendar"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPICalendar
    LIB_NAME KF5GAPICalendar
    DEPS "GAPICore CalendarCore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KF5_INCLUDE_INSTALL_DIR}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

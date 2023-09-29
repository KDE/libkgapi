add_library(KPim6GAPICalendar)
add_library(KPim6::GAPICalendar ALIAS KPim6GAPICalendar)


target_sources(KPim6GAPICalendar PRIVATE
    ${libkgapi_debug_SRCS}
    calendar.cpp
    calendarcreatejob.cpp
    calendarcreatejob.h
    calendardeletejob.cpp
    calendardeletejob.h
    calendarfetchjob.cpp
    calendarfetchjob.h
    calendar.h
    calendarmodifyjob.cpp
    calendarmodifyjob.h
    calendarservice.cpp
    calendarservice.h
    enums.h
    event.cpp
    eventcreatejob.cpp
    eventcreatejob.h
    eventdeletejob.cpp
    eventdeletejob.h
    eventfetchjob.cpp
    eventfetchjob.h
    event.h
    eventmodifyjob.cpp
    eventmodifyjob.h
    eventmovejob.cpp
    eventmovejob.h
    freebusyqueryjob.cpp
    freebusyqueryjob.h
    reminder.cpp
    reminder.h
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

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim6GAPICalendar PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KPim6GAPICalendar BASE_NAME kgapicalendar)


target_include_directories(KPim6GAPICalendar INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI>")

target_link_libraries(KPim6GAPICalendar
PUBLIC
    KPim6::GAPICore
    KF6::CalendarCore
    Qt::Gui
PRIVATE
    Qt::Network
)

set_target_properties(KPim6GAPICalendar PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPICalendar
)

install(TARGETS
    KPim6GAPICalendar
    EXPORT KPim6GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapicalendar_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/KGAPI/Calendar"
    COMPONENT Devel
)

install(FILES
    ${kgapicalendar_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapicalendar_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/kgapi/calendar"
    COMPONENT Devel
)

if (BUILD_QCH)
    ecm_add_qch(
        KPim6GAPICalendar_QCH
        NAME KPim6GAPICalendar
        BASE_NAME KPim6GAPICalendar
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kgapicalendar_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt6Core_QCH
            Qt6Gui_QCH
            Qt6Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KGAPICALENDAR_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

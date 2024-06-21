add_library(KPim6GAPITasks)
add_library(KPim6::GAPITasks ALIAS KPim6GAPITasks)


target_sources(KPim6GAPITasks PRIVATE
    ${libkgapi_debug_SRCS}
    task.cpp
    taskcreatejob.cpp
    taskcreatejob.h
    taskdeletejob.cpp
    taskdeletejob.h
    taskfetchjob.cpp
    taskfetchjob.h
    task.h
    tasklist.cpp
    tasklistcreatejob.cpp
    tasklistcreatejob.h
    tasklistdeletejob.cpp
    tasklistdeletejob.h
    tasklistfetchjob.cpp
    tasklistfetchjob.h
    tasklist.h
    tasklistmodifyjob.cpp
    tasklistmodifyjob.h
    taskmodifyjob.cpp
    taskmodifyjob.h
    taskmovejob.cpp
    taskmovejob.h
    tasksservice.cpp
    tasksservice.h
)

ecm_generate_headers(kgapitasks_CamelCase_HEADERS
    HEADER_NAMES
    Task
    TaskCreateJob
    TaskDeleteJob
    TaskFetchJob
    TaskList
    TaskListCreateJob
    TaskListDeleteJob
    TaskListFetchJob
    TaskListModifyJob
    TaskModifyJob
    TaskMoveJob
    PREFIX KGAPI/Tasks
    REQUIRED_HEADERS kgapitasks_HEADERS
)


if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim6GAPITasks PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPim6GAPITasks BASE_NAME kgapitasks)

target_include_directories(KPim6GAPITasks
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI>"
    INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_link_libraries(KPim6GAPITasks
PUBLIC
    KPim6::GAPICore
    KF6::CalendarCore
PRIVATE
    Qt::Network
)

set_target_properties(KPim6GAPITasks PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPITasks
)

install(TARGETS
    KPim6GAPITasks
    EXPORT KPim6GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapitasks_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/KGAPI/Tasks"
    COMPONENT Devel
)

install(FILES
    ${kgapitasks_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapitasks_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/kgapi/tasks"
    COMPONENT Devel
)

if(BUILD_QCH)
    ecm_add_qch(
        KPim6GAPITasks_QCH
        NAME KPim6GAPITasks
        BASE_NAME KPim6GAPITasks
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kgapitasks_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt6Core_QCH
            Qt6Gui_QCH
            Qt6Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KGAPITASKS_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
   )
endif()


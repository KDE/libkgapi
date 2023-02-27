add_library(KPim${KF_MAJOR_VERSION}GAPITasks)
add_library(KPim${KF_MAJOR_VERSION}::GAPITasks ALIAS KPim${KF_MAJOR_VERSION}GAPITasks)


target_sources(KPim${KF_MAJOR_VERSION}GAPITasks PRIVATE
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


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim${KF_MAJOR_VERSION}GAPITasks PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPim${KF_MAJOR_VERSION}GAPITasks BASE_NAME kgapitasks)

target_include_directories(KPim${KF_MAJOR_VERSION}GAPITasks
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI>"
    INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_link_libraries(KPim${KF_MAJOR_VERSION}GAPITasks
PUBLIC
    KPim${KF_MAJOR_VERSION}::GAPICore
    KF${KF_MAJOR_VERSION}::CalendarCore
PRIVATE
    Qt::Network
)

set_target_properties(KPim${KF_MAJOR_VERSION}GAPITasks PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPITasks
)

install(TARGETS
    KPim${KF_MAJOR_VERSION}GAPITasks
    EXPORT KPim${KF_MAJOR_VERSION}GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapitasks_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/KGAPI/Tasks"
    COMPONENT Devel
)

install(FILES
    ${kgapitasks_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapitasks_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/kgapi/tasks"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPITasks
    LIB_NAME KPim${KF_MAJOR_VERSION}GAPITasks
    DEPS "GAPICore CalendarCore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

add_library(KPimGAPITasks)
add_library(KPim::GAPITasks ALIAS KPimGAPITasks)


target_sources(KPimGAPITasks PRIVATE
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
    set_target_properties(KPimGAPITasks PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPimGAPITasks BASE_NAME kgapitasks)

target_include_directories(KPimGAPITasks
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI>"
    INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_link_libraries(KPimGAPITasks
PUBLIC
    KPim::GAPICore
    KF${KF_MAJOR_VERSION}::CalendarCore
PRIVATE
    Qt::Network
)

set_target_properties(KPimGAPITasks PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPITasks
)

install(TARGETS
    KPimGAPITasks
    EXPORT KPimGAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapitasks_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/KGAPI/Tasks"
    COMPONENT Devel
)

install(FILES
    ${kgapitasks_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapitasks_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/kgapi/tasks"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPITasks
    LIB_NAME KPimGAPITasks
    DEPS "GAPICore CalendarCore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

set(kgapitasks_SRCS
    task.cpp
    taskcreatejob.cpp
    taskdeletejob.cpp
    taskfetchjob.cpp
    tasklist.cpp
    tasklistcreatejob.cpp
    tasklistdeletejob.cpp
    tasklistfetchjob.cpp
    tasklistmodifyjob.cpp
    taskmodifyjob.cpp
    taskmovejob.cpp
    tasksservice.cpp

    ../debug.cpp
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

add_library(KF5GAPITasks
    ${kgapitasks_SRCS}
)

generate_export_header(KF5GAPITasks BASE_NAME kgapitasks)

add_library(KF5::GAPITasks ALIAS KF5GAPITasks)
target_include_directories(KF5GAPITasks INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI>")

target_link_libraries(KF5GAPITasks
PUBLIC
    KF5::GAPICore
    KF5::CalendarCore
)

set_target_properties(KF5GAPITasks PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPITasks
)

install(TARGETS
    KF5GAPITasks
    EXPORT KF5GAPITargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapitasks_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI/KGAPI/Tasks"
    COMPONENT Devel
)

install(FILES
    ${kgapitasks_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapitasks_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI/kgapi/tasks"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPITasks
    LIB_NAME KF5GAPITasks
    DEPS "GAPICore CalendarCore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

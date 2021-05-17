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

add_library(KPimGAPITasks
    ${kgapitasks_SRCS}
)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPimGAPITasks PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPimGAPITasks BASE_NAME kgapitasks)

add_library(KPim::GAPITasks ALIAS KPimGAPITasks)

# TODO: Backwards compatibility, remove in next major release
add_library(KF5::GAPITasks ALIAS KPimGAPITasks)
target_include_directories(KPimGAPITasks INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI>")

target_link_libraries(KPimGAPITasks
PUBLIC
    KPim::GAPICore
    KF5::CalendarCore
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

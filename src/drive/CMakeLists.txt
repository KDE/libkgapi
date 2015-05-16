set(kgapidrive_SRCS
    about.cpp
    aboutfetchjob.cpp
    app.cpp
    appfetchjob.cpp
    change.cpp
    changefetchjob.cpp
    childreference.cpp
    childreferencecreatejob.cpp
    childreferencedeletejob.cpp
    childreferencefetchjob.cpp
    driveservice.cpp
    file.cpp
    fileabstractdatajob.cpp
    fileabstractmodifyjob.cpp
    fileabstractuploadjob.cpp
    filecopyjob.cpp
    filecreatejob.cpp
    filedeletejob.cpp
    filefetchcontentjob.cpp
    filefetchjob.cpp
    filemodifyjob.cpp
    filesearchquery.cpp
    filetouchjob.cpp
    filetrashjob.cpp
    fileuntrashjob.cpp
    parentreference.cpp
    parentreferencecreatejob.cpp
    parentreferencedeletejob.cpp
    parentreferencefetchjob.cpp
    permission.cpp
    permissioncreatejob.cpp
    permissiondeletejob.cpp
    permissionfetchjob.cpp
    permissionmodifyjob.cpp
    revision.cpp
    revisiondeletejob.cpp
    revisionfetchjob.cpp
    revisionmodifyjob.cpp
    user.cpp

    ../debug.cpp
)

ecm_generate_headers(kgapidrive_CamelCase_HEADERS
    HEADER_NAMES
    About
    AboutFetchJob
    App
    AppFetchJob
    Change
    ChangeFetchJob
    ChildReference
    ChildReferenceCreateJob
    ChildReferenceDeleteJob
    ChildReferenceFetchJob
    File
    FileAbstractDataJob
    FileAbstractModifyJob
    FileAbstractUploadJob
    FileCopyJob
    FileCreateJob
    FileDeleteJob
    FileFetchContentJob
    FileFetchJob
    FileModifyJob
    FileSearchQuery
    FileTouchJob
    FileTrashJob
    FileUntrashJob
    ParentReference
    ParentReferenceCreateJob
    ParentReferenceDeleteJob
    ParentReferenceFetchJob
    Permission
    PermissionCreateJob
    PermissionDeleteJob
    PermissionFetchJob
    PermissionModifyJob
    Revision
    RevisionDeleteJob
    RevisionFetchJob
    RevisionModifyJob
    User
    PREFIX KGAPI/Drive
    REQUIRED_HEADERS kgapidrive_HEADERS
)

add_library(KF5GAPIDrive
    ${kgapidrive_SRCS}
)

generate_export_header(KF5GAPIDrive BASE_NAME kgapidrive)

add_library(KF5::GAPIDrive ALIAS KF5GAPIDrive)
target_include_directories(KF5GAPIDrive INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI>")

target_link_libraries(KF5GAPIDrive
PUBLIC
    KF5::GAPICore
PRIVATE
    Qt5::Network
)

set_target_properties(KF5GAPIDrive PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIDrive
)

install(TARGETS
    KF5GAPIDrive
    EXPORT KF5GAPITargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapidrive_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI/KGAPI/Drive"
    COMPONENT Devel
)

install(FILES
    ${kgapidrive_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapidrive_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI/kgapi/drive"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIDrive
    LIB_NAME KF5GAPIDrive
    DEPS "GAPICore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

add_library(KPim6GAPIDrive)
add_library(KPim6::GAPIDrive ALIAS KPim6GAPIDrive)


target_sources(KPim6GAPIDrive PRIVATE
    ${libkgapi_debug_SRCS}
    about.cpp
    aboutfetchjob.cpp
    aboutfetchjob.h
    about.h
    app.cpp
    appfetchjob.cpp
    appfetchjob.h
    app.h
    change.cpp
    changefetchjob.cpp
    changefetchjob.h
    change.h
    childreference.cpp
    childreferencecreatejob.cpp
    childreferencecreatejob.h
    childreferencedeletejob.cpp
    childreferencedeletejob.h
    childreferencefetchjob.cpp
    childreferencefetchjob.h
    childreference.h
    drives.cpp
    drivescreatejob.cpp
    drivescreatejob.h
    drivesdeletejob.cpp
    drivesdeletejob.h
    driveservice.cpp
    driveservice.h
    drivesfetchjob.cpp
    drivesfetchjob.h
    drives.h
    driveshidejob.cpp
    driveshidejob.h
    drivesmodifyjob.cpp
    drivesmodifyjob.h
    drivessearchquery.cpp
    drivessearchquery.h
    fileabstractdatajob.cpp
    fileabstractdatajob.h
    fileabstractmodifyjob.cpp
    fileabstractmodifyjob.h
    fileabstractresumablejob.cpp
    fileabstractresumablejob.h
    fileabstractuploadjob.cpp
    fileabstractuploadjob.h
    filecopyjob.cpp
    filecopyjob.h
    file.cpp
    filecreatejob.cpp
    filecreatejob.h
    filedeletejob.cpp
    filedeletejob.h
    filefetchcontentjob.cpp
    filefetchcontentjob.h
    filefetchjob.cpp
    filefetchjob.h
    file.h
    filemodifyjob.cpp
    filemodifyjob.h
    file_p.h
    fileresumablecreatejob.cpp
    fileresumablecreatejob.h
    fileresumablemodifyjob.cpp
    fileresumablemodifyjob.h
    filesearchquery.cpp
    filesearchquery.h
    filetouchjob.cpp
    filetouchjob.h
    filetrashjob.cpp
    filetrashjob.h
    fileuntrashjob.cpp
    fileuntrashjob.h
    parentreference.cpp
    parentreferencecreatejob.cpp
    parentreferencecreatejob.h
    parentreferencedeletejob.cpp
    parentreferencedeletejob.h
    parentreferencefetchjob.cpp
    parentreferencefetchjob.h
    parentreference.h
    parentreference_p.h
    permission.cpp
    permissioncreatejob.cpp
    permissioncreatejob.h
    permissiondeletejob.cpp
    permissiondeletejob.h
    permissionfetchjob.cpp
    permissionfetchjob.h
    permission.h
    permissionmodifyjob.cpp
    permissionmodifyjob.h
    permission_p.h
    revision.cpp
    revisiondeletejob.cpp
    revisiondeletejob.h
    revisionfetchjob.cpp
    revisionfetchjob.h
    revision.h
    revisionmodifyjob.cpp
    revisionmodifyjob.h
    searchquery.cpp
    searchquery.h
    teamdrive.cpp
    teamdrivecreatejob.cpp
    teamdrivecreatejob.h
    teamdrivedeletejob.cpp
    teamdrivedeletejob.h
    teamdrivefetchjob.cpp
    teamdrivefetchjob.h
    teamdrive.h
    teamdrivemodifyjob.cpp
    teamdrivemodifyjob.h
    teamdrivesearchquery.cpp
    teamdrivesearchquery.h
    user.cpp
    user.h
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
    SearchQuery
    File
    FileAbstractDataJob
    FileAbstractModifyJob
    FileAbstractUploadJob
    FileAbstractResumableJob
    FileCopyJob
    FileCreateJob
    FileDeleteJob
    FileFetchContentJob
    FileFetchJob
    FileModifyJob
    FileResumableCreateJob
    FileResumableModifyJob
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
    Drives
    DrivesCreateJob
    DrivesDeleteJob
    DrivesFetchJob
    DrivesModifyJob
    DrivesHideJob
    DrivesSearchQuery
    Teamdrive
    TeamdriveCreateJob
    TeamdriveDeleteJob
    TeamdriveFetchJob
    TeamdriveModifyJob
    TeamdriveSearchQuery
    User
    PREFIX KGAPI/Drive
    REQUIRED_HEADERS kgapidrive_HEADERS
)
#Disable for the moment
#if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
#    set_target_properties(KPimGAPIDrive PROPERTIES UNITY_BUILD ON)
#endif()

generate_export_header(KPim6GAPIDrive BASE_NAME kgapidrive)

target_include_directories(KPim6GAPIDrive
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI>"
    INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_link_libraries(KPim6GAPIDrive
PUBLIC
    KPim6::GAPICore
    Qt::Gui
PRIVATE
    Qt::Network
)

set_target_properties(KPim6GAPIDrive PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIDrive
)

install(TARGETS
    KPim6GAPIDrive
    EXPORT KPim6GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapidrive_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/KGAPI/Drive"
    COMPONENT Devel
)

install(FILES
    ${kgapidrive_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapidrive_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/kgapi/drive"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIDrive
    LIB_NAME KPim6GAPIDrive
    DEPS "GAPICore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

if (BUILD_QCH)
    ecm_add_qch(
        KPim6GAPIDrive_QCH
        NAME KPim6GAPIDrive
        BASE_NAME KPim6GAPIDrive
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kgapidrive_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt6Core_QCH
            Qt6Gui_QCH
            Qt6Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KGAPIDRIVE_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()


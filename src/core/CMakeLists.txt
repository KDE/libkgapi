ecm_create_qm_loader(QM_LOADER libkgapi_qt)
add_library(KPim${KF_MAJOR_VERSION}GAPICore)
add_library(KPim${KF_MAJOR_VERSION}::GAPICore ALIAS KPim${KF_MAJOR_VERSION}GAPICore)

target_sources(KPim${KF_MAJOR_VERSION}GAPICore PRIVATE
    ${libkgapi_debug_SRCS}
    account.cpp
    account.h
    accountinfo/accountinfo.cpp
    accountinfo/accountinfofetchjob.cpp
    accountinfo/accountinfofetchjob.h
    accountinfo/accountinfo.h
    accountmanager.cpp
    accountmanager.h
    accountstorage.cpp
    accountstorage_kwallet.cpp
    accountstorage_kwallet_p.h
    accountstorage_p.h
    authjob.cpp
    authjob.h
    createjob.cpp
    createjob.h
    deletejob.cpp
    deletejob.h
    fetchjob.cpp
    fetchjob.h
    job.cpp
    job.h
    job_p.h
    modifyjob.cpp
    modifyjob.h
    networkaccessmanagerfactory.cpp
    networkaccessmanagerfactory_p.h
    object.cpp
    object.h
    private/fullauthenticationjob.cpp
    private/fullauthenticationjob_p.h
    private/newtokensfetchjob.cpp
    private/newtokensfetchjob_p.h
    private/queuehelper_p.h
    private/refreshtokensjob.cpp
    private/refreshtokensjob_p.h
    types.h
    utils.cpp
    utils.h
    utils_p.h
    ${QM_LOADER}
)

ecm_generate_headers(kgapicore_base_CamelCase_HEADERS
    HEADER_NAMES
    Account
    AccountManager
    AuthJob
    CreateJob
    DeleteJob
    FetchJob
    Job
    ModifyJob
    Object
    Types
    Utils
    PREFIX KGAPI
    REQUIRED_HEADERS kgapicore_base_HEADERS
)

ecm_generate_headers(kgapicore_accountinfo_CamelCase_HEADERS
    HEADER_NAMES
    AccountInfo
    AccountInfoFetchJob
    REQUIRED_HEADERS kgapicore_accountinfo_HEADERS
    RELATIVE accountinfo
)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim${KF_MAJOR_VERSION}GAPICore PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPim${KF_MAJOR_VERSION}GAPICore BASE_NAME kgapicore)


target_include_directories(KPim${KF_MAJOR_VERSION}GAPICore
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI;${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/kgapi>"
    INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_link_libraries(KPim${KF_MAJOR_VERSION}GAPICore
PUBLIC
    Qt::Core
PRIVATE
    Qt::Network
    KF${KF_MAJOR_VERSION}::Wallet
)

set_target_properties(KPim${KF_MAJOR_VERSION}GAPICore PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPICore
)

install(TARGETS
    KPim${KF_MAJOR_VERSION}GAPICore
    EXPORT KPim${KF_MAJOR_VERSION}GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapicore_base_CamelCase_HEADERS}
    ${kgapicore_accountinfo_CamelCase_HEADERS}
    ${kgapicore_ui_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/KGAPI"
    COMPONENT Devel
)

install(FILES
    ${kgapicore_base_HEADERS}
    ${kgapicore_accountinfo_HEADERS}
    ${kgapicore_ui_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapicore_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/kgapi"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPICore
    LIB_NAME KPim${KF_MAJOR_VERSION}GAPICore
    DEPS "core network"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

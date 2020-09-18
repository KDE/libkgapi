ecm_create_qm_loader(QM_LOADER libkgapi_qt)

set(kgapicore_SRCS
    accountinfo/accountinfo.cpp
    accountinfo/accountinfofetchjob.cpp
    private/fullauthenticationjob.cpp
    private/newtokensfetchjob.cpp
    private/refreshtokensjob.cpp
    account.cpp
    accountmanager.cpp
    accountstorage.cpp
    accountstorage_kwallet.cpp
    authjob.cpp
    createjob.cpp
    deletejob.cpp
    fetchjob.cpp
    job.cpp
    networkaccessmanagerfactory.cpp
    modifyjob.cpp
    object.cpp
    utils.cpp
    ${QM_LOADER}

    ../debug.cpp
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

add_library(KPimGAPICore
    ${kgapicore_SRCS}
)

generate_export_header(KPimGAPICore BASE_NAME kgapicore)

add_library(KPim::GAPICore ALIAS KPimGAPICore)
# TODO: Backwards compatibility, remove in next major release
add_library(KF5::GAPICore ALIAS KPimGAPICore)
target_include_directories(KPimGAPICore INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI;${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/kgapi>")

target_link_libraries(KPimGAPICore
PUBLIC
    Qt5::Core
PRIVATE
    Qt5::Network
    KF5::Wallet
)

set_target_properties(KPimGAPICore PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPICore
)

install(TARGETS
    KPimGAPICore
    EXPORT KPimGAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapicore_base_CamelCase_HEADERS}
    ${kgapicore_accountinfo_CamelCase_HEADERS}
    ${kgapicore_ui_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/KGAPI"
    COMPONENT Devel
)

install(FILES
    ${kgapicore_base_HEADERS}
    ${kgapicore_accountinfo_HEADERS}
    ${kgapicore_ui_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapicore_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/kgapi"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPICore
    LIB_NAME KPimGAPICore
    DEPS "core network"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

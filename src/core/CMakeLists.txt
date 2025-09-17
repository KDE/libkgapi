ecm_create_qm_loader(QM_LOADER libkgapi6_qt)
add_library(KPim6GAPICore)
add_library(KPim6::GAPICore ALIAS KPim6GAPICore)

target_sources(KPim6GAPICore PRIVATE
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

if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim6GAPICore PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPim6GAPICore BASE_NAME kgapicore)


target_include_directories(KPim6GAPICore
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI;${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/kgapi>"
    INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_link_libraries(KPim6GAPICore
PUBLIC
    Qt::Core
PRIVATE
    Qt::Network
    KF6::Wallet
)

set_target_properties(KPim6GAPICore PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPICore
)

install(TARGETS
    KPim6GAPICore
    EXPORT KPim6GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapicore_base_CamelCase_HEADERS}
    ${kgapicore_accountinfo_CamelCase_HEADERS}
    ${kgapicore_ui_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/KGAPI"
    COMPONENT Devel
)

install(FILES
    ${kgapicore_base_HEADERS}
    ${kgapicore_accountinfo_HEADERS}
    ${kgapicore_ui_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapicore_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/kgapi"
    COMPONENT Devel
)

if(BUILD_QCH)
    ecm_add_qch(
        KPim6GAPICore_QCH
        NAME KPim6GAPICore
        BASE_NAME KPim6GAPICore
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kgapicore_base_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt6Core_QCH
            Qt6Gui_QCH
            Qt6Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KGAPICORE_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
   )
endif()

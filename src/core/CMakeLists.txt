set(kgapicore_SRCS
    accountinfo/accountinfo.cpp
    accountinfo/accountinfofetchjob.cpp
    private/newtokensfetchjob.cpp
    ui/authwidget.cpp
    ui/authwidget_p.cpp
    account.cpp
    authjob.cpp
    createjob.cpp
    debug.cpp
    deletejob.cpp
    fetchjob.cpp
    job.cpp
    modifyjob.cpp
    object.cpp
    utils.cpp
)

ecm_generate_headers(kgapicore_base_HEADERS
    HEADER_NAMES
    Account
    AuthJob
    CreateJob
    Debug
    DeleteJob
    FetchJob
    Job
    ModifyJob
    Object
    Types
    Utils
    REQUIRED_HEADERS kgapicore_HEADERS
)

ecm_generate_headers(kgapicore_accountinfo_HEADERS
    HEADER_NAMES
    AccountInfo
    AccountInfoFetchJob
    REQUIRED_HEADERS kgapicore_accountinfo_HEADERS
    RELATIVE accountinfo
)

ecm_generate_headers(kgapicore_ui_HEADERS
    HEADER_NAMES
    AuthWidget
    REQUIRED_HEADERS kgapicore_ui_HEADERS
    RELATIVE ui
)

add_library(KF5GAPICore
    ${kgapicore_SRCS}
)

generate_export_header(KF5GAPICore BASE_NAME kgapicore)

add_library(KF5::GAPICore ALIAS KF5GAPICore)
target_include_directories(KF5GAPICore INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KGAPI>")
target_include_directories(KF5GAPICore PUBLIC "$<BUILD_INTERFACE:${KGAPI_SOURCE_DIR}/src;${KAGAPI_BINARY_DIR}/src>")

target_link_libraries(KF5GAPICore
PUBLIC
    KF5::CoreAddons
    KF5::KDELibs4Support
    Qt5::WebKitWidgets
)

set_target_properties(KF5GAPICore PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPI
)

install(TARGETS
    KF5GAPICore
    EXPORT KF5GAPITargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapicore_base_HEADERS}
    ${kgapicore_accountinfo_HEADERS}
    ${kgapicore_ui_HEADERS}
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}/KGAPI/KGAPI"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPICore
    LIB_NAME KF5GAPICore
    DEPS "CoreAddons KDELibs4Support"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KF5_INCLUDE_INSTALL_DIR}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

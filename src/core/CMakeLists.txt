ecm_create_qm_loader(QM_LOADER libkgapi_qt)

set(kgapicore_SRCS
    accountinfo/accountinfo.cpp
    accountinfo/accountinfofetchjob.cpp
    private/newtokensfetchjob.cpp
    ui/authwidget.cpp
    ui/authwidget_p.cpp
    account.cpp
    authjob.cpp
    createjob.cpp
    deletejob.cpp
    fetchjob.cpp
    job.cpp
    modifyjob.cpp
    object.cpp
    utils.cpp
    ${QM_LOADER}

    ../debug.cpp
)

ecm_generate_headers(kgapicore_base_CamelCase_HEADERS
    HEADER_NAMES
    Account
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

ecm_generate_headers(kgapicore_ui_CamelCase_HEADERS
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
target_include_directories(KF5GAPICore INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI;${KF5_INCLUDE_INSTALL_DIR}/KGAPI/kgapi>")

target_link_libraries(KF5GAPICore
PRIVATE
    KF5::KIOWidgets
    KF5::WindowSystem
    Qt5::WebKitWidgets
PUBLIC
    Qt5::Widgets
)

set_target_properties(KF5GAPICore PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPICore
)

install(TARGETS
    KF5GAPICore
    EXPORT KF5GAPITargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapicore_base_CamelCase_HEADERS}
    ${kgapicore_accountinfo_CamelCase_HEADERS}
    ${kgapicore_ui_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI/KGAPI"
    COMPONENT Devel
)

install(FILES
    ${kgapicore_base_HEADERS}
    ${kgapicore_accountinfo_HEADERS}
    ${kgapicore_ui_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapicore_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI/kgapi"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPICore
    LIB_NAME KF5GAPICore
    DEPS "KF5::CoreAddons Qt5::Widgets Qt5::WebKitWidgets"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

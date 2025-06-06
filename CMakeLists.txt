cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "6.4.3")

set(KGAPI_LIB_VERSION ${PIM_VERSION})

project(kgapi VERSION ${KGAPI_LIB_VERSION})

# ECM setup
set(KF_MIN_VERSION "6.9.0")
set(QT_REQUIRED_VERSION "6.7.0")
find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(ECMFeatureSummary)

include(ECMPoQmTools)
include(ECMSetupVersion)
include(KDEGitCommitHooks)
include(KDEClangFormat)
include(ECMAddQch)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")
option(BUILD_SASL_PLUGIN "Build the SASL plugin (only disable this for co-installability)" ON)


set(CMAKE_AUTOMOC_MACRO_NAMES "Q_OBJECT" "Q_GADGET" "Q_NAMESPACE" "Q_NAMESPACE_EXPORT")


############## Find Packages ##############
find_package(Qt6 ${QT_REQUIRED_VERSION} REQUIRED COMPONENTS
    Core
    Network
    Widgets
    Xml
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Wallet
)

find_package(KF6CalendarCore ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6Contacts ${KF_MIN_VERSION} CONFIG REQUIRED)

if(BUILD_SASL_PLUGIN)
    find_package(Sasl2)
    set_package_properties(Sasl2 PROPERTIES TYPE REQUIRED)
endif()

ecm_setup_version(PROJECT
    VARIABLE_PREFIX KGAPI
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kgapi_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim6GAPIConfigVersion.cmake"
    SOVERSION 6
)


add_definitions(-DQT_NO_CONTEXTLESS_CONNECT)
ecm_set_disabled_deprecation_versions(QT 6.9.0 KF 6.12.0)


option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if(USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()


############## Targets ##############
add_subdirectory(src)
if(KGAPI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

############## CMake Config Files ##############
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim6GAPI")
set(KGAPI_KF6_COMPAT FALSE)

if(BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KPim6GAPIBlogger_QCH KPim6GAPICalendar_QCH
        KPim6GAPICore_QCH KPim6GAPIDrive_QCH
        KPim6GAPILatitude_QCH KPim6GAPIPeople_QCH
        KPim6GAPIMaps_QCH KPim6GAPITasks_QCH
        FILE KPim6GAPIQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
   )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KPim6GAPIQchTargets.cmake\")")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimGAPIConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6GAPIConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6GAPIConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6GAPIConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KPim6GAPITargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KPim6GAPITargets.cmake
    NAMESPACE KPim6::
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kgapi_version.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI"
    COMPONENT Devel
)



ecm_qt_install_logging_categories(
        EXPORT LIBKGAPI
        FILE libkgapi.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
       )


ecm_install_po_files_as_qm(poqm)

ecm_feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

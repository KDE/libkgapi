cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "5.24.2")

set(KGAPI_LIB_VERSION ${PIM_VERSION})

project(kgapi VERSION ${KGAPI_LIB_VERSION})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ECM setup
set(KF_MIN_VERSION "5.105.0")

find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)

include(ECMPoQmTools)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEGitCommitHooks)
include(KDEClangFormat)
include(ECMAddQch)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")


set(CMAKE_AUTOMOC_MACRO_NAMES "Q_OBJECT" "Q_GADGET" "Q_NAMESPACE" "Q_NAMESPACE_EXPORT")


############## Find Packages ##############
set(QT_REQUIRED_VERSION "5.15.2")
if (QT_MAJOR_VERSION STREQUAL "6")
    set(QT_MIN_VERSION "6.4.0")
    set(KF_MIN_VERSION "5.240.0")
    set(KF_MAJOR_VERSION "6")
else()
    set(KF_MAJOR_VERSION "5")
endif()
find_package(Qt${QT_MAJOR_VERSION} ${QT_REQUIRED_VERSION} REQUIRED COMPONENTS
    Core
    Network
    Widgets
    Xml
)

find_package(KF${KF_MAJOR_VERSION} ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Wallet
)

find_package(KF${KF_MAJOR_VERSION}CalendarCore ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Contacts ${KF_MIN_VERSION} CONFIG REQUIRED)

find_package(Sasl2)
set_package_properties(Sasl2 PROPERTIES TYPE REQUIRED)
ecm_setup_version(PROJECT
    VARIABLE_PREFIX KGAPI
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kgapi_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}GAPIConfigVersion.cmake"
    SOVERSION 5
)


ecm_set_disabled_deprecation_versions(QT 6.4  KF 5.105.0)


option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if (USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()


############## Targets ##############
add_subdirectory(src)
if (KGAPI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

############## CMake Config Files ##############
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim${KF_MAJOR_VERSION}GAPI")
set(KGAPI_KF5_COMPAT FALSE)

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KPim${KF_MAJOR_VERSION}GAPIBlogger_QCH KPim${KF_MAJOR_VERSION}GAPICalendar_QCH
        KPim${KF_MAJOR_VERSION}GAPICore_QCH KPim${KF_MAJOR_VERSION}GAPIDrive_QCH
        KPim${KF_MAJOR_VERSION}GAPILatitude_QCH KPim${KF_MAJOR_VERSION}GAPIPeople_QCH
        KPim${KF_MAJOR_VERSION}GAPIMaps_QCH KPim${KF_MAJOR_VERSION}GAPITasks_QCH
        FILE KPim${KF_MAJOR_VERSION}GAPIQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KPim${KF_MAJOR_VERSION}GAPIQchTargets.cmake\")")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimGAPIConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}GAPIConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}GAPIConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}GAPIConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KPim${KF_MAJOR_VERSION}GAPITargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KPim${KF_MAJOR_VERSION}GAPITargets.cmake
    NAMESPACE KPim${KF_MAJOR_VERSION}::
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kgapi_version.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI"
    COMPONENT Devel
)



#if (QT_MAJOR_VERSION STREQUAL "5")
##
# TODO: Backwards compatiblity. Remove in next major version
##
set(CMAKECONFIG_INSTALL_DIR_KF5 "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPimGAPI")
set(KGAPI_KF5_COMPAT TRUE)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimGAPIConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPimGAPIConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR_KF5}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPimGAPIConfig.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    COMPONENT Devel
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}GAPIConfigVersion.cmake"
    RENAME "KPimGAPIConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    COMPONENT Devel
)
install(EXPORT KPim${KF_MAJOR_VERSION}GAPITargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    FILE KPim${KF_MAJOR_VERSION}GAPITargets.cmake
    NAMESPACE KPim::
)

if (BUILD_QCH)
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}GAPIQchTargets.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
        COMPONENT Devel
    )
endif()
#endif()

ecm_qt_install_logging_categories(
        EXPORT LIBKGAPI
        FILE libkgapi.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
        )


ecm_install_po_files_as_qm(poqm)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

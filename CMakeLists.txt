cmake_minimum_required(VERSION 2.8.12)

project(kgapi)

# ECM setup
find_package(ECM 0.0.14 CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(ECMPackageConfigHelpers)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings)

set(KF5_VERSION "4.98.0")

ecm_setup_version(${KF5_VERSION}
    VARIABLE_PREFIX LIBKGAPI
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kgapi_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5GAPIConfigVersion.cmake"
    SOVERSION 5
)

############## Find Packages ##############
set(REQUIRED_QT_VERSION "5.2.0")
find_package(Qt5WebKitWidgets ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE)

find_package(KF5CoreAddons ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5CalendarCore ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5Abc ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5KDELibs4Support ${KF5_VERSION} CONFIG REQUIRED)


############## Targets ##############

add_definitions(-DTRANSLATION_DOMAIN=\"kgapi5\")

add_subdirectory(src)
#add_subdirectory(autotests)
#add_subdirectory(tests)

############## CMake Config Files ##############
message("${CMAKECONFIG_INSTALL_DIR}")
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5GAPI")

ecm_configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5GAPIConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5GAPIConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5GAPIConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5GAPIConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KF5GAPITargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KF5GAPITargets.cmake
    NAMESPACE KF5::
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kgapi_version.h"
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}"
    COMPONENT Devel
)


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

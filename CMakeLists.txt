cmake_minimum_required(VERSION 2.8.12)

project(kgapi)

# ECM setup
find_package(ECM 5.9.0 CONFIG REQUIRED)
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

set(KDEPIMLIBS_LIB_VERSION "4.74.0")
set(KF5_VERSION "5.9.0")
set(KGAPI_LIB_VERSION "4.72.0")

#add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x049999)

ecm_setup_version(${KGAPI_LIB_VERSION}
    VARIABLE_PREFIX KGAPI
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kgapi_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5GAPIConfigVersion.cmake"
    SOVERSION 5
)

############## Find Packages ##############
set(REQUIRED_QT_VERSION "5.2.0")
find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS
    Core
    Widgets
    WebKitWidgets
    Xml
)

find_package(KF5 ${KF5_VERSION} REQUIRED COMPONENTS
    CoreAddons
    KIO
    WindowSystem
)

find_package(KF5CalendarCore ${KDEPIMLIBS_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Contacts ${KDEPIMLIBS_LIB_VERSION} CONFIG REQUIRED)

############## Targets ##############

add_definitions(-DTRANSLATION_DOMAIN=\"kgapi5\")

add_subdirectory(src)
#add_subdirectory(autotests)
#add_subdirectory(tests)

############## CMake Config Files ##############
message("${CMAKECONFIG_INSTALL_DIR}")
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5GAPI")

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
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}"
    COMPONENT Devel
)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

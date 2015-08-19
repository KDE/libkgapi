cmake_minimum_required(VERSION 2.8.12)

project(kgapi)

# ECM setup
find_package(ECM 5.12.0 CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(ECMPackageConfigHelpers)
include(ECMPoQmTools)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings)

set(KGAPI_LIB_VERSION "5.0.0")

set(KDEPIMLIBS_LIB_VERSION "4.88.0")
set(KF5_VERSION "5.12.0")
set(KCALENDARCORE_LIB_VERSION "4.81.0")
set(KCONTACTS_LIB_VERSION "4.91.0")

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x049999)  #needed for QUrl::addQueryItems usage

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

find_package(KF5CalendarCore ${KCALENDARCORE_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Contacts ${KCONTACTS_LIB_VERSION} CONFIG REQUIRED)

############## Targets ##############

add_definitions(-DTRANSLATION_DOMAIN=\"libkgapi\")

add_subdirectory(src)
add_subdirectory(examples)
#add_subdirectory(autotests)

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
   ecm_install_po_files_as_qm(po)
endif()

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

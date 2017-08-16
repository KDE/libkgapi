cmake_minimum_required(VERSION 3.0)
set(PIM_VERSION "5.6.40")

set(KGAPI_LIB_VERSION ${PIM_VERSION})

project(kgapi VERSION ${KGAPI_LIB_VERSION})

# ECM setup
set(KF5_VERSION "5.37.0")
option(RUN_UNINSTALL "test run uninstalled apps (experimental)" FALSE)

set(ECM_VERSION ${KF5_VERSION})
# ECM setup
if (RUN_UNINSTALL)
   set(ECM_VERSION "5.38.0")
endif()

find_package(ECM ${ECM_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(CMakePackageConfigHelpers)
include(ECMPoQmTools)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)


set(KCALENDARCORE_LIB_VERSION "5.6.40")
set(KCONTACTS_LIB_VERSION "5.6.40")

ecm_setup_version(PROJECT
    VARIABLE_PREFIX KGAPI
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kgapi_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPimGAPIConfigVersion.cmake"
    SOVERSION 5
)

############## Find Packages ##############
set(REQUIRED_QT_VERSION "5.8.0")
find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS
    Core
    Widgets
    WebEngineWidgets
    Xml
)

find_package(KF5 ${KF5_VERSION} REQUIRED COMPONENTS
    KIO
    WindowSystem
)

find_package(KF5CalendarCore ${KCALENDARCORE_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Contacts ${KCONTACTS_LIB_VERSION} CONFIG REQUIRED)

add_definitions( -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT )
add_definitions("-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII")
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
############## Targets ##############
add_subdirectory(src)
add_subdirectory(examples)
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

############## CMake Config Files ##############
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPimGAPI")
set(KGAPI_KF5_COMPAT FALSE)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimGAPIConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPimGAPIConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPimGAPIConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPimGAPIConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KPimGAPITargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KPimGAPITargets.cmake
    NAMESPACE KPim::
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kgapi_version.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim"
    COMPONENT Devel
)


##
# TODO: Backwards compatiblity. Remove in next major version
##
set(CMAKECONFIG_INSTALL_DIR_KF5 "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5GAPI")
set(KGAPI_KF5_COMPAT TRUE)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimGAPIConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5GAPIConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR_KF5}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5GAPIConfig.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    COMPONENT Devel
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPimGAPIConfigVersion.cmake"
    RENAME "KF5GAPIConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    COMPONENT Devel
)
install(EXPORT KPimGAPITargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    FILE KPimGAPITargets.cmake
    NAMESPACE KF5::
)



feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

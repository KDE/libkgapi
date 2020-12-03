cmake_minimum_required(VERSION 3.5)
set(PIM_VERSION "5.16.40")

set(KGAPI_LIB_VERSION ${PIM_VERSION})

project(kgapi VERSION ${KGAPI_LIB_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ECM setup
set(KF5_MIN_VERSION "5.76.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMPoQmTools)
include(ECMSetupVersion)
include(FeatureSummary)

set(CMAKE_AUTOMOC_MACRO_NAMES "Q_OBJECT" "Q_GADGET" "Q_NAMESPACE" "Q_NAMESPACE_EXPORT")

ecm_setup_version(PROJECT
    VARIABLE_PREFIX KGAPI
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kgapi_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPimGAPIConfigVersion.cmake"
    SOVERSION 5
)

############## Find Packages ##############
set(QT_REQUIRED_VERSION "5.13.0")
find_package(Qt5 ${QT_REQUIRED_VERSION} REQUIRED COMPONENTS
    Core
    Network
    Widgets
    Xml
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Wallet
)

find_package(KF5CalendarCore ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Contacts ${KF5_MIN_VERSION} CONFIG REQUIRED)

find_package(Sasl2)
set_package_properties(Sasl2 PROPERTIES TYPE REQUIRED)


add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f00)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x054D00)

add_definitions(-DQT_NO_EMIT)

############## Targets ##############
add_subdirectory(src)
if (KGAPI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
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


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

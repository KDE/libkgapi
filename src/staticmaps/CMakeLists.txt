add_library(KPimGAPIMaps)
add_library(KPim::GAPIMaps ALIAS KPimGAPIMaps)
target_sources(KPimGAPIMaps PRIVATE
    ${libkgapi_debug_SRCS}
    staticmapmarker.cpp
    staticmapmarker.h
    staticmappath.cpp
    staticmappath.h
    staticmaptilefetchjob.cpp
    staticmaptilefetchjob.h
    staticmapurl.cpp
    staticmapurl.h
)

ecm_generate_headers(kgapimaps_CamelCase_HEADERS
    HEADER_NAMES
    StaticMapMarker
    StaticMapPath
    StaticMapTileFetchJob
    StaticMapUrl
    PREFIX KGAPI/Maps
    REQUIRED_HEADERS kgapimaps_HEADERS
)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPimGAPIMaps PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPimGAPIMaps BASE_NAME kgapimaps)

target_include_directories(KPimGAPIMaps INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI>")

target_link_libraries(KPimGAPIMaps
PUBLIC
    KPim::GAPICore
    KF${KF_MAJOR_VERSION}::Contacts
    Qt::Gui
PRIVATE
    Qt::Network
)

set_target_properties(KPimGAPIMaps PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIMaps
)

install(TARGETS
    KPimGAPIMaps
    EXPORT KPimGAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapimaps_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/KGAPI/Maps"
    COMPONENT Devel
)

install(FILES
    ${kgapimaps_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapimaps_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/kgapi/maps"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIMaps
    LIB_NAME KPimGAPIMaps
    DEPS "GAPICore KContacts"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

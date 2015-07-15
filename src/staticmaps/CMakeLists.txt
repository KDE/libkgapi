set(kgapimaps_SRCS
    staticmapmarker.cpp
    staticmappath.cpp
    staticmaptilefetchjob.cpp
    staticmapurl.cpp

    ../debug.cpp
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

add_library(KF5GAPIMaps
    ${kgapimaps_SRCS}
)

generate_export_header(KF5GAPIMaps BASE_NAME kgapimaps)

add_library(KF5::GAPIMaps ALIAS KF5GAPIMaps)
target_include_directories(KF5GAPIMaps INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI>")

target_link_libraries(KF5GAPIMaps
PUBLIC
    KF5::GAPICore
    KF5::Contacts
PRIVATE
    Qt5::Network
)

set_target_properties(KF5GAPIMaps PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIMaps
)

install(TARGETS
    KF5GAPIMaps
    EXPORT KF5GAPITargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapimaps_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI/KGAPI/Maps"
    COMPONENT Devel
)

install(FILES
    ${kgapimaps_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapimaps_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI/kgapi/maps"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIMaps
    LIB_NAME KF5GAPIMaps
    DEPS "GAPICore Contacts"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

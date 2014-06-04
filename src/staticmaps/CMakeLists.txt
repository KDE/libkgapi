set(kgapimaps_SRCS
    staticmapmarker.cpp
    staticmappath.cpp
    staticmaptilefetchjob.cpp
    staticmapurl.cpp
)

ecm_generate_headers(kgapimaps_CamelCase_HEADERS
    HEADER_NAMES
    StaticMapMarker
    StaticMapPath
    StaticMapTileFetchJob
    StaticMapUrl
    REQUIRED_HEADERS kgapimaps_HEADERS
)

add_library(KF5GAPIMaps
    ${kgapimaps_SRCS}
)

generate_export_header(KF5GAPIMaps BASE_NAME kgapimaps)

add_library(KF5::GAPIMaps ALIAS KF5GAPIMaps)
target_include_directories(KF5GAPIMaps INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KGAPI>")
target_include_directories(KF5GAPIMaps PUBLIC "$<BUILD_INTERFACE:${KGAPI_SOURCE_DIR}/src;${KAGAPI_BINARY_DIR}/src>")

target_link_libraries(KF5GAPIMaps
PUBLIC
    KF5::GAPICore
    KF5::Abc
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
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}/KGAPI/KGAPI/Maps"
    COMPONENT Devel
)

install(FILES
    ${kgapimaps_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapimaps_export.h"
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}/KGAPI/kgapi/maps"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIMaps
    LIB_NAME KF5GAPIMaps
    DEPS "GAPICore Abc"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KF5_INCLUDE_INSTALL_DIR}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

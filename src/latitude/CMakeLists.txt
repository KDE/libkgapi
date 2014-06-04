set(kgapilatitude_SRCS
    latitudeservice.cpp
    latitude.h
    location.cpp
    locationcreatejob.cpp
    locationdeletejob.cpp
    locationfetchhistoryjob.cpp
    locationfetchjob.cpp
)

ecm_generate_headers(kgapilatitude_CamelCase_HEADERS
    HEADER_NAMES
    Location
    LocationCreateJob
    LocationDeleteJob
    LocationFetchHistoryJob
    LocationFetchJob
    Latitude
    REQUIRED_HEADERS kgapilatitude_HEADERS
)

add_library(KF5GAPILatitude
    ${kgapilatitude_SRCS}
)

generate_export_header(KF5GAPILatitude BASE_NAME kgapilatitude)

add_library(KF5::GAPILatitude ALIAS KF5GAPILatitude)
target_include_directories(KF5GAPILatitude INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KGAPI>")
target_include_directories(KF5GAPILatitude PUBLIC "$<BUILD_INTERFACE:${KGAPI_SOURCE_DIR}/src;${KAGAPI_BINARY_DIR}/src>")

target_link_libraries(KF5GAPILatitude
PUBLIC
    KF5::GAPICore
    KF5::Abc
)

set_target_properties(KF5GAPILatitude PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPILatitude
)

install(TARGETS
    KF5GAPILatitude
    EXPORT KF5GAPITargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapilatitude_CamelCase_HEADERS}
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}/KGAPI/KGAPI/Latitude"
    COMPONENT Devel
)

install(FILES
    ${kgapilatitude_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapilatitude_export.h"
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}/KGAPI/kgapi/latitude"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPILatitude
    LIB_NAME KF5GAPILatitude
    DEPS "GAPICore Abc"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KF5_INCLUDE_INSTALL_DIR}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

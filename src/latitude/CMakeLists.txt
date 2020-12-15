set(kgapilatitude_SRCS
    latitudeservice.cpp
    latitude.h
    location.cpp
    locationcreatejob.cpp
    locationdeletejob.cpp
    locationfetchhistoryjob.cpp
    locationfetchjob.cpp

    ../debug.cpp
)

ecm_generate_headers(kgapilatitude_CamelCase_HEADERS
    HEADER_NAMES
    Location
    LocationCreateJob
    LocationDeleteJob
    LocationFetchHistoryJob
    LocationFetchJob
    Latitude
    PREFIX KGAPI/Latitude
    REQUIRED_HEADERS kgapilatitude_HEADERS
)

add_library(KPimGAPILatitude
    ${kgapilatitude_SRCS}
)
if (COMPILE_WITH_CMAKE_SUPPORT)
    set_target_properties(KPimGAPILatitude PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPimGAPILatitude BASE_NAME kgapilatitude)

add_library(KPim::GAPILatitude ALIAS KPimGAPILatitude)

# TODO: Backwards compatibility, remove in next major release
add_library(KF5::GAPILatitude ALIAS KPimGAPILatitude)
target_include_directories(KPimGAPILatitude INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI>")

target_link_libraries(KPimGAPILatitude
PUBLIC
    KPim::GAPICore
    KF5::Contacts
PRIVATE
    Qt5::Network
)

set_target_properties(KPimGAPILatitude PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPILatitude
)

install(TARGETS
    KPimGAPILatitude
    EXPORT KPimGAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapilatitude_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/KGAPI/Latitude"
    COMPONENT Devel
)

install(FILES
    ${kgapilatitude_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapilatitude_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/kgapi/latitude"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPILatitude
    LIB_NAME KPimGAPILatitude
    DEPS "GAPICore KContacts"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

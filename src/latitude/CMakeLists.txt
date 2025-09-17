add_library(KPim6GAPILatitude)
add_library(KPim6::GAPILatitude ALIAS KPim6GAPILatitude)

target_sources(KPim6GAPILatitude PRIVATE
    ${libkgapi_debug_SRCS}
    latitude.h
    latitudeservice.cpp
    latitudeservice.h
    location.cpp
    locationcreatejob.cpp
    locationcreatejob.h
    locationdeletejob.cpp
    locationdeletejob.h
    locationfetchhistoryjob.cpp
    locationfetchhistoryjob.h
    locationfetchjob.cpp
    locationfetchjob.h
    location.h
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

if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim6GAPILatitude PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPim6GAPILatitude BASE_NAME kgapilatitude)


target_include_directories(KPim6GAPILatitude INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI>")

target_link_libraries(KPim6GAPILatitude
PUBLIC
    KPim6::GAPICore
    KF6::Contacts
PRIVATE
    Qt::Network
)

set_target_properties(KPim6GAPILatitude PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPILatitude
)

install(TARGETS
    KPim6GAPILatitude
    EXPORT KPim6GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapilatitude_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/KGAPI/Latitude"
    COMPONENT Devel
)

install(FILES
    ${kgapilatitude_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapilatitude_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/kgapi/latitude"
    COMPONENT Devel
)

if(BUILD_QCH)
    ecm_add_qch(
        KPim6GAPILatitude_QCH
        NAME KPim6GAPILatitude
        BASE_NAME KPim6GAPILatitude
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kgapilatitude_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt6Core_QCH
            Qt6Gui_QCH
            Qt6Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KGAPILATITUDE_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
   )
endif()

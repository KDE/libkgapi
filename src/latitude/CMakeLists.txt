add_library(KPim${KF_MAJOR_VERSION}GAPILatitude)
add_library(KPim${KF_MAJOR_VERSION}::GAPILatitude ALIAS KPim${KF_MAJOR_VERSION}GAPILatitude)

target_sources(KPim${KF_MAJOR_VERSION}GAPILatitude PRIVATE
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

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim${KF_MAJOR_VERSION}GAPILatitude PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPim${KF_MAJOR_VERSION}GAPILatitude BASE_NAME kgapilatitude)


target_include_directories(KPim${KF_MAJOR_VERSION}GAPILatitude INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI>")

target_link_libraries(KPim${KF_MAJOR_VERSION}GAPILatitude
PUBLIC
    KPim${KF_MAJOR_VERSION}::GAPICore
    KF${KF_MAJOR_VERSION}::Contacts
PRIVATE
    Qt::Network
)

set_target_properties(KPim${KF_MAJOR_VERSION}GAPILatitude PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPILatitude
)

install(TARGETS
    KPim${KF_MAJOR_VERSION}GAPILatitude
    EXPORT KPim${KF_MAJOR_VERSION}GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapilatitude_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/KGAPI/Latitude"
    COMPONENT Devel
)

install(FILES
    ${kgapilatitude_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapilatitude_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/kgapi/latitude"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPILatitude
    LIB_NAME KPim${KF_MAJOR_VERSION}GAPILatitude
    DEPS "GAPICore KContacts"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

if (BUILD_QCH)
    ecm_add_qch(
        KPim${KF_MAJOR_VERSION}GAPILatitude_QCH
        NAME KPim${KF_MAJOR_VERSION}GAPILatitude
        BASE_NAME KPim${KF_MAJOR_VERSION}GAPILatitude
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kgapilatitude_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt${QT_MAJOR_VERSION}Core_QCH
            Qt${QT_MAJOR_VERSION}Gui_QCH
            Qt${QT_MAJOR_VERSION}Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KGAPILATITUDE_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()


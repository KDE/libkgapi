add_library(KPim6GAPIMaps)
add_library(KPim6::GAPIMaps ALIAS KPim6GAPIMaps)
target_sources(KPim6GAPIMaps PRIVATE
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
    set_target_properties(KPim6GAPIMaps PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPim6GAPIMaps BASE_NAME kgapimaps)

target_include_directories(KPim6GAPIMaps INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI>")

target_link_libraries(KPim6GAPIMaps
PUBLIC
    KPim6::GAPICore
    KF6::Contacts
    Qt::Gui
PRIVATE
    Qt::Network
)

set_target_properties(KPim6GAPIMaps PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIMaps
)

install(TARGETS
    KPim6GAPIMaps
    EXPORT KPim6GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapimaps_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/KGAPI/Maps"
    COMPONENT Devel
)

install(FILES
    ${kgapimaps_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapimaps_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim6/KGAPI/kgapi/maps"
    COMPONENT Devel
)

if (BUILD_QCH)
    ecm_add_qch(
        KPim6GAPIMaps_QCH
        NAME KPim6GAPIMaps
        BASE_NAME KPim6GAPIMaps
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kgapimaps_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt6Core_QCH
            Qt6Gui_QCH
            Qt6Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KGAPIMAPS_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

project(libkgoogle)
set(KGOOGLE_VERSION_MAJOR 0)
set(KGOOGLE_VESRION_MINOR 2)
set(KGOOGLE_VERSION 0.2)

cmake_minimum_required(VERSION 2.8)

add_definitions(-DLIBKGOOGLE_LIBRARY)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

set(libkgoogle_SRCS
    authdialog.cpp
    kgoogleaccessmanager.cpp 
    kgoogleauth.cpp
    kgoogleobject.cpp
    kgooglereply.cpp
    kgooglerequest.cpp 
    objects/calendar.cpp
    objects/contact.cpp
    objects/event.cpp
    services/addressbook.cpp
    services/calendar.cpp
)

set(libkgoogle_HDRS
    libkgoogle_export.h
    kgoogleaccessmanager.h
    kgoogleauth.h
    kgoogleobject.h
    kgooglereply.h
    kgooglerequest.h
    kgoogleservice.h)

set(services_HDRS
    services/addressbook.h
    services/calendar.h)

set(objects_HDRS
    objects/calendar.h
    objects/contact.h
    objects/event.h)

kde4_add_library(kgoogle SHARED ${libkgoogle_SRCS})

set_target_properties(kgoogle PROPERTIES VERSION ${KGOOGLE_VERSION} SOVERSION ${KGOOGLE_VERSION_MAJOR} )

target_link_libraries(kgoogle
		      ${KDE4_KDECORE_LIBS}
		      ${KDE4_KDEWEBKIT_LIBS}
		      ${KDE4_KDEUI_LIBS}
		      ${KDEPIMLIBS_KABC_LIBS}
		      ${KDEPIMLIBS_KCALCORE_LIBS}
		      ${KDEPIMLIBS_KMIME_LIBS}
		      ${QJSON_LIBRARIES}
		      ${QT_QTCORE_LIBRARY}
		      ${QT_QTWEBKIT_LIBRARY}
		      ${QT_QTNETWORK_LIBRARY}
)

install(TARGETS kgoogle ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${libkgoogle_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgoogle)
install(FILES ${services_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgoogle/services)
install(FILES ${objects_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgoogle/objects)

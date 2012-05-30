set(libkgapi_SRCS
    accessmanager.cpp
    accessmanager_p.cpp
    account.cpp
    auth.cpp
    auth_p.cpp
    fetchlistjob.cpp
    object.cpp
    request.cpp
    reply.cpp
    objects/accountinfo.cpp
    objects/calendar.cpp
    objects/contact.cpp
    objects/contactsgroup.cpp
    objects/event.cpp
    objects/task.cpp
    objects/tasklist.cpp
    services/accountinfo.cpp
    services/calendar.cpp
    services/contacts.cpp
    services/tasks.cpp
    ui/accountscombo.cpp
    ui/authwidget.cpp
    ui/authwidget_p.cpp
)

set(libkgapi_HDRS
    libkgapi_export.h
    accessmanager.h
    account.h
    auth.h
    common.h
    fetchlistjob.h
    object.h
    reply.h
    request.h
    service.h)

set(services_HDRS
    services/accountinfo.h
    services/calendar.h
    services/contacts.h
    services/tasks.h)

set(objects_HDRS
    objects/accountinfo.h
    objects/calendar.h
    objects/contact.h
    objects/contactsgroup.h
    objects/event.h
    objects/task.h
    objects/tasklist.h)

set(ui_HDRS
    ui/accountscombo.h
    ui/authwidget.h)

kde4_add_library(kgapi SHARED ${libkgapi_SRCS})

set_target_properties(kgapi PROPERTIES VERSION ${KGAPI_VERSION} SOVERSION ${KGAPI_VERSION_MAJOR} )

target_link_libraries(kgapi
		      ${KDE4_KDECORE_LIBS}
		      ${KDE4_KDEWEBKIT_LIBS}
		      ${KDE4_KDEUI_LIBS}
		      ${KDEPIMLIBS_KABC_LIBS}
		      ${KDEPIMLIBS_KMIME_LIBS}
              ${qjson_LIBRARIES}        # for Debian
		      ${QJSON_LIBRARIES}
		      ${QT_QTCORE_LIBRARY}
		      ${QT_QTWEBKIT_LIBRARY}
		      ${QT_QTNETWORK_LIBRARY}
		      ${QT_QTXML_LIBRARY}
		      ${KCALENDAR_LIBRARIES}
)

if(NOT WIN32)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libkgapi.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/libkgapi.pc)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libkgapi.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig )
endif(NOT WIN32)

install(TARGETS kgapi EXPORT kgapiLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${libkgapi_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi)
install(FILES ${services_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi/services)
install(FILES ${objects_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi/objects)
install(FILES ${ui_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi/ui)


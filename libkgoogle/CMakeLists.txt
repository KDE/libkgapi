set(libkgoogle_SRCS
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

set(libkgoogle_HDRS
    libkgoogle_export.h
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

kde4_add_library(kgoogle SHARED ${libkgoogle_SRCS})

set_target_properties(kgoogle PROPERTIES VERSION ${KGOOGLE_VERSION} SOVERSION ${KGOOGLE_VERSION_MAJOR} )

target_link_libraries(kgoogle
		      ${KDE4_KDECORE_LIBS}
		      ${KDE4_KDEWEBKIT_LIBS}
		      ${KDE4_KDEUI_LIBS}
		      ${KDEPIMLIBS_KABC_LIBS}
		      ${KDEPIMLIBS_KMIME_LIBS}
		      ${QJSON_LIBRARIES}
		      ${QT_QTCORE_LIBRARY}
		      ${QT_QTWEBKIT_LIBRARY}
		      ${QT_QTNETWORK_LIBRARY}
		      ${QT_QTXML_LIBRARY}
		      ${KCALENDAR_LIBRARIES}
)

if(NOT WIN32)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libkgoogle.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/libkgoogle.pc)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libkgoogle.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig )
endif(NOT WIN32)

install(TARGETS kgoogle EXPORT kgoogleLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${libkgoogle_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgoogle)
install(FILES ${services_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgoogle/services)
install(FILES ${objects_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgoogle/objects)
install(FILES ${ui_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgoogle/ui)


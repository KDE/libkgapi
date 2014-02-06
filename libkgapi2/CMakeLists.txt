add_subdirectory(includes)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(LibKGAPI_core_SRCS
    account.cpp
    authjob.cpp
    createjob.cpp
    debug.cpp
    deletejob.cpp
    fetchjob.cpp
    job.cpp
    modifyjob.cpp
    object.cpp
    utils.cpp
)

set(LibKGAPI_core_HDRS
    account.h
    authjob.h
    createjob.h
    deletejob.h
    fetchjob.h
    job.h
    modifyjob.h
    object.h
    types.h
    ${CMAKE_CURRENT_BINARY_DIR}/libkgapi2_export.h
)

set(LibKGAPI_accountinfo_SRCS
    accountinfo/accountinfo.cpp
    accountinfo/accountinfofetchjob.cpp
)

set(LibKGAPI_accountinfo_HDRS
    accountinfo/accountinfo.h
    accountinfo/accountinfofetchjob.h
)

set(LibKGAPI_contacts_SRCS
    contacts/contact.cpp
    contacts/contactcreatejob.cpp
    contacts/contactdeletejob.cpp
    contacts/contactfetchjob.cpp
    contacts/contactfetchphotojob.cpp
    contacts/contactmodifyjob.cpp
    contacts/contactsgroup.cpp
    contacts/contactsgroupcreatejob.cpp
    contacts/contactsgroupdeletejob.cpp
    contacts/contactsgroupfetchjob.cpp
    contacts/contactsgroupmodifyjob.cpp
    contacts/contactsservice.cpp
)

set(LibKGAPI_contacts_HDRS
    contacts/contact.h
    contacts/contactcreatejob.h
    contacts/contactdeletejob.h
    contacts/contactfetchjob.h
    contacts/contactfetchphotojob.h
    contacts/contactmodifyjob.h
    contacts/contactsgroup.h
    contacts/contactsgroupcreatejob.h
    contacts/contactsgroupdeletejob.h
    contacts/contactsgroupfetchjob.h
    contacts/contactsgroupmodifyjob.h
)

set(LibKGAPI_calendar_SRCS
    calendar/calendar.cpp
    calendar/calendarcreatejob.cpp
    calendar/calendardeletejob.cpp
    calendar/calendarfetchjob.cpp
    calendar/calendarmodifyjob.cpp
    calendar/calendarservice.cpp
    calendar/event.cpp
    calendar/eventcreatejob.cpp
    calendar/eventdeletejob.cpp
    calendar/eventfetchjob.cpp
    calendar/eventmodifyjob.cpp
    calendar/eventmovejob.cpp
    calendar/reminder.cpp
)

set(LibKGAPI_calendar_HDRS
    calendar/calendar.h
    calendar/calendarcreatejob.h
    calendar/calendardeletejob.h
    calendar/calendarfetchjob.h
    calendar/calendarmodifyjob.h
    calendar/event.h
    calendar/eventcreatejob.h
    calendar/eventdeletejob.h
    calendar/eventfetchjob.h
    calendar/eventmodifyjob.h
    calendar/eventmovejob.h
    calendar/reminder.h
)

set(LibKGAPI_drive_SRCS
    drive/about.cpp
    drive/aboutfetchjob.cpp
    drive/app.cpp
    drive/change.cpp
    drive/changefetchjob.cpp
    drive/childreference.cpp
    drive/childreferencecreatejob.cpp
    drive/childreferencedeletejob.cpp
    drive/childreferencefetchjob.cpp
    drive/driveservice.cpp
    drive/file.cpp
    drive/fileabstractdatajob.cpp
    drive/fileabstractmodifyjob.cpp
    drive/fileabstractuploadjob.cpp
    drive/filecopyjob.cpp
    drive/filecreatejob.cpp
    drive/filedeletejob.cpp
    drive/filefetchjob.cpp
    drive/filefetchcontentjob.cpp
    drive/filemodifyjob.cpp
    drive/filetouchjob.cpp
    drive/filetrashjob.cpp
    drive/fileuntrashjob.cpp
    drive/parentreference.cpp
    drive/parentreferencecreatejob.cpp
    drive/parentreferencedeletejob.cpp
    drive/parentreferencefetchjob.cpp
    drive/permission.cpp
    drive/permissioncreatejob.cpp
    drive/permissiondeletejob.cpp
    drive/permissionfetchjob.cpp
    drive/permissionmodifyjob.cpp
    drive/revision.cpp
    drive/revisiondeletejob.cpp
    drive/revisionfetchjob.cpp
    drive/revisionmodifyjob.cpp
    drive/user.cpp
)

set(LibKGAPI_drive_HDRS
    drive/about.h
    drive/aboutfetchjob.h
    drive/app.h
    drive/change.h
    drive/changefetchjob.h
    drive/childreference.h
    drive/childreferencecreatejob.h
    drive/childreferencedeletejob.h
    drive/childreferencefetchjob.h
    drive/driveservice.h
    drive/file.h
    drive/fileabstractdatajob.h
    drive/fileabstractmodifyjob.h
    drive/fileabstractuploadjob.h
    drive/filecopyjob.h
    drive/filecreatejob.h
    drive/filedeletejob.h
    drive/filefetchjob.h
    drive/filefetchcontentjob.h
    drive/filemodifyjob.h
    drive/filetouchjob.h
    drive/filetrashjob.h
    drive/fileuntrashjob.h
    drive/parentreference.h
    drive/parentreferencecreatejob.h
    drive/parentreferencedeletejob.h
    drive/parentreferencefetchjob.h
    drive/permission.h
    drive/permissioncreatejob.h
    drive/permissiondeletejob.h
    drive/permissionfetchjob.h
    drive/permissionmodifyjob.h
    drive/revision.h
    drive/revisiondeletejob.h
    drive/revisionfetchjob.h
    drive/revisionmodifyjob.h
    drive/user.h
)

set(LibKGAPI_latitude_SRCS
    latitude/latitudeservice.cpp
    latitude/location.cpp
    latitude/locationcreatejob.cpp
    latitude/locationdeletejob.cpp
    latitude/locationfetchhistoryjob.cpp
    latitude/locationfetchjob.cpp
)

set(LibKGAPI_latitude_HDRS
    latitude/location.h
    latitude/locationcreatejob.h
    latitude/locationdeletejob.h
    latitude/locationfetchhistoryjob.h
    latitude/locationfetchjob.h
)

set(LibKGAPI_private_SRCS
    private/newtokensfetchjob.cpp
)

set(LibKGAPI_staticMaps_SRCS
    staticmaps/staticmapmarker.cpp
    staticmaps/staticmappath.cpp
    staticmaps/staticmaptilefetchjob.cpp
    staticmaps/staticmapurl.cpp
)

set(LibKGAPI_staticMaps_HDRS
    staticmaps/staticmapmarker.h
    staticmaps/staticmappath.h
    staticmaps/staticmaptilefetchjob.h
    staticmaps/staticmapurl.h
)

set(LibKGAPI_tasks_SRCS
    tasks/task.cpp
    tasks/taskcreatejob.cpp
    tasks/taskdeletejob.cpp
    tasks/taskfetchjob.cpp
    tasks/taskmodifyjob.cpp
    tasks/taskmovejob.cpp
    tasks/tasklist.cpp
    tasks/tasklistcreatejob.cpp
    tasks/tasklistfetchjob.cpp
    tasks/tasklistmodifyjob.cpp
    tasks/tasklistdeletejob.cpp
    tasks/tasksservice.cpp
)

set(LibKGAPI_tasks_HDRS
    tasks/task.h
    tasks/taskcreatejob.h
    tasks/taskdeletejob.h
    tasks/taskfetchjob.h
    tasks/taskmodifyjob.h
    tasks/taskmovejob.h
    tasks/tasklist.h
    tasks/tasklistcreatejob.h
    tasks/tasklistdeletejob.h
    tasks/tasklistfetchjob.h
    tasks/tasklistmodifyjob.h
)

set(LibKGAPI_ui_SRCS
    ui/authwidget.cpp
    ui/authwidget_p.cpp
)

set(LibKGAPI_ui_HDRS
    ui/authwidget.h
)

# === Create target

kde4_add_library(kgapi2 SHARED
    ${LibKGAPI_core_SRCS}
    ${LibKGAPI_accountinfo_SRCS}
    ${LibKGAPI_contacts_SRCS}
    ${LibKGAPI_calendar_SRCS}
    ${LibKGAPI_drive_SRCS}
    ${LibKGAPI_latitude_SRCS}
    ${LibKGAPI_private_SRCS}
    ${LibKGAPI_staticMaps_SRCS}
    ${LibKGAPI_tasks_SRCS}
    ${LibKGAPI_ui_SRCS}
)

generate_export_header(kgapi2 BASE_NAME libkgapi2)

set_target_properties(kgapi2
    PROPERTIES VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_VERSION_MAJOR})

target_link_libraries(kgapi2
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEWEBKIT_LIBS}
    ${KDE4_KDEUI_LIBS}
    ${KDEPIMLIBS_KABC_LIBS}
    ${KDEPIMLIBS_KCALCORE_LIBS}
    ${KDEPIMLIBS_KMIME_LIBS}
    ${qjson_LIBRARIES}        # for Debian
    ${QJSON_LIBRARY}          # for Mac OSX
    ${QJSON_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTXML_LIBRARY}
)

# === Install target

install(TARGETS kgapi2 EXPORT kgapi2LibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${LibKGAPI_core_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi2)
install(FILES ${LibKGAPI_accountinfo_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi2/accountinfo)
install(FILES ${LibKGAPI_contacts_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi2/contacts)
install(FILES ${LibKGAPI_calendar_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi2/calendar)
install(FILES ${LibKGAPI_drive_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi2/drive)
install(FILES ${LibKGAPI_latitude_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi2/latitude)
install(FILES ${LibKGAPI_staticMaps_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi2/staticmaps)
install(FILES ${LibKGAPI_tasks_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi2/tasks)
install(FILES ${LibKGAPI_ui_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/libkgapi2/ui)

# === CMake stuff

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/LibKGAPI2Config.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/LibKGAPI2Config.cmake @ONLY)

# this file is used by to check if the installed version can be used.
macro_write_basic_cmake_version_file(${CMAKE_CURRENT_BINARY_DIR}/LibKGAPI2ConfigVersion.cmake
                                     ${KGAPI_VERSION_MAJOR} ${KGAPI_VERSION_MINOR} ${KGAPI_VERSION_RELEASE})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LibKGAPI2Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/LibKGAPI2ConfigVersion.cmake
        DESTINATION ${LIB_INSTALL_DIR}/cmake/LibKGAPI2)

# Install the file with the exported targets
install(EXPORT kgapi2LibraryTargets
        DESTINATION ${LIB_INSTALL_DIR}/cmake/LibKGAPI2
        FILE LibKGAPI2TargetsWithPrefix.cmake)

add_subdirectory(blogger)
add_subdirectory(calendar)
add_subdirectory(contacts)
add_subdirectory(core)
add_subdirectory(drive)
add_subdirectory(latitude)
add_subdirectory(staticmaps)
add_subdirectory(tasks)

if (NOT WIN32)
    add_subdirectory(saslplugin)
endif()

if (${ECM_VERSION} STRGREATER "5.58.0")
    install(FILES libkgapi.categories  DESTINATION  ${KDE_INSTALL_LOGGINGCATEGORIESDIR})
else()
    install(FILES libkgapi.categories  DESTINATION ${KDE_INSTALL_CONFDIR})
endif()


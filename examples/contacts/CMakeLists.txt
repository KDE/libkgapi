add_executable(contacts-example)

target_sources(contacts-example PRIVATE
    main.cpp
    mainwindow.cpp mainwindow.h
    mainwindow.ui
)

target_link_libraries(contacts-example
        KF${KF_MAJOR_VERSION}::Contacts
        Qt::Widgets
        KPim${KF_MAJOR_VERSION}GAPICore
        KPim${KF_MAJOR_VERSION}GAPIContacts
)

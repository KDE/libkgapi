add_executable(contacts-example)

target_sources(contacts-example PRIVATE
    main.cpp
    mainwindow.cpp
    mainwindow.ui
)

target_link_libraries(contacts-example
        KF5::Contacts
        Qt::Widgets
        KPimGAPICore
        KPimGAPIContacts
)

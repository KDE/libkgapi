add_executable(contacts-example)

target_sources(contacts-example PRIVATE
    main.cpp
    mainwindow.cpp mainwindow.h
    mainwindow.ui
)

target_link_libraries(contacts-example
        KF6::Contacts
        Qt::Widgets
        KPim6GAPICore
        KPim6GAPIContacts
)

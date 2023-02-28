add_executable(staticmaps-example)

target_sources(staticmaps-example PRIVATE
    main.cpp
    mainwindow.cpp mainwindow.h
    mainwindow.ui
)

target_link_libraries(staticmaps-example
        Qt::Widgets
        KF${KF_MAJOR_VERSION}::Contacts
        KPim${KF_MAJOR_VERSION}::GapiStaticMaps
)


add_executable(files-example)

target_sources(files-example PRIVATE
    main.cpp
    mainwindow.cpp mainwindow.h
    mainwindow.ui
)

target_link_libraries(files-example
        Qt::Widgets
        Qt::Core
        KPim${KF_MAJOR_VERSION}GAPICore
        KPim${KF_MAJOR_VERSION}GAPIDrive
)

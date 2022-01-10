add_executable(drives-example)

target_sources(drives-example PRIVATE
    main.cpp
    mainwindow.cpp
    mainwindow.ui
)

target_link_libraries(drives-example
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Widgets
        KPimGAPICore
        KPimGAPIDrive
)

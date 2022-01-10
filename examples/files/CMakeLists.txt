add_executable(files-example)

target_sources(files-example PRIVATE
    main.cpp
    mainwindow.cpp
    mainwindow.ui
)

target_link_libraries(files-example
        Qt${QT_MAJOR_VERSION}::Widgets
        Qt${QT_MAJOR_VERSION}::Core
        KPimGAPICore
        KPimGAPIDrive
)

add_executable(files-example)

target_sources(files-example PRIVATE
    main.cpp
    mainwindow.cpp
    mainwindow.ui
)

target_link_libraries(files-example
        Qt::Widgets
        Qt::Core
        KPimGAPICore
        KPimGAPIDrive
)

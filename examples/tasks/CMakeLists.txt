add_executable(tasks-example)

target_sources(tasks-example PRIVATE
    main.cpp
    mainwindow.cpp
    mainwindow.ui
)

target_link_libraries(tasks-example
        Qt::Widgets
        Qt::Core
        KPimGAPICore
        KPimGAPITasks
)

add_executable(drives-example)

target_sources(drives-example PRIVATE
    main.cpp
    mainwindow.cpp mainwindow.h
    mainwindow.ui
)

target_link_libraries(drives-example
        Qt::Core
        Qt::Widgets
        KPim6GAPICore
        KPim6GAPIDrive
)

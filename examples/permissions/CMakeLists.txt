add_executable(permissions-example)

target_sources(permissions-example PRIVATE
    main.cpp
    mainwindow.cpp mainwindow.h
    mainwindow.ui
)

target_link_libraries(permissions-example
        Qt::Widgets
        Qt::Core
        KPim6GAPICore
        KPim6GAPIDrive
)

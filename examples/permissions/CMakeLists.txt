add_executable(permissions-example)

target_sources(permissions-example PRIVATE
    main.cpp
    mainwindow.cpp
    mainwindow.ui
)

target_link_libraries(permissions-example
        Qt${QT_MAJOR_VERSION}::Widgets
        Qt${QT_MAJOR_VERSION}::Core
        KPimGAPICore
        KPimGAPIDrive
)

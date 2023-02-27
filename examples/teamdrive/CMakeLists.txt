add_executable(teamdrive-example)

target_sources(teamdrive-example PRIVATE
    main.cpp
    mainwindow.cpp mainwindow.h
    mainwindow.ui
)

# The Team Drives example is, inevitably, dependent on deprecated APIs.
target_compile_options(teamdrive-example PRIVATE -Wno-deprecated-declarations)

target_link_libraries(teamdrive-example
        Qt::Widgets
        Qt::Core
        KPim${KF_MAJOR_VERSION}GAPICore
        KPim${KF_MAJOR_VERSION}GAPIDrive
)

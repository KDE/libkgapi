add_executable(teamdrive-example)

target_sources(teamdrive-example PRIVATE
    main.cpp
    mainwindow.cpp
    mainwindow.ui
)

# The Team Drives example is, inevitably, dependent on deprecated APIs.
target_compile_options(teamdrive-example PRIVATE -Wno-deprecated-declarations)

target_link_libraries(teamdrive-example
        Qt${QT_MAJOR_VERSION}::Widgets
        Qt${QT_MAJOR_VERSION}::Core
        KPimGAPICore
        KPimGAPIDrive
)

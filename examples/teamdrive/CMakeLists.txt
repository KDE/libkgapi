kde_enable_exceptions()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
)

set(teamdrive_example_SRCS main.cpp mainwindow.cpp)
set(teamdrive_example_HDRS mainwindow.h)
qt_wrap_ui(teamdrive_example_SRCS ui/main.ui)

add_executable(teamdrive-example
        ${teamdrive_example_SRCS}
        ${teamdrive_example_HDRS_MOC}
)

# The Team Drives example is, inevitably, dependent on deprecated APIs.
target_compile_options(teamdrive-example PRIVATE -Wno-deprecated-declarations)

target_link_libraries(teamdrive-example
        Qt::Widgets
        Qt::Core
        KF5::GAPICore
        KF5::GAPIDrive
)

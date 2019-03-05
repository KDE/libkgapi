kde_enable_exceptions()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
)

set(teamdrive_example_SRCS main.cpp mainwindow.cpp)
set(teamdrive_example_HDRS mainwindow.h)
qt5_wrap_ui(teamdrive_example_SRCS ui/main.ui)

add_executable(teamdrive-example
        ${teamdrive_example_SRCS}
        ${teamdrive_example_HDRS_MOC}
)

target_link_libraries(teamdrive-example
        Qt5::Widgets
        Qt5::Core
        KF5::GAPICore
        KF5::GAPIDrive
)

kde_enable_exceptions()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
)

set(drives_example_SRCS main.cpp mainwindow.cpp)
set(drives_example_HDRS mainwindow.h)
qt_wrap_ui(drives_example_SRCS ui/main.ui)

add_executable(drives-example
        ${drives_example_SRCS}
        ${drives_example_HDRS_MOC}
)

target_link_libraries(drives-example
        Qt5::Widgets
        Qt5::Core
        KF5::GAPICore
        KF5::GAPIDrive
)

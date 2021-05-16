kde_enable_exceptions()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
)

set(permissions_example_SRCS main.cpp mainwindow.cpp)
set(permissions_example_HDRS mainwindow.h)
qt_wrap_ui(permissions_example_SRCS ui/main.ui)

add_executable(permissions-example
        ${permissions_example_SRCS}
        ${permissions_example_HDRS_MOC}
)

target_link_libraries(permissions-example
        Qt5::Widgets
        Qt5::Core
        KF5::GAPICore
        KF5::GAPIDrive
)

kde_enable_exceptions()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
)

set(tasks_example_SRCS main.cpp mainwindow.cpp)
set(tasks_example_HDRS mainwindow.h)
qt_wrap_ui(tasks_example_SRCS ui/main.ui)

add_executable(tasks-example
        ${tasks_example_SRCS}
        ${tasks_example_HDRS_MOC}
)

target_link_libraries(tasks-example
        Qt5::Widgets
        Qt5::Core
        KF5::GAPICore
        KF5::GAPITasks
)

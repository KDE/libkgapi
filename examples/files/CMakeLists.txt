kde_enable_exceptions()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
)

set(files_example_SRCS main.cpp mainwindow.cpp)
set(files_example_HDRS mainwindow.h)
qt5_wrap_ui(files_example_SRCS ui/main.ui)

add_executable(files-example
        ${files_example_SRCS}
        ${files_example_HDRS_MOC}
)

target_link_libraries(files-example
        Qt5::Widgets
        Qt5::Core
        KF5::GAPICore
        KF5::GAPIDrive
)

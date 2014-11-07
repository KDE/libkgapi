project(staticmaps-example)

kde_enable_exceptions()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

set(staticmaps_example_SRCS main.cpp mainwindow.cpp)
set(staticmaps_example_HDRS mainwindow.h)
qt5_wrap_ui(staticmaps_example_SRCS ui/main.ui)

qt5_wrap_cpp(staticmaps_example_HDRS_MOC ${staticmaps_example_HDRS})

add_executable(staticmaps-example
        ${staticmaps_example_SRCS}
        ${staticmaps_example_HDRS_MOC}
)

target_link_libraries(staticmaps-example
        KF5::Contacts
        kgapi2
)

add_dependencies(staticmaps-example kgapi2)

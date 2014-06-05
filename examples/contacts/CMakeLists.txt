project(contacts-example)

kde_enable_exceptions()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

set(contacts_example_SRCS main.cpp mainwindow.cpp)
set(contacts_example_HDRS mainwindow.h)
qt5_wrap_ui(contacts_example_SRCS ui/main.ui)

qt5_wrap_cpp(contacts_example_HDRS_MOC ${contacts_example_HDRS})

add_executable(contacts-example
        ${contacts_example_SRCS}
        ${contacts_example_HDRS_MOC}
)

target_link_libraries(contacts-example
        KF5::Abc
        kgapi2
)

add_dependencies(contacts-example kgapi2)

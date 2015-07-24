project(contacts-example)

kde_enable_exceptions()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
)

set(contacts_example_SRCS main.cpp mainwindow.cpp)
set(contacts_example_HDRS mainwindow.h)
qt5_wrap_ui(contacts_example_SRCS ui/main.ui)

add_executable(contacts-example
        ${contacts_example_SRCS}
        ${contacts_example_HDRS_MOC}
)

target_link_libraries(contacts-example
        KF5::Contacts
        Qt5::Widgets
        Qt5::Core
        KF5::GAPICore
        KF5::GAPIContacts
)

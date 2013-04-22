project(staticmaps-example)

find_package(KDE4 REQUIRED)
find_package(KdepimLibs REQUIRED)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${KDE4_INCLUDES})

set(staticmaps_example_SRCS main.cpp mainwindow.cpp)
set(staticmaps_example_HDRS mainwindow.h)
kde4_add_ui_files(staticmaps_example_SRCS ui/main.ui)

qt4_wrap_cpp(staticmaps_example_HDRS_MOC ${staticmaps_example_HDRS})

add_executable(staticmaps-example
        ${staticmaps_example_SRCS}
        ${staticmaps_example_HDRS_MOC}
)

target_link_libraries(staticmaps-example
        ${QT_QTCORE_LIBRARY}
        ${KDE4_KDECORE_LIBS}
        ${KDEPIMLIBS_KABC_LIBS}
        kgapi2
)

add_dependencies(staticmaps-example kgapi2)

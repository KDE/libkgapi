project(libkgoogle)
set(KGOOGLE_VERSION_MAJOR 0)
set(KGOOGLE_VESRION_MINOR 1)
set(KGOOGLE_VERSION 0.1)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

set(libkgoogle_SRCS 
    authdialog.cpp
)

kde4_add_library(kgoogle SHARED ${libkgoogle_SRCS})
set_target_properties(kgoogle PROPERTIES VERSION ${KGOOGLE_VERSION} SOVERSION ${KGOOGLE_VERSION_MAJOR} )

target_link_libraries(kgoogle
		      ${KDE4_KDECORE_LIBS}
		      ${KDE4_KDEWEBKIT_LIBS}
		      ${QJSON_LIBRARIES}
)

install(TARGETS kgoogle ${INSTALL_TARGETS_DEFAULT_ARGS})
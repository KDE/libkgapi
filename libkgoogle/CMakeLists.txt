project(googlecontactsresource)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

set(libkgoogle_SRCS 
    authdialog.cpp
)

kde4_add_library(akonadi_kgoogle SHARED ${libkgoogle_SRCS})

target_link_libraries(akonadi_kgoogle
		      ${QT_QTDBUS_LIBRARY}
		      ${KDE4_KDECORE_LIBS}
		      ${KDE4_KDEWEBKIT_LIBS}
		      ${QJSON_LIBRARIES}
)

install(TARGETS akonadi_kgoogle ${INSTALL_TARGETS_DEFAULT_ARGS})
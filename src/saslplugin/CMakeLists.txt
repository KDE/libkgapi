include(CheckIncludeFile)
include(CheckStructHasMember)

if (NOT CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    add_definitions(-D_GNU_SOURCE)
endif()

if (WIN32)
    add_definitions(-DWIN32)
elseif (APPLE)
    add_definitions(-Dmacintosh)
endif()

check_include_file(unistd.h HAVE_UNISTD_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_struct_has_member("struct sockaddr" sa_len sys/socket.h HAVE_SOCKADDR_SA_LEN)

configure_file(config.h.in config.h)

add_library(kdexoauth2 SHARED
    xoauth2plugin.c
    xoauth2plugin_init.c
    plugin_common.c
)
target_include_directories(kdexoauth2 PRIVATE ${Sasl2_INCLUDE_DIRS})
set_target_properties(kdexoauth2 PROPERTIES
    C_VISIBILITY_PRESET default
    SOVERSION 3
    VERSION 3.0.0)

install(TARGETS kdexoauth2 DESTINATION ${KDE_INSTALL_LIBDIR}/sasl2)

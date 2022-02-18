set(libkgapi_debug_SRCS)
ecm_qt_declare_logging_category(libkgapi_debug_SRCS HEADER debug.h IDENTIFIER KGAPIDebug CATEGORY_NAME org.kde.kgapi DESCRIPTION "libkgapi (General debug)" EXPORT LIBKGAPI)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/)

add_subdirectory(blogger)
add_subdirectory(calendar)
add_subdirectory(core)
add_subdirectory(drive)
add_subdirectory(latitude)
add_subdirectory(people)
add_subdirectory(staticmaps)
add_subdirectory(tasks)

if (NOT WIN32)
    add_subdirectory(saslplugin)
endif()


project (tests)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ../cmake/modules ${CMAKE_MODULE_PATH})

# search packages used by KDE
find_package (KDE4 REQUIRED)
include(KDE4Defaults)

include_directories(
    ${KDE4_INCLUDES}
    ${QCA2_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../contacts
    ${CMAKE_CURRENT_BINARY_DIR}/../contacts
)

########### AuthDialog Test ###############

set(authdialog_testSRCS authdialog_test.cpp)
kde4_add_executable(authdialog_test ${authdialog_testSRCS} ${authdialog_testMOCS})
target_link_libraries(authdialog_test ${QT_QTCORE_LIBRARY} ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} 
		      ${KDE4_KDEWEBKIT_LIBS} ${CMAKE_CURRENT_BINARY_DIR}/../lib/libkgoogle.so)
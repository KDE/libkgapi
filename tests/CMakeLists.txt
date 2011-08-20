project (tests)
cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0017)
  cmake_policy(SET CMP0017 NEW)
endif()

find_package(Qt4 4.7.0 REQUIRED) 

add_definitions(${QT_DEFINITIONS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/libkgoogle
)

########### Test Contacts ###############

set(test_contacts_SRCS 
    testcontacts.cpp)
kde4_add_executable(test_contacts ${test_contacts_SRCS})

target_link_libraries(test_contacts
		      ${QT_QTCORE_LIBRARY} 
		      ${QT_QTTEST_LIBRARY}
		      kgoogle)

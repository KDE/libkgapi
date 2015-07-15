include(ECMMarkAsTest)

find_package(Qt5Test CONFIG REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

remove_definitions(-DQT_NO_CAST_FROM_ASCII)

MACRO(LIBKGAPI_TESTS)
       FOREACH(_testname ${ARGN})
               add_executable( ${_testname} ${_testname}.cpp )
               add_test( ${_testname} ${_testname} )
               ecm_mark_as_test(libkgapi-${_testname})
               target_link_libraries(${_testname}
                                     Qt5::Test
                                     KF5::Contacts
                                     KF5::Mime
                                     KF5::CalendarCore
                                     KF5::KIOCore
                                     kgapi)
       ENDFOREACH(_testname)
ENDMACRO(LIBKGAPI_TESTS)

macro(add_libkgapi2_test _module _testname)
    add_executable( ${_module}-${_testname} ${_module}/${_testname}.cpp )
    add_test( ${_module}-${_testname} ${_module}-${_testname} )
    ecm_mark_as_test(libkgapi2-${_module}-${_testname})
    target_link_libraries(${_module}-${_testname}
                          Qt5::Test
                          KF5::Contacts
                          KF5::CalendarCore
                          KF5GAPIDrive)
endmacro(add_libkgapi2_test)

#LIBKGAPI_TESTS(
#    objectsaccountinfotests
#    objectscalendartests
#    objectscontacttests
#    objectscontactsgrouptests
#    objectseventtests
#    objectstasktests
#    objectstasklisttests
#    objectslocationtests
#    objectstaticmapurltests
#    servicesaccountinfotests
#    servicescalendartests
#    servicescontactstests
#    serviceslatitudetests
#    servicestaskstests
#)

add_libkgapi2_test(drive filesearchquerytest)

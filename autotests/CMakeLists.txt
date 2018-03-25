include(ECMMarkAsTest)

find_package(Qt5Test CONFIG REQUIRED)

macro(add_libkgapi2_test _module _testname)
    add_executable( ${_module}-${_testname} ${_module}/${_testname}.cpp )
    add_test(NAME ${_module}-${_testname} COMMAND ${_module}-${_testname} )
    ecm_mark_as_test(libkgapi2-${_module}-${_testname})
    target_link_libraries(${_module}-${_testname}
                          Qt5::Test
                          KF5::Contacts
                          KF5::CalendarCore
                          KPimGAPIDrive)
endmacro(add_libkgapi2_test)

add_libkgapi2_test(drive filesearchquerytest)

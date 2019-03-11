include(ECMAddTests)

find_package(Qt5Test CONFIG REQUIRED)

set(kgapitest_SRCS
    fakeaccountstorage.cpp
    fakeauthwidget.cpp
    fakenetworkreply.cpp
    fakenetworkaccessmanager.cpp
    fakenetworkaccessmanagerfactory.cpp
    testutils.cpp
)
add_library(kgapitest STATIC ${kgapitest_SRCS})
target_link_libraries(kgapitest Qt5::Core Qt5::Network Qt5::Test KPimGAPICore)



macro(add_libkgapi2_test _module _testname)
    set(testSources ${_module}/${_testname}.cpp)
    string(SUBSTRING ${_module} 0 1 moduleFirst)
    string(SUBSTRING ${_module} 1 -1 moduleLast)
    string(TOUPPER ${moduleFirst} moduleFirst)
    string(CONCAT moduleName ${moduleFirst} ${moduleLast})
    set(extraLibs "KPimGAPI${moduleName}")
    set(utilsFile ${CMAKE_CURRENT_SOURCE_DIR}/${_module}/${_module}testutils.cpp)
    if (EXISTS ${utilsFile})
        list(APPEND testSources ${utilsFile})
    endif()
    ecm_add_test(${testSources}
        LINK_LIBRARIES kgapitest KPimGAPICore ${extraLibs}
        TEST_NAME ${_testname}
        NAME_PREFIX ${_module}-
    )
endmacro(add_libkgapi2_test)

ecm_add_test(fakenamtest.cpp LINK_LIBRARIES kgapitest KPimGAPICore TEST_NAME fakenamtest NAME_PREFIX fake-)

add_libkgapi2_test(core accountinfofetchjobtest)
add_libkgapi2_test(core accountmanagertest)
add_libkgapi2_test(core createjobtest)
add_libkgapi2_test(core fetchjobtest)

add_libkgapi2_test(contacts contactcreatejobtest)
add_libkgapi2_test(contacts contactdeletejobtest)
add_libkgapi2_test(contacts contactfetchjobtest)
add_libkgapi2_test(contacts contactfetchphotojobtest)
add_libkgapi2_test(contacts contactmodifyjobtest)
add_libkgapi2_test(contacts contactsgroupcreatejobtest)
add_libkgapi2_test(contacts contactsgroupdeletejobtest)
add_libkgapi2_test(contacts contactsgroupfetchjobtest)
add_libkgapi2_test(contacts contactsgroupmodifyjobtest)

add_libkgapi2_test(calendar calendarcreatejobtest)
add_libkgapi2_test(calendar calendardeletejobtest)
add_libkgapi2_test(calendar calendarfetchjobtest)
add_libkgapi2_test(calendar calendarmodifyjobtest)
add_libkgapi2_test(calendar eventcreatejobtest)
add_libkgapi2_test(calendar eventdeletejobtest)
add_libkgapi2_test(calendar eventfetchjobtest)
add_libkgapi2_test(calendar eventmodifyjobtest)
add_libkgapi2_test(calendar freebusyqueryjobtest)

add_libkgapi2_test(tasks taskcreatejobtest)
add_libkgapi2_test(tasks taskdeletejobtest)
add_libkgapi2_test(tasks taskfetchjobtest)
add_libkgapi2_test(tasks taskmodifyjobtest)
add_libkgapi2_test(tasks taskmovejobtest)
add_libkgapi2_test(tasks tasklistcreatejobtest)
add_libkgapi2_test(tasks tasklistdeletejobtest)
add_libkgapi2_test(tasks tasklistfetchjobtest)
add_libkgapi2_test(tasks tasklistmodifyjobtest)

add_libkgapi2_test(drive aboutfetchjobtest)
add_libkgapi2_test(drive changefetchjobtest)
add_libkgapi2_test(drive filecopyjobtest)
add_libkgapi2_test(drive filecreatejobtest)
add_libkgapi2_test(drive filesearchquerytest)
add_libkgapi2_test(drive teamdrivecreatejobtest)
add_libkgapi2_test(drive teamdrivefetchjobtest)
add_libkgapi2_test(drive teamdrivesearchquerytest)

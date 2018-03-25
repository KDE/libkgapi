include(ECMAddTests)

find_package(Qt5Test CONFIG REQUIRED)

set(kgapitest_SRCS
    fakenetworkreply.cpp
    fakenetworkaccessmanager.cpp
    fakenetworkaccessmanagerfactory.cpp
    testutils.cpp
)
add_library(kgapitest STATIC ${kgapitest_SRCS})
target_link_libraries(kgapitest Qt5::Core Qt5::Network Qt5::Test KPimGAPICore)



macro(add_libkgapi2_test _module _testname)
    string(SUBSTRING ${_module} 0 1 moduleFirst)
    string(SUBSTRING ${_module} 1 -1 moduleLast)
    string(TOUPPER ${moduleFirst} moduleFirst)
    string(CONCAT moduleName ${moduleFirst} ${moduleLast})
    set(extraLibs "KPimGAPI${moduleName}")
    ecm_add_test(${_module}/${_testname}.cpp
        LINK_LIBRARIES kgapitest KPimGAPICore ${extraLibs}
        TEST_NAME ${_testname}
        NAME_PREFIX ${_module}-
    )
endmacro(add_libkgapi2_test)

ecm_add_test(fakenamtest.cpp LINK_LIBRARIES kgapitest KPimGAPICore TEST_NAME fakenamtest NAME_PREFIX fake-)

add_libkgapi2_test(core fetchjobtest)
add_libkgapi2_test(core createjobtest)
add_libkgapi2_test(drive filesearchquerytest)

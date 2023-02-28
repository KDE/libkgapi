include(ECMAddTests)

find_package(Qt${QT_MAJOR_VERSION}Test CONFIG REQUIRED)

set(kgapitest_SRCS
    fakeaccountstorage.cpp
    fakeauthbrowser.cpp
    fakenetworkreply.cpp
    fakenetworkaccessmanager.cpp
    fakenetworkaccessmanagerfactory.cpp
    testutils.cpp
    fakeaccountstorage.h
    fakeauthbrowser.h
    fakenetworkreply.h
    fakenetworkaccessmanager.h
    fakenetworkaccessmanagerfactory.h
    testutils.h
)


if (NOT WIN32)
# Silence deprecation warnings for deprecated APIs tests.
set_source_files_properties(
    drive/teamdrivecreatejobtest.cpp
    drive/teamdrivedeletejobtest.cpp
    drive/teamdrivefetchjobtest.cpp
    drive/teamdrivemodifyjobtest.cpp
    PROPERTIES COMPILE_FLAGS -Wno-deprecated-declarations
)
endif()

add_library(kgapitest STATIC ${kgapitest_SRCS})
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(kgapitest PROPERTIES UNITY_BUILD ON)
endif()

target_link_libraries(kgapitest Qt::Core Qt::Network Qt::Gui Qt::Test KPim${KF_MAJOR_VERSION}GAPICore)



macro(add_libkgapi2_test _module _testname)
    set(_extraLibs ${ARGN})
    set(testSources ${_module}/${_testname}.cpp)
    string(SUBSTRING ${_module} 0 1 moduleFirst)
    string(SUBSTRING ${_module} 1 -1 moduleLast)
    string(TOUPPER ${moduleFirst} moduleFirst)
    string(CONCAT moduleName ${moduleFirst} ${moduleLast})
    set(extraLibs "${_extraLibs};KPim${KF_MAJOR_VERSION}GAPI${moduleName}")
    set(utilsFile ${CMAKE_CURRENT_SOURCE_DIR}/${_module}/${_module}testutils.cpp)
    if (EXISTS ${utilsFile})
        list(APPEND testSources ${utilsFile})
    endif()
    ecm_add_test(${testSources}
        LINK_LIBRARIES kgapitest KPim${KF_MAJOR_VERSION}GAPICore ${extraLibs}
        TEST_NAME ${_testname}
        NAME_PREFIX ${_module}-
    )
endmacro(add_libkgapi2_test)

ecm_add_test(fakenamtest.cpp LINK_LIBRARIES kgapitest KPim${KF_MAJOR_VERSION}GAPICore TEST_NAME fakenamtest NAME_PREFIX fake-)

add_libkgapi2_test(core accountinfofetchjobtest)
add_libkgapi2_test(core accountmanagertest)
add_libkgapi2_test(core createjobtest)
add_libkgapi2_test(core fetchjobtest)

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
add_libkgapi2_test(drive filecopyjobtest Qt::Gui)
add_libkgapi2_test(drive filecreatejobtest Qt::Gui)
add_libkgapi2_test(drive filesearchquerytest)
add_libkgapi2_test(drive drivescreatejobtest)
add_libkgapi2_test(drive drivesdeletejobtest)
add_libkgapi2_test(drive drivesmodifyjobtest)
add_libkgapi2_test(drive driveshidejobtest)
add_libkgapi2_test(drive drivesfetchjobtest)
add_libkgapi2_test(drive drivessearchquerytest)
add_libkgapi2_test(drive teamdrivecreatejobtest)
add_libkgapi2_test(drive teamdrivedeletejobtest)
add_libkgapi2_test(drive teamdrivemodifyjobtest)
add_libkgapi2_test(drive teamdrivefetchjobtest)
add_libkgapi2_test(drive teamdrivesearchquerytest)

add_libkgapi2_test(people contactgroupcreatejobtest)
add_libkgapi2_test(people contactgroupdeletejobtest)
add_libkgapi2_test(people contactgroupfetchjobtest)
add_libkgapi2_test(people contactgroupmodifyjobtest)
add_libkgapi2_test(people personcreatejobtest)
add_libkgapi2_test(people persondeletejobtest)
add_libkgapi2_test(people personfetchjobtest)
add_libkgapi2_test(people personmodifyjobtest)
add_libkgapi2_test(people personphotoupdatejobtest)

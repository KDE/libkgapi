set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${CMAKE_SOURCE_DIR}/libkgapi)

remove_definitions(-DQT_NO_CAST_FROM_ASCII)

MACRO(LIBKGAPI_TESTS)
       FOREACH(_testname ${ARGN})
               kde4_add_unit_test(${_testname} TESTNAME libkgapi-${_testname} ${_testname}.cpp)
               target_link_libraries(${_testname}
                                     ${QT_QTTEST_LIBRARY}
                                     ${KDEPIMLIBS_KABC_LIBS}
                                     ${KDEPIMLIBS_KMIME_LIBS}
                                     ${KDEPIMLIBS_KCALCORE_LIBS}
                                     ${qjson_LIBRARIES}        # for Debian
                                     ${QJSON_LIBRARY}          # for Mac OSX
                                     ${QJSON_LIBRARIES}
                                     ${KDE4_KIO_LIBS}
                                     ${KDE4_KDEUI_LIBS}
                                     kgapi)
       ENDFOREACH(_testname)
ENDMACRO(LIBKGAPI_TESTS)

LIBKGAPI_TESTS(
    objectsaccountinfotests
    objectscalendartests
    objectscontacttests
    objectscontactsgrouptests
    objectseventtests
    objectstasktests
    objectstasklisttests
    objectslocationtests
    objectstaticmapurltests
    servicesaccountinfotests
    servicescalendartests
    servicescontactstests
    serviceslatitudetests
    servicestaskstests
)

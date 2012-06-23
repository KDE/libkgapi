set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

MACRO(LIBKGOOGLE_TESTS)
       FOREACH(_testname ${ARGN})
               kde4_add_unit_test(${_testname} TESTNAME libkgapi-${_testname} ${_testname}.cpp)
               target_link_libraries(${_testname}
                                     ${QT_QTTEST_LIBRARY}
                                     ${KDEPIMLIBS_KABC_LIBS}
                                     ${KDEPIMLIBS_KMIME_LIBS}
                                     ${KDEPIMLIBS_KCALCORE_LIBS}
                                     ${qjson_LIBRARIES}        # for Debian
                                     ${QJSON_LIBRARIES}
                                     ${KDE4_KIO_LIBS}
                                     ${KDE4_KDEUI_LIBS}
                                     kgapi)
       ENDFOREACH(_testname)
ENDMACRO(LIBKGOOGLE_TESTS)

LIBKGOOGLE_TESTS(
    objectsaccountinfotests
    objectscalendartests
    objectscontacttests
    objectscontactsgrouptests
    objectseventtests
    objectstasktests
    objectstasklisttests
    servicesaccountinfotests
    servicescalendartests
)
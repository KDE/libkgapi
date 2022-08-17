add_library(KPimGAPIContacts)
add_library(KPim::GAPIContacts ALIAS KPimGAPIContacts)


target_sources(KPimGAPIContacts PRIVATE
    ${libkgapi_debug_SRCS}
    contact.cpp
    contactcreatejob.cpp
    contactcreatejob.h
    contactdeletejob.cpp
    contactdeletejob.h
    contactfetchjob.cpp
    contactfetchjob.h
    contactfetchphotojob.cpp
    contactfetchphotojob.h
    contact.h
    contactmodifyjob.cpp
    contactmodifyjob.h
    contactsgroup.cpp
    contactsgroupcreatejob.cpp
    contactsgroupcreatejob.h
    contactsgroupdeletejob.cpp
    contactsgroupdeletejob.h
    contactsgroupfetchjob.cpp
    contactsgroupfetchjob.h
    contactsgroup.h
    contactsgroupmodifyjob.cpp
    contactsgroupmodifyjob.h
    contactsservice.cpp
    contactsservice.h
)

ecm_generate_headers(kgapicontacts_CamelCase_HEADERS
    HEADER_NAMES
    Contact
    ContactCreateJob
    ContactDeleteJob
    ContactFetchJob
    ContactFetchPhotoJob
    ContactModifyJob
    ContactsGroup
    ContactsGroupCreateJob
    ContactsGroupDeleteJob
    ContactsGroupFetchJob
    ContactsGroupModifyJob
    PREFIX KGAPI/Contacts
    REQUIRED_HEADERS kgapicontacts_HEADERS
)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPimGAPIContacts PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPimGAPIContacts BASE_NAME kgapicontacts)


target_include_directories(KPimGAPIContacts
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI>"
    INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_link_libraries(KPimGAPIContacts
PUBLIC
    KPim::GAPICore
    KF5::Contacts
    Qt::Gui
PRIVATE
    Qt::Xml
    Qt::Network
)

set_target_properties(KPimGAPIContacts PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIContacts
)

install(TARGETS
    KPimGAPIContacts
    EXPORT KPimGAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapicontacts_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/KGAPI/Contacts"
    COMPONENT Devel
)

install(FILES
    ${kgapicontacts_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapicontacts_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/kgapi/contacts"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIContacts
    LIB_NAME KPimGAPIContacts
    DEPS "KPim::GAPICore KF5::Contacts Qt::Xml"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

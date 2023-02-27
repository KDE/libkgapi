add_library(KPim${KF_MAJOR_VERSION}GAPIContacts)
add_library(KPim${KF_MAJOR_VERSION}::GAPIContacts ALIAS KPim${KF_MAJOR_VERSION}GAPIContacts)


target_sources(KPim${KF_MAJOR_VERSION}GAPIContacts PRIVATE
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
    set_target_properties(KPim${KF_MAJOR_VERSION}GAPIContacts PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPim${KF_MAJOR_VERSION}GAPIContacts BASE_NAME kgapicontacts)


target_include_directories(KPim${KF_MAJOR_VERSION}GAPIContacts
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI>"
    INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_link_libraries(KPim${KF_MAJOR_VERSION}GAPIContacts
PUBLIC
    KPim${KF_MAJOR_VERSION}::GAPICore
    KF${KF_MAJOR_VERSION}::Contacts
    Qt::Gui
PRIVATE
    Qt::Xml
    Qt::Network
)

set_target_properties(KPim${KF_MAJOR_VERSION}GAPIContacts PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIContacts
)

install(TARGETS
    KPim${KF_MAJOR_VERSION}GAPIContacts
    EXPORT KPim${KF_MAJOR_VERSION}GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapicontacts_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/KGAPI/Contacts"
    COMPONENT Devel
)

install(FILES
    ${kgapicontacts_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapicontacts_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/kgapi/contacts"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIContacts
    LIB_NAME KPim${KF_MAJOR_VERSION}GAPIContacts
    DEPS "KPim${KF_MAJOR_VERSION}::GAPICore KF${KF_MAJOR_VERSION}::Contacts Qt::Xml"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

set(kgapicontacts_SRCS
    contact.cpp
    contactcreatejob.cpp
    contactdeletejob.cpp
    contactfetchjob.cpp
    contactfetchphotojob.cpp
    contactmodifyjob.cpp
    contactsgroup.cpp
    contactsgroupcreatejob.cpp
    contactsgroupdeletejob.cpp
    contactsgroupfetchjob.cpp
    contactsgroupmodifyjob.cpp
    contactsservice.cpp

    ../debug.cpp
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

add_library(KF5GAPIContacts
    ${kgapicontacts_SRCS}
)

generate_export_header(KF5GAPIContacts BASE_NAME kgapicontacts)

add_library(KF5::GAPIContacts ALIAS KF5GAPIContacts)
target_include_directories(KF5GAPIContacts INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI>")

target_link_libraries(KF5GAPIContacts
PUBLIC
    KF5::GAPICore
    KF5::Contacts
PRIVATE
    Qt5::Xml
    Qt5::Network
)

set_target_properties(KF5GAPIContacts PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIContacts
)

install(TARGETS
    KF5GAPIContacts
    EXPORT KF5GAPITargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapicontacts_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI/KGAPI/Contacts"
    COMPONENT Devel
)

install(FILES
    ${kgapicontacts_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapicontacts_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI/kgapi/contacts"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIContacts
    LIB_NAME KF5GAPIContacts
    DEPS "KF5::GAPICore KF5::Contacts Qt5::Xml"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR_KF5}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

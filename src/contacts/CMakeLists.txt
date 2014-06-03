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
)

ecm_generate_headers(kgapicontacts_HEADERS
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
    REQUIRED_HEADERS kgapicontacts_HEADERS
)

add_library(KF5GAPIContacts
    ${kgapicontacts_SRCS}
)

generate_export_header(KF5GAPIContacts BASE_NAME kgapicontacts)

add_library(KF5::GAPIContacts ALIAS KF5GAPIContacts)
target_include_directories(KF5GAPIContacts INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KGAPI>")
target_include_directories(KF5GAPIContacts PUBLIC "$<BUILD_INTERFACE:${KGAPI_SOURCE_DIR}/src;${KAGAPI_BINARY_DIR}/src>")

target_link_libraries(KF5GAPIContacts
PUBLIC
    KF5::GAPICore
    KF5::Abc
PRIVATE
    KF5::I18n
)

set_target_properties(KF5GAPIContacts PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPI
)

install(TARGETS
    KF5GAPIContacts
    EXPORT KF5GAPITargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapicontacts_HEADERS}
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}/KGAPI/KGAPI/Contacts"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIContacts
    LIB_NAME KF5GAPIContacts
    DEPS "GAPICore Abc"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KF5_INCLUDE_INSTALL_DIR}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

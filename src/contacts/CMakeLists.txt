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

add_library(KPimGAPIContacts
    ${kgapicontacts_SRCS}
)
if (COMPILE_WITH_CMAKE_SUPPORT)
    set_target_properties(KPimGAPIContacts PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPimGAPIContacts BASE_NAME kgapicontacts)

add_library(KPim::GAPIContacts ALIAS KPimGAPIContacts)

# TODO: Backwards compatibility, remove in next major release
add_library(KF5::GAPIContacts ALIAS KPimGAPIContacts)
target_include_directories(KPimGAPIContacts INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI>")

target_link_libraries(KPimGAPIContacts
PUBLIC
    KPim::GAPICore
    KF5::Contacts
    Qt5::Gui
PRIVATE
    Qt5::Xml
    Qt5::Network
)

set_target_properties(KPimGAPIContacts PROPERTIES
    VERSION ${KGAPI_VERSION_STRING}
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
    DEPS "KPim::GAPICore KF5::Contacts Qt5::Xml"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

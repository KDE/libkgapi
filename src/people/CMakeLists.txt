add_library(KPimGAPIPeople)
add_library(KPim::GAPIPeople ALIAS KPimGAPIPeople)

target_sources(KPimGAPIPeople PRIVATE
    address.cpp
    agerangetype.cpp
    biography.cpp
    birthday.cpp
    braggingrights.cpp
    calendarurl.cpp
    clientdata.cpp
    contactgroup.cpp
    contactgroupmembership.cpp
    contactgroupmetadata.cpp
    coverphoto.cpp
    domainmembership.cpp
    emailaddress.cpp
    event.cpp
    externalid.cpp
    fieldmetadata.cpp
    fileas.cpp
    gender.cpp
    groupclientdata.cpp
    imclient.cpp
    interest.cpp
    location.cpp
    membership.cpp
    misckeyword.cpp
    name.cpp
    nickname.cpp
    occupation.cpp
    organization.cpp
    person.cpp
    personlocale.cpp
    personmetadata.cpp
    phonenumber.cpp
    photo.cpp
    profilemetadata.cpp
    relation.cpp
    relationshipinterest.cpp
    relationshipstatus.cpp
    residence.cpp
    sipaddress.cpp
    skill.cpp
    source.cpp
    tagline.cpp
    url.cpp
    userdefined.cpp
    ../debug.cpp
)

ecm_generate_headers(
    kgapipeople_CamelCase_HEADERS
    HEADER_NAMES
    Address,
    AgeRangeType,
    Biography
    Birthday
    BraggingRights
    CalendarUrl
    ClientData
    ContactGroup
    ContactGroupMembership
    ContactGroupMetadata
    CoverPhoto
    DomainMembership
    EmailAddress
    Event
    ExternalId
    FieldMetadata
    FileAs
    Gender
    GroupClientData
    IMClient
    Interest
    Location
    Membership
    MiscKeyword
    Name
    Nickname
    Occupation
    Organization
    Person
    PersonLocale
    PersonMetadata
    PhoneNumber
    Photo
    ProfileMetadata
    Relation
    RelationshipInterest
    RelationshipStatus
    Residence
    SIPAddress
    Skill
    Source
    Tagline
    Url
    UserDefined
    PREFIX KGAPI/People
)

generate_export_header(KPimGAPIPeople BASE_NAME kgapipeople)

target_include_directories(KPimGAPIPeople INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI>")

target_link_libraries(KPimGAPIPeople
PUBLIC
    KPim::GAPICore
    KF5::Contacts
PRIVATE
    Qt::Network
)

set_target_properties(KPimGAPIPeople PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIPeople
)

install(TARGETS
    KPimGAPIPeople
    EXPORT KPimGAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapipeople_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/KGAPI/People"
    COMPONENT Devel
)

install(FILES
    ${kgapipeople_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapipeople_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI/kgapi/people"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIPeople
    LIB_NAME KPimGAPIContacts
    DEPS "KPim::GAPICore KF5::Contacts"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)

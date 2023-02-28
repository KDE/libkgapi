add_library(KPim${KF_MAJOR_VERSION}GAPIPeople)
add_library(KPim${KF_MAJOR_VERSION}::GAPIPeople ALIAS KPim${KF_MAJOR_VERSION}GAPIPeople)

target_sources(KPim${KF_MAJOR_VERSION}GAPIPeople PRIVATE
    address.cpp
    address.h
    agerangetype.cpp
    agerangetype.h
    biography.cpp
    biography.h
    birthday.cpp
    birthday.h
    braggingrights.cpp
    braggingrights.h
    calendarurl.cpp
    calendarurl.h
    clientdata.cpp
    clientdata.h
    contactgroup.cpp
    contactgroup.h
    contactgroupcreatejob.cpp
    contactgroupcreatejob.h
    contactgroupdeletejob.cpp
    contactgroupdeletejob.h
    contactgroupfetchjob.cpp
    contactgroupfetchjob.h
    contactgroupmembership.cpp
    contactgroupmembership.h
    contactgroupmetadata.cpp
    contactgroupmetadata.h
    contactgroupmodifyjob.cpp
    contactgroupmodifyjob.h
    coverphoto.cpp
    coverphoto.h
    domainmembership.cpp
    domainmembership.h
    emailaddress.cpp
    emailaddress.h
    event.cpp
    event.h
    externalid.cpp
    externalid.h
    fieldmetadata.cpp
    fieldmetadata.h
    fileas.cpp
    fileas.h
    gender.cpp
    gender.h
    groupclientdata.cpp
    groupclientdata.h
    imclient.cpp
    imclient.h
    interest.cpp
    interest.h
    location.cpp
    location.h
    membership.cpp
    membership.h
    misckeyword.cpp
    misckeyword.h
    name.cpp
    name.h
    nickname.cpp
    nickname.h
    occupation.cpp
    occupation.h
    organization.cpp
    organization.h
    person.cpp
    person.h
    personcreatejob.cpp
    personcreatejob.h
    persondeletejob.cpp
    persondeletejob.h
    personfetchjob.cpp
    personfetchjob.h
    personlocale.cpp
    personlocale.h
    personmetadata.cpp
    personmetadata.h
    personmodifyjob.cpp
    personmodifyjob.h
    personphotodeletejob.cpp
    personphotodeletejob.h
    personphotoupdatejob.cpp
    personphotoupdatejob.h
    peopleservice.cpp
    peopleservice.h
    phonenumber.cpp
    phonenumber.h
    photo.cpp
    photo.h
    profilemetadata.cpp
    profilemetadata.h
    relation.cpp
    relation.h
    relationshipinterest.cpp
    relationshipinterest.h
    relationshipstatus.cpp
    relationshipstatus.h
    residence.cpp
    residence.h
    sipaddress.cpp
    sipaddress.h
    skill.cpp
    skill.h
    source.cpp
    source.h
    tagline.cpp
    tagline.h
    url.cpp
    url.h
    userdefined.cpp
    userdefined.h
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
    ContactGroupCreateJob
    ContactGroupDeleteJob
    ContactGroupFetchJob
    ContactGroupMembership
    ContactGroupMetadata
    ContactGroupModifyJob
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
    PersonCreateJob
    PersonDeleteJob
    PersonFetchJob
    PersonLocale
    PersonMetadata
    PersonModifyJob
    PersonPhotoDeleteJob
    PersonPhotoUpdateJob
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
    REQUIRED_HEADERS kgapipeople_HEADERS
)

generate_export_header(KPim${KF_MAJOR_VERSION}GAPIPeople BASE_NAME kgapipeople)

target_include_directories(KPim${KF_MAJOR_VERSION}GAPIPeople
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI>"
    INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_link_libraries(KPim${KF_MAJOR_VERSION}GAPIPeople
PUBLIC
    KPim${KF_MAJOR_VERSION}::GAPICore
    KF${KF_MAJOR_VERSION}::Contacts
PRIVATE
    Qt::Network
)

set_target_properties(KPim${KF_MAJOR_VERSION}GAPIPeople PROPERTIES
    VERSION ${KGAPI_VERSION}
    SOVERSION ${KGAPI_SOVERSION}
    EXPORT_NAME GAPIPeople
)

install(TARGETS
    KPim${KF_MAJOR_VERSION}GAPIPeople
    EXPORT KPim${KF_MAJOR_VERSION}GAPITargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kgapipeople_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/KGAPI/People"
    COMPONENT Devel
)

install(FILES
    ${kgapipeople_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kgapipeople_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI/kgapi/people"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KGAPIPeople
    LIB_NAME KPim${KF_MAJOR_VERSION}GAPIPeople
    DEPS "KPim${KF_MAJOR_VERSION}::GAPICore KF5::Contacts"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KGAPI"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)
#

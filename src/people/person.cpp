/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "person.h"

#include "address.h"
#include "agerangetype.h"
#include "biography.h"
#include "birthday.h"
#include "braggingrights.h"
#include "calendarurl.h"
#include "clientdata.h"
#include "coverphoto.h"
#include "emailaddress.h"
#include "event.h"
#include "externalid.h"
#include "fileas.h"
#include "gender.h"
#include "imclient.h"
#include "interest.h"
#include "location.h"
#include "membership.h"
#include "misckeyword.h"
#include "name.h"
#include "nickname.h"
#include "occupation.h"
#include "organization.h"
#include "personlocale.h"
#include "personmetadata.h"
#include "phonenumber.h"
#include "photo.h"
#include "relation.h"
#include "relationshipinterest.h"
#include "relationshipstatus.h"
#include "residence.h"
#include "sipaddress.h"
#include "skill.h"
#include "tagline.h"
#include "url.h"
#include "userdefined.h"

#include <algorithm>

namespace KGAPI2::People
{
class Person::Private
{
public:
    explicit Private() = default;
    Private(const Private &) = default;
    Private(Private &&) noexcept = delete;
    Private &operator=(const Private &) = delete;
    Private &operator=(Private &&) noexcept = delete;
    ~Private() = default;

    QVector<Nickname> nicknames{};
    QVector<EmailAddress> emailAddresses{};
    QVector<ClientData> clientData{};
    QVector<BraggingRights> braggingRights{};
    QVector<RelationshipStatus> relationshipStatuses{};
    QVector<Birthday> birthdays{};
    QVector<Relation> relations{};
    QVector<PersonLocale> locales{};
    QVector<Location> locations{};
    QVector<ExternalId> externalIds{};
    QVector<AgeRangeType> ageRanges{};
    QVector<Tagline> taglines{};
    QVector<UserDefined> userDefined{};
    QVector<Biography> biographies{};
    QVector<SipAddress> sipAddresses{};
    QVector<Membership> memberships{};
    QVector<Skill> skills{};
    QVector<Residence> residences{};
    QVector<Name> names{};
    QVector<CalendarUrl> calendarUrls{};
    QVector<CoverPhoto> coverPhotos{};
    QVector<Address> addresses{};
    QString etag{};
    QVector<PhoneNumber> phoneNumbers{};
    QVector<ImClient> imClients{};
    QVector<Event> events{};
    QVector<Photo> photos{};
    QVector<Occupation> occupations{};
    QVector<MiscKeyword> miscKeywords{};
    QVector<RelationshipInterest> relationshipInterests{};
    QString resourceName{};
    QVector<Interest> interests{};
    QVector<Organization> organizations{};
    QVector<Url> urls{};
    QVector<Gender> genders{};
    QVector<FileAs> fileAses{};
    Person::AgeRange ageRange{};
    PersonMetadata metadata{};
};

Person::Person()
    : d(new Private)
{
}

Person::~Person() = default;

QVector<Nickname> Person::nicknames() const
{
    return d->nicknames;
}

void Person::setNicknames(const QVector<Nickname> &value)
{
    d->nicknames = value;
}

void Person::addNickname(const Nickname &value)
{
    d->nicknames.push_back(value);
}

void Person::removeNickname(const Nickname &value)
{
    d->nicknames.removeOne(value);
}

void Person::clearNicknames()
{
    d->nicknames.clear();
}

QVector<EmailAddress> Person::emailAddresses() const
{
    return d->emailAddresses;
}

void Person::setEmailAddresses(const QVector<EmailAddress> &value)
{
    d->emailAddresses = value;
}

void Person::addEmailAddress(const EmailAddress &value)
{
    d->emailAddresses.push_back(value);
}

void Person::removeEmailAddress(const EmailAddress &value)
{
    d->emailAddresses.removeOne(value);
}

void Person::clearEmailAddresses()
{
    d->emailAddresses.clear();
}

QVector<ClientData> Person::clientData() const
{
    return d->clientData;
}

void Person::setClientData(const QVector<ClientData> &value)
{
    d->clientData = value;
}

void Person::addClientData(const ClientData &value)
{
    d->clientData.push_back(value);
}

void Person::removeClientData(const ClientData &value)
{
    d->clientData.removeOne(value);
}

void Person::clearClientData()
{
    d->clientData.clear();
}

QVector<BraggingRights> Person::braggingRights() const
{
    return d->braggingRights;
}

void Person::setBraggingRights(const QVector<BraggingRights> &value)
{
    d->braggingRights = value;
}

void Person::addBraggingRights(const BraggingRights &value)
{
    d->braggingRights.push_back(value);
}

void Person::removeBraggingRights(const BraggingRights &value)
{
    d->braggingRights.removeOne(value);
}

void Person::clearBraggingRights()
{
    d->braggingRights.clear();
}

QVector<RelationshipStatus> Person::relationshipStatuses() const
{
    return d->relationshipStatuses;
}
QVector<Birthday> Person::birthdays() const
{
    return d->birthdays;
}

void Person::setBirthdays(const QVector<Birthday> &value)
{
    d->birthdays = value;
}

void Person::addBirthday(const Birthday &value)
{
    d->birthdays.push_back(value);
}

void Person::removeBirthday(const Birthday &value)
{
    d->birthdays.removeOne(value);
}

void Person::clearBirthdays()
{
    d->birthdays.clear();
}

QVector<Relation> Person::relations() const
{
    return d->relations;
}

void Person::setRelations(const QVector<Relation> &value)
{
    d->relations = value;
}

void Person::addRelation(const Relation &value)
{
    d->relations.push_back(value);
}

void Person::removeRelation(const Relation &value)
{
    d->relations.removeOne(value);
}

void Person::clearRelations()
{
    d->relations.clear();
}

QVector<PersonLocale> Person::locales() const
{
    return d->locales;
}

void Person::setLocales(const QVector<PersonLocale> &value)
{
    d->locales = value;
}

void Person::addPersonLocale(const PersonLocale &value)
{
    d->locales.push_back(value);
}

void Person::removePersonLocale(const PersonLocale &value)
{
    d->locales.removeOne(value);
}

void Person::clearLocales()
{
    d->locales.clear();
}

QVector<Location> Person::locations() const
{
    return d->locations;
}

void Person::setLocations(const QVector<Location> &value)
{
    d->locations = value;
}

void Person::addLocation(const Location &value)
{
    d->locations.push_back(value);
}

void Person::removeLocation(const Location &value)
{
    d->locations.removeOne(value);
}

void Person::clearLocations()
{
    d->locations.clear();
}

QVector<ExternalId> Person::externalIds() const
{
    return d->externalIds;
}

void Person::setExternalIds(const QVector<ExternalId> &value)
{
    d->externalIds = value;
}

void Person::addExternalId(const ExternalId &value)
{
    d->externalIds.push_back(value);
}

void Person::removeExternalId(const ExternalId &value)
{
    d->externalIds.removeOne(value);
}

void Person::clearExternalIds()
{
    d->externalIds.clear();
}

QVector<AgeRangeType> Person::ageRanges() const
{
    return d->ageRanges;
}
QVector<Tagline> Person::taglines() const
{
    return d->taglines;
}
QVector<UserDefined> Person::userDefined() const
{
    return d->userDefined;
}

void Person::setUserDefined(const QVector<UserDefined> &value)
{
    d->userDefined = value;
}

void Person::addUserDefined(const UserDefined &value)
{
    d->userDefined.push_back(value);
}

void Person::removeUserDefined(const UserDefined &value)
{
    d->userDefined.removeOne(value);
}

void Person::clearUserDefined()
{
    d->userDefined.clear();
}

QVector<Biography> Person::biographies() const
{
    return d->biographies;
}

void Person::setBiographies(const QVector<Biography> &value)
{
    d->biographies = value;
}

void Person::addBiography(const Biography &value)
{
    d->biographies.push_back(value);
}

void Person::removeBiography(const Biography &value)
{
    d->biographies.removeOne(value);
}

void Person::clearBiographies()
{
    d->biographies.clear();
}

QVector<SipAddress> Person::sipAddresses() const
{
    return d->sipAddresses;
}

void Person::setSipAddresses(const QVector<SipAddress> &value)
{
    d->sipAddresses = value;
}

void Person::addSipAddress(const SipAddress &value)
{
    d->sipAddresses.push_back(value);
}

void Person::removeSipAddress(const SipAddress &value)
{
    d->sipAddresses.removeOne(value);
}

void Person::clearSipAddresses()
{
    d->sipAddresses.clear();
}

QVector<Membership> Person::memberships() const
{
    return d->memberships;
}

void Person::setMemberships(const QVector<Membership> &value)
{
    d->memberships = value;
}

void Person::addMembership(const Membership &value)
{
    d->memberships.push_back(value);
}

void Person::removeMembership(const Membership &value)
{
    d->memberships.removeOne(value);
}

void Person::clearMemberships()
{
    d->memberships.clear();
}

QVector<Skill> Person::skills() const
{
    return d->skills;
}

void Person::setSkills(const QVector<Skill> &value)
{
    d->skills = value;
}

void Person::addSkill(const Skill &value)
{
    d->skills.push_back(value);
}

void Person::removeSkill(const Skill &value)
{
    d->skills.removeOne(value);
}

void Person::clearSkills()
{
    d->skills.clear();
}

QVector<Residence> Person::residences() const
{
    return d->residences;
}

void Person::setResidences(const QVector<Residence> &value)
{
    d->residences = value;
}

void Person::addResidence(const Residence &value)
{
    d->residences.push_back(value);
}

void Person::removeResidence(const Residence &value)
{
    d->residences.removeOne(value);
}

void Person::clearResidences()
{
    d->residences.clear();
}

QVector<Name> Person::names() const
{
    return d->names;
}

void Person::setNames(const QVector<Name> &value)
{
    d->names = value;
}

void Person::addName(const Name &value)
{
    d->names.push_back(value);
}

void Person::removeName(const Name &value)
{
    d->names.removeOne(value);
}

void Person::clearNames()
{
    d->names.clear();
}

QVector<CalendarUrl> Person::calendarUrls() const
{
    return d->calendarUrls;
}

void Person::setCalendarUrls(const QVector<CalendarUrl> &value)
{
    d->calendarUrls = value;
}

void Person::addCalendarUrl(const CalendarUrl &value)
{
    d->calendarUrls.push_back(value);
}

void Person::removeCalendarUrl(const CalendarUrl &value)
{
    d->calendarUrls.removeOne(value);
}

void Person::clearCalendarUrls()
{
    d->calendarUrls.clear();
}

QVector<CoverPhoto> Person::coverPhotos() const
{
    return d->coverPhotos;
}
QVector<Address> Person::addresses() const
{
    return d->addresses;
}

void Person::setAddresses(const QVector<Address> &value)
{
    d->addresses = value;
}

void Person::addAddress(const Address &value)
{
    d->addresses.push_back(value);
}

void Person::removeAddress(const Address &value)
{
    d->addresses.removeOne(value);
}

void Person::clearAddresses()
{
    d->addresses.clear();
}

QString Person::etag() const
{
    return d->etag;
}

void Person::setEtag(const QString &value)
{
    d->etag = value;
}
QVector<PhoneNumber> Person::phoneNumbers() const
{
    return d->phoneNumbers;
}

void Person::setPhoneNumbers(const QVector<PhoneNumber> &value)
{
    d->phoneNumbers = value;
}

void Person::addPhoneNumber(const PhoneNumber &value)
{
    d->phoneNumbers.push_back(value);
}

void Person::removePhoneNumber(const PhoneNumber &value)
{
    d->phoneNumbers.removeOne(value);
}

void Person::clearPhoneNumbers()
{
    d->phoneNumbers.clear();
}

QVector<ImClient> Person::imClients() const
{
    return d->imClients;
}

void Person::setImClients(const QVector<ImClient> &value)
{
    d->imClients = value;
}

void Person::addImClient(const ImClient &value)
{
    d->imClients.push_back(value);
}

void Person::removeImClient(const ImClient &value)
{
    d->imClients.removeOne(value);
}

void Person::clearImClients()
{
    d->imClients.clear();
}

QVector<Event> Person::events() const
{
    return d->events;
}

void Person::setEvents(const QVector<Event> &value)
{
    d->events = value;
}

void Person::addEvent(const Event &value)
{
    d->events.push_back(value);
}

void Person::removeEvent(const Event &value)
{
    d->events.removeOne(value);
}

void Person::clearEvents()
{
    d->events.clear();
}

QVector<Photo> Person::photos() const
{
    return d->photos;
}
QVector<Occupation> Person::occupations() const
{
    return d->occupations;
}

void Person::setOccupations(const QVector<Occupation> &value)
{
    d->occupations = value;
}

void Person::addOccupation(const Occupation &value)
{
    d->occupations.push_back(value);
}

void Person::removeOccupation(const Occupation &value)
{
    d->occupations.removeOne(value);
}

void Person::clearOccupations()
{
    d->occupations.clear();
}

QVector<MiscKeyword> Person::miscKeywords() const
{
    return d->miscKeywords;
}

void Person::setMiscKeywords(const QVector<MiscKeyword> &value)
{
    d->miscKeywords = value;
}

void Person::addMiscKeyword(const MiscKeyword &value)
{
    d->miscKeywords.push_back(value);
}

void Person::removeMiscKeyword(const MiscKeyword &value)
{
    d->miscKeywords.removeOne(value);
}

void Person::clearMiscKeywords()
{
    d->miscKeywords.clear();
}

QVector<RelationshipInterest> Person::relationshipInterests() const
{
    return d->relationshipInterests;
}
QString Person::resourceName() const
{
    return d->resourceName;
}

void Person::setResourceName(const QString &value)
{
    d->resourceName = value;
}
QVector<Interest> Person::interests() const
{
    return d->interests;
}

void Person::setInterests(const QVector<Interest> &value)
{
    d->interests = value;
}

void Person::addInterest(const Interest &value)
{
    d->interests.push_back(value);
}

void Person::removeInterest(const Interest &value)
{
    d->interests.removeOne(value);
}

void Person::clearInterests()
{
    d->interests.clear();
}

QVector<Organization> Person::organizations() const
{
    return d->organizations;
}

void Person::setOrganizations(const QVector<Organization> &value)
{
    d->organizations = value;
}

void Person::addOrganization(const Organization &value)
{
    d->organizations.push_back(value);
}

void Person::removeOrganization(const Organization &value)
{
    d->organizations.removeOne(value);
}

void Person::clearOrganizations()
{
    d->organizations.clear();
}

QVector<Url> Person::urls() const
{
    return d->urls;
}

void Person::setUrls(const QVector<Url> &value)
{
    d->urls = value;
}

void Person::addUrl(const Url &value)
{
    d->urls.push_back(value);
}

void Person::removeUrl(const Url &value)
{
    d->urls.removeOne(value);
}

void Person::clearUrls()
{
    d->urls.clear();
}

QVector<Gender> Person::genders() const
{
    return d->genders;
}

void Person::setGenders(const QVector<Gender> &value)
{
    d->genders = value;
}

void Person::addGender(const Gender &value)
{
    d->genders.push_back(value);
}

void Person::removeGender(const Gender &value)
{
    d->genders.removeOne(value);
}

void Person::clearGenders()
{
    d->genders.clear();
}

QVector<FileAs> Person::fileAses() const
{
    return d->fileAses;
}

void Person::setFileAses(const QVector<FileAs> &value)
{
    d->fileAses = value;
}

void Person::addFileAs(const FileAs &value)
{
    d->fileAses.push_back(value);
}

void Person::removeFileAs(const FileAs &value)
{
    d->fileAses.removeOne(value);
}

void Person::clearFileAses()
{
    d->fileAses.clear();
}

Person::Person::AgeRange Person::ageRange() const
{
    return d->ageRange;
}
PersonMetadata Person::metadata() const
{
    return d->metadata;
}

} // namespace KGAPI2::People

/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "object.h"
#include "types.h"
#include "kgapipeople_export.h"

#include <QList>
#include <QString>

#include <QSharedPointer>

#include <optional>

class QJsonObject;
class QJsonValue;

namespace KContacts
{
class Addressee;
}

namespace KGAPI2::People
{
class Person;
using PersonPtr = QSharedPointer<Person>;

class Address;
class AgeRangeType;
class Biography;
class Birthday;
class BraggingRights;
class CalendarUrl;
class ClientData;
class CoverPhoto;
class EmailAddress;
class Event;
class ExternalId;
class FileAs;
class Gender;
class ImClient;
class Interest;
class Location;
class Membership;
class MiscKeyword;
class Name;
class Nickname;
class Occupation;
class Organization;
class PersonLocale;
class PersonMetadata;
class PhoneNumber;
class Photo;
class Relation;
class RelationshipInterest;
class RelationshipStatus;
class Residence;
class SipAddress;
class Skill;
class Tagline;
class Url;
class UserDefined;

/**
 * Information about a person merged from various data sources such as the
 * authenticated user's contacts and profile data. Most fields can have multiple
 * items. The items in a field have no guaranteed order, but each non-empty field
 * is guaranteed to have exactly one field with `metadata.primary` set to true.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#person
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT Person : public KGAPI2::Object
{
public:
    enum class AgeRange {
        AGE_RANGE_UNSPECIFIED, ///< Unspecified.
        LESS_THAN_EIGHTEEN, ///< Younger than eighteen.
        EIGHTEEN_TO_TWENTY, ///< Between eighteen and twenty.
        TWENTY_ONE_OR_OLDER, ///< Twenty-one and older.
    };

    /** Construcuts a new Person **/
    explicit Person();
    /** Destructor. **/
    ~Person();

    static PersonPtr fromKContactsAddressee(const KContacts::Addressee &addressee);
    KContacts::Addressee toKContactsAddressee() const;

    bool operator==(const Person &) const;
    bool operator!=(const Person &) const;

    /** The person's nicknames. **/
    Q_REQUIRED_RESULT QList<Nickname> nicknames() const;
    /** Sets value of the nicknames property. **/
    void setNicknames(const QList<Nickname> &value);
    /** Appends the given @c value to the list of nicknames. **/
    void addNickname(const Nickname &value);
    /** Removes the given @c value from the list of nicknames if it exists. **/
    void removeNickname(const Nickname &value);
    /** Clears the list of nicknames. **/
    void clearNicknames();

    /** The person's email addresses. For `people.connections.list` and `otherContacts.list` the number of email addresses is limited to 100. If a Person has
     * more email addresses the entire set can be obtained by calling GetPeople. **/
    Q_REQUIRED_RESULT QList<EmailAddress> emailAddresses() const;
    /** Sets value of the emailAddresses property. **/
    void setEmailAddresses(const QList<EmailAddress> &value);
    /** Appends the given @c value to the list of emailAddresses. **/
    void addEmailAddress(const EmailAddress &value);
    /** Removes the given @c value from the list of emailAddresses if it exists. **/
    void removeEmailAddress(const EmailAddress &value);
    /** Clears the list of emailAddresses. **/
    void clearEmailAddresses();

    /** The person's client data. **/
    Q_REQUIRED_RESULT QList<ClientData> clientData() const;
    /** Sets value of the clientData property. **/
    void setClientData(const QList<ClientData> &value);
    /** Appends the given @c value to the list of clientData. **/
    void addClientData(const ClientData &value);
    /** Removes the given @c value from the list of clientData if it exists. **/
    void removeClientData(const ClientData &value);
    /** Clears the list of clientData. **/
    void clearClientData();

    /** **DEPRECATED**: No data will be returned The person's bragging rights. **/
    Q_REQUIRED_RESULT QList<BraggingRights> braggingRights() const;
    /** Sets value of the braggingRights property. **/
    void setBraggingRights(const QList<BraggingRights> &value);
    /** Appends the given @c value to the list of braggingRights. **/
    void addBraggingRights(const BraggingRights &value);
    /** Removes the given @c value from the list of braggingRights if it exists. **/
    void removeBraggingRights(const BraggingRights &value);
    /** Clears the list of braggingRights. **/
    void clearBraggingRights();

    /** Output only. **DEPRECATED**: No data will be returned The person's relationship statuses. **/
    Q_REQUIRED_RESULT QList<RelationshipStatus> relationshipStatuses() const;

    /** The person's birthdays. This field is a singleton for contact sources. **/
    Q_REQUIRED_RESULT QList<Birthday> birthdays() const;
    /** Sets value of the birthdays property. **/
    void setBirthdays(const QList<Birthday> &value);
    /** Appends the given @c value to the list of birthdays. **/
    void addBirthday(const Birthday &value);
    /** Removes the given @c value from the list of birthdays if it exists. **/
    void removeBirthday(const Birthday &value);
    /** Clears the list of birthdays. **/
    void clearBirthdays();

    /** The person's relations. **/
    Q_REQUIRED_RESULT QList<Relation> relations() const;
    /** Sets value of the relations property. **/
    void setRelations(const QList<Relation> &value);
    /** Appends the given @c value to the list of relations. **/
    void addRelation(const Relation &value);
    /** Removes the given @c value from the list of relations if it exists. **/
    void removeRelation(const Relation &value);
    /** Clears the list of relations. **/
    void clearRelations();

    /** The person's locale preferences. **/
    Q_REQUIRED_RESULT QList<PersonLocale> locales() const;
    /** Sets value of the locales property. **/
    void setLocales(const QList<PersonLocale> &value);
    /** Appends the given @c value to the list of locales. **/
    void addPersonLocale(const PersonLocale &value);
    /** Removes the given @c value from the list of locales if it exists. **/
    void removePersonLocale(const PersonLocale &value);
    /** Clears the list of locales. **/
    void clearLocales();

    /** The person's locations. **/
    Q_REQUIRED_RESULT QList<Location> locations() const;
    /** Sets value of the locations property. **/
    void setLocations(const QList<Location> &value);
    /** Appends the given @c value to the list of locations. **/
    void addLocation(const Location &value);
    /** Removes the given @c value from the list of locations if it exists. **/
    void removeLocation(const Location &value);
    /** Clears the list of locations. **/
    void clearLocations();

    /** The person's external IDs. **/
    Q_REQUIRED_RESULT QList<ExternalId> externalIds() const;
    /** Sets value of the externalIds property. **/
    void setExternalIds(const QList<ExternalId> &value);
    /** Appends the given @c value to the list of externalIds. **/
    void addExternalId(const ExternalId &value);
    /** Removes the given @c value from the list of externalIds if it exists. **/
    void removeExternalId(const ExternalId &value);
    /** Clears the list of externalIds. **/
    void clearExternalIds();

    /** Output only. The person's age ranges. **/
    Q_REQUIRED_RESULT QList<AgeRangeType> ageRanges() const;

    /** Output only. **DEPRECATED**: No data will be returned The person's taglines. **/
    Q_REQUIRED_RESULT QList<Tagline> taglines() const;

    /** The person's user defined data. **/
    Q_REQUIRED_RESULT QList<UserDefined> userDefined() const;
    /** Sets value of the userDefined property. **/
    void setUserDefined(const QList<UserDefined> &value);
    /** Appends the given @c value to the list of userDefined. **/
    void addUserDefined(const UserDefined &value);
    /** Removes the given @c value from the list of userDefined if it exists. **/
    void removeUserDefined(const UserDefined &value);
    /** Clears the list of userDefined. **/
    void clearUserDefined();

    /** The person's biographies. This field is a singleton for contact sources. **/
    Q_REQUIRED_RESULT QList<Biography> biographies() const;
    /** Sets value of the biographies property. **/
    void setBiographies(const QList<Biography> &value);
    /** Appends the given @c value to the list of biographies. **/
    void addBiography(const Biography &value);
    /** Removes the given @c value from the list of biographies if it exists. **/
    void removeBiography(const Biography &value);
    /** Clears the list of biographies. **/
    void clearBiographies();

    /** The person's SIP addresses. **/
    Q_REQUIRED_RESULT QList<SipAddress> sipAddresses() const;
    /** Sets value of the sipAddresses property. **/
    void setSipAddresses(const QList<SipAddress> &value);
    /** Appends the given @c value to the list of sipAddresses. **/
    void addSipAddress(const SipAddress &value);
    /** Removes the given @c value from the list of sipAddresses if it exists. **/
    void removeSipAddress(const SipAddress &value);
    /** Clears the list of sipAddresses. **/
    void clearSipAddresses();

    /** The person's group memberships. **/
    Q_REQUIRED_RESULT QList<Membership> memberships() const;
    /** Sets value of the memberships property. **/
    void setMemberships(const QList<Membership> &value);
    /** Appends the given @c value to the list of memberships. **/
    void addMembership(const Membership &value);
    /** Removes the given @c value from the list of memberships if it exists. **/
    void removeMembership(const Membership &value);
    /** Clears the list of memberships. **/
    void clearMemberships();

    /** The person's skills. **/
    Q_REQUIRED_RESULT QList<Skill> skills() const;
    /** Sets value of the skills property. **/
    void setSkills(const QList<Skill> &value);
    /** Appends the given @c value to the list of skills. **/
    void addSkill(const Skill &value);
    /** Removes the given @c value from the list of skills if it exists. **/
    void removeSkill(const Skill &value);
    /** Clears the list of skills. **/
    void clearSkills();

    /** **DEPRECATED**: (Please use `person.locations` instead) The person's residences. **/
    Q_REQUIRED_RESULT QList<Residence> residences() const;
    /** Sets value of the residences property. **/
    void setResidences(const QList<Residence> &value);
    /** Appends the given @c value to the list of residences. **/
    void addResidence(const Residence &value);
    /** Removes the given @c value from the list of residences if it exists. **/
    void removeResidence(const Residence &value);
    /** Clears the list of residences. **/
    void clearResidences();

    /** The person's names. This field is a singleton for contact sources. **/
    Q_REQUIRED_RESULT QList<Name> names() const;
    /** Sets value of the names property. **/
    void setNames(const QList<Name> &value);
    /** Appends the given @c value to the list of names. **/
    void addName(const Name &value);
    /** Removes the given @c value from the list of names if it exists. **/
    void removeName(const Name &value);
    /** Clears the list of names. **/
    void clearNames();

    /** The person's calendar URLs. **/
    Q_REQUIRED_RESULT QList<CalendarUrl> calendarUrls() const;
    /** Sets value of the calendarUrls property. **/
    void setCalendarUrls(const QList<CalendarUrl> &value);
    /** Appends the given @c value to the list of calendarUrls. **/
    void addCalendarUrl(const CalendarUrl &value);
    /** Removes the given @c value from the list of calendarUrls if it exists. **/
    void removeCalendarUrl(const CalendarUrl &value);
    /** Clears the list of calendarUrls. **/
    void clearCalendarUrls();

    /** Output only. The person's cover photos. **/
    Q_REQUIRED_RESULT QList<CoverPhoto> coverPhotos() const;

    /** The person's street addresses. **/
    Q_REQUIRED_RESULT QList<Address> addresses() const;
    /** Sets value of the addresses property. **/
    void setAddresses(const QList<Address> &value);
    /** Appends the given @c value to the list of addresses. **/
    void addAddress(const Address &value);
    /** Removes the given @c value from the list of addresses if it exists. **/
    void removeAddress(const Address &value);
    /** Clears the list of addresses. **/
    void clearAddresses();

    /** The [HTTP entity tag](https://en.wikipedia.org/wiki/HTTP_ETag) of the resource. Used for web cache validation. **/
    Q_REQUIRED_RESULT QString etag() const;
    /** Sets value of the etag property. **/
    void setEtag(const QString &value);

    /** The person's phone numbers. For `people.connections.list` and `otherContacts.list` the number of phone numbers is limited to 100. If a Person has more
     * phone numbers the entire set can be obtained by calling GetPeople. **/
    Q_REQUIRED_RESULT QList<PhoneNumber> phoneNumbers() const;
    /** Sets value of the phoneNumbers property. **/
    void setPhoneNumbers(const QList<PhoneNumber> &value);
    /** Appends the given @c value to the list of phoneNumbers. **/
    void addPhoneNumber(const PhoneNumber &value);
    /** Removes the given @c value from the list of phoneNumbers if it exists. **/
    void removePhoneNumber(const PhoneNumber &value);
    /** Clears the list of phoneNumbers. **/
    void clearPhoneNumbers();

    /** The person's instant messaging clients. **/
    Q_REQUIRED_RESULT QList<ImClient> imClients() const;
    /** Sets value of the imClients property. **/
    void setImClients(const QList<ImClient> &value);
    /** Appends the given @c value to the list of imClients. **/
    void addImClient(const ImClient &value);
    /** Removes the given @c value from the list of imClients if it exists. **/
    void removeImClient(const ImClient &value);
    /** Clears the list of imClients. **/
    void clearImClients();

    /** The person's events. **/
    Q_REQUIRED_RESULT QList<Event> events() const;
    /** Sets value of the events property. **/
    void setEvents(const QList<Event> &value);
    /** Appends the given @c value to the list of events. **/
    void addEvent(const Event &value);
    /** Removes the given @c value from the list of events if it exists. **/
    void removeEvent(const Event &value);
    /** Clears the list of events. **/
    void clearEvents();

    /** Output only. The person's photos. **/
    Q_REQUIRED_RESULT QList<Photo> photos() const;

    /** The person's occupations. **/
    Q_REQUIRED_RESULT QList<Occupation> occupations() const;
    /** Sets value of the occupations property. **/
    void setOccupations(const QList<Occupation> &value);
    /** Appends the given @c value to the list of occupations. **/
    void addOccupation(const Occupation &value);
    /** Removes the given @c value from the list of occupations if it exists. **/
    void removeOccupation(const Occupation &value);
    /** Clears the list of occupations. **/
    void clearOccupations();

    /** The person's miscellaneous keywords. **/
    Q_REQUIRED_RESULT QList<MiscKeyword> miscKeywords() const;
    /** Sets value of the miscKeywords property. **/
    void setMiscKeywords(const QList<MiscKeyword> &value);
    /** Appends the given @c value to the list of miscKeywords. **/
    void addMiscKeyword(const MiscKeyword &value);
    /** Removes the given @c value from the list of miscKeywords if it exists. **/
    void removeMiscKeyword(const MiscKeyword &value);
    /** Clears the list of miscKeywords. **/
    void clearMiscKeywords();

    /** Output only. **DEPRECATED**: No data will be returned The person's relationship interests. **/
    Q_REQUIRED_RESULT QList<RelationshipInterest> relationshipInterests() const;

    /** The resource name for the person, assigned by the server. An ASCII string with a max length of 27 characters, in the form of `people/{person_id}`. **/
    Q_REQUIRED_RESULT QString resourceName() const;
    /** Sets value of the resourceName property. **/
    void setResourceName(const QString &value);

    /** The person's interests. **/
    Q_REQUIRED_RESULT QList<Interest> interests() const;
    /** Sets value of the interests property. **/
    void setInterests(const QList<Interest> &value);
    /** Appends the given @c value to the list of interests. **/
    void addInterest(const Interest &value);
    /** Removes the given @c value from the list of interests if it exists. **/
    void removeInterest(const Interest &value);
    /** Clears the list of interests. **/
    void clearInterests();

    /** The person's past or current organizations. **/
    Q_REQUIRED_RESULT QList<Organization> organizations() const;
    /** Sets value of the organizations property. **/
    void setOrganizations(const QList<Organization> &value);
    /** Appends the given @c value to the list of organizations. **/
    void addOrganization(const Organization &value);
    /** Removes the given @c value from the list of organizations if it exists. **/
    void removeOrganization(const Organization &value);
    /** Clears the list of organizations. **/
    void clearOrganizations();

    /** The person's associated URLs. **/
    Q_REQUIRED_RESULT QList<Url> urls() const;
    /** Sets value of the urls property. **/
    void setUrls(const QList<Url> &value);
    /** Appends the given @c value to the list of urls. **/
    void addUrl(const Url &value);
    /** Removes the given @c value from the list of urls if it exists. **/
    void removeUrl(const Url &value);
    /** Clears the list of urls. **/
    void clearUrls();

    /** The person's genders. This field is a singleton for contact sources. **/
    Q_REQUIRED_RESULT QList<Gender> genders() const;
    /** Sets value of the genders property. **/
    void setGenders(const QList<Gender> &value);
    /** Appends the given @c value to the list of genders. **/
    void addGender(const Gender &value);
    /** Removes the given @c value from the list of genders if it exists. **/
    void removeGender(const Gender &value);
    /** Clears the list of genders. **/
    void clearGenders();

    /** The person's file-ases. **/
    Q_REQUIRED_RESULT QList<FileAs> fileAses() const;
    /** Sets value of the fileAses property. **/
    void setFileAses(const QList<FileAs> &value);
    /** Appends the given @c value to the list of fileAses. **/
    void addFileAs(const FileAs &value);
    /** Removes the given @c value from the list of fileAses if it exists. **/
    void removeFileAs(const FileAs &value);
    /** Clears the list of fileAses. **/
    void clearFileAses();

    /** Output only. **DEPRECATED** (Please use `person.ageRanges` instead) The person's age range. **/
    Q_REQUIRED_RESULT Person::AgeRange ageRange() const;

    /** Output only. Metadata about the person. **/
    Q_REQUIRED_RESULT PersonMetadata metadata() const;

    Q_REQUIRED_RESULT static PersonPtr fromJSON(const QJsonObject &obj);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

private:
    class Private;
    std::unique_ptr<Private> d;
}; // Person

} // namespace KGAPI2::People

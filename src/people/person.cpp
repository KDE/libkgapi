/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "person.h"

#include "debug.h"

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
#include <QJsonObject>
#include <QJsonArray>

#include <KContacts/Addressee>

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

    bool operator==(const Private &other) const
    {
        return nicknames == other.nicknames &&
            emailAddresses == other.emailAddresses &&
            clientData == other.clientData &&
            braggingRights == other.braggingRights &&
            relationshipStatuses == other.relationshipStatuses &&
            birthdays == other.birthdays &&
            relations == other.relations &&
            locales == other.locales &&
            locations == other.locations &&
            externalIds == other.externalIds &&
            ageRanges == other.ageRanges &&
            taglines == other.taglines &&
            userDefined == other.userDefined &&
            biographies == other.biographies &&
            sipAddresses == other.sipAddresses &&
            memberships == other.memberships &&
            skills == other.skills &&
            residences == other.residences &&
            names == other.names &&
            calendarUrls == other.calendarUrls &&
            coverPhotos == other.coverPhotos &&
            addresses == other.addresses &&
            etag == other.etag &&
            phoneNumbers == other.phoneNumbers &&
            imClients == other.imClients &&
            events == other.events &&
            photos == other.photos &&
            occupations == other.occupations &&
            miscKeywords == other.miscKeywords &&
            relationshipInterests == other.relationshipInterests &&
            resourceName == other.resourceName &&
            interests == other.interests &&
            organizations == other.organizations &&
            urls == other.urls &&
            genders == other.genders &&
            fileAses == other.fileAses &&
            ageRange == other.ageRange &&
            metadata == other.metadata;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    KContacts::Addressee toKContactsAddressee()
    {
        KContacts::Addressee addressee;

        setKContactAddresseeNameFields(addressee);
        setKContactAddresseeNicknameFields(addressee);
        setKContactAddresseeBirthdayFields(addressee);
        setKContactAddresseeEmailFields(addressee);
        setKContactAddresseePhoneFields(addressee);
        setKContactAddresseeUrlFields(addressee);
        setKContactAddresseeOrganizationFields(addressee);
        setKContactAddresseeProfessionFields(addressee);
        setKContactAddresseePhoto(addressee);

        return addressee;
    }

    void setFromKContactsAddressee(const KContacts::Addressee &addressee)
    {
        if (!addressee.familyName().isEmpty() ||
            !addressee.givenName().isEmpty() ||
            !addressee.prefix().isEmpty() ||
            !addressee.suffix().isEmpty()) {

            names = {Name::fromKContactsAddressee(addressee)};
        }

        const auto addresseeNickName = addressee.nickName();
        if (!addresseeNickName.isEmpty()) {
            Nickname nickname;
            nickname.setValue(addresseeNickName);
            nicknames = {nickname};
        }

        const auto addresseeBirthday = addressee.birthday();
        if (addresseeBirthday.isValid()) {
            Birthday birthday;
            birthday.setDate(addresseeBirthday.date());
            birthdays = {birthday};
        }

        const auto addresseeEmailList = addressee.emailList();
        if (!addresseeEmailList.isEmpty()) {
            emailAddresses = EmailAddress::fromKContactsEmailList(addresseeEmailList);
        }

        const auto addresseePhoneNumbers = addressee.phoneNumbers();
        if (!addresseePhoneNumbers.isEmpty()) {
            phoneNumbers = PhoneNumber::fromKContactsPhoneNumberList(addressee.phoneNumbers());
        }

        const auto addresseeProfession = addressee.profession();
        if (!addresseeProfession.isEmpty()) {
            Occupation occupation;
            occupation.setValue(addresseeProfession);
            occupations = {occupation};
        }

        const auto addresseeOrganization = addressee.organization();
        const auto addresseeDepartment = addressee.department();
        if (!addresseeOrganization.isEmpty() || !addresseeDepartment.isEmpty()) {
            Organization organization;
            organization.setName(addresseeOrganization);
            organization.setDepartment(addresseeDepartment);
            organizations = {organization};
        }

        const auto addresseePhoto = addressee.photo();
        if (!addresseePhoto.isEmpty()) {
            Photo photo;
            photo.setUrl(addressee.photo().url());
            photos = {photo};
        }

        const auto blogFeed = addressee.blogFeed();
        if (!blogFeed.isEmpty()) {
            Url url;
            url.setValue(blogFeed.toString());
            url.setType(QStringLiteral("blog"));
            urls.append(url);
        }

        const auto addresseeUrls = addressee.extraUrlList();
        for (const auto &addresseeUrl : addresseeUrls) {
            Url url;
            url.setValue(addresseeUrl.toString());

            switch (addresseeUrl.type()) {
            case KContacts::ResourceLocatorUrl::Home:
                url.setType(QStringLiteral("home"));
                break;
            case KContacts::ResourceLocatorUrl::Profile:
                url.setType(QStringLiteral("profile"));
                break;
            case KContacts::ResourceLocatorUrl::Work:
                url.setType(QStringLiteral("work"));
                break;
            case KContacts::ResourceLocatorUrl::Ftp:
                url.setType(QStringLiteral("ftp"));
                break;
            case KContacts::ResourceLocatorUrl::AppInstallPage:
                url.setType(QStringLiteral("appInstallPage"));
                break;
            case KContacts::ResourceLocatorUrl::Reservation:
                url.setType(QStringLiteral("reservations"));
                break;
            default:
                url.setType(QStringLiteral("other"));
            }

            urls.append(url);
        }

        const auto addressessCalendarUrls = addressee.calendarUrlList();
        for (const auto &calendarUrl : addressessCalendarUrls) {
            CalendarUrl gCalendarUrl;
            if (calendarUrl.type() == KContacts::CalendarUrl::FBUrl) {
                gCalendarUrl.setType(QStringLiteral("freeBusy"));
            } else if (calendarUrl.type() == KContacts::CalendarUrl::CALUri) {
                gCalendarUrl.setType(QStringLiteral("CALUri"));
            } else if (calendarUrl.type() == KContacts::CalendarUrl::CALADRUri) {
                gCalendarUrl.setType(QStringLiteral("CALADRUri"));
            } else {
                gCalendarUrl.setType(QStringLiteral("other"));
            }
            gCalendarUrl.setUrl(calendarUrl.url().toString());
            calendarUrls.append(gCalendarUrl);
        }
    }

    QList<Nickname> nicknames{};
    QList<EmailAddress> emailAddresses{};
    QList<ClientData> clientData{};
    QList<BraggingRights> braggingRights{};
    QList<RelationshipStatus> relationshipStatuses{};
    QList<Birthday> birthdays{};
    QList<Relation> relations{};
    QList<PersonLocale> locales{};
    QList<Location> locations{};
    QList<ExternalId> externalIds{};
    QList<AgeRangeType> ageRanges{};
    QList<Tagline> taglines{};
    QList<UserDefined> userDefined{};
    QList<Biography> biographies{};
    QList<SipAddress> sipAddresses{};
    QList<Membership> memberships{};
    QList<Skill> skills{};
    QList<Residence> residences{};
    QList<Name> names{};
    QList<CalendarUrl> calendarUrls{};
    QList<CoverPhoto> coverPhotos{};
    QList<Address> addresses{};
    QString etag{};
    QList<PhoneNumber> phoneNumbers{};
    QList<ImClient> imClients{};
    QList<Event> events{};
    QList<Photo> photos{};
    QList<Occupation> occupations{};
    QList<MiscKeyword> miscKeywords{};
    QList<RelationshipInterest> relationshipInterests{};
    QString resourceName{};
    QList<Interest> interests{};
    QList<Organization> organizations{};
    QList<Url> urls{};
    QList<Gender> genders{};
    QList<FileAs> fileAses{};
    Person::AgeRange ageRange{};
    PersonMetadata metadata{};

private:
    void setKContactAddresseeNameFields(KContacts::Addressee &addressee)
    {
        if (names.isEmpty()) {
            return;
        }

        const auto nameToUse = names.first();
        nameToUse.applyToKContactsAddressee(addressee);
    }

    void setKContactAddresseeNicknameFields(KContacts::Addressee &addressee)
    {
        if(nicknames.isEmpty()) {
            addressee.setNickName(QString());
            return;
        }

        const auto nicknameToUse = nicknames.first();
        addressee.setNickName(nicknameToUse.value());
    }

    void setKContactAddresseeBirthdayFields(KContacts::Addressee &addressee)
    {
        if(birthdays.isEmpty()) {
            addressee.setBirthday(QDate());
            return;
        }

        const auto birthdayToUse = birthdays.first();
        addressee.setBirthday(birthdayToUse.date());
    }

    void setKContactAddresseeEmailFields(KContacts::Addressee &addressee)
    {
        KContacts::Email::List convertedEmails;

        std::transform(emailAddresses.cbegin(),
                       emailAddresses.cend(),
                       std::back_inserter(convertedEmails),
                       [](const EmailAddress &emailAddress) {
                           return emailAddress.toKContactsEmail();
                       });

        addressee.setEmailList(convertedEmails);
    }

    void setKContactAddresseePhoneFields(KContacts::Addressee &addressee)
    {
        KContacts::PhoneNumber::List convertedPhoneNumbers;

        std::transform(phoneNumbers.cbegin(),
                       phoneNumbers.cend(),
                       std::back_inserter(convertedPhoneNumbers),
                       [](const People::PhoneNumber &phoneNumber) {
                           return phoneNumber.toKContactsPhoneNumber();
                       });

        addressee.setPhoneNumbers(convertedPhoneNumbers);
    }

    void setKContactAddresseeOrganizationFields(KContacts::Addressee &addressee)
    {
        if (organizations.isEmpty()) {
            addressee.setOrganization(QString());
            addressee.setDepartment(QString());
            return;
        }

        const auto organizationToUse = organizations.first();
        addressee.setOrganization(organizationToUse.name());
        addressee.setDepartment(organizationToUse.department());
    }

    void setKContactAddresseeProfessionFields(KContacts::Addressee &addressee)
    {
        if (occupations.isEmpty()) {
            addressee.setProfession(QString());
            return;
        }

        const auto occupationToUse = occupations.first();
        addressee.setProfession(occupationToUse.value());
    }

    void setKContactAddresseePhoto(KContacts::Addressee &addressee)
    {
        if (photos.isEmpty()) {
            addressee.setPhoto({});
            return;
        }

        const auto photoToUse = photos.first();
        KContacts::Picture picture(photoToUse.url());
        addressee.setPhoto(picture);
    }

    void setKContactAddresseeUrlFields(KContacts::Addressee &addressee)
    {
        if (urls.isEmpty()) {
            addressee.setBlogFeed({});
            addressee.setUrl(QUrl{});
            addressee.setExtraUrlList({});
        } else {
            for (const auto &url : std::as_const(urls)) {
                if (url.type() == QStringLiteral("blog")) {
                    addressee.setBlogFeed(QUrl(url.value()));
                } else {
                    KContacts::ResourceLocatorUrl::Type type;

                    if (url.type() == QStringLiteral("home")
                        || url.type() == QStringLiteral("homePage")) {
                        type = KContacts::ResourceLocatorUrl::Home;
                    } else if (url.type() == QStringLiteral("profile")) {
                        type = KContacts::ResourceLocatorUrl::Profile;
                    } else if (url.type() == QStringLiteral("work")) {
                        type = KContacts::ResourceLocatorUrl::Work;
                    } else if (url.type() == QStringLiteral("ftp")) {
                        type = KContacts::ResourceLocatorUrl::Ftp;
                    } else if (url.type() == QStringLiteral("appInstallPage")) {
                        type = KContacts::ResourceLocatorUrl::AppInstallPage;
                    } else if (url.type() == QStringLiteral("reservations")) {
                        type = KContacts::ResourceLocatorUrl::Reservation;
                    } else {
                        type = KContacts::ResourceLocatorUrl::Other;
                    }

                    KContacts::ResourceLocatorUrl resourceUrl;
                    resourceUrl.setUrl(QUrl(url.value()));
                    resourceUrl.setType(type);
                    addressee.insertExtraUrl(resourceUrl);
                }
            }
        }


        for (const auto &calendarUrl : std::as_const(calendarUrls)) {
            KContacts::CalendarUrl kCalendarUrl;
            if (calendarUrl.type() == QStringLiteral("freeBusy")) {
                kCalendarUrl.setType(KContacts::CalendarUrl::FBUrl);
            } else if (calendarUrl.type() == QStringLiteral("CALUri")) {
                kCalendarUrl.setType(KContacts::CalendarUrl::CALUri);
            } else if (calendarUrl.type() == QStringLiteral("CALADRUri")) {
                kCalendarUrl.setType(KContacts::CalendarUrl::CALADRUri);
            } else {
                kCalendarUrl.setType(KContacts::CalendarUrl::Unknown);
            }
            kCalendarUrl.setUrl(QUrl(calendarUrl.url()));
            addressee.insertCalendarUrl(kCalendarUrl);
        }
    }
};

Person::Person()
    : KGAPI2::Object()
    , d(new Private)
{
}

Person::~Person() = default;

QList<Nickname> Person::nicknames() const
{
    return d->nicknames;
}

void Person::setNicknames(const QList<Nickname> &value)
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

QList<EmailAddress> Person::emailAddresses() const
{
    return d->emailAddresses;
}

void Person::setEmailAddresses(const QList<EmailAddress> &value)
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

QList<ClientData> Person::clientData() const
{
    return d->clientData;
}

void Person::setClientData(const QList<ClientData> &value)
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

QList<BraggingRights> Person::braggingRights() const
{
    return d->braggingRights;
}

void Person::setBraggingRights(const QList<BraggingRights> &value)
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

QList<RelationshipStatus> Person::relationshipStatuses() const
{
    return d->relationshipStatuses;
}
QList<Birthday> Person::birthdays() const
{
    return d->birthdays;
}

void Person::setBirthdays(const QList<Birthday> &value)
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

QList<Relation> Person::relations() const
{
    return d->relations;
}

void Person::setRelations(const QList<Relation> &value)
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

QList<PersonLocale> Person::locales() const
{
    return d->locales;
}

void Person::setLocales(const QList<PersonLocale> &value)
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

QList<Location> Person::locations() const
{
    return d->locations;
}

void Person::setLocations(const QList<Location> &value)
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

QList<ExternalId> Person::externalIds() const
{
    return d->externalIds;
}

void Person::setExternalIds(const QList<ExternalId> &value)
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

QList<AgeRangeType> Person::ageRanges() const
{
    return d->ageRanges;
}
QList<Tagline> Person::taglines() const
{
    return d->taglines;
}
QList<UserDefined> Person::userDefined() const
{
    return d->userDefined;
}

void Person::setUserDefined(const QList<UserDefined> &value)
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

QList<Biography> Person::biographies() const
{
    return d->biographies;
}

void Person::setBiographies(const QList<Biography> &value)
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

QList<SipAddress> Person::sipAddresses() const
{
    return d->sipAddresses;
}

void Person::setSipAddresses(const QList<SipAddress> &value)
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

QList<Membership> Person::memberships() const
{
    return d->memberships;
}

void Person::setMemberships(const QList<Membership> &value)
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

QList<Skill> Person::skills() const
{
    return d->skills;
}

void Person::setSkills(const QList<Skill> &value)
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

QList<Residence> Person::residences() const
{
    return d->residences;
}

void Person::setResidences(const QList<Residence> &value)
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

QList<Name> Person::names() const
{
    return d->names;
}

void Person::setNames(const QList<Name> &value)
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

QList<CalendarUrl> Person::calendarUrls() const
{
    return d->calendarUrls;
}

void Person::setCalendarUrls(const QList<CalendarUrl> &value)
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

QList<CoverPhoto> Person::coverPhotos() const
{
    return d->coverPhotos;
}
QList<Address> Person::addresses() const
{
    return d->addresses;
}

void Person::setAddresses(const QList<Address> &value)
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
QList<PhoneNumber> Person::phoneNumbers() const
{
    return d->phoneNumbers;
}

void Person::setPhoneNumbers(const QList<PhoneNumber> &value)
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

QList<ImClient> Person::imClients() const
{
    return d->imClients;
}

void Person::setImClients(const QList<ImClient> &value)
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

QList<Event> Person::events() const
{
    return d->events;
}

void Person::setEvents(const QList<Event> &value)
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

QList<Photo> Person::photos() const
{
    return d->photos;
}
QList<Occupation> Person::occupations() const
{
    return d->occupations;
}

void Person::setOccupations(const QList<Occupation> &value)
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

QList<MiscKeyword> Person::miscKeywords() const
{
    return d->miscKeywords;
}

void Person::setMiscKeywords(const QList<MiscKeyword> &value)
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

QList<RelationshipInterest> Person::relationshipInterests() const
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
QList<Interest> Person::interests() const
{
    return d->interests;
}

void Person::setInterests(const QList<Interest> &value)
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

QList<Organization> Person::organizations() const
{
    return d->organizations;
}

void Person::setOrganizations(const QList<Organization> &value)
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

QList<Url> Person::urls() const
{
    return d->urls;
}

void Person::setUrls(const QList<Url> &value)
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

QList<Gender> Person::genders() const
{
    return d->genders;
}

void Person::setGenders(const QList<Gender> &value)
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

QList<FileAs> Person::fileAses() const
{
    return d->fileAses;
}

void Person::setFileAses(const QList<FileAs> &value)
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

PersonPtr Person::fromJSON(const QJsonObject &obj)
{
    auto person = new Person;

    if (!obj.isEmpty()) {
        person->d->resourceName = obj.value(QStringLiteral("resourceName")).toString();
        person->d->etag = obj.value(QStringLiteral("etag")).toString();

        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        person->d->metadata = PersonMetadata::fromJSON(metadata);

        const auto addresses = obj.value(QStringLiteral("addresses")).toArray();
        person->d->addresses = Address::fromJSONArray(addresses);

        const auto ageRanges = obj.value(QStringLiteral("ageRanges")).toArray();
        person->d->ageRanges = AgeRangeType::fromJSONArray(ageRanges);

        const auto biographies = obj.value(QStringLiteral("biographies")).toArray();
        person->d->biographies = Biography::fromJSONArray(biographies);

        const auto birthdays = obj.value(QStringLiteral("birthdays")).toArray();
        person->d->birthdays = Birthday::fromJSONArray(birthdays);

        // Bragging rights are deprecated and return nothing. Pass

        const auto calendarUrls = obj.value(QStringLiteral("calendarUrls")).toArray();
        person->d->calendarUrls = CalendarUrl::fromJSONArray(calendarUrls);

        const auto clientData = obj.value(QStringLiteral("clientData")).toArray();
        person->d->clientData = ClientData::fromJSONArray(clientData);

        const auto coverPhotos = obj.value(QStringLiteral("coverPhotos")).toArray();
        person->d->coverPhotos = CoverPhoto::fromJSONArray(coverPhotos);

        const auto emailAddresses = obj.value(QStringLiteral("emailAddresses")).toArray();
        person->d->emailAddresses = EmailAddress::fromJSONArray(emailAddresses);

        const auto events = obj.value(QStringLiteral("events")).toArray();
        person->d->events = Event::fromJSONArray(events);

        const auto externalIds = obj.value(QStringLiteral("externalIds")).toArray();
        person->d->externalIds = ExternalId::fromJSONArray(externalIds);

        const auto fileAses = obj.value(QStringLiteral("fileAses")).toArray();
        person->d->fileAses = FileAs::fromJSONArray(fileAses);

        const auto genders = obj.value(QStringLiteral("genders")).toArray();
        person->d->genders = Gender::fromJSONArray(genders);

        const auto imClients = obj.value(QStringLiteral("imClients")).toArray();
        person->d->imClients = ImClient::fromJSONArray(imClients);

        const auto interests = obj.value(QStringLiteral("interests")).toArray();
        person->d->interests = Interest::fromJSONArray(interests);

        const auto locales = obj.value(QStringLiteral("locales")).toArray();
        person->d->locales = PersonLocale::fromJSONArray(locales);

        const auto locations = obj.value(QStringLiteral("locations")).toArray();
        person->d->locations = Location::fromJSONArray(locations);

        const auto memberships = obj.value(QStringLiteral("memberships")).toArray();
        person->d->memberships = Membership::fromJSONArray(memberships);

        const auto miscKeywords = obj.value(QStringLiteral("miscKeywords")).toArray();
        person->d->miscKeywords = MiscKeyword::fromJSONArray(miscKeywords);

        const auto names = obj.value(QStringLiteral("names")).toArray();
        person->d->names = Name::fromJSONArray(names);

        const auto nicknames = obj.value(QStringLiteral("nicknames")).toArray();
        person->d->nicknames = Nickname::fromJSONArray(nicknames);

        const auto occupations = obj.value(QStringLiteral("occupations")).toArray();
        person->d->occupations = Occupation::fromJSONArray(occupations);

        const auto organizations = obj.value(QStringLiteral("organizations")).toArray();
        person->d->organizations = Organization::fromJSONArray(organizations);

        const auto phoneNumbers = obj.value(QStringLiteral("phoneNumbers")).toArray();
        person->d->phoneNumbers = PhoneNumber::fromJSONArray(phoneNumbers);

        const auto photos = obj.value(QStringLiteral("photos")).toArray();
        person->d->photos = Photo::fromJSONArray(photos);

        const auto relations = obj.value(QStringLiteral("relations")).toArray();
        person->d->relations = Relation::fromJSONArray(relations);

        // relationshipInterest is deprecated, provides no data
        // relationshipStatus is also deprecated
        // residence is also deprecated

        const auto sipAddresses = obj.value(QStringLiteral("sipAddresses")).toArray();
        person->d->sipAddresses = SipAddress::fromJSONArray(sipAddresses);

        const auto skills = obj.value(QStringLiteral("skills")).toArray();
        person->d->skills = Skill::fromJSONArray(skills);

        // tagline is deprecated, provides no data

        const auto urls = obj.value(QStringLiteral("urls")).toArray();
        person->d->urls = Url::fromJSONArray(urls);

        const auto userDefined = obj.value(QStringLiteral("userDefined")).toArray();
        person->d->userDefined = UserDefined::fromJSONArray(userDefined);
    }

    return People::PersonPtr(person);
}

QJsonValue Person::toJSON() const
{
    QJsonObject returnObject;
    returnObject.insert(QStringLiteral("resourceName"), d->resourceName);
    returnObject.insert(QStringLiteral("etag"), d->etag);

    returnObject.insert(QStringLiteral("metadata"), d->metadata.toJSON());

    QJsonArray addressesArray;
    for (const auto &address : std::as_const(d->addresses)) {
        addressesArray.append(address.toJSON());
    }
    if (!addressesArray.isEmpty()) {
        returnObject.insert(QStringLiteral("addresses"), addressesArray);
    }

    /* Output only field
    QJsonArray ageRangesArray;
    for (const auto &ageRange : d->ageRanges) {
        ageRangesArray.append(ageRange.toJSON());
    }
    if (!ageRangesArray.isEmpty()) {
        returnObject.insert(QStringLiteral("ageRanges"), ageRangesArray);
    }
    */

    QJsonArray biographiesArray;
    for (const auto &biography : std::as_const(d->biographies)) {
        biographiesArray.append(biography.toJSON());
    }
    if (!biographiesArray.isEmpty()) {
        returnObject.insert(QStringLiteral("biographies"), biographiesArray);
    }

    QJsonArray birthdaysArray;
    for (const auto &birthday : std::as_const(d->birthdays)) {
        birthdaysArray.append(birthday.toJSON());
    }
    if (!birthdaysArray.isEmpty()) {
        returnObject.insert(QStringLiteral("birthdays"), birthdaysArray);
    }

    QJsonArray calendarUrlsArray;
    for (const auto &calendarUrl : std::as_const(d->calendarUrls)) {
        calendarUrlsArray.append(calendarUrl.toJSON());
    }
    if (!calendarUrlsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("calendarUrls"), calendarUrlsArray);
    }

    QJsonArray clientDataArray;
    for (const auto &clientData : std::as_const(d->clientData)) {
        clientDataArray.append(clientData.toJSON());
    }
    if (!clientDataArray.isEmpty()) {
        returnObject.insert(QStringLiteral("clientData"), clientDataArray);
    }

    /* Output only field
    QJsonArray coverPhotosArray;
    for (const auto &coverPhoto : d->coverPhotos) {
        coverPhotosArray.append(coverPhoto.toJSON());
    }
    if (!coverPhotosArray.isEmpty()) {
        returnObject.insert(QStringLiteral("coverPhotos"), coverPhotosArray);
    }
    */

    QJsonArray emailAddressesArray;
    for (const auto &emailAddress : std::as_const(d->emailAddresses)) {
        emailAddressesArray.append(emailAddress.toJSON());
    }
    if (!emailAddressesArray.isEmpty()) {
        returnObject.insert(QStringLiteral("emailAddresses"), emailAddressesArray);
    }

    QJsonArray eventsArray;
    for (const auto &event : std::as_const(d->events)) {
        eventsArray.append(event.toJSON());
    }
    if (!eventsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("events"), eventsArray);
    }

    QJsonArray externalIdsArray;
    for (const auto &externalId : std::as_const(d->externalIds)) {
        externalIdsArray.append(externalId.toJSON());
    }
    if (!externalIdsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("externalIds"), externalIdsArray);
    }

    QJsonArray fileAsesArray;
    for (const auto &fileAs : std::as_const(d->fileAses)) {
        fileAsesArray.append(fileAs.toJSON());
    }
    if (!fileAsesArray.isEmpty()) {
        returnObject.insert(QStringLiteral("fileAses"), fileAsesArray);
    }

    QJsonArray gendersArray;
    for (const auto &gender : std::as_const(d->genders)) {
        gendersArray.append(gender.toJSON());
    }
    if (!gendersArray.isEmpty()) {
        returnObject.insert(QStringLiteral("genders"), gendersArray);
    }

    QJsonArray imClientsArray;
    for (const auto &imClient : std::as_const(d->imClients)) {
        imClientsArray.append(imClient.toJSON());
    }
    if (!imClientsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("imClients"), imClientsArray);
    }

    QJsonArray interestsArray;
    for (const auto &interest : std::as_const(d->interests)) {
        interestsArray.append(interest.toJSON());
    }
    if (!interestsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("interests"), interestsArray);
    }

    QJsonArray localesArray;
    for (const auto &locale : std::as_const(d->locales)) {
        localesArray.append(locale.toJSON());
    }
    if (!localesArray.isEmpty()) {
        returnObject.insert(QStringLiteral("locales"), localesArray);
    }

    QJsonArray locationsArray;
    for (const auto &location : std::as_const(d->locations)) {
        locationsArray.append(location.toJSON());
    }
    if (!locationsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("locations"), locationsArray);
    }

    QJsonArray membershipsArray;
    for (const auto &membership : std::as_const(d->memberships)) {
        membershipsArray.append(membership.toJSON());
    }
    if (!membershipsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("memberships"), membershipsArray);
    }

    QJsonArray miscKeywordsArray;
    for (const auto &miscKeyword : std::as_const(d->miscKeywords)) {
        miscKeywordsArray.append(miscKeyword.toJSON());
    }
    if (!miscKeywordsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("miscKeywords"), miscKeywordsArray);
    }

    QJsonArray namesArray;
    for (const auto &name : std::as_const(d->names)) {
        namesArray.append(name.toJSON());
    }
    if (!namesArray.isEmpty()) {
        returnObject.insert(QStringLiteral("names"), namesArray);
    }

    QJsonArray nicknamesArray;
    for (const auto &nickname : std::as_const(d->nicknames)) {
        nicknamesArray.append(nickname.toJSON());
    }
    if (!nicknamesArray.isEmpty()) {
        returnObject.insert(QStringLiteral("nicknames"), nicknamesArray);
    }

    QJsonArray occupationsArray;
    for (const auto &occupation : std::as_const(d->occupations)) {
        occupationsArray.append(occupation.toJSON());
    }
    if (!occupationsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("occupations"), occupationsArray);
    }

    QJsonArray organizationsArray;
    for (const auto &organization : std::as_const(d->organizations)) {
        organizationsArray.append(organization.toJSON());
    }
    if (!organizationsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("organizations"), organizationsArray);
    }

    QJsonArray phoneNumbersArray;
    for (const auto &phoneNumber : std::as_const(d->phoneNumbers)) {
        phoneNumbersArray.append(phoneNumber.toJSON());
    }
    if (!phoneNumbersArray.isEmpty()) {
        returnObject.insert(QStringLiteral("phoneNumbers"), phoneNumbersArray);
    }

    /* Output only field
    QJsonArray photosArray;
    for (const auto &photo : d->photos) {
        photosArray.append(photo.toJSON());
    }
    if (!photosArray.isEmpty()) {
        returnObject.insert(QStringLiteral("photos"), photosArray);
    }
    */

    QJsonArray relationsArray;
    for (const auto &relation : std::as_const(d->relations)) {
        relationsArray.append(relation.toJSON());
    }
    if (!relationsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("relations"), relationsArray);
    }

    // relationshipInterest is deprecated, provides no data
    // relationshipStatus is also deprecated
    // residence is also deprecated

    QJsonArray sipAddressesArray;
    for (const auto &sipAddress : std::as_const(d->sipAddresses)) {
        sipAddressesArray.append(sipAddress.toJSON());
    }
    if (!sipAddressesArray.isEmpty()) {
        returnObject.insert(QStringLiteral("sipAddresses"), sipAddressesArray);
    }

    QJsonArray skillsArray;
    for (const auto &skill : std::as_const(d->skills)) {
        skillsArray.append(skill.toJSON());
    }
    if (!skillsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("skills"), skillsArray);
    }

    QJsonArray urlsArray;
    for (const auto &url : std::as_const(d->urls)) {
        urlsArray.append(url.toJSON());
    }
    if (!urlsArray.isEmpty()) {
        returnObject.insert(QStringLiteral("urls"), urlsArray);
    }

    QJsonArray userDefinedArray;
    for (const auto &userDefined : std::as_const(d->userDefined)) {
        userDefinedArray.append(userDefined.toJSON());
    }
    if (!userDefinedArray.isEmpty()) {
        returnObject.insert(QStringLiteral("userDefined"), userDefinedArray);
    }

    return returnObject;
}

KContacts::Addressee Person::toKContactsAddressee() const
{
    return d->toKContactsAddressee();
}

PersonPtr Person::fromKContactsAddressee(const KContacts::Addressee &addressee)
{
    auto person = new Person;
    person->d->setFromKContactsAddressee(addressee);
    return PersonPtr(person);
}

bool Person::operator==(const Person &other) const
{
    if (!Object::operator==(other)) {
        return false;
    }

    if (d->resourceName != other.d->resourceName) {
        qCDebug(KGAPIDebug) << "Resource name does not match"
                            << d->resourceName
                            << other.d->resourceName;
        return false;
    }
    if (d->etag != other.d->etag) {
        qCDebug(KGAPIDebug) << "Etag does not match"
                            << d->etag
                            << other.d->etag;
        return false;
    }
    if (d->metadata != other.d->metadata) {
        qCDebug(KGAPIDebug) << "Metadata does not match"
                            << d->metadata.deleted() << other.d->metadata.deleted()
                            << d->metadata.linkedPeopleResourceNames() << other.d->metadata.linkedPeopleResourceNames()
                            << d->metadata.previousResourceNames() << other.d->metadata.previousResourceNames();
        return false;
    }

    return *d.get() == *d.get();
}

} // namespace KGAPI2::People

/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#include "contact.h"
#include "common_p.h"
#include "../debug.h"

#include <KContacts/Address>
#include <KContacts/PhoneNumber>
#include <QMap>

namespace {
static const QString schemeUrl = QStringLiteral("http://schemas.google.com/g/2005#");

static const QString imSchemeJabber = QStringLiteral("JABBER");
static const QString imSchemeICQ = QStringLiteral("ICQ");
static const QString imSchemeGoogle_Talk = QStringLiteral("GOOGLE_TALK");
static const QString imSchemeGoogleTalk = QStringLiteral("GOOGLETALK");
static const QString imSchemeQQ = QStringLiteral("QQ");
static const QString imSchemeSkype = QStringLiteral("SKYPE");
static const QString imSchemeYahoo = QStringLiteral("YAHOO");
static const QString imSchemeMSN = QStringLiteral("MSN");
static const QString imSchemeAIM = QStringLiteral("AIM");
static const QString imSchemeOther = QStringLiteral("Other");
static const QString imSchemeXMPP = QStringLiteral("XMPP");
static const QString addressTypeWork = QStringLiteral("work");
static const QString addressTypeHome = QStringLiteral("home");
static const QString addressTypeOther = QStringLiteral("other");
static const QString phoneSchemeWorkMobile = QStringLiteral("work_mobile");
static const QString phoneSchemeWorkFax = QStringLiteral("work_fax");
static const QString phoneSchemeWorkPager = QStringLiteral("work_pager");
static const QString phoneSchemeCompanyMain = QStringLiteral("company_main");
static const QString phoneSchemeWork = QStringLiteral("work");
static const QString phoneSchemeHomeFax = QStringLiteral("home_fax");
static const QString phoneSchemeHome = QStringLiteral("home");
static const QString phoneSchemeMain = QStringLiteral("main");
static const QString phoneSchemeCar = QStringLiteral("car");
static const QString phoneSchemeISDN = QStringLiteral("isdn");
static const QString phoneSchemeFax = QStringLiteral("fax");
static const QString phoneSchemeMobile = QStringLiteral("mobile");
static const QString phoneSchemePager = QStringLiteral("pager");
static const QString phoneSchemeOther = QStringLiteral("other");
static const QString emailSchemeWork = QStringLiteral("work");
static const QString emailSchemeHome = QStringLiteral("home");
static const QString emailSchemeOther = QStringLiteral("other");
}

using namespace KGAPI2;

class Q_DECL_HIDDEN Contact::Private
{
public:
    bool deleted = false;
    QUrl photoUrl;

    /* group URI, isDeleted */
    QMap<QString, bool> groups;

    QDateTime updated;
    QDateTime created;
};

Contact::Contact():
    Object(),
    KContacts::Addressee(),
    d(new Private)
{
}

Contact::Contact(const Contact &other):
    Object(other),
    KContacts::Addressee(other),
    d(new Private(*(other.d)))
{
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    const QStringList groups = custom(propertyGCalendar, propertyGroupMembershipInfo).split(QLatin1Char(','), QString::SkipEmptyParts);
#else
    const QStringList groups = custom(propertyGCalendar, propertyGroupMembershipInfo).split(QLatin1Char(','), Qt::SkipEmptyParts);
#endif
    for (const QString &group : groups) {
        d->groups.insert(group, false);
    }
}

Contact::Contact(const KContacts::Addressee& other):
    Object(),
    KContacts::Addressee(other),
    d(new Private)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    const QStringList groups = custom(propertyGCalendar, propertyGroupMembershipInfo).split(QLatin1Char(','), QString::SkipEmptyParts);
#else
    const QStringList groups = custom(propertyGCalendar, propertyGroupMembershipInfo).split(QLatin1Char(','), Qt::SkipEmptyParts);
#endif
    for (const QString &group : groups) {
        d->groups.insert(group, false);
    }
}

Contact::~Contact() = default;

bool Contact::operator==(const Contact &other) const
{
    if (!KContacts::Addressee::operator==(other)) {
        return false;
    }
    if (!Object::operator==(other)) {
        return false;
    }

    if (d->deleted != other.d->deleted) {
        qCDebug(KGAPIDebug) << "Deleted does not match";
        return false;
    }
    if (d->photoUrl != other.d->photoUrl) {
        qCDebug(KGAPIDebug) << "PhotoUrl does not match";
        return false;
    }
    if (d->groups != other.d->groups) {
        qCDebug(KGAPIDebug) << "Groups don't match";
        return false;
    }
    if (d->updated != other.d->updated) {
        qCDebug(KGAPIDebug) << "Updated does not match";
        return false;
    }
    if (d->created != other.d->created) {
        qCDebug(KGAPIDebug) << "Created does not match";
        return false;
    }

    return true;
}


void Contact::setDeleted(const bool deleted)
{
    d->deleted = deleted;
}

bool Contact::deleted() const
{
    return d->deleted;
}

void Contact::setPhotoUrl(const QString& photoUrl)
{
    d->photoUrl = QUrl(photoUrl);
}

void Contact::setPhotoUrl(const QUrl& photoUrl)
{
    d->photoUrl = photoUrl;
}

QUrl Contact::photoUrl() const
{
    return d->photoUrl;
}

void Contact::setCreated(const QDateTime &created)
{
    d->created = created;
}

QDateTime Contact::created() const
{
    return d->created;
}

void Contact::setUpdated(const QDateTime& updated)
{
    d->updated = updated;
}

QDateTime Contact::updated() const
{
    return d->updated;
}

void Contact::addGroup(const QString &group)
{
    if (d->groups.contains(group))
        return;

    d->groups.insert(group, false);

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QStringList groups = custom(propertyGCalendar, propertyGroupMembershipInfo).split(QLatin1Char(','), QString::SkipEmptyParts);
#else
    QStringList groups = custom(propertyGCalendar, propertyGroupMembershipInfo).split(QLatin1Char(','), Qt::SkipEmptyParts);
#endif
    if (!groups.contains(group)) {
        groups.append(group);
    }

    insertCustom(propertyGCalendar, propertyGroupMembershipInfo, groups.join(QLatin1Char(',')));
}

void Contact::setGroups(const QStringList &groups)
{
    insertCustom(propertyGCalendar, propertyGroupMembershipInfo, groups.join(QLatin1Char(',')));

    d->groups.clear();
    for (const QString &group : groups) {
        d->groups.insert(group, false);
    }
}

QStringList Contact::groups() const
{
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    return custom(propertyGCalendar, propertyGroupMembershipInfo).split(QLatin1Char(','), QString::SkipEmptyParts);
#else
    return custom(propertyGCalendar, propertyGroupMembershipInfo).split(QLatin1Char(','), Qt::SkipEmptyParts);
#endif
}

void Contact::clearGroups()
{
    const QStringList groups = d->groups.keys();
    for (const QString &group : groups) {
        d->groups.insert(group, true);
    }
}

void Contact::removeGroup(const QString& group)
{
    if (d->groups.contains(group)) {
        d->groups.insert(group, true);
    }
}

bool Contact::groupIsDeleted(const QString& group) const
{
    if (d->groups.contains(group)) {
        return d->groups.value(group);
    }

    return false;
}

QString Contact::IMProtocolToScheme(const Contact::IMProtocol protocol)
{
    switch (protocol) {
    case Jabber:
      return imSchemeJabber;
    case ICQ:
      return imSchemeICQ;
    case GoogleTalk:
      return imSchemeGoogle_Talk;
    case QQ:
      return imSchemeQQ;
    case Skype:
      return imSchemeSkype;
    case Yahoo:
      return imSchemeYahoo;
    case MSN:
      return imSchemeMSN;
    case AIM:
      return imSchemeAIM;
    default:
      return imSchemeOther;
    }

    return imSchemeOther;
}

QString Contact::IMSchemeToProtocolName(const QString& scheme)
{
    auto newScheme = scheme.midRef(scheme.lastIndexOf(QLatin1Char('#')) + 1);
    if (newScheme.compare(imSchemeGoogle_Talk, Qt::CaseInsensitive) == 0) {
        return imSchemeGoogleTalk.toLower();
    }
    return newScheme.toString();
}

QString Contact::IMProtocolNameToScheme(const QString& protocolName)
{
    QString proto;
    const auto protoName = protocolName.toUpper();
    if (protoName == imSchemeXMPP) {
      return schemeUrl + imSchemeJabber;
    } else if (protoName == imSchemeICQ ||
               protoName == imSchemeGoogle_Talk ||
               protoName == imSchemeQQ ||
               protoName == imSchemeSkype ||
               protoName == imSchemeYahoo ||
               protoName == imSchemeMSN ||
               protoName == imSchemeAIM)
    {
        return schemeUrl + protoName;
    } else if (protoName == imSchemeGoogleTalk) {
        return schemeUrl + imSchemeGoogle_Talk;
    }

    /* If the protocolName is not officially supported by Google, then instead
     * of full scheme, Google expects just a name of the protocol. */
    return protocolName;
}

Contact::IMProtocol Contact::IMSchemeToProtocol(const QString& scheme)
{
    const auto protoName = scheme.midRef(scheme.lastIndexOf(QLatin1Char('#')) + 1);

    if (protoName == imSchemeJabber || protoName == imSchemeXMPP) {
        return Jabber;
    } else if(protoName == imSchemeICQ ) {
        return ICQ;
    } else if (protoName == imSchemeGoogleTalk || protoName == imSchemeGoogle_Talk) {
        return GoogleTalk;
    } else if (protoName == imSchemeQQ) {
        return QQ;
    } else if (protoName == imSchemeSkype) {
        return Skype;
    } else if (protoName == imSchemeYahoo) {
        return Yahoo;
    } else if (protoName == imSchemeMSN) {
        return MSN;
    } else if (protoName == imSchemeAIM) {
        return AIM;
    }

    return Other;
}

QString Contact::addressTypeToScheme(const KContacts::Address::Type type, bool *primary)
{
    QString typeName;

    if (primary)
        *primary = (type & KContacts::Address::Pref);

    if (type & KContacts::Address::Work) {
        typeName = addressTypeWork;
    } else if (type & KContacts::Address::Home) {
        typeName = addressTypeHome;
    } else {
        typeName = addressTypeOther;
    }

    return schemeUrl + typeName;
}

KContacts::Address::Type Contact::addressSchemeToType(const QString& scheme, const bool primary)
{
    const auto typeName = scheme.midRef(scheme.lastIndexOf(QLatin1Char('#')) + 1);
    KContacts::Address::Type type;

    if (typeName == addressTypeWork) {
        type = KContacts::Address::Work;
    } else {
        type = KContacts::Address::Home;
    }

    if (primary) {
        type |= KContacts::Address::Pref;
    }

    return type;
}

QString Contact::phoneTypeToScheme(const KContacts::PhoneNumber::Type type)
{
    QString typeName;

    if ((type & (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Cell)) == (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Cell)) {
        typeName = phoneSchemeWorkMobile;
    } else if ((type & (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Fax)) == (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Fax)) {
        typeName = phoneSchemeWorkFax;
    } else if ((type & (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Pager)) == (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Pager)) {
        typeName = phoneSchemeWorkPager;
    } else if ((type & (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Pref)) == (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Pref)) {
        typeName = phoneSchemeCompanyMain;
    } else if (type & KContacts::PhoneNumber::Work) {
        typeName = phoneSchemeWork;
    } else if ((type & (KContacts::PhoneNumber::Home | KContacts::PhoneNumber::Fax)) == (KContacts::PhoneNumber::Home | KContacts::PhoneNumber::Fax)) {
        typeName = phoneSchemeHomeFax;
    } else if (type & KContacts::PhoneNumber::Home) {
        typeName = phoneSchemeHome;
    } else if (type & KContacts::PhoneNumber::Pref) {
        typeName = phoneSchemeMain;
    } else if (type & KContacts::PhoneNumber::Car) {
        typeName = phoneSchemeCar;
    } else if (type & KContacts::PhoneNumber::Isdn) {
        typeName = phoneSchemeISDN;
    } else if (type & KContacts::PhoneNumber::Fax) {
        typeName = phoneSchemeFax;
    } else if (type & KContacts::PhoneNumber::Cell) {
        typeName = phoneSchemeMobile;
    } else if (type & KContacts::PhoneNumber::Pager) {
        typeName = phoneSchemePager;
    } else {
        typeName = phoneSchemeOther;
    }

    return schemeUrl + typeName;
}

KContacts::PhoneNumber::Type Contact::phoneSchemeToType(const QString& scheme)
{
    const auto typeName = scheme.midRef(scheme.lastIndexOf(QLatin1Char('#')) + 1);
    KContacts::PhoneNumber::Type type = {};

    if (typeName == phoneSchemeCar) {
        type |= KContacts::PhoneNumber::Car;
    } else if (typeName == phoneSchemeFax) {
        type |= KContacts::PhoneNumber::Fax;
    } else if (typeName == phoneSchemeISDN) {
        type |= KContacts::PhoneNumber::Isdn;
    } else if (typeName == phoneSchemeMobile) {
        type |= KContacts::PhoneNumber::Cell;
    } else if (typeName == phoneSchemePager) {
        type |= KContacts::PhoneNumber::Pager;
    } else if (typeName == phoneSchemeMain) {
        type |= KContacts::PhoneNumber::Pref;
    } else if (typeName == phoneSchemeHome) {
        type |= KContacts::PhoneNumber::Home;
    } else if (typeName == phoneSchemeHomeFax) {
        type |= KContacts::PhoneNumber::Home | KContacts::PhoneNumber::Fax;
    } else if (typeName == phoneSchemeWork) {
        type |= KContacts::PhoneNumber::Work;
    } else if (typeName == phoneSchemeWorkFax) {
        type |= KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Fax;
    } else if (typeName == phoneSchemeWorkMobile) {
        type |= KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Cell;
    } else if (typeName == phoneSchemeWorkPager) {
        type |= KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Pager;
    } else if (typeName == phoneSchemeCompanyMain) {
        type |= KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Pref;
    } else {
        type |= KContacts::PhoneNumber::Home;
    }

    return type;
}

QString Contact::emailSchemeToProtocolName(const QString &scheme)
{
    const auto type = scheme.midRef(scheme.lastIndexOf(QLatin1Char('#')) + 1);
    if (type == emailSchemeWork) {
        return emailSchemeWork.toUpper();
    } else if (type == emailSchemeHome) {
        return emailSchemeHome.toUpper();
    } else if (type == emailSchemeOther) {
        return emailSchemeOther.toUpper();
    }
    return type.toString();
}


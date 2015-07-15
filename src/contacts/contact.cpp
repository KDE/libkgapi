/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */


#include "contact.h"

#include <KContacts/Address>
#include <KContacts/PhoneNumber>
#include <QMap>

#define SCHEME_URL QLatin1String("http://schemas.google.com/g/2005#")

using namespace KGAPI2;

class Contact::Private
{

  public:
    Private() : deleted(false) { };
    Private(const Private &other);
    ~Private() { };

    bool deleted;
    QUrl photoUrl;

    /* group URI, isDeleted */
    QMap< QString, bool > groups;

    QDateTime updated;
    QDateTime created;
};


Contact::Private::Private(const Private &other):
    deleted(other.deleted),
    photoUrl(other.photoUrl),
    groups(other.groups),
    updated(other.updated),
    created(other.created)
{
}

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
    QStringList groups = custom(QLatin1String("GCALENDAR"), QLatin1String("groupMembershipInfo")).split(QLatin1Char(','), QString::SkipEmptyParts);
    Q_FOREACH(const QString &group, groups) {
        d->groups.insert(group, false);
    }
}

Contact::Contact(const KContacts::Addressee& other):
    Object(),
    KContacts::Addressee(other),
    d(new Private)
{
    QStringList groups = custom(QLatin1String("GCALENDAR"), QLatin1String("groupMembershipInfo")).split(QLatin1Char(','), QString::SkipEmptyParts);
    Q_FOREACH(const QString &group, groups) {
        d->groups.insert(group, false);
    }
}

Contact::~Contact()
{
    delete d;
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

void Contact::setSpousesName(const QString &name)
{
    insertCustom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-SpousesName"), name);
}

QString Contact::spousesName() const
{
    return custom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-SpousesName"));
}

void Contact::setManagersName(const QString &name)
{
    insertCustom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-ManagersName"), name);
}

QString Contact::managersName() const
{
    return custom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-ManagersName"));
}

void Contact::setAssistantsName(const QString &name)
{
    insertCustom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-AssistantsName"), name);
}

QString Contact::assistantsName() const
{
    return custom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-AssistantsName"));
}

void Contact::setProfession(const QString &profession)
{
    insertCustom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-Profession"), profession);
}

QString Contact::profession() const
{
    return custom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-Profession"));
}

void Contact::setOffice(const QString &office)
{
    insertCustom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-Office"), office);
}

QString Contact::office() const
{
    return custom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-Office"));
}

void Contact::setAnniversary(const QString &anniversary)
{
    insertCustom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-Anniversary"), anniversary);
}

QString Contact::anniversary() const
{
    return custom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-Anniversary"));
}

void Contact::setBlogFeed(const QString &url)
{
    insertCustom(QLatin1String("KADDRESSBOOK"), QLatin1String("BlogFeed"), url);
}

QString Contact::blogFeed() const
{
    return custom(QLatin1String("KADDRESSBOOK"), QLatin1String("BlogFeed"));
}

void Contact::addGroup(const QString &group)
{
    if (d->groups.contains(group))
        return;

    d->groups.insert(group, false);

    QStringList groups = custom(QLatin1String("GCALENDAR"), QLatin1String("groupMembershipInfo")).split(QLatin1Char(','), QString::SkipEmptyParts);
    if (!groups.contains(group)) {
        groups.append(group);
    }

    insertCustom(QLatin1String("GCALENDAR"), QLatin1String("groupMembershipInfo"), groups.join(QLatin1String(",")));
}

void Contact::setGroups(const QStringList &groups)
{
    insertCustom(QLatin1String("GCALENDAR"), QLatin1String("groupMembershipInfo"), groups.join(QLatin1String(",")));

    d->groups.clear();
    Q_FOREACH(const QString &group, groups) {
        d->groups.insert(group, false);
    }
}

QStringList Contact::groups() const
{
    return custom(QLatin1String("GCALENDAR"), QLatin1String("groupMembershipInfo")).split(QLatin1Char(','), QString::SkipEmptyParts);
}

void Contact::clearGroups()
{
    QStringList groups = d->groups.keys();
    Q_FOREACH(const QString &group, groups) {
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
        return QLatin1String("JABBER");
    case ICQ:
        return QLatin1String("ICQ");
    case GoogleTalk:
        return QLatin1String("GOOGLE_TALK");
    case QQ:
        return QLatin1String("QQ");
    case Skype:
        return QLatin1String("SKYPE");
    case Yahoo:
        return QLatin1String("YAHOO");
    case MSN:
        return QLatin1String("MSN");
    case AIM:
        return QLatin1String("AIM");
    default:
        return QLatin1String("Other");
    }

    return QLatin1String("Other");
}

QString Contact::IMSchemeToProtocolName(const QString& scheme)
{
    return scheme.mid(scheme.lastIndexOf(QLatin1Char('#')) + 1).toLower();
}

QString Contact::IMProtocolNameToScheme(const QString& protocolName)
{
    QString proto;
    if (protocolName.toUpper() == QLatin1String("XMPP")) {
        return SCHEME_URL + QLatin1String("JABBER");
    } else if ((protocolName.toUpper() == QLatin1String("ICQ")) ||
               (protocolName.toUpper() == QLatin1String("GOOGLE_TALK")) ||
               (protocolName.toUpper() == QLatin1String("QQ")) ||
               (protocolName.toUpper() == QLatin1String("SKYPE")) ||
               (protocolName.toUpper() == QLatin1String("YAHOO")) ||
               (protocolName.toUpper() == QLatin1String("MSN")) ||
               (protocolName.toUpper() == QLatin1String("AIM")))
    {
        return SCHEME_URL + protocolName.toUpper();
    }

    /* If the protocolName is not officially supported by Google, then instead
     * of full scheme, Google expects just a name of the protocol. */
    return protocolName;
}

Contact::IMProtocol Contact::IMSchemeToProtocol(const QString& scheme)
{
    QString protoName = scheme.mid(scheme.lastIndexOf(QLatin1Char('#')) + 1).toUpper();

    if ((protoName == QLatin1String("JABBER")) || (protoName == QLatin1String("XMPP")))
        return Jabber;
    if (protoName == QLatin1String("ICQ"))
        return ICQ;
    if (protoName == QLatin1String("GOOGLE_TALK"))
        return GoogleTalk;
    if (protoName == QLatin1String("QQ"))
        return QQ;
    if (protoName == QLatin1String("SKYPE"))
        return Skype;
    if (protoName == QLatin1String("YAHOO"))
        return Yahoo;
    if (protoName == QLatin1String("MSN"))
        return MSN;
    if (protoName == QLatin1String("AIM"))
        return AIM;

    return Other;
}

QString Contact::addressTypeToScheme(const KContacts::Address::Type type, bool *primary)
{
    QString typeName;

    if (primary)
        *primary = (type & KContacts::Address::Pref);

    if (type & KContacts::Address::Work) {
        typeName = QLatin1String("work");
    } else if (type & KContacts::Address::Home) {
        typeName = QLatin1String("home");
    } else {
        typeName = QLatin1String("other");
    }

    return SCHEME_URL + typeName;
}

KContacts::Address::Type Contact::addressSchemeToType(const QString& scheme, const bool primary)
{
    QString typeName = scheme.mid(scheme.lastIndexOf(QLatin1Char('#')) + 1);
    KContacts::Address::Type type;

    if (typeName == QLatin1String("work")) {
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

    if ((type & (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Cell)) == (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Cell))
        typeName = QStringLiteral("work_mobile");
    else if ((type & (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Fax)) == (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Fax))
        typeName = QStringLiteral("work_fax");
    else if ((type & (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Pager)) == (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Pager))
        typeName = QStringLiteral("work_pager");
    else if ((type & (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Pref)) == (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Pref))
        typeName = QStringLiteral("company_main");
    else if (type & KContacts::PhoneNumber::Work)
        typeName = QStringLiteral("work");
    else if ((type & (KContacts::PhoneNumber::Home | KContacts::PhoneNumber::Fax)) == (KContacts::PhoneNumber::Home | KContacts::PhoneNumber::Fax))
        typeName = QStringLiteral("home_fax");
    else if (type & KContacts::PhoneNumber::Home)
        typeName = QStringLiteral("home");
    else if (type & KContacts::PhoneNumber::Pref)
        typeName = QStringLiteral("main");
    else if (type & KContacts::PhoneNumber::Car)
        typeName = QStringLiteral("car");
    else if (type & KContacts::PhoneNumber::Isdn)
        typeName = QStringLiteral("isdn");
    else if (type & KContacts::PhoneNumber::Fax)
        typeName = QStringLiteral("fax");
    else if (type & KContacts::PhoneNumber::Cell)
        typeName = QStringLiteral("mobile");
    else if (type & KContacts::PhoneNumber::Pager)
        typeName = QStringLiteral("pager");
    else
        typeName = QStringLiteral("other");

    return SCHEME_URL + typeName;
}

KContacts::PhoneNumber::Type Contact::phoneSchemeToType(const QString& scheme)
{
    QString typeName = scheme.mid(scheme.lastIndexOf(QLatin1Char('#')) + 1);
    KContacts::PhoneNumber::Type type = 0;

    if (typeName == QLatin1String("car"))
        type |= KContacts::PhoneNumber::Car;
    else if (typeName == QLatin1String("fax"))
        type |= KContacts::PhoneNumber::Fax;
    else if (typeName == QLatin1String("isdn"))
        type |= KContacts::PhoneNumber::Isdn;
    else if (typeName == QLatin1String("mobile"))
        type |= KContacts::PhoneNumber::Cell;
    else if (typeName == QLatin1String("pager"))
        type |= KContacts::PhoneNumber::Pager;
    else if (typeName == QLatin1String("main"))
        type |= KContacts::PhoneNumber::Pref;
    else if (typeName == QLatin1String("home"))
        type |= KContacts::PhoneNumber::Home;
    else if (typeName == QLatin1String("home_fax"))
        type |= KContacts::PhoneNumber::Home | KContacts::PhoneNumber::Fax;
    else if (typeName == QLatin1String("work"))
        type |= KContacts::PhoneNumber::Work;
    else if (typeName == QLatin1String("work_fax"))
        type |= KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Fax;
    else if (typeName == QLatin1String("work_mobile"))
        type |= KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Cell;
    else if (typeName == QLatin1String("work_pager"))
        type |= KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Pager;
    else if (typeName == QLatin1String("company_main"))
        type |= KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Pref;
    else
        type |= KContacts::PhoneNumber::Home;

    return type;
}

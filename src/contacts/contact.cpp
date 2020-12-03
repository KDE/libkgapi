/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#include "contact.h"
#include "../debug.h"

#include <KContacts/Address>
#include <KContacts/PhoneNumber>
#include <QMap>

#define SCHEME_URL QLatin1String("http://schemas.google.com/g/2005#")

using namespace KGAPI2;

class Q_DECL_HIDDEN Contact::Private
{

  public:
    Private() { }
    Private(const Private &other);
    ~Private() { }

    bool deleted = false;
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
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    const QStringList groups = custom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo")).split(QLatin1Char(','), QString::SkipEmptyParts);
#else
    const QStringList groups = custom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo")).split(QLatin1Char(','), Qt::SkipEmptyParts);
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
    const QStringList groups = custom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo")).split(QLatin1Char(','), QString::SkipEmptyParts);
#else
    const QStringList groups = custom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo")).split(QLatin1Char(','), Qt::SkipEmptyParts);
#endif
    for (const QString &group : groups) {
        d->groups.insert(group, false);
    }
}

Contact::~Contact()
{
    delete d;
}

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
    QStringList groups = custom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo")).split(QLatin1Char(','), QString::SkipEmptyParts);
#else
    QStringList groups = custom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo")).split(QLatin1Char(','), Qt::SkipEmptyParts);
#endif
    if (!groups.contains(group)) {
        groups.append(group);
    }

    insertCustom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo"), groups.join(QLatin1Char(',')));
}

void Contact::setGroups(const QStringList &groups)
{
    insertCustom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo"), groups.join(QLatin1Char(',')));

    d->groups.clear();
    for (const QString &group : groups) {
        d->groups.insert(group, false);
    }
}

QStringList Contact::groups() const
{
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    return custom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo")).split(QLatin1Char(','), QString::SkipEmptyParts);
#else
    return custom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo")).split(QLatin1Char(','), Qt::SkipEmptyParts);
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
        return QStringLiteral("JABBER");
    case ICQ:
        return QStringLiteral("ICQ");
    case GoogleTalk:
        return QStringLiteral("GOOGLE_TALK");
    case QQ:
        return QStringLiteral("QQ");
    case Skype:
        return QStringLiteral("SKYPE");
    case Yahoo:
        return QStringLiteral("YAHOO");
    case MSN:
        return QStringLiteral("MSN");
    case AIM:
        return QStringLiteral("AIM");
    default:
        return QStringLiteral("Other");
    }

    return QStringLiteral("Other");
}

QString Contact::IMSchemeToProtocolName(const QString& scheme)
{
    QString newScheme = scheme.mid(scheme.lastIndexOf(QLatin1Char('#')) + 1).toLower();
    if (newScheme == QLatin1String("google_talk")) {
        newScheme = QStringLiteral("googletalk");
    }
    return newScheme;
}

QString Contact::IMProtocolNameToScheme(const QString& protocolName)
{
    QString proto {protocolName.toUpper()};
    if (proto == QLatin1String("XMPP")) {
        return SCHEME_URL + QLatin1String("JABBER");
    } else if ((proto == QLatin1String("ICQ")) ||
               (proto == QLatin1String("GOOGLE_TALK")) ||
               (proto == QLatin1String("QQ")) ||
               (proto == QLatin1String("SKYPE")) ||
               (proto == QLatin1String("YAHOO")) ||
               (proto == QLatin1String("MSN")) ||
               (proto == QLatin1String("AIM")))
    {
        return SCHEME_URL + proto;
    } else if (proto == QLatin1String("GOOGLETALK")) {
        return SCHEME_URL + QStringLiteral("GOOGLE_TALK");
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
    if (protoName == QLatin1String("GOOGLE_TALK") || protoName == QLatin1String("GOOGLETALK"))
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
        typeName = QStringLiteral("work");
    } else if (type & KContacts::Address::Home) {
        typeName = QStringLiteral("home");
    } else {
        typeName = QStringLiteral("other");
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
    KContacts::PhoneNumber::Type type = {};

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

QString Contact::emailSchemeToProtocolName(const QString &scheme)
{
    QString type = scheme.mid(scheme.lastIndexOf(QLatin1Char('#')) + 1);
    if (type == QLatin1String("work")) {
        return QStringLiteral("WORK");
    } else if (type == QLatin1String("home")) {
        return QStringLiteral("HOME");
    } else if (type == QLatin1String("other")) {
        return QStringLiteral("OTHER");
    }
    return type;
}


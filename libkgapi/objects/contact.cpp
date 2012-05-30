/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "contact.h"
#include "contact_p.h"
#include "object.h"
#include "services/contacts.h"

#include <QStringList>

#include <KABC/Address>
#include <KABC/PhoneNumber>

using namespace KGAPI::Objects;

#define SCHEME_URL "http://schemas.google.com/g/2005#"

/*************************** PRIVATE DATA ******************************/

ContactData::ContactData(const ContactData &other):
    QSharedData(other),
    deleted(other.deleted),
    photoUrl(other.photoUrl)
{ }

/***************************** CONTACT ************************************/

Contact::Contact()
{
    d = new ContactData;
}

Contact::Contact(const Contact &other):
    KGAPI::Object(other),
    KABC::Addressee(other),
    d(other.d)
{
    QStringList groups = custom("GCALENDAR", "groupMembershipInfo").split(',', QString::SkipEmptyParts);
    Q_FOREACH(const QString &group, groups) {
        d->groups.insert(group, false);
    }
}

Contact::Contact(const KABC::Addressee& other):
    Addressee(other),
    d(new ContactData)
{
    QStringList groups = custom("GCALENDAR", "groupMembershipInfo").split(',', QString::SkipEmptyParts);
    Q_FOREACH(const QString &group, groups) {
        d->groups.insert(group, false);
    }
}

Contact::~Contact()
{ }

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
    d->photoUrl = KUrl(photoUrl);
}

void Contact::setPhotoUrl(const QUrl& photoUrl)
{
    d->photoUrl = photoUrl;
}

QUrl Contact::photoUrl() const
{
    return d->photoUrl;
}

void Contact::setCreated(const KDateTime &created)
{
    d->created = created;
}

KDateTime Contact::created() const
{
    return d->created;
}

void Contact::setUpdated(const KDateTime& updated)
{
    d->updated = updated;
}

KDateTime Contact::updated() const
{
    return d->updated;
}

void Contact::setSpousesName(const QString &name)
{
    insertCustom("KADDRESSBOOK", "X-SpousesName", name);
}

QString Contact::spousesName() const
{
    return custom("KADDRESSBOOK", "X-SpousesName");
}

void Contact::setManagersName(const QString &name)
{
    insertCustom("KADDRESSBOOK", "X-ManagersName", name);
}

QString Contact::managersName() const
{
    return custom("KADDRESSBOOK", "X-ManagersName");
}

void Contact::setAssistantsName(const QString &name)
{
    insertCustom("KADDRESSBOOK", "X-AssistantsName", name);
}

QString Contact::assistantsName() const
{
    return custom("KADDRESSBOOK", "X-AssistantsName");
}

void Contact::setProfession(const QString &profession)
{
    insertCustom("KADDRESSBOOK", "X-Profession", profession);
}

QString Contact::profession() const
{
    return custom("KADDRESSBOOK", "X-Profession");
}

void Contact::setOffice(const QString &office)
{
    insertCustom("KADDRESSBOOK", "X-Office", office);
}

QString Contact::office() const
{
    return custom("KADDRESSBOOK", "X-Office");
}

void Contact::setAnniversary(const QString &anniversary)
{
    insertCustom("KADDRESSBOOK", "X-Anniversary", anniversary);
}

QString Contact::anniversary() const
{
    return custom("KADDRESSBOOK", "X-Anniversary");
}

void Contact::setBlogFeed(const QString &url)
{
    insertCustom("KADDRESSBOOK", "BlogFeed", url);
}

QString Contact::blogFeed() const
{
    return custom("KADDRESSBOOK", "BlogFeed");
}

void Contact::addGroup(const QString &group)
{
    if (d->groups.contains(group))
        return;

    d->groups.insert(group, false);

    QStringList groups = custom("GCALENDAR", "groupMembershipInfo").split(',', QString::SkipEmptyParts);
    if (!groups.contains(group)) {
        groups.append(group);
    }

    insertCustom("GCALENDAR", "groupMembershipInfo", groups.join(","));
}

void Contact::setGroups(const QStringList &groups)
{
    insertCustom("GCALENDAR", "groupMembershipInfo", groups.join(","));

    d->groups.clear();
    Q_FOREACH(const QString &group, groups) {
        d->groups.insert(group, false);
    }
}

QStringList Contact::groups() const
{
    return custom("GCALENDAR", "groupMembershipInfo").split(',', QString::SkipEmptyParts);
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
        return "JABBER";
    case ICQ:
        return "ICQ";
    case GoogleTalk:
        return "GOOGLE_TALK";
    case QQ:
        return "QQ";
    case Skype:
        return "SKYPE";
    case Yahoo:
        return "YAHOO";
    case MSN:
        return "MSN";
    case AIM:
        return "AIM";
    default:
        return "OTHER";
    }

    return "OTHER";
}

QString Contact::IMSchemeToProtocolName(const QString& scheme)
{
    return scheme.mid(scheme.lastIndexOf("#") + 1).toLower();
}

QString Contact::IMProtocolNameToScheme(const QString& protocolName)
{
    QString proto;
    if (protocolName.toUpper() == "XMPP") {
        proto = "JABBER";
    } else {
        proto = protocolName.toUpper();
    }

    return SCHEME_URL + proto;
}

Contact::IMProtocol Contact::IMSchemeToProtocol(const QString& scheme)
{
    QString protoName = scheme.mid(scheme.lastIndexOf("#") + 1).toUpper();

    if ((protoName == "JABBER") || (protoName == "XMPP"))
        return Jabber;
    if (protoName == "ICQ")
        return ICQ;
    if (protoName == "GOOGLE_TALK")
        return GoogleTalk;
    if (protoName == "QQ")
        return QQ;
    if (protoName == "SKYPE")
        return Skype;
    if (protoName == "YAHOO")
        return Yahoo;
    if (protoName == "MSN")
        return MSN;
    if (protoName == "AIM")
        return AIM;

    return Other;
}

QString Contact::addressTypeToScheme(const KABC::Address::Type type, bool *primary)
{
    QString typeName;

    if (primary)
        *primary = (type & KABC::Address::Pref);

    if (type & KABC::Address::Work) {
        typeName = "work";
    } else if (type & KABC::Address::Home) {
        typeName = "home";
    } else {
        typeName = "other";
    }

    return SCHEME_URL + typeName;
}

KABC::Address::Type Contact::addressSchemeToType(const QString& scheme, const bool primary)
{
    QString typeName = scheme.mid(scheme.lastIndexOf("#") + 1);
    KABC::Address::Type type;

    if (typeName == "work") {
        type = KABC::Address::Work;
    } else {
        type = KABC::Address::Home;
    }

    if (primary) {
        type |= KABC::Address::Pref;
    }

    return type;
}

QString Contact::phoneTypeToScheme(const KABC::PhoneNumber::Type type)
{
    QString typeName;

    if (type & (KABC::PhoneNumber::Work & KABC::PhoneNumber::Cell))
        typeName = "work_mobile";
    else if (type & (KABC::PhoneNumber::Work & KABC::PhoneNumber::Fax))
        typeName = "work_fax";
    else if (type & (KABC::PhoneNumber::Work & KABC::PhoneNumber::Pager))
        typeName = "work_pager";
    else if (type & (KABC::PhoneNumber::Work & KABC::PhoneNumber::Pref))
        typeName = "company_main";
    else if (type & KABC::PhoneNumber::Work)
        typeName = "work";
    else if (type & (KABC::PhoneNumber::Home & KABC::PhoneNumber::Fax))
        typeName = "home_fax";
    else if (type & KABC::PhoneNumber::Home)
        typeName = "home";
    else if (type & KABC::PhoneNumber::Pref)
        typeName = "main";
    else if (type & KABC::PhoneNumber::Car)
        typeName = "car";
    else if (type & KABC::PhoneNumber::Isdn)
        typeName = "isdn";
    else if (type & KABC::PhoneNumber::Fax)
        typeName = "fax";
    else if (type & KABC::PhoneNumber::Cell)
        typeName = "mobile";
    else if (type & KABC::PhoneNumber::Pager)
        typeName = "pager";
    else
        typeName = "other";

    return SCHEME_URL + typeName;
}

KABC::PhoneNumber::Type Contact::phoneSchemeToType(const QString& scheme)
{
    QString typeName = scheme.mid(scheme.lastIndexOf("#") + 1);
    KABC::PhoneNumber::Type type;

    if (typeName == "car")
        type = KABC::PhoneNumber::Car;
    else if (typeName == "fax")
        type = KABC::PhoneNumber::Fax;
    else if (typeName == "isdn")
        type = KABC::PhoneNumber::Isdn;
    else if (typeName == "mobile")
        type = KABC::PhoneNumber::Cell;
    else if (typeName == "pager")
        type = KABC::PhoneNumber::Pager;
    else if (typeName == "main")
        type = KABC::PhoneNumber::Pref;
    else if (typeName == "home")
        type = KABC::PhoneNumber::Home;
    else if (typeName == "home_fax")
        type = KABC::PhoneNumber::Home & KABC::PhoneNumber::Fax;
    else if (typeName == "work")
        type = KABC::PhoneNumber::Work;
    else if (typeName == "work_fax")
        type = KABC::PhoneNumber::Work & KABC::PhoneNumber::Fax;
    else if (typeName == "work_mobile")
        type = KABC::PhoneNumber::Work & KABC::PhoneNumber::Cell;
    else if (typeName == "work_pager")
        type = KABC::PhoneNumber::Work & KABC::PhoneNumber::Pager;
    else if (typeName == "company_main")
        type = KABC::PhoneNumber::Work & KABC::PhoneNumber::Pref;
    else
        type = KABC::PhoneNumber::Home;

    return type;
}

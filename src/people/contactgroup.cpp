/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "contactgroup.h"

#include "contactgroupmetadata.h"
#include "groupclientdata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class ContactGroup::Private : public QSharedData
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
        return formattedName == other.formattedName && memberCount == other.memberCount && etag == other.etag && groupType == other.groupType
            && clientData == other.clientData && name == other.name && metadata == other.metadata && resourceName == other.resourceName
            && memberResourceNames == other.memberResourceNames;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString formattedName{};
    int memberCount{};
    QString etag{};
    ContactGroup::GroupType groupType{};
    QVector<GroupClientData> clientData{};
    QString name{};
    ContactGroupMetadata metadata{};
    QString resourceName{};
    QVector<QString> memberResourceNames{};
};

ContactGroup::ContactGroup()
    : d(new Private)
{
}

ContactGroup::ContactGroup(const ContactGroup &) = default;
ContactGroup::ContactGroup(ContactGroup &&) noexcept = default;
ContactGroup &ContactGroup::operator=(const ContactGroup &) = default;
ContactGroup &ContactGroup::operator=(ContactGroup &&) noexcept = default;
ContactGroup::~ContactGroup() = default;

bool ContactGroup::operator==(const ContactGroup &other) const
{
    return *d == *other.d;
}

bool ContactGroup::operator!=(const ContactGroup &other) const
{
    return !(*this == other);
}

QString ContactGroup::formattedName() const
{
    return d->formattedName;
}
int ContactGroup::memberCount() const
{
    return d->memberCount;
}
QString ContactGroup::etag() const
{
    return d->etag;
}

void ContactGroup::setEtag(const QString &value)
{
    d->etag = value;
}
ContactGroup::ContactGroup::GroupType ContactGroup::groupType() const
{
    return d->groupType;
}
QVector<GroupClientData> ContactGroup::clientData() const
{
    return d->clientData;
}

void ContactGroup::setClientData(const QVector<GroupClientData> &value)
{
    d->clientData = value;
}

void ContactGroup::addGroupClientData(const GroupClientData &value)
{
    d->clientData.push_back(value);
}

void ContactGroup::removeGroupClientData(const GroupClientData &value)
{
    d->clientData.removeOne(value);
}

void ContactGroup::clearClientData()
{
    d->clientData.clear();
}

QString ContactGroup::name() const
{
    return d->name;
}

void ContactGroup::setName(const QString &value)
{
    d->name = value;
}
ContactGroupMetadata ContactGroup::metadata() const
{
    return d->metadata;
}
QString ContactGroup::resourceName() const
{
    return d->resourceName;
}

void ContactGroup::setResourceName(const QString &value)
{
    d->resourceName = value;
}
QVector<QString> ContactGroup::memberResourceNames() const
{
    return d->memberResourceNames;
}

ContactGroup ContactGroup::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return ContactGroup();
}

QJsonValue ContactGroup::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"formattedName"}, d->formattedName);
    obj.insert(QStringView{u"memberCount"}, d->memberCount);
    obj.insert(QStringView{u"etag"}, d->etag);
    switch (d->groupType) {
    case GroupType::GROUP_TYPE_UNSPECIFIED:
        obj.insert(QStringView{u"groupType"}, QStringLiteral(u"GROUP_TYPE_UNSPECIFIED"));
        break;
    case GroupType::USER_CONTACT_GROUP:
        obj.insert(QStringView{u"groupType"}, QStringLiteral(u"USER_CONTACT_GROUP"));
        break;
    case GroupType::SYSTEM_CONTACT_GROUP:
        obj.insert(QStringView{u"groupType"}, QStringLiteral(u"SYSTEM_CONTACT_GROUP"));
        break;
    }
    {
        QJsonArray arr;
        std::transform(d->clientData.cbegin(), d->clientData.cend(), std::back_inserter(arr), [](const auto &val) {
            return val.toJSON();
        });
        obj.insert(QStringView{u"clientData"}, std::move(arr));
    }
    obj.insert(QStringView{u"name"}, d->name);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"resourceName"}, d->resourceName);
    {
        QJsonArray arr;
        std::transform(d->memberResourceNames.cbegin(), d->memberResourceNames.cend(), std::back_inserter(arr), [](const auto &val) {
            return val;
        });
        obj.insert(QStringView{u"memberResourceNames"}, std::move(arr));
    }
    return obj;
}

} // namespace KGAPI2::People

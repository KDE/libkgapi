/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "contactgroup.h"

#include "contactgroupmetadata.h"
#include "groupclientdata.h"
#include "clientdata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class ContactGroup::Private
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

ContactGroupPtr ContactGroup::fromJSON(const QJsonObject &obj)
{
    auto contactGroup = new ContactGroup;

    if (!obj.isEmpty()) {
        contactGroup->d->resourceName = obj.value(QStringLiteral("resourceName")).toString();
        contactGroup->d->etag = obj.value(QStringLiteral("etag")).toString();

        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        contactGroup->d->metadata = ContactGroupMetadata::fromJSON(metadata);

        const auto groupType = obj.value(QStringLiteral("groupType"));
        contactGroup->d->groupType = ContactGroup::GroupType(groupType.toInt());

        contactGroup->d->name = obj.value(QStringLiteral("name")).toString();
        contactGroup->d->formattedName = obj.value(QStringLiteral("formattedName")).toString();

        const auto memberResourceNames = obj.value(QStringLiteral("memberResourceNames")).toArray();
        std::transform(memberResourceNames.cbegin(),
                       memberResourceNames.cend(),
                       std::back_inserter(contactGroup->d->memberResourceNames),
                       [](const QJsonValue &jsonMemberResourceName) {
                           return jsonMemberResourceName.toString();
                       });

        contactGroup->d->memberCount = obj.value(QStringLiteral("memberCount")).toInt();

        const auto clientData = obj.value(QStringLiteral("clientData")).toArray();
        contactGroup->d->clientData = GroupClientData::fromJSONArray(clientData);
    }

    return ContactGroupPtr(contactGroup);
}

QJsonValue ContactGroup::toJSON() const
{
    QJsonObject obj;

    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "formattedName", d->formattedName);
    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "memberCount", d->memberCount);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "etag", d->etag);
    /* Output only
    switch (d->groupType) {
    case GroupType::GROUP_TYPE_UNSPECIFIED:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "groupType", QStringLiteral("GROUP_TYPE_UNSPECIFIED"));
        break;
    case GroupType::USER_CONTACT_GROUP:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "groupType", QStringLiteral("USER_CONTACT_GROUP"));
        break;
    case GroupType::SYSTEM_CONTACT_GROUP:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "groupType", QStringLiteral("SYSTEM_CONTACT_GROUP"));
        break;
    }*/
    if (!d->clientData.isEmpty()) {
        QJsonArray arr;
        std::transform(d->clientData.cbegin(), d->clientData.cend(), std::back_inserter(arr), [](const auto &val) {
            return val.toJSON();
        });
        PeopleUtils::addValueToJsonObjectIfValid(obj, "clientData", std::move(arr));
    }
    PeopleUtils::addValueToJsonObjectIfValid(obj, "name", d->name);
    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata", d->metadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "resourceName", d->resourceName);
    /* Output only
    if (!d->memberResourceNames.isEmpty()) {
        QJsonArray arr;
        std::transform(d->memberResourceNames.cbegin(), d->memberResourceNames.cend(), std::back_inserter(arr), [](const auto &val) {
            return val;
        });
        PeopleUtils::addValueToJsonObjectIfValid(obj, "memberResourceNames", std::move(arr));
    }*/
    return obj;
}

} // namespace KGAPI2::People

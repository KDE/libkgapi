/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "groupclientdata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class GroupClientData::Private : public QSharedData
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
        return key == other.key && value == other.value;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString key{};
    QString value{};
};

GroupClientData::GroupClientData()
    : d(new Private)
{
}

GroupClientData::GroupClientData(const GroupClientData &) = default;
GroupClientData::GroupClientData(GroupClientData &&) noexcept = default;
GroupClientData &GroupClientData::operator=(const GroupClientData &) = default;
GroupClientData &GroupClientData::operator=(GroupClientData &&) noexcept = default;
GroupClientData::~GroupClientData() = default;

bool GroupClientData::operator==(const GroupClientData &other) const
{
    return *d == *other.d;
}

bool GroupClientData::operator!=(const GroupClientData &other) const
{
    return !(*this == other);
}

QString GroupClientData::key() const
{
    return d->key;
}

void GroupClientData::setKey(const QString &value)
{
    d->key = value;
}
QString GroupClientData::value() const
{
    return d->value;
}

void GroupClientData::setValue(const QString &value)
{
    d->value = value;
}

GroupClientData GroupClientData::fromJSON(const QJsonObject &obj)
{
    GroupClientData groupClientData;

    if (!obj.isEmpty()) {
        groupClientData.setKey(obj.value(QStringLiteral("key")).toString());
        groupClientData.setValue(obj.value(QStringLiteral("value")).toString());
    }

    return groupClientData;
}

QList<GroupClientData> GroupClientData::fromJSONArray(const QJsonArray &data)
{
    QList<GroupClientData> returnGroupClientData;

    for(const auto &groupClientData : data) {
        if(groupClientData.isObject()) {
            const auto objectifiedGroupClientData = groupClientData.toObject();
            returnGroupClientData.append(fromJSON(objectifiedGroupClientData));
        }
    }

    return returnGroupClientData;
}

QJsonValue GroupClientData::toJSON() const
{
    QJsonObject obj;

    PeopleUtils::addValueToJsonObjectIfValid(obj, "key", d->key);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "value", d->value);
    return obj;
}

} // namespace KGAPI2::People

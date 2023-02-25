/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "contactgroupmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>
#include <QDateTime>

#include <algorithm>

namespace KGAPI2::People
{
class ContactGroupMetadata::Private : public QSharedData
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
        return updateTime == other.updateTime && deleted == other.deleted;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QDateTime updateTime{};
    bool deleted{};
};

ContactGroupMetadata::ContactGroupMetadata()
    : d(new Private)
{
}

ContactGroupMetadata::ContactGroupMetadata(const ContactGroupMetadata &) = default;
ContactGroupMetadata::ContactGroupMetadata(ContactGroupMetadata &&) noexcept = default;
ContactGroupMetadata &ContactGroupMetadata::operator=(const ContactGroupMetadata &) = default;
ContactGroupMetadata &ContactGroupMetadata::operator=(ContactGroupMetadata &&) noexcept = default;
ContactGroupMetadata::~ContactGroupMetadata() = default;

bool ContactGroupMetadata::operator==(const ContactGroupMetadata &other) const
{
    return *d == *other.d;
}

bool ContactGroupMetadata::operator!=(const ContactGroupMetadata &other) const
{
    return !(*this == other);
}

QDateTime ContactGroupMetadata::updateTime() const
{
    return d->updateTime;
}
bool ContactGroupMetadata::deleted() const
{
    return d->deleted;
}

ContactGroupMetadata ContactGroupMetadata::fromJSON(const QJsonObject &obj)
{
    ContactGroupMetadata contactGroupMetadata;

    if (!obj.isEmpty()) {
        contactGroupMetadata.d->updateTime = obj.value(QStringLiteral("updateTime")).toVariant().toDateTime();
        contactGroupMetadata.d->deleted = obj.value(QStringLiteral("deleted")).toBool();
    }

    return contactGroupMetadata;
}

QJsonValue ContactGroupMetadata::toJSON() const
{
    QJsonObject obj;

    PeopleUtils::addValueToJsonObjectIfValid(obj, "updateTime", d->updateTime.toString(Qt::ISODate));
    PeopleUtils::addValueToJsonObjectIfValid(obj, "deleted", d->deleted);
    return obj;
}

} // namespace KGAPI2::People

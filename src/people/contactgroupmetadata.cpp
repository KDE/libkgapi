/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "contactgroupmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

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

    QString updateTime{};
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

QString ContactGroupMetadata::updateTime() const
{
    return d->updateTime;
}
bool ContactGroupMetadata::deleted() const
{
    return d->deleted;
}

ContactGroupMetadata ContactGroupMetadata::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return ContactGroupMetadata();
}

QJsonValue ContactGroupMetadata::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"updateTime"}, d->updateTime);
    obj.insert(QStringView{u"deleted"}, d->deleted);
    return obj;
}

} // namespace KGAPI2::People

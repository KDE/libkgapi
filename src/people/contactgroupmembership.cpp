/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "contactgroupmembership.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class ContactGroupMembership::Private : public QSharedData
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
        return contactGroupResourceName == other.contactGroupResourceName && contactGroupId == other.contactGroupId;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString contactGroupResourceName{};
    QString contactGroupId{};
};

ContactGroupMembership::ContactGroupMembership()
    : d(new Private)
{
}

ContactGroupMembership::ContactGroupMembership(const ContactGroupMembership &) = default;
ContactGroupMembership::ContactGroupMembership(ContactGroupMembership &&) noexcept = default;
ContactGroupMembership &ContactGroupMembership::operator=(const ContactGroupMembership &) = default;
ContactGroupMembership &ContactGroupMembership::operator=(ContactGroupMembership &&) noexcept = default;
ContactGroupMembership::~ContactGroupMembership() = default;

bool ContactGroupMembership::operator==(const ContactGroupMembership &other) const
{
    return *d == *other.d;
}

bool ContactGroupMembership::operator!=(const ContactGroupMembership &other) const
{
    return !(*this == other);
}

QString ContactGroupMembership::contactGroupResourceName() const
{
    return d->contactGroupResourceName;
}

void ContactGroupMembership::setContactGroupResourceName(const QString &value)
{
    d->contactGroupResourceName = value;
}
QString ContactGroupMembership::contactGroupId() const
{
    return d->contactGroupId;
}

ContactGroupMembership ContactGroupMembership::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return ContactGroupMembership();
}

QJsonValue ContactGroupMembership::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"contactGroupResourceName"}, d->contactGroupResourceName);
    obj.insert(QStringView{u"contactGroupId"}, d->contactGroupId);
    return obj;
}

} // namespace KGAPI2::People

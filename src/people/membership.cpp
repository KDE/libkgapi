/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "membership.h"

#include "contactgroupmembership.h"
#include "domainmembership.h"
#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Membership::Private : public QSharedData
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
        return domainMembership == other.domainMembership && contactGroupMembership == other.contactGroupMembership && metadata == other.metadata;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    DomainMembership domainMembership{};
    ContactGroupMembership contactGroupMembership{};
    FieldMetadata metadata{};
};

Membership::Membership()
    : d(new Private)
{
}

Membership::Membership(const Membership &) = default;
Membership::Membership(Membership &&) noexcept = default;
Membership &Membership::operator=(const Membership &) = default;
Membership &Membership::operator=(Membership &&) noexcept = default;
Membership::~Membership() = default;

bool Membership::operator==(const Membership &other) const
{
    return *d == *other.d;
}

bool Membership::operator!=(const Membership &other) const
{
    return !(*this == other);
}

DomainMembership Membership::domainMembership() const
{
    return d->domainMembership;
}
ContactGroupMembership Membership::contactGroupMembership() const
{
    return d->contactGroupMembership;
}

void Membership::setContactGroupMembership(const ContactGroupMembership &value)
{
    d->contactGroupMembership = value;
}
FieldMetadata Membership::metadata() const
{
    return d->metadata;
}

void Membership::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

Membership Membership::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return Membership();
}

QJsonValue Membership::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"domainMembership"}, d->domainMembership.toJSON());
    obj.insert(QStringView{u"contactGroupMembership"}, d->contactGroupMembership.toJSON());
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People

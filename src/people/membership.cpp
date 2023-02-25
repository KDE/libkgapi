/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "membership.h"

#include "contactgroupmembership.h"
#include "domainmembership.h"
#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{

struct MembershipDefinition
{
    FieldMetadata metadata;
    ContactGroupMembership contactGroupMembership;
    DomainMembership domainMembership;
};

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

Membership::Membership(const MembershipDefinition &definition)
    : d(new Private)
{
    d->metadata = definition.metadata;
    d->contactGroupMembership = definition.contactGroupMembership;
    d->domainMembership = definition.domainMembership;
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
    if(obj.isEmpty()) {
        return Membership();
    }

    MembershipDefinition definition;

    const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
    definition.metadata = FieldMetadata::fromJSON(metadata);

    const auto contactGroupMembership = obj.value(QStringLiteral("contactGroupMembership")).toObject();
    definition.contactGroupMembership = ContactGroupMembership::fromJSON(contactGroupMembership);

    const auto domainMembership = obj.value(QStringLiteral("domainMembership")).toObject();
    definition.domainMembership = DomainMembership::fromJSON(domainMembership);

    return Membership(definition);
}

QVector<Membership> Membership::fromJSONArray(const QJsonArray& data)
{
    QVector<Membership> memberships;

    for(const auto &membership : data) {
        if(membership.isObject()) {
            const auto objectifiedMembership = membership.toObject();
            memberships.append(fromJSON(objectifiedMembership));
        }
    }

    return memberships;
}

QJsonValue Membership::toJSON() const
{
    QJsonObject obj;

    if (!d->contactGroupMembership.contactGroupResourceName().isEmpty()) {
        PeopleUtils::addValueToJsonObjectIfValid(obj, "contactGroupMembership", d->contactGroupMembership.toJSON());
    } else if (d->domainMembership.inViewerDomain()) {
        PeopleUtils::addValueToJsonObjectIfValid(obj, "domainMembership", d->domainMembership.toJSON());
    }
    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata", d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People

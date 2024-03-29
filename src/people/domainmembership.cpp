/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "domainmembership.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class DomainMembership::Private : public QSharedData
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
        return inViewerDomain == other.inViewerDomain;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    bool inViewerDomain{};
};

DomainMembership::DomainMembership()
    : d(new Private)
{
}

DomainMembership::DomainMembership(const DomainMembership &) = default;
DomainMembership::DomainMembership(DomainMembership &&) noexcept = default;
DomainMembership &DomainMembership::operator=(const DomainMembership &) = default;
DomainMembership &DomainMembership::operator=(DomainMembership &&) noexcept = default;
DomainMembership::~DomainMembership() = default;

bool DomainMembership::operator==(const DomainMembership &other) const
{
    return *d == *other.d;
}

bool DomainMembership::operator!=(const DomainMembership &other) const
{
    return !(*this == other);
}

bool DomainMembership::inViewerDomain() const
{
    return d->inViewerDomain;
}

void DomainMembership::setInViewerDomain(bool value)
{
    d->inViewerDomain = value;
}

DomainMembership DomainMembership::fromJSON(const QJsonObject &obj)
{
    DomainMembership domainMembership;

    if(!obj.isEmpty()) {
        domainMembership.setInViewerDomain(obj.value(QStringLiteral("inViewerDomain")).toBool());
    }

    return domainMembership;
}

QJsonValue DomainMembership::toJSON() const
{
    return QJsonObject {
        { QStringLiteral("inViewerDomain"), d->inViewerDomain }
    };
}

} // namespace KGAPI2::People

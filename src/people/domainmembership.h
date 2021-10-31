/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <QSharedDataPointer>
#include <kgapipeople_export.h>

#include <optional>

class QJsonObject;
class QJsonValue;

namespace KGAPI2::People
{
/**
 * A Google Workspace Domain membership.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#domainmembership
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT DomainMembership
{
public:
    /** Constructs a new DomainMembership **/
    explicit DomainMembership();
    DomainMembership(const DomainMembership &);
    DomainMembership(DomainMembership &&) noexcept;
    DomainMembership &operator=(const DomainMembership &);
    DomainMembership &operator=(DomainMembership &&) noexcept;
    /** Destructor. **/
    ~DomainMembership();

    bool operator==(const DomainMembership &) const;
    bool operator!=(const DomainMembership &) const;

    static DomainMembership fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** True if the person is in the viewer's Google Workspace domain. **/
    bool inViewerDomain() const;
    /** Sets value of the inViewerDomain property. **/
    void setInViewerDomain(const bool &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // DomainMembership

} // namespace KGAPI2::People
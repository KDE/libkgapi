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
class ContactGroupMembership;
class DomainMembership;
class FieldMetadata;

/**
 * A person's membership in a group. Only contact group memberships can be
 * modified.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#membership
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT Membership
{
public:
    /** Constructs a new Membership **/
    explicit Membership();
    Membership(const Membership &);
    Membership(Membership &&) noexcept;
    Membership &operator=(const Membership &);
    Membership &operator=(Membership &&) noexcept;
    /** Destructor. **/
    ~Membership();

    bool operator==(const Membership &) const;
    bool operator!=(const Membership &) const;

    static Membership fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Output only. The domain membership. **/
    DomainMembership domainMembership() const;

    /** The contact group membership. **/
    ContactGroupMembership contactGroupMembership() const;
    /** Sets value of the contactGroupMembership property. **/
    void setContactGroupMembership(const ContactGroupMembership &value);

    /** Metadata about the membership. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Membership

} // namespace KGAPI2::People
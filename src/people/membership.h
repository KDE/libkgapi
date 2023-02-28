/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <QSharedDataPointer>
#include "kgapipeople_export.h"

#include <optional>

class QJsonObject;
class QJsonValue;
class QJsonArray;

namespace KGAPI2::People
{
class ContactGroupMembership;
class DomainMembership;
class FieldMetadata;
struct MembershipDefinition;

/**
 * A person's membership in a group. Only contact group memberships can be
 * modified.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#membership
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT Membership
{
public:
    /** Constructs a new Membership **/
    explicit Membership();
    Membership(const MembershipDefinition &definition);
    Membership(const Membership &);
    Membership(Membership &&) noexcept;
    Membership &operator=(const Membership &);
    Membership &operator=(Membership &&) noexcept;
    /** Destructor. **/
    ~Membership();

    bool operator==(const Membership &) const;
    bool operator!=(const Membership &) const;

    Q_REQUIRED_RESULT static Membership fromJSON(const QJsonObject &obj);
    Q_REQUIRED_RESULT static QVector<Membership> fromJSONArray(const QJsonArray& data);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    /** Output only. The domain membership. **/
    Q_REQUIRED_RESULT DomainMembership domainMembership() const;

    /** The contact group membership. **/
    Q_REQUIRED_RESULT ContactGroupMembership contactGroupMembership() const;
    /** Sets value of the contactGroupMembership property. **/
    void setContactGroupMembership(const ContactGroupMembership &value);

    /** Metadata about the membership. **/
    Q_REQUIRED_RESULT FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Membership

} // namespace KGAPI2::People

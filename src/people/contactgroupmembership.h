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

#include <QString>

#include <optional>

class QJsonObject;
class QJsonValue;

namespace KGAPI2::People
{
/**
 * A Google contact group membership.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#contactgroupmembership
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT ContactGroupMembership
{
public:
    /** Constructs a new ContactGroupMembership **/
    explicit ContactGroupMembership();
    ContactGroupMembership(const ContactGroupMembership &);
    ContactGroupMembership(ContactGroupMembership &&) noexcept;
    ContactGroupMembership &operator=(const ContactGroupMembership &);
    ContactGroupMembership &operator=(ContactGroupMembership &&) noexcept;
    /** Destructor. **/
    ~ContactGroupMembership();

    bool operator==(const ContactGroupMembership &) const;
    bool operator!=(const ContactGroupMembership &) const;

    static ContactGroupMembership fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The resource name for the contact group, assigned by the server. An ASCII string, in the form of `contactGroups/{contact_group_id}`. Only
     * contact_group_resource_name can be used for modifying memberships. Any contact group membership can be removed, but only user group or "myContacts" or
     * "starred" system groups memberships can be added. A contact must always have at least one contact group membership. **/
    QString contactGroupResourceName() const;
    /** Sets value of the contactGroupResourceName property. **/
    void setContactGroupResourceName(const QString &value);

    /** Output only. The contact group ID for the contact group membership. **/
    QString contactGroupId() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // ContactGroupMembership

} // namespace KGAPI2::People
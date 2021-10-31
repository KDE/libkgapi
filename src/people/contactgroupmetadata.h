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
 * The metadata about a contact group.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#contactgroupmetadata
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT ContactGroupMetadata
{
public:
    /** Constructs a new ContactGroupMetadata **/
    explicit ContactGroupMetadata();
    ContactGroupMetadata(const ContactGroupMetadata &);
    ContactGroupMetadata(ContactGroupMetadata &&) noexcept;
    ContactGroupMetadata &operator=(const ContactGroupMetadata &);
    ContactGroupMetadata &operator=(ContactGroupMetadata &&) noexcept;
    /** Destructor. **/
    ~ContactGroupMetadata();

    bool operator==(const ContactGroupMetadata &) const;
    bool operator!=(const ContactGroupMetadata &) const;

    static ContactGroupMetadata fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Output only. The time the group was last updated. **/
    QString updateTime() const;

    /** Output only. True if the contact group resource has been deleted. Populated only for [`ListContactGroups`](/people/api/rest/v1/contactgroups/list)
     * requests that include a sync token. **/
    bool deleted() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // ContactGroupMetadata

} // namespace KGAPI2::People
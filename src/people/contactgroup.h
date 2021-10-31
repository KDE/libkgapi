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
#include <QVector>

#include <optional>

class QJsonObject;
class QJsonValue;

namespace KGAPI2::People
{
class ContactGroupMetadata;
class GroupClientData;

/**
 * A contact group.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#contactgroup
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT ContactGroup
{
public:
    enum class GroupType {
        GROUP_TYPE_UNSPECIFIED, ///< Unspecified.
        USER_CONTACT_GROUP, ///< User defined contact group.
        SYSTEM_CONTACT_GROUP, ///< System defined contact group.
    };

    /** Constructs a new ContactGroup **/
    explicit ContactGroup();
    ContactGroup(const ContactGroup &);
    ContactGroup(ContactGroup &&) noexcept;
    ContactGroup &operator=(const ContactGroup &);
    ContactGroup &operator=(ContactGroup &&) noexcept;
    /** Destructor. **/
    ~ContactGroup();

    bool operator==(const ContactGroup &) const;
    bool operator!=(const ContactGroup &) const;

    static ContactGroup fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Output only. The name translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale for system groups names. Group
     * names set by the owner are the same as name. **/
    QString formattedName() const;

    /** Output only. The total number of contacts in the group irrespective of max members in specified in the request. **/
    int memberCount() const;

    /** The [HTTP entity tag](https://en.wikipedia.org/wiki/HTTP_ETag) of the resource. Used for web cache validation. **/
    QString etag() const;
    /** Sets value of the etag property. **/
    void setEtag(const QString &value);

    /** Output only. The contact group type. **/
    ContactGroup::GroupType groupType() const;

    /** The group's client data. **/
    QVector<GroupClientData> clientData() const;
    /** Sets value of the clientData property. **/
    void setClientData(const QVector<GroupClientData> &value);
    /** Appends the given @c value to the list of clientData. **/
    void addGroupClientData(const GroupClientData &value);
    /** Removes the given @c value from the list of clientData if it exists. **/
    void removeGroupClientData(const GroupClientData &value);
    /** Clears the list of clientData. **/
    void clearClientData();

    /** The contact group name set by the group owner or a system provided name for system groups. For
     * [`contactGroups.create`](/people/api/rest/v1/contactGroups/create) or [`contactGroups.update`](/people/api/rest/v1/contactGroups/update) the name must be
     * unique to the users contact groups. Attempting to create a group with a duplicate name will return a HTTP 409 error. **/
    QString name() const;
    /** Sets value of the name property. **/
    void setName(const QString &value);

    /** Output only. Metadata about the contact group. **/
    ContactGroupMetadata metadata() const;

    /** The resource name for the contact group, assigned by the server. An ASCII string, in the form of `contactGroups/{contact_group_id}`. **/
    QString resourceName() const;
    /** Sets value of the resourceName property. **/
    void setResourceName(const QString &value);

    /** Output only. The list of contact person resource names that are members of the contact group. The field is only populated for GET requests and will only
     * return as many members as `maxMembers` in the get request. **/
    QVector<QString> memberResourceNames() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // ContactGroup

} // namespace KGAPI2::People
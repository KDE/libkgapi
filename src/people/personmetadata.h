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

#include <QList>
#include <QString>

#include <optional>

class QJsonObject;
class QJsonValue;

namespace KGAPI2::People
{
class Source;

/**
 * The metadata about a person.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#personmetadata
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT PersonMetadata
{
public:
    enum class ObjectType {
        OBJECT_TYPE_UNSPECIFIED, ///< Unspecified.
        PERSON, ///< Person.
        PAGE, ///< [Currents Page.](https://workspace.google.com/products/currents/)
    };

    /** Constructs a new PersonMetadata **/
    explicit PersonMetadata();
    PersonMetadata(const PersonMetadata &);
    PersonMetadata(PersonMetadata &&) noexcept;
    PersonMetadata &operator=(const PersonMetadata &);
    PersonMetadata &operator=(PersonMetadata &&) noexcept;
    /** Destructor. **/
    ~PersonMetadata();

    bool operator==(const PersonMetadata &) const;
    bool operator!=(const PersonMetadata &) const;

    [[nodiscard]] static PersonMetadata fromJSON(const QJsonObject &);
    [[nodiscard]] QJsonValue toJSON() const;

    /** Output only. Resource names of people linked to this resource. **/
    [[nodiscard]] QList<QString> linkedPeopleResourceNames() const;

    /** Output only. **DEPRECATED** (Please use `person.metadata.sources.profileMetadata.objectType` instead) The type of the person object. **/
    [[nodiscard]] PersonMetadata::ObjectType objectType() const;

    /** Output only. Any former resource names this person has had. Populated only for `people.connections.list` requests that include a sync token. The
     * resource name may change when adding or removing fields that link a contact and profile such as a verified email, verified phone number, or profile URL.
     * **/
    [[nodiscard]] QList<QString> previousResourceNames() const;

    /** Output only. True if the person resource has been deleted. Populated only for `people.connections.list` and `otherContacts.list` sync requests. **/
    [[nodiscard]] bool deleted() const;

    /** The sources of data for the person. **/
    [[nodiscard]] QList<Source> sources() const;
    /** Sets value of the sources property. **/
    void setSources(const QList<Source> &value);
    /** Appends the given @c value to the list of sources. **/
    void addSource(const Source &value);
    /** Removes the given @c value from the list of sources if it exists. **/
    void removeSource(const Source &value);
    /** Clears the list of sources. **/
    void clearSources();

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // PersonMetadata

} // namespace KGAPI2::People

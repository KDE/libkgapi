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
class FieldMetadata;

/**
 * A person's photo. A picture shown next to the person's name to help others
 * recognize the person.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#photo
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT Photo
{
public:
    /** Constructs a new Photo **/
    explicit Photo();
    Photo(const Photo &);
    Photo(Photo &&) noexcept;
    Photo &operator=(const Photo &);
    Photo &operator=(Photo &&) noexcept;
    /** Destructor. **/
    ~Photo();

    bool operator==(const Photo &) const;
    bool operator!=(const Photo &) const;

    static Photo fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Metadata about the photo. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** True if the photo is a default photo; false if the photo is a user-provided photo. **/
    bool isDefault() const;
    /** Sets value of the isDefault property. **/
    void setIsDefault(const bool &value);

    /** The URL of the photo. You can change the desired size by appending a query parameter `sz={size}` at the end of the url, where {size} is the size in
     * pixels. Example: https://lh3.googleusercontent.com/-T_wVWLlmg7w/AAAAAAAAAAI/AAAAAAAABa8/00gzXvDBYqw/s100/photo.jpg?sz=50 **/
    QString url() const;
    /** Sets value of the url property. **/
    void setUrl(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Photo

} // namespace KGAPI2::People
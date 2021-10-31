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
 * A person's short biography.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#biography
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT Biography
{
public:
    enum class ContentType {
        CONTENT_TYPE_UNSPECIFIED, ///< Unspecified.
        TEXT_PLAIN, ///< Plain text.
        TEXT_HTML, ///< HTML text.
    };

    /** Constructs a new Biography **/
    explicit Biography();
    Biography(const Biography &);
    Biography(Biography &&) noexcept;
    Biography &operator=(const Biography &);
    Biography &operator=(Biography &&) noexcept;
    /** Destructor. **/
    ~Biography();

    bool operator==(const Biography &) const;
    bool operator!=(const Biography &) const;

    static Biography fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Metadata about the biography. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The content type of the biography. **/
    Biography::ContentType contentType() const;
    /** Sets value of the contentType property. **/
    void setContentType(const Biography::ContentType &value);

    /** The short biography. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Biography

} // namespace KGAPI2::People
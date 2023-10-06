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

#include <QString>

#include <optional>

class QJsonObject;
class QJsonValue;
class QJsonArray;

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * A person's short biography.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#biography
 * @since 5.23.0
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

    [[nodiscard]] static Biography fromJSON(const QJsonObject &obj);
    [[nodiscard]] static QList<Biography> fromJSONArray(const QJsonArray &data);
    [[nodiscard]] QJsonValue toJSON() const;

    /** Metadata about the biography. **/
    [[nodiscard]] FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The content type of the biography. **/
    [[nodiscard]] Biography::ContentType contentType() const;
    /** Sets value of the contentType property. **/
    void setContentType(Biography::ContentType value);

    /** The short biography. **/
    [[nodiscard]] QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Biography

} // namespace KGAPI2::People

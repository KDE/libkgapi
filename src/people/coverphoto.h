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
 * A person's cover photo. A large image shown on the person's profile page that
 * represents who they are or what they care about.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#coverphoto
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT CoverPhoto
{
public:
    /** Constructs a new CoverPhoto **/
    explicit CoverPhoto();
    CoverPhoto(const CoverPhoto &);
    CoverPhoto(CoverPhoto &&) noexcept;
    CoverPhoto &operator=(const CoverPhoto &);
    CoverPhoto &operator=(CoverPhoto &&) noexcept;
    /** Destructor. **/
    ~CoverPhoto();

    bool operator==(const CoverPhoto &) const;
    bool operator!=(const CoverPhoto &) const;

    [[nodiscard]] static CoverPhoto fromJSON(const QJsonObject &obj);
    [[nodiscard]] static QList<CoverPhoto> fromJSONArray(const QJsonArray &data);
    [[nodiscard]] QJsonValue toJSON() const;

    /** True if the cover photo is the default cover photo; false if the cover photo is a user-provided cover photo. **/
    [[nodiscard]] bool isDefault() const;
    /** Sets value of the isDefault property. **/
    void setIsDefault(bool value);

    /** Metadata about the cover photo. **/
    [[nodiscard]] FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The URL of the cover photo. **/
    [[nodiscard]] QString url() const;
    /** Sets value of the url property. **/
    void setUrl(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // CoverPhoto

} // namespace KGAPI2::People

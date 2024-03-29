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

#include <QDate>
#include <QString>

#include <optional>

class QJsonObject;
class QJsonValue;
class QJsonArray;

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * A person's birthday. At least one of the `date` and `text` fields are specified.
 * The `date` and `text` fields typically represent the same date, but are not
 * guaranteed to.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#birthday
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT Birthday
{
public:
    /** Constructs a new Birthday **/
    explicit Birthday();
    Birthday(const Birthday &);
    Birthday(Birthday &&) noexcept;
    Birthday &operator=(const Birthday &);
    Birthday &operator=(Birthday &&) noexcept;
    /** Destructor. **/
    ~Birthday();

    bool operator==(const Birthday &) const;
    bool operator!=(const Birthday &) const;

    [[nodiscard]] static Birthday fromJSON(const QJsonObject &obj);
    [[nodiscard]] static QList<Birthday> fromJSONArray(const QJsonArray &data);
    [[nodiscard]] QJsonValue toJSON() const;

    /** A free-form string representing the user's birthday. **/
    [[nodiscard]] QString text() const;
    /** Sets value of the text property. **/
    void setText(const QString &value);

    /** Metadata about the birthday. **/
    [[nodiscard]] FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The date of the birthday. **/
    [[nodiscard]] QDate date() const;
    /** Sets value of the date property. **/
    void setDate(const QDate &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Birthday

} // namespace KGAPI2::People

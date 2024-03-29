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
 * A person's nickname.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#nickname
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT Nickname
{
public:
    enum class Type {
        DEFAULT, ///< Generic nickname.
        MAIDEN_NAME, ///< Maiden name or birth family name. Used when the person's family name has changed as a result of marriage.
        INITIALS, ///< Initials.
        GPLUS, ///< Google+ profile nickname.
        OTHER_NAME, ///< A professional affiliation or other name; for example, `Dr. Smith.`
        ALTERNATE_NAME, ///< Alternate name person is known by.
        SHORT_NAME, ///< A shorter version of the person's name.
    };

    /** Constructs a new Nickname **/
    explicit Nickname();
    Nickname(const Nickname &);
    Nickname(Nickname &&) noexcept;
    Nickname &operator=(const Nickname &);
    Nickname &operator=(Nickname &&) noexcept;
    /** Destructor. **/
    ~Nickname();

    bool operator==(const Nickname &) const;
    bool operator!=(const Nickname &) const;

    [[nodiscard]] static Nickname fromJSON(const QJsonObject &);
    [[nodiscard]] static QList<Nickname> fromJSONArray(const QJsonArray &data);
    [[nodiscard]] QJsonValue toJSON() const;

    /** The nickname. **/
    [[nodiscard]] QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** The type of the nickname. **/
    [[nodiscard]] Nickname::Type type() const;
    /** Sets value of the type property. **/
    void setType(Nickname::Type value);

    /** Metadata about the nickname. **/
    [[nodiscard]] FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Nickname

} // namespace KGAPI2::People

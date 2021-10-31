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
 * A person's locale preference.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#personlocale
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT PersonLocale
{
public:
    /** Constructs a new PersonLocale **/
    explicit PersonLocale();
    PersonLocale(const PersonLocale &);
    PersonLocale(PersonLocale &&) noexcept;
    PersonLocale &operator=(const PersonLocale &);
    PersonLocale &operator=(PersonLocale &&) noexcept;
    /** Destructor. **/
    ~PersonLocale();

    bool operator==(const PersonLocale &) const;
    bool operator!=(const PersonLocale &) const;

    static PersonLocale fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Metadata about the locale. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The well-formed [IETF BCP 47](https://tools.ietf.org/html/bcp47) language tag representing the locale. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // PersonLocale

} // namespace KGAPI2::People
/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
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

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * **DEPRECATED**: No data will be returned A brief one-line description of the
 * person.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#tagline
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT Tagline
{
public:
    /** Constructs a new Tagline **/
    explicit Tagline();
    Tagline(const Tagline &);
    Tagline(Tagline &&) noexcept;
    Tagline &operator=(const Tagline &);
    Tagline &operator=(Tagline &&) noexcept;
    /** Destructor. **/
    ~Tagline();

    bool operator==(const Tagline &) const;
    bool operator!=(const Tagline &) const;

    Q_REQUIRED_RESULT static Tagline fromJSON(const QJsonObject &);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    /** The tagline. **/
    Q_REQUIRED_RESULT QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the tagline. **/
    Q_REQUIRED_RESULT FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Tagline

} // namespace KGAPI2::People

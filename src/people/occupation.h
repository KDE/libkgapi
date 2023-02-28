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
 * A person's occupation.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#occupation
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT Occupation
{
public:
    /** Constructs a new Occupation **/
    explicit Occupation();
    Occupation(const Occupation &);
    Occupation(Occupation &&) noexcept;
    Occupation &operator=(const Occupation &);
    Occupation &operator=(Occupation &&) noexcept;
    /** Destructor. **/
    ~Occupation();

    bool operator==(const Occupation &) const;
    bool operator!=(const Occupation &) const;

    Q_REQUIRED_RESULT static Occupation fromJSON(const QJsonObject &);
    Q_REQUIRED_RESULT static QVector<Occupation> fromJSONArray(const QJsonArray& data);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    /** The occupation; for example, `carpenter`. **/
    Q_REQUIRED_RESULT QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the occupation. **/
    Q_REQUIRED_RESULT FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Occupation

} // namespace KGAPI2::People

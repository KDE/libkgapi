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
 * One of the person's interests.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#interest
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT Interest
{
public:
    /** Constructs a new Interest **/
    explicit Interest();
    Interest(const Interest &);
    Interest(Interest &&) noexcept;
    Interest &operator=(const Interest &);
    Interest &operator=(Interest &&) noexcept;
    /** Destructor. **/
    ~Interest();

    bool operator==(const Interest &) const;
    bool operator!=(const Interest &) const;

    Q_REQUIRED_RESULT static Interest fromJSON(const QJsonObject &);
    Q_REQUIRED_RESULT static QList<Interest> fromJSONArray(const QJsonArray &data);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    /** The interest; for example, `stargazing`. **/
    Q_REQUIRED_RESULT QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the interest. **/
    Q_REQUIRED_RESULT FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Interest

} // namespace KGAPI2::People

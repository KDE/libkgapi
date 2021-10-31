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
 * One of the person's interests.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#interest
 * @since 5.19.0
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

    static Interest fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The interest; for example, `stargazing`. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the interest. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Interest

} // namespace KGAPI2::People
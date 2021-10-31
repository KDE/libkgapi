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
 * **DEPRECATED**: No data will be returned A person's bragging rights.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#braggingrights
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT BraggingRights
{
public:
    /** Constructs a new BraggingRights **/
    explicit BraggingRights();
    BraggingRights(const BraggingRights &);
    BraggingRights(BraggingRights &&) noexcept;
    BraggingRights &operator=(const BraggingRights &);
    BraggingRights &operator=(BraggingRights &&) noexcept;
    /** Destructor. **/
    ~BraggingRights();

    bool operator==(const BraggingRights &) const;
    bool operator!=(const BraggingRights &) const;

    static BraggingRights fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Metadata about the bragging rights. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The bragging rights; for example, `climbed mount everest`. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // BraggingRights

} // namespace KGAPI2::People
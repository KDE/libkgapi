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
/**
 * Arbitrary client data that is populated by clients. Duplicate keys and values
 * are allowed.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#groupclientdata
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT GroupClientData
{
public:
    /** Constructs a new GroupClientData **/
    explicit GroupClientData();
    GroupClientData(const GroupClientData &);
    GroupClientData(GroupClientData &&) noexcept;
    GroupClientData &operator=(const GroupClientData &);
    GroupClientData &operator=(GroupClientData &&) noexcept;
    /** Destructor. **/
    ~GroupClientData();

    bool operator==(const GroupClientData &) const;
    bool operator!=(const GroupClientData &) const;

    static GroupClientData fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The client specified key of the client data. **/
    QString key() const;
    /** Sets value of the key property. **/
    void setKey(const QString &value);

    /** The client specified value of the client data. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // GroupClientData

} // namespace KGAPI2::People
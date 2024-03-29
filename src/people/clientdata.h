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
 * Arbitrary client data that is populated by clients. Duplicate keys and values
 * are allowed.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#clientdata
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT ClientData
{
public:
    /** Constructs a new ClientData **/
    explicit ClientData();
    ClientData(const ClientData &);
    ClientData(ClientData &&) noexcept;
    ClientData &operator=(const ClientData &);
    ClientData &operator=(ClientData &&) noexcept;
    /** Destructor. **/
    ~ClientData();

    bool operator==(const ClientData &) const;
    bool operator!=(const ClientData &) const;

    [[nodiscard]] static ClientData fromJSON(const QJsonObject &obj);
    [[nodiscard]] static QList<ClientData> fromJSONArray(const QJsonArray &data);
    [[nodiscard]] QJsonValue toJSON() const;

    /** Metadata about the client data. **/
    [[nodiscard]] FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The client specified key of the client data. **/
    [[nodiscard]] QString key() const;
    /** Sets value of the key property. **/
    void setKey(const QString &value);

    /** The client specified value of the client data. **/
    [[nodiscard]] QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // ClientData

} // namespace KGAPI2::People

/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "clientdata.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class ClientData::Private : public QSharedData
{
public:
    explicit Private() = default;
    Private(const Private &) = default;
    Private(Private &&) noexcept = delete;
    Private &operator=(const Private &) = delete;
    Private &operator=(Private &&) noexcept = delete;
    ~Private() = default;

    bool operator==(const Private &other) const
    {
        return metadata == other.metadata && key == other.key && value == other.value;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    FieldMetadata metadata{};
    QString key{};
    QString value{};
};

ClientData::ClientData()
    : d(new Private)
{
}

ClientData::ClientData(const ClientData &) = default;
ClientData::ClientData(ClientData &&) noexcept = default;
ClientData &ClientData::operator=(const ClientData &) = default;
ClientData &ClientData::operator=(ClientData &&) noexcept = default;
ClientData::~ClientData() = default;

bool ClientData::operator==(const ClientData &other) const
{
    return *d == *other.d;
}

bool ClientData::operator!=(const ClientData &other) const
{
    return !(*this == other);
}

FieldMetadata ClientData::metadata() const
{
    return d->metadata;
}

void ClientData::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString ClientData::key() const
{
    return d->key;
}

void ClientData::setKey(const QString &value)
{
    d->key = value;
}
QString ClientData::value() const
{
    return d->value;
}

void ClientData::setValue(const QString &value)
{
    d->value = value;
}

ClientData ClientData::fromJSON(const QJsonObject &obj)
{
    ClientData clientData;

    if (!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        clientData.setMetadata(FieldMetadata::fromJSON(metadata));
        clientData.setKey(obj.value(QStringLiteral("key")).toString());
        clientData.setValue(obj.value(QStringLiteral("value")).toString());
    }

    return clientData;
}

QVector<ClientData> ClientData::fromJSONArray(const QJsonArray &data)
{
    QVector<ClientData> clientData;

    for(const auto &jsonClientData : data) {
        if(jsonClientData.isObject()) {
            const auto objectifiedClientData = jsonClientData.toObject();
            clientData.append(fromJSON(objectifiedClientData));
        }
    }

    return clientData;
}

QJsonValue ClientData::toJSON() const
{
    QJsonObject obj;

    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata"}, d->metadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "key", d->key);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "value", d->value);
    return obj;
}

} // namespace KGAPI2::People

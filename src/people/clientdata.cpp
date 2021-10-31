/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "clientdata.h"

#include "fieldmetadata.h"

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
    Q_UNUSED(obj);
    return ClientData();
}

QJsonValue ClientData::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"key"}, d->key);
    obj.insert(QStringView{u"value"}, d->value);
    return obj;
}

} // namespace KGAPI2::People

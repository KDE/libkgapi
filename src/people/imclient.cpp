/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "imclient.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{

struct ImClientDefinition {
    FieldMetadata metadata;
    QString username;
    QString type;
    QString formattedType;
    QString protocol;
    QString formattedProtocol;
};

class ImClient::Private : public QSharedData
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
        return protocol == other.protocol && username == other.username && type == other.type && metadata == other.metadata
            && formattedType == other.formattedType && formattedProtocol == other.formattedProtocol;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString protocol{};
    QString username{};
    QString type{};
    FieldMetadata metadata{};
    QString formattedType{};
    QString formattedProtocol{};
};

ImClient::ImClient()
    : d(new Private)
{
}

ImClient::ImClient(const ImClientDefinition &definition)
    : d(new Private)
{
    d->metadata = definition.metadata;
    d->username = definition.username;
    d->type = definition.type;
    d->formattedType = definition.formattedType;
    d->protocol = definition.protocol;
    d->formattedProtocol = definition.formattedProtocol;
}

ImClient::ImClient(const ImClient &) = default;
ImClient::ImClient(ImClient &&) noexcept = default;
ImClient &ImClient::operator=(const ImClient &) = default;
ImClient &ImClient::operator=(ImClient &&) noexcept = default;
ImClient::~ImClient() = default;

bool ImClient::operator==(const ImClient &other) const
{
    return *d == *other.d;
}

bool ImClient::operator!=(const ImClient &other) const
{
    return !(*this == other);
}

QString ImClient::protocol() const
{
    return d->protocol;
}

void ImClient::setProtocol(const QString &value)
{
    d->protocol = value;
}
QString ImClient::username() const
{
    return d->username;
}

void ImClient::setUsername(const QString &value)
{
    d->username = value;
}
QString ImClient::type() const
{
    return d->type;
}

void ImClient::setType(const QString &value)
{
    d->type = value;
}
FieldMetadata ImClient::metadata() const
{
    return d->metadata;
}

void ImClient::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString ImClient::formattedType() const
{
    return d->formattedType;
}
QString ImClient::formattedProtocol() const
{
    return d->formattedProtocol;
}

ImClient ImClient::fromJSON(const QJsonObject &obj)
{
    if(obj.isEmpty()) {
        return ImClient();
    }

    ImClientDefinition definition;

    const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
    definition.metadata = FieldMetadata::fromJSON(metadata);
    definition.username = obj.value(QStringLiteral("username")).toString();
    definition.type = obj.value(QStringLiteral("type")).toString();
    definition.formattedType = obj.value(QStringLiteral("formattedType")).toString();
    definition.protocol = obj.value(QStringLiteral("protocol")).toString();
    definition.formattedProtocol = obj.value(QStringLiteral("formattedProtocol")).toString();

    return ImClient(definition);
}

QVector<ImClient> ImClient::fromJSONArray(const QJsonArray& data)
{
    QVector<ImClient> imClients;

    for(const auto &imClient : data) {
        if(imClient.isObject()) {
            const auto objectifiedImClient = imClient.toObject();
            imClients.append(fromJSON(objectifiedImClient));
        }
    }

    return imClients;
}

QJsonValue ImClient::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"protocol"}, d->protocol);
    obj.insert(QStringView{u"username"}, d->username);
    obj.insert(QStringView{u"type"}, d->type);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"formattedType"}, d->formattedType);
    obj.insert(QStringView{u"formattedProtocol"}, d->formattedProtocol);
    return obj;
}

} // namespace KGAPI2::People

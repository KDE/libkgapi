/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "photo.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Photo::Private : public QSharedData
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
        return metadata == other.metadata && isDefault == other.isDefault && url == other.url;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    FieldMetadata metadata{};
    bool isDefault{};
    QString url{};
};

Photo::Photo()
    : d(new Private)
{
}

Photo::Photo(const Photo &) = default;
Photo::Photo(Photo &&) noexcept = default;
Photo &Photo::operator=(const Photo &) = default;
Photo &Photo::operator=(Photo &&) noexcept = default;
Photo::~Photo() = default;

bool Photo::operator==(const Photo &other) const
{
    return *d == *other.d;
}

bool Photo::operator!=(const Photo &other) const
{
    return !(*this == other);
}

FieldMetadata Photo::metadata() const
{
    return d->metadata;
}

void Photo::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
bool Photo::isDefault() const
{
    return d->isDefault;
}

void Photo::setIsDefault(bool value)
{
    d->isDefault = value;
}
QString Photo::url() const
{
    return d->url;
}

void Photo::setUrl(const QString &value)
{
    d->url = value;
}

Photo Photo::fromJSON(const QJsonObject &obj)
{
    Photo photo;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        photo.setMetadata(FieldMetadata::fromJSON(metadata));
        photo.setUrl(obj.value(QStringLiteral("url")).toString());
        photo.setIsDefault(obj.value(QStringLiteral("default")).toBool());
    }

    return photo;
}

QVector<Photo> Photo::fromJSONArray(const QJsonArray &data)
{
    QVector<Photo> photos;

    for(const auto &photo : data) {
        if(photo.isObject()) {
            const auto objectifiedPhoto = photo.toObject();
            photos.append(fromJSON(objectifiedPhoto));
        }
    }

    return photos;
}

QJsonValue Photo::toJSON() const
{
    QJsonObject obj;

    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata", d->metadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "default", d->isDefault);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "url", d->url);
    return obj;
}

} // namespace KGAPI2::People

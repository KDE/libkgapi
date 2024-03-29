/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "coverphoto.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class CoverPhoto::Private : public QSharedData
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
        return isDefault == other.isDefault && metadata == other.metadata && url == other.url;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    bool isDefault{};
    FieldMetadata metadata{};
    QString url{};
};

CoverPhoto::CoverPhoto()
    : d(new Private)
{
}

CoverPhoto::CoverPhoto(const CoverPhoto &) = default;
CoverPhoto::CoverPhoto(CoverPhoto &&) noexcept = default;
CoverPhoto &CoverPhoto::operator=(const CoverPhoto &) = default;
CoverPhoto &CoverPhoto::operator=(CoverPhoto &&) noexcept = default;
CoverPhoto::~CoverPhoto() = default;

bool CoverPhoto::operator==(const CoverPhoto &other) const
{
    return *d == *other.d;
}

bool CoverPhoto::operator!=(const CoverPhoto &other) const
{
    return !(*this == other);
}

bool CoverPhoto::isDefault() const
{
    return d->isDefault;
}

void CoverPhoto::setIsDefault(bool value)
{
    d->isDefault = value;
}
FieldMetadata CoverPhoto::metadata() const
{
    return d->metadata;
}

void CoverPhoto::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString CoverPhoto::url() const
{
    return d->url;
}

void CoverPhoto::setUrl(const QString &value)
{
    d->url = value;
}

CoverPhoto CoverPhoto::fromJSON(const QJsonObject &obj)
{
    CoverPhoto coverPhoto;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        coverPhoto.setMetadata(FieldMetadata::fromJSON(metadata));
        coverPhoto.setUrl(obj.value(QStringLiteral("url")).toString());
        coverPhoto.setIsDefault(obj.value(QStringLiteral("default")).toBool());
    }

    return coverPhoto;
}

QList<CoverPhoto> CoverPhoto::fromJSONArray(const QJsonArray &data)
{
    QList<CoverPhoto> coverPhotos;

    for(const auto &coverPhoto : data) {
        if(coverPhoto.isObject()) {
            const auto objectifiedCoverPhoto = coverPhoto.toObject();
            coverPhotos.append(fromJSON(objectifiedCoverPhoto));
        }
    }

    return coverPhotos;
}

QJsonValue CoverPhoto::toJSON() const
{
    QJsonObject obj;

    PeopleUtils::addValueToJsonObjectIfValid(obj, "default", d->isDefault);
    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata"}, d->metadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "url", d->url);
    return obj;
}

} // namespace KGAPI2::People

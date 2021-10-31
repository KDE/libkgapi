/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "coverphoto.h"

#include "fieldmetadata.h"

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

void CoverPhoto::setIsDefault(const bool &value)
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
    Q_UNUSED(obj);
    return CoverPhoto();
}

QJsonValue CoverPhoto::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"default"}, d->isDefault);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"url"}, d->url);
    return obj;
}

} // namespace KGAPI2::People

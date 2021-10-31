/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "biography.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Biography::Private : public QSharedData
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
        return metadata == other.metadata && contentType == other.contentType && value == other.value;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    FieldMetadata metadata{};
    Biography::ContentType contentType{};
    QString value{};
};

Biography::Biography()
    : d(new Private)
{
}

Biography::Biography(const Biography &) = default;
Biography::Biography(Biography &&) noexcept = default;
Biography &Biography::operator=(const Biography &) = default;
Biography &Biography::operator=(Biography &&) noexcept = default;
Biography::~Biography() = default;

bool Biography::operator==(const Biography &other) const
{
    return *d == *other.d;
}

bool Biography::operator!=(const Biography &other) const
{
    return !(*this == other);
}

FieldMetadata Biography::metadata() const
{
    return d->metadata;
}

void Biography::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
Biography::Biography::ContentType Biography::contentType() const
{
    return d->contentType;
}

void Biography::setContentType(const Biography::ContentType &value)
{
    d->contentType = value;
}
QString Biography::value() const
{
    return d->value;
}

void Biography::setValue(const QString &value)
{
    d->value = value;
}

Biography Biography::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return Biography();
}

QJsonValue Biography::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    switch (d->contentType) {
    case ContentType::CONTENT_TYPE_UNSPECIFIED:
        obj.insert(QStringView{u"contentType"}, QStringLiteral(u"CONTENT_TYPE_UNSPECIFIED"));
        break;
    case ContentType::TEXT_PLAIN:
        obj.insert(QStringView{u"contentType"}, QStringLiteral(u"TEXT_PLAIN"));
        break;
    case ContentType::TEXT_HTML:
        obj.insert(QStringView{u"contentType"}, QStringLiteral(u"TEXT_HTML"));
        break;
    }
    obj.insert(QStringView{u"value"}, d->value);
    return obj;
}

} // namespace KGAPI2::People

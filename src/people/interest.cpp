/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "interest.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Interest::Private : public QSharedData
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
        return value == other.value && metadata == other.metadata;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString value{};
    FieldMetadata metadata{};
};

Interest::Interest()
    : d(new Private)
{
}

Interest::Interest(const Interest &) = default;
Interest::Interest(Interest &&) noexcept = default;
Interest &Interest::operator=(const Interest &) = default;
Interest &Interest::operator=(Interest &&) noexcept = default;
Interest::~Interest() = default;

bool Interest::operator==(const Interest &other) const
{
    return *d == *other.d;
}

bool Interest::operator!=(const Interest &other) const
{
    return !(*this == other);
}

QString Interest::value() const
{
    return d->value;
}

void Interest::setValue(const QString &value)
{
    d->value = value;
}
FieldMetadata Interest::metadata() const
{
    return d->metadata;
}

void Interest::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

Interest Interest::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return Interest();
}

QJsonValue Interest::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"value"}, d->value);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People

/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "birthday.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Birthday::Private : public QSharedData
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
        return text == other.text && metadata == other.metadata && date == other.date;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString text{};
    FieldMetadata metadata{};
    QDate date{};
};

Birthday::Birthday()
    : d(new Private)
{
}

Birthday::Birthday(const Birthday &) = default;
Birthday::Birthday(Birthday &&) noexcept = default;
Birthday &Birthday::operator=(const Birthday &) = default;
Birthday &Birthday::operator=(Birthday &&) noexcept = default;
Birthday::~Birthday() = default;

bool Birthday::operator==(const Birthday &other) const
{
    return *d == *other.d;
}

bool Birthday::operator!=(const Birthday &other) const
{
    return !(*this == other);
}

QString Birthday::text() const
{
    return d->text;
}

void Birthday::setText(const QString &value)
{
    d->text = value;
}
FieldMetadata Birthday::metadata() const
{
    return d->metadata;
}

void Birthday::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QDate Birthday::date() const
{
    return d->date;
}

void Birthday::setDate(const QDate &value)
{
    d->date = value;
}

Birthday Birthday::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return Birthday();
}

QJsonValue Birthday::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"text"}, d->text);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People

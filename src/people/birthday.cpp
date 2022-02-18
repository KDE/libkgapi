/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
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
    Birthday birthday;

    if(!obj.isEmpty()) {
        const auto jsonMetadata = obj.value(QStringLiteral("metadata")).toObject();
        birthday.setMetadata(FieldMetadata::fromJSON(jsonMetadata));

        const auto jsonDate = obj.value(QStringLiteral("date")).toObject();
        const auto year = jsonDate.value(QStringLiteral("year")).toInt();
        const auto month = jsonDate.value(QStringLiteral("month")).toInt();
        const auto day = jsonDate.value(QStringLiteral("day")).toInt();
        QDate date(year, month, day);
        birthday.setDate(date);
    }

    return birthday;
}

QVector<Birthday> Birthday::fromJSONArray(const QJsonArray &data)
{
    QVector<Birthday> birthdays;

    for(const auto &birthday : data) {
        if(birthday.isObject()) {
            const auto objectifiedBirthday = birthday.toObject();
            birthdays.append(fromJSON(objectifiedBirthday));
        }
    }

    return birthdays;
}


QJsonValue Birthday::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"text"}, d->text);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People

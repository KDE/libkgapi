/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "nickname.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Nickname::Private : public QSharedData
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
        return value == other.value && type == other.type && metadata == other.metadata;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString value{};
    Nickname::Type type{};
    FieldMetadata metadata{};
};

Nickname::Nickname()
    : d(new Private)
{
}

Nickname::Nickname(const Nickname &) = default;
Nickname::Nickname(Nickname &&) noexcept = default;
Nickname &Nickname::operator=(const Nickname &) = default;
Nickname &Nickname::operator=(Nickname &&) noexcept = default;
Nickname::~Nickname() = default;

bool Nickname::operator==(const Nickname &other) const
{
    return *d == *other.d;
}

bool Nickname::operator!=(const Nickname &other) const
{
    return !(*this == other);
}

QString Nickname::value() const
{
    return d->value;
}

void Nickname::setValue(const QString &value)
{
    d->value = value;
}
Nickname::Nickname::Type Nickname::type() const
{
    return d->type;
}

void Nickname::setType(const Nickname::Type &value)
{
    d->type = value;
}
FieldMetadata Nickname::metadata() const
{
    return d->metadata;
}

void Nickname::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

Nickname Nickname::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return Nickname();
}

QJsonValue Nickname::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"value"}, d->value);
    switch (d->type) {
    case Type::DEFAULT:
        obj.insert(QStringView{u"type"}, QStringLiteral(u"DEFAULT"));
        break;
    case Type::MAIDEN_NAME:
        obj.insert(QStringView{u"type"}, QStringLiteral(u"MAIDEN_NAME"));
        break;
    case Type::INITIALS:
        obj.insert(QStringView{u"type"}, QStringLiteral(u"INITIALS"));
        break;
    case Type::GPLUS:
        obj.insert(QStringView{u"type"}, QStringLiteral(u"GPLUS"));
        break;
    case Type::OTHER_NAME:
        obj.insert(QStringView{u"type"}, QStringLiteral(u"OTHER_NAME"));
        break;
    case Type::ALTERNATE_NAME:
        obj.insert(QStringView{u"type"}, QStringLiteral(u"ALTERNATE_NAME"));
        break;
    case Type::SHORT_NAME:
        obj.insert(QStringView{u"type"}, QStringLiteral(u"SHORT_NAME"));
        break;
    }
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People

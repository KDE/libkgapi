/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "skill.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Skill::Private : public QSharedData
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
        return metadata == other.metadata && value == other.value;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    FieldMetadata metadata{};
    QString value{};
};

Skill::Skill()
    : d(new Private)
{
}

Skill::Skill(const Skill &) = default;
Skill::Skill(Skill &&) noexcept = default;
Skill &Skill::operator=(const Skill &) = default;
Skill &Skill::operator=(Skill &&) noexcept = default;
Skill::~Skill() = default;

bool Skill::operator==(const Skill &other) const
{
    return *d == *other.d;
}

bool Skill::operator!=(const Skill &other) const
{
    return !(*this == other);
}

FieldMetadata Skill::metadata() const
{
    return d->metadata;
}

void Skill::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString Skill::value() const
{
    return d->value;
}

void Skill::setValue(const QString &value)
{
    d->value = value;
}

Skill Skill::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return Skill();
}

QJsonValue Skill::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"value"}, d->value);
    return obj;
}

} // namespace KGAPI2::People

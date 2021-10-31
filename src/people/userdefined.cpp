/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "userdefined.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class UserDefined::Private : public QSharedData
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
        return value == other.value && key == other.key && metadata == other.metadata;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString value{};
    QString key{};
    FieldMetadata metadata{};
};

UserDefined::UserDefined()
    : d(new Private)
{
}

UserDefined::UserDefined(const UserDefined &) = default;
UserDefined::UserDefined(UserDefined &&) noexcept = default;
UserDefined &UserDefined::operator=(const UserDefined &) = default;
UserDefined &UserDefined::operator=(UserDefined &&) noexcept = default;
UserDefined::~UserDefined() = default;

bool UserDefined::operator==(const UserDefined &other) const
{
    return *d == *other.d;
}

bool UserDefined::operator!=(const UserDefined &other) const
{
    return !(*this == other);
}

QString UserDefined::value() const
{
    return d->value;
}

void UserDefined::setValue(const QString &value)
{
    d->value = value;
}
QString UserDefined::key() const
{
    return d->key;
}

void UserDefined::setKey(const QString &value)
{
    d->key = value;
}
FieldMetadata UserDefined::metadata() const
{
    return d->metadata;
}

void UserDefined::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

UserDefined UserDefined::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return UserDefined();
}

QJsonValue UserDefined::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"value"}, d->value);
    obj.insert(QStringView{u"key"}, d->key);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People

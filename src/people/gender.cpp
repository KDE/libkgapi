/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "gender.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Gender::Private : public QSharedData
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
        return metadata == other.metadata && formattedValue == other.formattedValue && value == other.value && addressMeAs == other.addressMeAs;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    FieldMetadata metadata{};
    QString formattedValue{};
    QString value{};
    QString addressMeAs{};
};

Gender::Gender()
    : d(new Private)
{
}

Gender::Gender(const Gender &) = default;
Gender::Gender(Gender &&) noexcept = default;
Gender &Gender::operator=(const Gender &) = default;
Gender &Gender::operator=(Gender &&) noexcept = default;
Gender::~Gender() = default;

bool Gender::operator==(const Gender &other) const
{
    return *d == *other.d;
}

bool Gender::operator!=(const Gender &other) const
{
    return !(*this == other);
}

FieldMetadata Gender::metadata() const
{
    return d->metadata;
}

void Gender::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString Gender::formattedValue() const
{
    return d->formattedValue;
}
QString Gender::value() const
{
    return d->value;
}

void Gender::setValue(const QString &value)
{
    d->value = value;
}
QString Gender::addressMeAs() const
{
    return d->addressMeAs;
}

void Gender::setAddressMeAs(const QString &value)
{
    d->addressMeAs = value;
}

Gender Gender::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return Gender();
}

QJsonValue Gender::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"formattedValue"}, d->formattedValue);
    obj.insert(QStringView{u"value"}, d->value);
    obj.insert(QStringView{u"addressMeAs"}, d->addressMeAs);
    return obj;
}

} // namespace KGAPI2::People

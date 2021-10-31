/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "phonenumber.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class PhoneNumber::Private : public QSharedData
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
        return value == other.value && formattedType == other.formattedType && canonicalForm == other.canonicalForm && type == other.type
            && metadata == other.metadata;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString value{};
    QString formattedType{};
    QString canonicalForm{};
    QString type{};
    FieldMetadata metadata{};
};

PhoneNumber::PhoneNumber()
    : d(new Private)
{
}

PhoneNumber::PhoneNumber(const PhoneNumber &) = default;
PhoneNumber::PhoneNumber(PhoneNumber &&) noexcept = default;
PhoneNumber &PhoneNumber::operator=(const PhoneNumber &) = default;
PhoneNumber &PhoneNumber::operator=(PhoneNumber &&) noexcept = default;
PhoneNumber::~PhoneNumber() = default;

bool PhoneNumber::operator==(const PhoneNumber &other) const
{
    return *d == *other.d;
}

bool PhoneNumber::operator!=(const PhoneNumber &other) const
{
    return !(*this == other);
}

QString PhoneNumber::value() const
{
    return d->value;
}

void PhoneNumber::setValue(const QString &value)
{
    d->value = value;
}
QString PhoneNumber::formattedType() const
{
    return d->formattedType;
}
QString PhoneNumber::canonicalForm() const
{
    return d->canonicalForm;
}
QString PhoneNumber::type() const
{
    return d->type;
}

void PhoneNumber::setType(const QString &value)
{
    d->type = value;
}
FieldMetadata PhoneNumber::metadata() const
{
    return d->metadata;
}

void PhoneNumber::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

PhoneNumber PhoneNumber::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return PhoneNumber();
}

QJsonValue PhoneNumber::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"value"}, d->value);
    obj.insert(QStringView{u"formattedType"}, d->formattedType);
    obj.insert(QStringView{u"canonicalForm"}, d->canonicalForm);
    obj.insert(QStringView{u"type"}, d->type);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People

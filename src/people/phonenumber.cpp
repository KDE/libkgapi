/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
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

#include <KContacts/PhoneNumber>

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
    PhoneNumber phoneNumber;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        phoneNumber.d->metadata = FieldMetadata::fromJSON(metadata);
        phoneNumber.d->value = obj.value(QStringLiteral("value")).toString();
        phoneNumber.d->canonicalForm = obj.value(QStringLiteral("canonicalForm")).toString();
        phoneNumber.d->type = obj.value(QStringLiteral("type")).toString();
        phoneNumber.d->formattedType = obj.value(QStringLiteral("formattedType")).toString();
    }

    return phoneNumber;
}

QVector<PhoneNumber> PhoneNumber::fromJSONArray(const QJsonArray& data)
{
    QVector<PhoneNumber> phoneNumbers;

    for(const auto &phoneNumber : data) {
        if(phoneNumber.isObject()) {
            const auto objectifiedPhoneNumber = phoneNumber.toObject();
            phoneNumbers.append(fromJSON(objectifiedPhoneNumber));
        }
    }

    return phoneNumbers;
}

QJsonValue PhoneNumber::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"value"}, d->value);
    obj.insert(QStringView{u"formattedType"}, d->formattedType);
    obj.insert(QStringView{u"canonicalForm"}, d->canonicalForm);
    obj.insert(QStringView{u"type"}, d->type);
    // Skip, field metadata is only useful for receiving -> obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

PhoneNumber PhoneNumber::fromKContactsPhoneNumber(const KContacts::PhoneNumber &phoneNumber)
{
    PhoneNumber convertedPhoneNumber;
    convertedPhoneNumber.setValue(phoneNumber.number());

    switch(phoneNumber.type()) {
    case KContacts::PhoneNumber::Home:
        convertedPhoneNumber.setType(QStringLiteral("home"));
        break;
    case KContacts::PhoneNumber::Work:
        convertedPhoneNumber.setType(QStringLiteral("work"));
        break;
    case KContacts::PhoneNumber::Cell:
        convertedPhoneNumber.setType(QStringLiteral("mobile"));
        break;
    case KContacts::PhoneNumber::Fax:
        convertedPhoneNumber.setType(QStringLiteral("otherFax"));
        break;
    case KContacts::PhoneNumber::Pager:
        convertedPhoneNumber.setType(QStringLiteral("pager"));
        break;
    case KContacts::PhoneNumber::Pref:
        convertedPhoneNumber.setType(QStringLiteral("main"));
        break;
    case KContacts::PhoneNumber::Voice: // Unclear if it would be Google Voice
    default:
        break;
    }

    return convertedPhoneNumber;
}

QVector<PhoneNumber> PhoneNumber::fromKContactsPhoneNumberList(const QVector<KContacts::PhoneNumber> &phoneNumberList)
{
    QVector<PhoneNumber> convertedPhoneNumbers;
    std::transform(phoneNumberList.cbegin(),
                   phoneNumberList.cend(),
                   std::back_inserter(convertedPhoneNumbers),
                   [](const KContacts::PhoneNumber &phoneNumber) {
                       return PhoneNumber::fromKContactsPhoneNumber(phoneNumber);
                   });
    return convertedPhoneNumbers;
}

KContacts::PhoneNumber PhoneNumber::toKContactsPhoneNumber() const
{
    KContacts::PhoneNumber convertedPhoneNumber;
    convertedPhoneNumber.setNumber(value());

    if(QString::compare(type(), QStringLiteral("home"), Qt::CaseInsensitive)) {
        convertedPhoneNumber.setType(KContacts::PhoneNumber::Home);
    } else if(QString::compare(type(), QStringLiteral("work"), Qt::CaseInsensitive) ||
               QString::compare(type(), QStringLiteral("workMobile"), Qt::CaseInsensitive) ||
               QString::compare(type(), QStringLiteral("workPager"), Qt::CaseInsensitive)) {
        convertedPhoneNumber.setType(KContacts::PhoneNumber::Work);
    } else if(QString::compare(type(), QStringLiteral("mobile"), Qt::CaseInsensitive)) {
        convertedPhoneNumber.setType(KContacts::PhoneNumber::Cell);
    } else if(QString::compare(type(), QStringLiteral("homeFax"), Qt::CaseInsensitive) ||
               QString::compare(type(), QStringLiteral("workFax"), Qt::CaseInsensitive) ||
               QString::compare(type(), QStringLiteral("otherFax"), Qt::CaseInsensitive)) {
        convertedPhoneNumber.setType(KContacts::PhoneNumber::Fax);
    } else if(QString::compare(type(), QStringLiteral("pager"), Qt::CaseInsensitive)) {
        convertedPhoneNumber.setType(KContacts::PhoneNumber::Pager);
    } else if(QString::compare(type(), QStringLiteral("main"), Qt::CaseInsensitive)) {
        convertedPhoneNumber.setType(KContacts::PhoneNumber::Pref);
    } else if(QString::compare(type(), QStringLiteral("googleVoice"), Qt::CaseInsensitive)) {
        convertedPhoneNumber.setType(KContacts::PhoneNumber::Voice);
    }

    return convertedPhoneNumber;
}

} // namespace KGAPI2::People

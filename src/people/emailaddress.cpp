/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "emailaddress.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <KContacts/Email>

#include <algorithm>

namespace KGAPI2::People
{
class EmailAddress::Private : public QSharedData
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
        return value == other.value && metadata == other.metadata && type == other.type && displayName == other.displayName
            && formattedType == other.formattedType;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString value{};
    FieldMetadata metadata{};
    QString type{};
    QString displayName{};
    QString formattedType{};
};

EmailAddress::EmailAddress()
    : d(new Private)
{
}

EmailAddress::EmailAddress(const EmailAddress &) = default;
EmailAddress::EmailAddress(EmailAddress &&) noexcept = default;
EmailAddress &EmailAddress::operator=(const EmailAddress &) = default;
EmailAddress &EmailAddress::operator=(EmailAddress &&) noexcept = default;
EmailAddress::~EmailAddress() = default;

bool EmailAddress::operator==(const EmailAddress &other) const
{
    return *d == *other.d;
}

bool EmailAddress::operator!=(const EmailAddress &other) const
{
    return !(*this == other);
}

QString EmailAddress::value() const
{
    return d->value;
}

void EmailAddress::setValue(const QString &value)
{
    d->value = value;
}
FieldMetadata EmailAddress::metadata() const
{
    return d->metadata;
}

void EmailAddress::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString EmailAddress::type() const
{
    return d->type;
}

void EmailAddress::setType(const QString &value)
{
    d->type = value;
}
QString EmailAddress::displayName() const
{
    return d->displayName;
}

void EmailAddress::setDisplayName(const QString &value)
{
    d->displayName = value;
}
QString EmailAddress::formattedType() const
{
    return d->formattedType;
}

EmailAddress EmailAddress::fromJSON(const QJsonObject &obj)
{
    EmailAddress emailAddress;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        emailAddress.d->metadata = FieldMetadata::fromJSON(metadata);
        emailAddress.d->value = obj.value(QStringLiteral("value")).toString();
        emailAddress.d->type = obj.value(QStringLiteral("type")).toString();
        emailAddress.d->formattedType = obj.value(QStringLiteral("formattedType")).toString();
        emailAddress.d->displayName = obj.value(QStringLiteral("displayName")).toString();
    }

    return emailAddress;
}

QVector<EmailAddress> EmailAddress::fromJSONArray(const QJsonArray& data)
{
    QVector<EmailAddress> emailAddresses;

    for(const auto &emailAddress : data) {
        if(emailAddress.isObject()) {
            const auto objectifiedEmailAddress = emailAddress.toObject();
            emailAddresses.append(fromJSON(objectifiedEmailAddress));
        }
    }

    return emailAddresses;
}


QJsonValue EmailAddress::toJSON() const
{
    QJsonObject obj;

    PeopleUtils::addValueToJsonObjectIfValid(obj, "value", d->value);
    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata", d->metadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "type", d->type);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "displayName", d->displayName);
    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "formattedType", d->formattedType);
    return obj;
}

KContacts::Email EmailAddress::toKContactsEmail() const
{
    KContacts::Email convertedEmail;
    convertedEmail.setEmail(value());

    const auto emailType = type();

    if(QString::compare(emailType, QStringLiteral("home"), Qt::CaseInsensitive)) {
        convertedEmail.setType(KContacts::Email::Home);
    } else if(QString::compare(emailType, QStringLiteral("work"), Qt::CaseInsensitive)) {
        convertedEmail.setType(KContacts::Email::Work);
    } else if(QString::compare(emailType, QStringLiteral("other"), Qt::CaseInsensitive)) {
        convertedEmail.setType(KContacts::Email::Other);
    }

    return convertedEmail;
}

EmailAddress EmailAddress::fromKContactsEmail(const KContacts::Email &email)
{
    EmailAddress convertedEmail;
    convertedEmail.setValue(email.mail());

    switch(email.type()) {
    case KContacts::Email::Home:
        convertedEmail.setType(QStringLiteral("home"));
        break;
    case KContacts::Email::Work:
        convertedEmail.setType(QStringLiteral("work"));
        break;
    case KContacts::Email::Other:
    default:
        convertedEmail.setType(QStringLiteral("other"));
        break;
    }

    return convertedEmail;
}

QVector<EmailAddress> EmailAddress::fromKContactsEmailList(const QVector<KContacts::Email> &emailList)
{
    QVector<EmailAddress> convertedEmails;
    std::transform(emailList.cbegin(),
                   emailList.cend(),
                   std::back_inserter(convertedEmails),
                   [](const KContacts::Email &email) {
                       return EmailAddress::fromKContactsEmail(email);
                   });
    return convertedEmails;
}

} // namespace KGAPI2::People

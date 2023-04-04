/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "name.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <KContacts/Addressee>

#include <algorithm>

namespace KGAPI2::People
{
class Name::Private : public QSharedData
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
        return unstructuredName == other.unstructuredName && familyName == other.familyName && phoneticMiddleName == other.phoneticMiddleName
            && middleName == other.middleName && honorificPrefix == other.honorificPrefix && givenName == other.givenName
            && phoneticGivenName == other.phoneticGivenName && phoneticHonorificPrefix == other.phoneticHonorificPrefix && displayName == other.displayName
            && displayNameLastFirst == other.displayNameLastFirst && phoneticFamilyName == other.phoneticFamilyName && honorificSuffix == other.honorificSuffix
            && phoneticHonorificSuffix == other.phoneticHonorificSuffix && phoneticFullName == other.phoneticFullName && metadata == other.metadata;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString unstructuredName{};
    QString familyName{};
    QString phoneticMiddleName{};
    QString middleName{};
    QString honorificPrefix{};
    QString givenName{};
    QString phoneticGivenName{};
    QString phoneticHonorificPrefix{};
    QString displayName{};
    QString displayNameLastFirst{};
    QString phoneticFamilyName{};
    QString honorificSuffix{};
    QString phoneticHonorificSuffix{};
    QString phoneticFullName{};
    FieldMetadata metadata{};
};

Name::Name()
    : d(new Private)
{
}

Name::Name(const Name &) = default;
Name::Name(Name &&) noexcept = default;
Name &Name::operator=(const Name &) = default;
Name &Name::operator=(Name &&) noexcept = default;
Name::~Name() = default;

bool Name::operator==(const Name &other) const
{
    return *d == *other.d;
}

bool Name::operator!=(const Name &other) const
{
    return !(*this == other);
}

QString Name::unstructuredName() const
{
    return d->unstructuredName;
}

void Name::setUnstructuredName(const QString &value)
{
    d->unstructuredName = value;
}
QString Name::familyName() const
{
    return d->familyName;
}

void Name::setFamilyName(const QString &value)
{
    d->familyName = value;
}
QString Name::phoneticMiddleName() const
{
    return d->phoneticMiddleName;
}

void Name::setPhoneticMiddleName(const QString &value)
{
    d->phoneticMiddleName = value;
}
QString Name::middleName() const
{
    return d->middleName;
}

void Name::setMiddleName(const QString &value)
{
    d->middleName = value;
}
QString Name::honorificPrefix() const
{
    return d->honorificPrefix;
}

void Name::setHonorificPrefix(const QString &value)
{
    d->honorificPrefix = value;
}
QString Name::givenName() const
{
    return d->givenName;
}

void Name::setGivenName(const QString &value)
{
    d->givenName = value;
}
QString Name::phoneticGivenName() const
{
    return d->phoneticGivenName;
}

void Name::setPhoneticGivenName(const QString &value)
{
    d->phoneticGivenName = value;
}
QString Name::phoneticHonorificPrefix() const
{
    return d->phoneticHonorificPrefix;
}

void Name::setPhoneticHonorificPrefix(const QString &value)
{
    d->phoneticHonorificPrefix = value;
}
QString Name::displayName() const
{
    return d->displayName;
}
QString Name::displayNameLastFirst() const
{
    return d->displayNameLastFirst;
}
QString Name::phoneticFamilyName() const
{
    return d->phoneticFamilyName;
}

void Name::setPhoneticFamilyName(const QString &value)
{
    d->phoneticFamilyName = value;
}
QString Name::honorificSuffix() const
{
    return d->honorificSuffix;
}

void Name::setHonorificSuffix(const QString &value)
{
    d->honorificSuffix = value;
}
QString Name::phoneticHonorificSuffix() const
{
    return d->phoneticHonorificSuffix;
}

void Name::setPhoneticHonorificSuffix(const QString &value)
{
    d->phoneticHonorificSuffix = value;
}
QString Name::phoneticFullName() const
{
    return d->phoneticFullName;
}

void Name::setPhoneticFullName(const QString &value)
{
    d->phoneticFullName = value;
}
FieldMetadata Name::metadata() const
{
    return d->metadata;
}

void Name::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

Name Name::fromJSON(const QJsonObject &obj)
{
    Name name;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        name.d->metadata = FieldMetadata::fromJSON(metadata);
        name.d->displayName = obj.value(QStringLiteral("displayName")).toString();
        name.d->displayNameLastFirst = obj.value(QStringLiteral("displayNameLastFirst")).toString();
        name.d->unstructuredName = obj.value(QStringLiteral("unstructuredName")).toString();
        name.d->familyName = obj.value(QStringLiteral("familyName")).toString();
        name.d->givenName = obj.value(QStringLiteral("givenName")).toString();
        name.d->middleName = obj.value(QStringLiteral("middleName")).toString();
        name.d->honorificPrefix = obj.value(QStringLiteral("honorificPrefix")).toString();
        name.d->honorificSuffix = obj.value(QStringLiteral("honorificSuffix")).toString();
        name.d->phoneticFullName = obj.value(QStringLiteral("phoneticFullName")).toString();
        name.d->phoneticFamilyName = obj.value(QStringLiteral("phoneticFamilyName")).toString();
        name.d->phoneticGivenName = obj.value(QStringLiteral("phoneticGivenName")).toString();
        name.d->phoneticMiddleName = obj.value(QStringLiteral("phoneticMiddleName")).toString();
        name.d->phoneticHonorificPrefix = obj.value(QStringLiteral("phoneticHonorificPrefix")).toString();
        name.d->phoneticHonorificSuffix = obj.value(QStringLiteral("phoneticHonorificSuffix")).toString();
    }

    return name;
}

QList<Name> Name::fromJSONArray(const QJsonArray &data)
{
    QList<Name> names;

    for(const auto &name : data) {
        if(name.isObject()) {
            const auto objectifiedName = name.toObject();
            names.append(fromJSON(objectifiedName));
        }
    }

    return names;
}

QJsonValue Name::toJSON() const
{
    QJsonObject obj;

    PeopleUtils::addValueToJsonObjectIfValid(obj, "unstructuredName", d->unstructuredName);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "familyName", d->familyName);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "phoneticMiddleName", d->phoneticMiddleName);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "middleName", d->middleName);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "honorificPrefix", d->honorificPrefix);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "givenName", d->givenName);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "phoneticGivenName", d->phoneticGivenName);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "phoneticHonorificPrefix", d->phoneticHonorificPrefix);
    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "displayName", d->displayName);
    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "displayNameLastFirst", d->displayNameLastFirst);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "phoneticFamilyName", d->phoneticFamilyName);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "honorificSuffix", d->honorificSuffix);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "phoneticHonorificSuffix", d->phoneticHonorificSuffix);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "phoneticFullName", d->phoneticFullName);
    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata", d->metadata.toJSON());
    return obj;
}

Name Name::fromKContactsAddressee(const KContacts::Addressee &addressee)
{
    Name name;
    name.setFamilyName(addressee.familyName());
    name.setGivenName(addressee.givenName());
    name.setHonorificPrefix(addressee.prefix());
    name.setHonorificSuffix(addressee.suffix());
    return name;
}

void Name::applyToKContactsAddressee(KContacts::Addressee &addressee) const
{
    addressee.setName(unstructuredName());
    addressee.setFamilyName(familyName());
    addressee.setGivenName(givenName());
    addressee.setPrefix(honorificPrefix());
    addressee.setSuffix(honorificSuffix());
}

} // namespace KGAPI2::People

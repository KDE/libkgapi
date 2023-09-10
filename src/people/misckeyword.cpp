/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "misckeyword.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{

struct MiscKeywordDefinition
{
    FieldMetadata metadata;
    QString value;
    MiscKeyword::Type type;
    QString formattedType;
};

class MiscKeyword::Private : public QSharedData
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
        return metadata == other.metadata && value == other.value && type == other.type && formattedType == other.formattedType;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    FieldMetadata metadata{};
    QString value{};
    MiscKeyword::Type type{};
    QString formattedType{};
};

MiscKeyword::MiscKeyword()
    : d(new Private)
{
}

MiscKeyword::MiscKeyword(const MiscKeywordDefinition &definition)
    : d(new Private)
{
    d->metadata = definition.metadata;
    d->value = definition.value;
    d->type = definition.type;
    d->formattedType = definition.formattedType;
}

MiscKeyword::MiscKeyword(const MiscKeyword &) = default;
MiscKeyword::MiscKeyword(MiscKeyword &&) noexcept = default;
MiscKeyword &MiscKeyword::operator=(const MiscKeyword &) = default;
MiscKeyword &MiscKeyword::operator=(MiscKeyword &&) noexcept = default;
MiscKeyword::~MiscKeyword() = default;

bool MiscKeyword::operator==(const MiscKeyword &other) const
{
    return *d == *other.d;
}

bool MiscKeyword::operator!=(const MiscKeyword &other) const
{
    return !(*this == other);
}

FieldMetadata MiscKeyword::metadata() const
{
    return d->metadata;
}

void MiscKeyword::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString MiscKeyword::value() const
{
    return d->value;
}

void MiscKeyword::setValue(const QString &value)
{
    d->value = value;
}
MiscKeyword::MiscKeyword::Type MiscKeyword::type() const
{
    return d->type;
}

void MiscKeyword::setType(MiscKeyword::Type value)
{
    d->type = value;
}
QString MiscKeyword::formattedType() const
{
    return d->formattedType;
}

MiscKeyword MiscKeyword::fromJSON(const QJsonObject &obj)
{
    if(obj.isEmpty()) {
        return MiscKeyword();
    }

    MiscKeywordDefinition definition;

    const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
    definition.metadata = FieldMetadata::fromJSON(metadata);
    definition.value = obj.value(QStringLiteral("value")).toString();

    const auto type = obj.value(QStringLiteral("type")).toString();
    if(type == QLatin1String("OUTLOOK_BILLING_INFORMATION")) {
        definition.type = Type::OUTLOOK_BILLING_INFORMATION;
    } else if(type == QLatin1String("OUTLOOK_DIRECTORY_SERVER")) {
        definition.type = Type::OUTLOOK_DIRECTORY_SERVER;
    } else if(type == QLatin1String("OUTLOOK_KEYWORD")) {
        definition.type = Type::OUTLOOK_KEYWORD;
    } else if(type == QLatin1String("OUTLOOK_MILEAGE")) {
        definition.type = Type::OUTLOOK_MILEAGE;
    } else if(type == QLatin1String("OUTLOOK_PRIORITY")) {
        definition.type = Type::OUTLOOK_PRIORITY;
    } else if(type == QLatin1String("OUTLOOK_SENSITIVITY")) {
        definition.type = Type::OUTLOOK_SENSITIVITY;
    } else if(type == QLatin1String("OUTLOOK_SUBJECT")) {
        definition.type = Type::OUTLOOK_SUBJECT;
    } else if(type == QLatin1String("OUTLOOK_USER")) {
        definition.type = Type::OUTLOOK_USER;
    } else if(type == QLatin1String("HOME")) {
        definition.type = Type::HOME;
    } else if(type == QLatin1String("WORK")) {
        definition.type = Type::WORK;
    } else if(type == QLatin1String("OTHER")) {
        definition.type = Type::OTHER;
    } else {
        definition.type = Type::TYPE_UNSPECIFIED;
    }

    definition.formattedType = obj.value(QStringLiteral("formattedType")).toString();

    return MiscKeyword(definition);
}

QList<MiscKeyword> MiscKeyword::fromJSONArray(const QJsonArray &data)
{
    QList<MiscKeyword> miscKeywords;

    for(const auto &miscKeyword : data) {
        if(miscKeyword.isObject()) {
            const auto objectifiedMiscKeyword = miscKeyword.toObject();
            miscKeywords.append(fromJSON(objectifiedMiscKeyword));
        }
    }

    return miscKeywords;
}

QJsonValue MiscKeyword::toJSON() const
{
    QJsonObject obj;

    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata"}, d->metadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "value", d->value);
    switch (d->type) {
    case Type::TYPE_UNSPECIFIED:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("TYPE_UNSPECIFIED"));
        break;
    case Type::OUTLOOK_BILLING_INFORMATION:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("OUTLOOK_BILLING_INFORMATION"));
        break;
    case Type::OUTLOOK_DIRECTORY_SERVER:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("OUTLOOK_DIRECTORY_SERVER"));
        break;
    case Type::OUTLOOK_KEYWORD:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("OUTLOOK_KEYWORD"));
        break;
    case Type::OUTLOOK_MILEAGE:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("OUTLOOK_MILEAGE"));
        break;
    case Type::OUTLOOK_PRIORITY:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("OUTLOOK_PRIORITY"));
        break;
    case Type::OUTLOOK_SENSITIVITY:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("OUTLOOK_SENSITIVITY"));
        break;
    case Type::OUTLOOK_SUBJECT:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("OUTLOOK_SUBJECT"));
        break;
    case Type::OUTLOOK_USER:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("OUTLOOK_USER"));
        break;
    case Type::HOME:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("HOME"));
        break;
    case Type::WORK:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("WORK"));
        break;
    case Type::OTHER:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("OTHER"));
        break;
    }
    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "formattedType"}, d->formattedType);
    return obj;
}

} // namespace KGAPI2::People

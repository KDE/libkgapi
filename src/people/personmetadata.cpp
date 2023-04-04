/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "personmetadata.h"

#include "peopleservice.h"
#include "source.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class PersonMetadata::Private : public QSharedData
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
        return linkedPeopleResourceNames == other.linkedPeopleResourceNames && objectType == other.objectType
            && previousResourceNames == other.previousResourceNames && deleted == other.deleted && sources == other.sources;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QList<QString> linkedPeopleResourceNames{};
    PersonMetadata::ObjectType objectType{};
    QList<QString> previousResourceNames{};
    bool deleted{};
    QList<Source> sources{};
};

PersonMetadata::PersonMetadata()
    : d(new Private)
{
}

PersonMetadata::PersonMetadata(const PersonMetadata &) = default;
PersonMetadata::PersonMetadata(PersonMetadata &&) noexcept = default;
PersonMetadata &PersonMetadata::operator=(const PersonMetadata &) = default;
PersonMetadata &PersonMetadata::operator=(PersonMetadata &&) noexcept = default;
PersonMetadata::~PersonMetadata() = default;

bool PersonMetadata::operator==(const PersonMetadata &other) const
{
    return *d == *other.d;
}

bool PersonMetadata::operator!=(const PersonMetadata &other) const
{
    return !(*this == other);
}

QList<QString> PersonMetadata::linkedPeopleResourceNames() const
{
    return d->linkedPeopleResourceNames;
}
PersonMetadata::PersonMetadata::ObjectType PersonMetadata::objectType() const
{
    return d->objectType;
}
QList<QString> PersonMetadata::previousResourceNames() const
{
    return d->previousResourceNames;
}
bool PersonMetadata::deleted() const
{
    return d->deleted;
}
QList<Source> PersonMetadata::sources() const
{
    return d->sources;
}

void PersonMetadata::setSources(const QList<Source> &value)
{
    d->sources = value;
}

void PersonMetadata::addSource(const Source &value)
{
    d->sources.push_back(value);
}

void PersonMetadata::removeSource(const Source &value)
{
    d->sources.removeOne(value);
}

void PersonMetadata::clearSources()
{
    d->sources.clear();
}

PersonMetadata PersonMetadata::fromJSON(const QJsonObject &obj)
{
    PersonMetadata personMetadata;

    if(!obj.isEmpty()) {
        for(const auto &jsonSource : obj.value(QStringLiteral("sources")).toArray()) {
            personMetadata.d->sources.append(Source::fromJSON(jsonSource.toObject()));
        }

        for(const auto &jsonPrevResName : obj.value(QStringLiteral("previousResourceNames")).toArray()) {
            personMetadata.d->previousResourceNames.append(jsonPrevResName.toString());
        }

        for(const auto &jsonLinkedPeopleResName : obj.value(QStringLiteral("linkedPeopleResourceNames")).toArray()) {
            personMetadata.d->linkedPeopleResourceNames.append(jsonLinkedPeopleResName.toString());
        }

        personMetadata.d->deleted = obj.value(QStringLiteral("deleted")).toBool();

        // Don't add objectType here, is deprecated
    }

    return personMetadata;
}

QJsonValue PersonMetadata::toJSON() const
{
    QJsonObject obj;

    /* Output only
    {
        QJsonArray arr;
        std::transform(d->linkedPeopleResourceNames.cbegin(), d->linkedPeopleResourceNames.cend(), std::back_inserter(arr), [](const auto &val) {
            return val;
        });
        PeopleUtils::addValueToJsonObjectIfValid(obj, "linkedPeopleResourceNames", std::move(arr));
    }
    */
    /* Output only
    switch (d->objectType) {
    case ObjectType::OBJECT_TYPE_UNSPECIFIED:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "objectType", QStringLiteral("OBJECT_TYPE_UNSPECIFIED"));
        break;
    case ObjectType::PERSON:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "objectType", QStringLiteral("PERSON"));
        break;
    case ObjectType::PAGE:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "objectType", QStringLiteral("PAGE"));
        break;
    }*/
    /* Output only
    {
        QJsonArray arr;
        std::transform(d->previousResourceNames.cbegin(), d->previousResourceNames.cend(), std::back_inserter(arr), [](const auto &val) {
            return val;
        });
        PeopleUtils::addValueToJsonObjectIfValid(obj, "previousResourceNames", std::move(arr));
    }
    */
    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "deleted", d->deleted);
    {
        QJsonArray arr;
        std::transform(d->sources.cbegin(), d->sources.cend(), std::back_inserter(arr), [](const auto &val) {
            return val.toJSON();
        });
        if (!arr.isEmpty()) {
            PeopleUtils::addValueToJsonObjectIfValid(obj, "sources", std::move(arr));
        }
    }

    return obj;
}

} // namespace KGAPI2::People

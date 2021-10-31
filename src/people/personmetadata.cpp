/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "personmetadata.h"

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

    QVector<QString> linkedPeopleResourceNames{};
    PersonMetadata::ObjectType objectType{};
    QVector<QString> previousResourceNames{};
    bool deleted{};
    QVector<Source> sources{};
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

QVector<QString> PersonMetadata::linkedPeopleResourceNames() const
{
    return d->linkedPeopleResourceNames;
}
PersonMetadata::PersonMetadata::ObjectType PersonMetadata::objectType() const
{
    return d->objectType;
}
QVector<QString> PersonMetadata::previousResourceNames() const
{
    return d->previousResourceNames;
}
bool PersonMetadata::deleted() const
{
    return d->deleted;
}
QVector<Source> PersonMetadata::sources() const
{
    return d->sources;
}

void PersonMetadata::setSources(const QVector<Source> &value)
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
    Q_UNUSED(obj);
    return PersonMetadata();
}

QJsonValue PersonMetadata::toJSON() const
{
    QJsonObject obj;

    {
        QJsonArray arr;
        std::transform(d->linkedPeopleResourceNames.cbegin(), d->linkedPeopleResourceNames.cend(), std::back_inserter(arr), [](const auto &val) {
            return val;
        });
        obj.insert(QStringView{u"linkedPeopleResourceNames"}, std::move(arr));
    }
    switch (d->objectType) {
    case ObjectType::OBJECT_TYPE_UNSPECIFIED:
        obj.insert(QStringView{u"objectType"}, QStringLiteral(u"OBJECT_TYPE_UNSPECIFIED"));
        break;
    case ObjectType::PERSON:
        obj.insert(QStringView{u"objectType"}, QStringLiteral(u"PERSON"));
        break;
    case ObjectType::PAGE:
        obj.insert(QStringView{u"objectType"}, QStringLiteral(u"PAGE"));
        break;
    }
    {
        QJsonArray arr;
        std::transform(d->previousResourceNames.cbegin(), d->previousResourceNames.cend(), std::back_inserter(arr), [](const auto &val) {
            return val;
        });
        obj.insert(QStringView{u"previousResourceNames"}, std::move(arr));
    }
    obj.insert(QStringView{u"deleted"}, d->deleted);
    {
        QJsonArray arr;
        std::transform(d->sources.cbegin(), d->sources.cend(), std::back_inserter(arr), [](const auto &val) {
            return val.toJSON();
        });
        obj.insert(QStringView{u"sources"}, std::move(arr));
    }
    return obj;
}

} // namespace KGAPI2::People

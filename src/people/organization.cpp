/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "organization.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Organization::Private : public QSharedData
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
        return location == other.location && title == other.title && type == other.type && metadata == other.metadata && symbol == other.symbol
            && formattedType == other.formattedType && name == other.name && current == other.current && costCenter == other.costCenter
            && department == other.department && domain == other.domain && startDate == other.startDate && jobDescription == other.jobDescription
            && endDate == other.endDate && phoneticName == other.phoneticName && fullTimeEquivalentMillipercent == other.fullTimeEquivalentMillipercent;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString location{};
    QString title{};
    QString type{};
    FieldMetadata metadata{};
    QString symbol{};
    QString formattedType{};
    QString name{};
    bool current{};
    bool hasSetCurrent{};
    QString costCenter{};
    QString department{};
    QString domain{};
    QDate startDate{};
    QString jobDescription{};
    QDate endDate{};
    QString phoneticName{};
    int fullTimeEquivalentMillipercent{};
    bool hasSetFullTimeEquivalentMillipercent{};
};

Organization::Organization()
    : d(new Private)
{
}

Organization::Organization(const Organization &) = default;
Organization::Organization(Organization &&) noexcept = default;
Organization &Organization::operator=(const Organization &) = default;
Organization &Organization::operator=(Organization &&) noexcept = default;
Organization::~Organization() = default;

bool Organization::operator==(const Organization &other) const
{
    return *d == *other.d;
}

bool Organization::operator!=(const Organization &other) const
{
    return !(*this == other);
}

QString Organization::location() const
{
    return d->location;
}

void Organization::setLocation(const QString &value)
{
    d->location = value;
}
QString Organization::title() const
{
    return d->title;
}

void Organization::setTitle(const QString &value)
{
    d->title = value;
}
QString Organization::type() const
{
    return d->type;
}

void Organization::setType(const QString &value)
{
    d->type = value;
}
FieldMetadata Organization::metadata() const
{
    return d->metadata;
}

void Organization::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString Organization::symbol() const
{
    return d->symbol;
}

void Organization::setSymbol(const QString &value)
{
    d->symbol = value;
}
QString Organization::formattedType() const
{
    return d->formattedType;
}
QString Organization::name() const
{
    return d->name;
}

void Organization::setName(const QString &value)
{
    d->name = value;
}
bool Organization::current() const
{
    return d->current;
}

void Organization::setCurrent(const bool &value)
{
    d->current = value;
    d->hasSetCurrent = true;
}

bool Organization::hasSetCurrent() const
{
    return d->hasSetCurrent;
}

QString Organization::costCenter() const
{
    return d->costCenter;
}

void Organization::setCostCenter(const QString &value)
{
    d->costCenter = value;
}
QString Organization::department() const
{
    return d->department;
}

void Organization::setDepartment(const QString &value)
{
    d->department = value;
}
QString Organization::domain() const
{
    return d->domain;
}

void Organization::setDomain(const QString &value)
{
    d->domain = value;
}
QDate Organization::startDate() const
{
    return d->startDate;
}

void Organization::setStartDate(const QDate &value)
{
    d->startDate = value;
}
QString Organization::jobDescription() const
{
    return d->jobDescription;
}

void Organization::setJobDescription(const QString &value)
{
    d->jobDescription = value;
}
QDate Organization::endDate() const
{
    return d->endDate;
}

void Organization::setEndDate(const QDate &value)
{
    d->endDate = value;
}
QString Organization::phoneticName() const
{
    return d->phoneticName;
}

void Organization::setPhoneticName(const QString &value)
{
    d->phoneticName = value;
}
int Organization::fullTimeEquivalentMillipercent() const
{
    return d->fullTimeEquivalentMillipercent;
}

void Organization::setFullTimeEquivalentMillipercent(const int &value)
{
    d->fullTimeEquivalentMillipercent = value;
    d->hasSetFullTimeEquivalentMillipercent = true;
}

bool Organization::hasSetFullTimeEquivalentMillipercent() const
{
    return d->hasSetFullTimeEquivalentMillipercent;
}

Organization Organization::fromJSON(const QJsonObject &obj)
{
    Organization organization;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        organization.d->metadata = FieldMetadata::fromJSON(metadata);
        organization.d->type = obj.value(QStringLiteral("type")).toString();
        organization.d->formattedType = obj.value(QStringLiteral("formattedType")).toString();

        const auto jsonStartDate = obj.value(QStringLiteral("startDate")).toObject();
        const auto startYear = jsonStartDate.value(QStringLiteral("year")).toInt();
        const auto startMonth = jsonStartDate.value(QStringLiteral("month")).toInt();
        const auto startDay = jsonStartDate.value(QStringLiteral("day")).toInt();
        organization.d->startDate = QDate(startYear, startMonth, startDay);

        const auto jsonEndDate = obj.value(QStringLiteral("endDate")).toObject();
        const auto endYear = jsonEndDate.value(QStringLiteral("year")).toInt();
        const auto endMonth = jsonEndDate.value(QStringLiteral("month")).toInt();
        const auto endDay = jsonEndDate.value(QStringLiteral("day")).toInt();
        organization.d->endDate = QDate(endYear, endMonth, endDay);

        organization.d->current = obj.value(QStringLiteral("current")).toBool();
        organization.d->hasSetCurrent = obj.contains(QStringLiteral("current"));
        organization.d->name = obj.value(QStringLiteral("name")).toString();
        organization.d->phoneticName = obj.value(QStringLiteral("phoneticName")).toString();
        organization.d->department = obj.value(QStringLiteral("department")).toString();
        organization.d->title = obj.value(QStringLiteral("title")).toString();
        organization.d->jobDescription = obj.value(QStringLiteral("jobDescription")).toString();
        organization.d->symbol = obj.value(QStringLiteral("symbol")).toString();
        organization.d->domain = obj.value(QStringLiteral("domain")).toString();
        organization.d->location = obj.value(QStringLiteral("location")).toString();
        organization.d->costCenter = obj.value(QStringLiteral("costCenter")).toString();
        organization.d->fullTimeEquivalentMillipercent = obj.value(QStringLiteral("fullTimeEquivalentMillipercent")).toInt();
        organization.d->hasSetFullTimeEquivalentMillipercent = obj.contains(QStringLiteral("fullTimeEquivalentMillipercent"));
    }

    return organization;
}

QVector<Organization> Organization::fromJSONArray(const QJsonArray& data)
{
    QVector<Organization> organizations;

    for(const auto &organization : data) {
        if(organization.isObject()) {
            const auto objectifiedOrganization = organization.toObject();
            organizations.append(fromJSON(objectifiedOrganization));
        }
    }

    return organizations;
}

QJsonValue Organization::toJSON() const
{
    QJsonObject obj;

    PeopleUtils::addValueToJsonObjectIfValid(obj, "location", d->location);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "title", d->title);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "type", d->type);
    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata", d->metadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "symbol", d->symbol);
    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "formattedType", d->formattedType);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "name", d->name);

    if (d->hasSetCurrent) {
        PeopleUtils::addValueToJsonObjectIfValid(obj, "current", d->current);
    }

    PeopleUtils::addValueToJsonObjectIfValid(obj, "costCenter", d->costCenter);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "department", d->department);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "domain", d->domain);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "jobDescription", d->jobDescription);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "phoneticName", d->phoneticName);

    if(d->hasSetFullTimeEquivalentMillipercent) {
        PeopleUtils::addValueToJsonObjectIfValid(obj, "fullTimeEquivalentMillipercent", d->fullTimeEquivalentMillipercent);
    }

    return obj;
}

} // namespace KGAPI2::People

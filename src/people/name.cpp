/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "name.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

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
    Q_UNUSED(obj);
    return Name();
}

QJsonValue Name::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"unstructuredName"}, d->unstructuredName);
    obj.insert(QStringView{u"familyName"}, d->familyName);
    obj.insert(QStringView{u"phoneticMiddleName"}, d->phoneticMiddleName);
    obj.insert(QStringView{u"middleName"}, d->middleName);
    obj.insert(QStringView{u"honorificPrefix"}, d->honorificPrefix);
    obj.insert(QStringView{u"givenName"}, d->givenName);
    obj.insert(QStringView{u"phoneticGivenName"}, d->phoneticGivenName);
    obj.insert(QStringView{u"phoneticHonorificPrefix"}, d->phoneticHonorificPrefix);
    obj.insert(QStringView{u"displayName"}, d->displayName);
    obj.insert(QStringView{u"displayNameLastFirst"}, d->displayNameLastFirst);
    obj.insert(QStringView{u"phoneticFamilyName"}, d->phoneticFamilyName);
    obj.insert(QStringView{u"honorificSuffix"}, d->honorificSuffix);
    obj.insert(QStringView{u"phoneticHonorificSuffix"}, d->phoneticHonorificSuffix);
    obj.insert(QStringView{u"phoneticFullName"}, d->phoneticFullName);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People

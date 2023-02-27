/*
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "peopletestutils.h"

#include "people/contactgroup.h"
#include "people/peopleservice.h"
#include "people/person.h"
#include "testutils.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace KGAPI2 {
namespace People {
namespace TestUtils {

PersonPtr personFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    const auto jsonObject = QJsonDocument::fromJson(f.readAll()).object();
    const auto person = Person::fromJSON(jsonObject);
    VERIFY_RET(person, {});
    return person;
}

ContactGroupPtr contactGroupFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    const auto jsonObject = QJsonDocument::fromJson(f.readAll()).object();
    auto group = ContactGroup::fromJSON(jsonObject);
    VERIFY_RET(group, {});
    return group;
}

PersonList connectionsFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    FeedData feedData;
    const auto connections = PeopleService::parseConnectionsJSONFeed(feedData, f.readAll());

    PersonList people;
    std::transform(connections.cbegin(), connections.cend(), std::back_inserter(people), [](const ObjectPtr &object) {
        return object.dynamicCast<Person>();
    });

    return people;
}

ContactGroupList contactGroupsFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    FeedData feedData;
    const auto contactGroups = PeopleService::parseContactGroupsJSONFeed(feedData, f.readAll());

    ContactGroupList convertedContactGroups;
    std::transform(contactGroups.cbegin(), contactGroups.cend(), std::back_inserter(convertedContactGroups), [](const ObjectPtr &object) {
        return object.dynamicCast<ContactGroup>();
    });

    return convertedContactGroups;
}

QByteArray rawPhotoDataFromPhotoRequestData(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    const auto jsonDocument = QJsonDocument::fromJson(f.readAll());
    VERIFY_RET(jsonDocument.isObject(), {});

    const auto jsonObject = jsonDocument.object();
    VERIFY_RET(jsonObject.contains(QStringLiteral("personFields")), {});
    VERIFY_RET(jsonObject.contains(QStringLiteral("photoBytes")), {});

    const auto rawData = jsonObject.value(QStringLiteral("photoBytes")).toString().toUtf8();
    return QByteArray::fromBase64(rawData);
}

PersonPtr personFromPhotoUpdateResponseFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    const auto jsonObject = QJsonDocument::fromJson(f.readAll()).object();
    const auto personObject = jsonObject.value(QStringLiteral("person")).toObject();
    const auto person = Person::fromJSON(personObject);
    VERIFY_RET(person, {});
    return person;
}

ContactGroupPtr contactGroupFromCreateDataFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    const auto jsonObject = QJsonDocument::fromJson(f.readAll()).object();
    const auto groupObject = jsonObject.value(QStringLiteral("contactGroup")).toObject();
    auto group = ContactGroup::fromJSON(groupObject);
    VERIFY_RET(group, {});
    return group;
}

} // TestUtils
} // People
} // KGAPI2

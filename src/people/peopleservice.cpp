/*
 *  SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "peopleservice.h"
#include "contactgroup.h"
#include "debug.h"
#include "person.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

/* Qt::escape() */
#include <QTextDocument>

namespace KGAPI2::People
{

namespace PeopleService
{

namespace Private
{

enum FetchType {
    PersonFetch,
    ContactGroupFetch
};

static const QUrl GoogleApisUrl(QStringLiteral("https://people.googleapis.com"));
static const auto PeopleV1Path = QStringLiteral("/v1/");
static const auto PeopleBasePath = QString(PeopleV1Path % QStringLiteral("people"));
static const auto ContactGroupsBasePath = QString(PeopleV1Path % QStringLiteral("contactGroups"));

static const auto AllPersonFields = QStringLiteral("addresses,"
                                                   "ageRanges,"
                                                   "biographies,"
                                                   "birthdays,"
                                                   "calendarUrls,"
                                                   "clientData,"
                                                   "coverPhotos,"
                                                   "emailAddresses,"
                                                   "events,"
                                                   "externalIds,"
                                                   "genders,"
                                                   "imClients,"
                                                   "interests,"
                                                   "locales,"
                                                   "locations,"
                                                   "memberships,"
                                                   "metadata,"
                                                   "miscKeywords,"
                                                   "names,"
                                                   "nicknames,"
                                                   "occupations,"
                                                   "organizations,"
                                                   "phoneNumbers,"
                                                   "photos,"
                                                   "relations,"
                                                   "sipAddresses,"
                                                   "skills,"
                                                   "urls,"
                                                   "userDefined");

static const auto AllUpdatablePersonFields = QStringLiteral("addresses,"
                                                            "biographies,"
                                                            "birthdays,"
                                                            "calendarUrls,"
                                                            "clientData,"
                                                            "emailAddresses,"
                                                            "events,"
                                                            "externalIds,"
                                                            "genders,"
                                                            "imClients,"
                                                            "interests,"
                                                            "locales,"
                                                            "locations,"
                                                            "memberships,"
                                                            "miscKeywords,"
                                                            "names,"
                                                            "nicknames,"
                                                            "occupations,"
                                                            "organizations,"
                                                            "phoneNumbers,"
                                                            "relations,"
                                                            "sipAddresses,"
                                                            "urls,"
                                                            "userDefined");

static const auto AllGroupFields = QStringLiteral("clientData,"
                                                  "groupType,"
                                                  "memberCount,"
                                                  "metadata,"
                                                  "name");

static const auto AllRecentlyCreatedAvailableGroupFields = QStringLiteral("clientData,"
                                                                          "groupType,"
                                                                          "metadata,"
                                                                          "name");

void writeNextPageDataQuery(FetchType fetchType, FeedData &feedData, const QJsonObject &replyRootObject, const QString &syncToken = {})
{
    if(!replyRootObject.contains(QStringLiteral("nextPageToken"))) {
        return;
    }

    QUrl url;
    if (fetchType == PersonFetch) {
        url = fetchAllContactsUrl(syncToken);
    } else if (fetchType == ContactGroupFetch) {
        url = fetchAllContactGroupsUrl();
    } else {
        qCDebug(KGAPIDebug) << "Unknown type of fetch, cannot write next page data query";
        return;
    }

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("pageToken"), replyRootObject.value(QStringLiteral("nextPageToken")).toString());

    url.setQuery(query);
    feedData.nextPageUrl = url;
}

} // Private

QString allPersonFields()
{
    return Private::AllPersonFields;
}

QString allUpdatablePersonFields()
{
    return Private::AllUpdatablePersonFields;
}

QString allContactGroupRecentlyCreatedAvailableFields()
{
    return Private::AllRecentlyCreatedAvailableGroupFields;
}

ObjectPtr JSONToPerson(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if(document.isObject()) {
        const auto objectifiedDocument = document.object();
        const auto resourceName = objectifiedDocument.value(QStringLiteral("resourceName")).toString();

        return resourceName.startsWith(QStringLiteral("people")) ? People::Person::fromJSON(objectifiedDocument) : People::PersonPtr();
    }

    return People::PersonPtr();
}

QUrl fetchAllContactsUrl(const QString &syncToken)
{
    QUrl url(Private::GoogleApisUrl);
    const QString path = Private::PeopleBasePath % QStringLiteral("/me/connections");
    url.setPath(path);

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("personFields"), Private::AllPersonFields);
    query.addQueryItem(QStringLiteral("requestSyncToken"), QStringLiteral("true"));

    if (!syncToken.isEmpty()) {
        query.addQueryItem(QStringLiteral("syncToken"), syncToken);
    }

    url.setQuery(query);
    return url;
}

// https://developers.google.com/people/api/rest/v1/people/searchContacts
QUrl fetchContactUrl(const QString &resourceName)
{
    QUrl url(Private::GoogleApisUrl);
    const QString path = Private::PeopleV1Path % resourceName;
    url.setPath(path);

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("personFields"), Private::AllPersonFields);

    url.setQuery(query);
    return url;
}

QUrl createContactUrl()
{
    QUrl url(Private::GoogleApisUrl);
    const QString path = Private::PeopleBasePath % QStringLiteral(":createContact");
    url.setPath(path);
    return url;
}

QUrl updateContactUrl(const QString &resourceName, const QString &personFields)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::PeopleV1Path % resourceName % QStringLiteral(":updateContact"));

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("updatePersonFields"), personFields);

    url.setQuery(query);
    return url;
}

QUrl deleteContactUrl(const QString &resourceName)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::PeopleV1Path % resourceName % QStringLiteral(":deleteContact"));
    return url;
}

QUrl fetchAllContactGroupsUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactGroupsBasePath);

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("groupFields"), Private::AllGroupFields);

    url.setQuery(query);

    return url;
}

// https://developers.google.com/people/api/rest/v1/contactGroups/get
QUrl fetchContactGroupUrl(const QString &resourceName)
{
    QUrl url(Private::GoogleApisUrl);
    const QString path = Private::PeopleV1Path % resourceName;
    url.setPath(path);

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("groupFields"), Private::AllGroupFields);

    url.setQuery(query);
    return url;
}

QUrl createContactGroupUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactGroupsBasePath);
    return url;
}

QUrl updateContactGroupUrl(const QString &resourceName)
{
    QUrl url(Private::GoogleApisUrl);
    const QString path = Private::PeopleV1Path % resourceName;
    url.setPath(path);
    return url;
}

QUrl deleteContactGroupUrl(const QString &resourceName, const bool deleteContacts)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::PeopleV1Path % resourceName);

    const auto deleteContactsString = deleteContacts ? QStringLiteral("true") : QStringLiteral("false");
    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("deleteContacts"), deleteContactsString);

    url.setQuery(query);
    return url;
}

QUrl updateContactPhotoUrl(const QString &resourceName)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::PeopleV1Path % resourceName % QStringLiteral(":updateContactPhoto"));
    return url;
}

QUrl deleteContactPhotoUrl(const QString &resourceName, const QString &personFields)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::PeopleV1Path % resourceName % QStringLiteral(":deleteContactPhoto"));

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("personFields"), personFields);

    url.setQuery(query);
    return url;
}

ObjectsList parseConnectionsJSONFeed(FeedData &feedData, const QByteArray &jsonFeed, const QString &syncToken)
{
    const auto document = QJsonDocument::fromJson(jsonFeed);

    if (!document.isObject()) {
        return {};
    }

    ObjectsList output;

    const auto rootObject = document.object();
    const auto connections = rootObject.value(QStringLiteral("connections")).toArray();
    for(const auto connection : connections) {
        output.append(People::Person::fromJSON(connection.toObject()));
    }

    feedData.totalResults = rootObject.value(QStringLiteral("totalItems")).toInt();

    Private::writeNextPageDataQuery(Private::PersonFetch, feedData, rootObject, syncToken);
    feedData.syncToken = rootObject.value(QStringLiteral("nextSyncToken")).toString();

    return output;
}

ObjectsList parseContactGroupsJSONFeed(FeedData &feedData, const QByteArray &jsonFeed)
{
    qDebug() << jsonFeed;
    const auto document = QJsonDocument::fromJson(jsonFeed);

    if (!document.isObject()) {
        return {};
    }

    ObjectsList output;

    const auto rootObject = document.object();
    const auto contactGroups = rootObject.value(QStringLiteral("contactGroups")).toArray();
    for(const auto contactGroup : contactGroups) {
        output.append(People::ContactGroup::fromJSON(contactGroup.toObject()));
    }

    feedData.totalResults = rootObject.value(QStringLiteral("totalItems")).toInt();

    Private::writeNextPageDataQuery(Private::ContactGroupFetch, feedData, rootObject);

    return output;
}

}

namespace PeopleUtils
{

void addValueToJsonObjectIfValid(QJsonObject &object, const QByteArray &key, const int value)
{
    object.insert(QString::fromUtf8(key), value);
}

void addValueToJsonObjectIfValid(QJsonObject &object, const QByteArray &key, const bool value)
{
    object.insert(QString::fromUtf8(key), value);
}

void addValueToJsonObjectIfValid(QJsonObject &object, const QByteArray &key, const QString &value)
{
    if (!value.isEmpty()) {
        object.insert(QString::fromUtf8(key), value);
    }
}

void addValueToJsonObjectIfValid(QJsonObject &object, const QByteArray &key, const QJsonValue &value)
{
    if (!value.isNull() || !value.isUndefined()) {
        object.insert(QString::fromUtf8(key), value);
    }
}


}

}

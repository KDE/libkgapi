/*
 *  SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "peopleservice.h"
#include "person.h"
#include "contactgroup.h"
#include "address.h"
#include "agerangetype.h"
#include "utils.h"
#include "debug.h"

//#include <QDomDocument>
//#include <QDomElement>
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

void writeNextPageDataQuery(FeedData &feedData, const QJsonObject &replyRootObject)
{
    if(!replyRootObject.contains(QStringLiteral("nextPageToken"))) {
        return;
    }

    auto url = fetchAllContactsUrl();

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
QUrl fetchContactUrl(const QString &fetchQuery, const QString &readMask)
{
    QUrl url(Private::GoogleApisUrl);
    const QString path = Private::PeopleBasePath % QStringLiteral(":searchContacts");
    url.setPath(path);

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("query"), fetchQuery);
    query.addQueryItem(QStringLiteral("readMask"), readMask);

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

ObjectsList parseConnectionsJSONFeed(FeedData &feedData, const QByteArray &jsonFeed)
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

    Private::writeNextPageDataQuery(feedData, rootObject);
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

    Private::writeNextPageDataQuery(feedData, rootObject);

    return output;
}

}

}

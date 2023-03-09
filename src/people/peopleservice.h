/*
    SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "types.h"
#include "kgapipeople_export.h"

#include <QUrl>

namespace KGAPI2::People
{

/**
 * @brief Additional methods for implementing support for Google People service
 *
 * You should never need to use these methods, unless implementing your own Job
 */

namespace PeopleService
{

    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QString allPersonFields();
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QString allUpdatablePersonFields();
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QString allContactGroupRecentlyCreatedAvailableFields();

    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QUrl fetchAllContactsUrl(const QString &syncToken = {});
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QUrl fetchContactUrl(const QString &resourceName);
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QUrl createContactUrl();
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QUrl updateContactUrl(const QString &resourceName, const QString &personFields);
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QUrl deleteContactUrl(const QString &resourceName);
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QUrl updateContactPhotoUrl(const QString &resourceName);
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QUrl deleteContactPhotoUrl(const QString &resourceName, const QString &personFields);


    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QUrl fetchAllContactGroupsUrl();
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QUrl fetchContactGroupUrl(const QString &resourceName);
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QUrl createContactGroupUrl();
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QUrl updateContactGroupUrl(const QString &resourceName);
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT QUrl deleteContactGroupUrl(const QString &resourceName, const bool deleteContacts);

    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT ObjectsList parseConnectionsJSONFeed(FeedData &feedData, const QByteArray &jsonFeed, const QString &syncToken = {});
    Q_REQUIRED_RESULT KGAPIPEOPLE_EXPORT ObjectsList parseContactGroupsJSONFeed(FeedData &feedData, const QByteArray &jsonFeed);
}

namespace PeopleUtils
{
    void addValueToJsonObjectIfValid(QJsonObject &object, const QByteArray &key, const int value);
    void addValueToJsonObjectIfValid(QJsonObject &object, const QByteArray &key, const bool value);
    void addValueToJsonObjectIfValid(QJsonObject &object, const QByteArray &key, const QString &value);
    void addValueToJsonObjectIfValid(QJsonObject &object, const QByteArray &key, const QJsonValue &value);
}

}

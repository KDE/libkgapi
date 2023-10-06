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

[[nodiscard]] KGAPIPEOPLE_EXPORT QString allPersonFields();
[[nodiscard]] KGAPIPEOPLE_EXPORT QString allUpdatablePersonFields();
[[nodiscard]] KGAPIPEOPLE_EXPORT QString allContactGroupRecentlyCreatedAvailableFields();

[[nodiscard]] KGAPIPEOPLE_EXPORT QUrl fetchAllContactsUrl(const QString &syncToken = {});
[[nodiscard]] KGAPIPEOPLE_EXPORT QUrl fetchContactUrl(const QString &resourceName);
[[nodiscard]] KGAPIPEOPLE_EXPORT QUrl createContactUrl();
[[nodiscard]] KGAPIPEOPLE_EXPORT QUrl updateContactUrl(const QString &resourceName, const QString &personFields);
[[nodiscard]] KGAPIPEOPLE_EXPORT QUrl deleteContactUrl(const QString &resourceName);
[[nodiscard]] KGAPIPEOPLE_EXPORT QUrl updateContactPhotoUrl(const QString &resourceName);
[[nodiscard]] KGAPIPEOPLE_EXPORT QUrl deleteContactPhotoUrl(const QString &resourceName, const QString &personFields);

[[nodiscard]] KGAPIPEOPLE_EXPORT QUrl fetchAllContactGroupsUrl();
[[nodiscard]] KGAPIPEOPLE_EXPORT QUrl fetchContactGroupUrl(const QString &resourceName);
[[nodiscard]] KGAPIPEOPLE_EXPORT QUrl createContactGroupUrl();
[[nodiscard]] KGAPIPEOPLE_EXPORT QUrl updateContactGroupUrl(const QString &resourceName);
[[nodiscard]] KGAPIPEOPLE_EXPORT QUrl deleteContactGroupUrl(const QString &resourceName, const bool deleteContacts);

[[nodiscard]] KGAPIPEOPLE_EXPORT ObjectsList parseConnectionsJSONFeed(FeedData &feedData, const QByteArray &jsonFeed, const QString &syncToken = {});
[[nodiscard]] KGAPIPEOPLE_EXPORT ObjectsList parseContactGroupsJSONFeed(FeedData &feedData, const QByteArray &jsonFeed);
}

namespace PeopleUtils
{
    void addValueToJsonObjectIfValid(QJsonObject &object, const QByteArray &key, const int value);
    void addValueToJsonObjectIfValid(QJsonObject &object, const QByteArray &key, const bool value);
    void addValueToJsonObjectIfValid(QJsonObject &object, const QByteArray &key, const QString &value);
    void addValueToJsonObjectIfValid(QJsonObject &object, const QByteArray &key, const QJsonValue &value);
}

}

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

    KGAPIPEOPLE_EXPORT ObjectPtr JSONtoPerson(const QByteArray &jsonData);

    //KGAPIPEOPLE_EXPORT People::ContactGroupPtr JSONToContactGroup(const QByteArray &jsonData);

    KGAPIPEOPLE_EXPORT QUrl fetchAllContactsUrl();
    KGAPIPEOPLE_EXPORT QUrl fetchContactUrl(const QString &fetchQuery, const QString &readMask);
    KGAPIPEOPLE_EXPORT QUrl createContactUrl();
    KGAPIPEOPLE_EXPORT QUrl updateContactUrl(const QString &resourceName, const QString &personFields);
    KGAPIPEOPLE_EXPORT QUrl deleteContactUrl(const QString &resourceName);

    KGAPIPEOPLE_EXPORT QUrl fetchAllContactGroupsUrl();
    KGAPIPEOPLE_EXPORT QUrl fetchContactGroupUrl(const QString &resourceName);
    KGAPIPEOPLE_EXPORT QUrl createContactGroupUrl();
    KGAPIPEOPLE_EXPORT QUrl updateContactGroupUrl(const QString &resourceName, const QString &groupFields);
    KGAPIPEOPLE_EXPORT QUrl deleteContactGroupUrl(const QString &resourceName, const bool deleteContacts);

    KGAPIPEOPLE_EXPORT ObjectsList parseConnectionsJSONFeed(FeedData &feedData, const QByteArray &jsonFeed);
    KGAPIPEOPLE_EXPORT ObjectsList parseContactGroupsJSONFeed(FeedData &feedData, const QByteArray &jsonFeed);
}

}

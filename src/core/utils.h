/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <QString>

#include "kgapicore_export.h"
#include "types.h"

namespace Utils
{

/**
 * @brief Converts string content type into enum.
 *
 * @param contentType Content type in form of "text/plain" or "application/xml"
 * @return Corresponding enum member or KGAPI2::UnknownContentType
 *
 * @since 2.0
 */
KGAPICORE_EXPORT KGAPI2::ContentType stringToContentType(const QString &contentType);

/**
 * @brief Converts given boolean value to strings "true" or "false".
 *
 * @param val
 */
KGAPICORE_EXPORT QString bool2Str(bool val);

/**
 * @brief Converts given timestamp into RFC3339 date string
 *
 * @param timestamp
 */
KGAPICORE_EXPORT QString ts2Str(quint64 timestamp);

/**
 * @brief Converts given string in RFC3339 format into QDateTime
 */
KGAPICORE_EXPORT QDateTime rfc3339DateFromString(const QString &string);

/**
 * @brief Converts given date time to RFC3339 format
 */
KGAPICORE_EXPORT QString rfc3339DateToString(const QDateTime &dt);

template<typename Value, template<typename> class Container>
bool compareSharedPtrContainers(const Container<QSharedPointer<Value>> &left, const Container<QSharedPointer<Value>> &right)
{
    if (left.size() != right.size()) {
        return false;
    }
    auto it1 = left.cbegin(), it2 = right.cbegin();
    const auto end1 = left.cend();
    for (; it1 != end1; ++it1, ++it2) {
        if ((*it1 == nullptr) != (*it2 == nullptr)) {
            return false;
        }
        if (!*it1 && !*it2) {
            continue;
        }
        if (**it1 != **it2) {
            return false;
        }
    }
    return true;
}
} // namespace Utils

/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef LIBKGAPI2_UTILS_H
#define LIBKGAPI2_UTILS_H

#include <QString>

#include "types.h"
#include "kgapicore_export.h"

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

} // namespace Utils

#endif // LIBKGAPI2_UTILS_H

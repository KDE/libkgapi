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

#ifndef LIBKGAPI2_ACCOUNTINFOSERVICE_H
#define LIBKGAPI2_ACCOUNTINFOSERVICE_H

#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi_export.h>

namespace KGAPI2
{

/**
 * @brief Additional methods for implementing support for AccountInfo service
 *
 * You should never need to use these methods, unless implementing your own Job
 */
namespace AccountInfoService
{

    /**
     * @brief Parses JSON data into AccountInfo object
     *
     * @param jsonData
     */
    LIBKGAPI_EXPORT AccountInfoPtr JSONToAccountInfo(const QByteArray &jsonData);

    /**
     * @brief Returns supported API version
     */
    LIBKGAPI_EXPORT QString APIVersion();

    /**
     * @brief Returns scope URL for AccountInfo service.
     */
    LIBKGAPI_EXPORT QUrl scopeUrl();

    /**
     * @brief Returns scope URL for AccountInfo service.
     */
    LIBKGAPI_EXPORT QUrl emailScopeUrl();

    /**
     * @brief Returns KGAPI::Request::Fetch URL.
     *
     * https://www.googleapis.com/oauth2/v1/userinfo
     */
    LIBKGAPI_EXPORT QUrl fetchUrl();

} /* namespace AccountInfoService */

} /* namespace KGAPI */

#endif // LIBKGAPI2_ACCOUNTINFOSERVICE_H

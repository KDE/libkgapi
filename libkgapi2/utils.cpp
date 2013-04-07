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

#include "utils.h"

#include <KDE/KDateTime>

KGAPI2::ContentType Utils::stringToContentType(const QString& contentType)
{
    if (contentType.contains(QLatin1String("application/json")) ||
        contentType.contains(QLatin1String("text/plain")) ||
        contentType.contains(QLatin1String("text/javascript"))) {
            return KGAPI2::JSON;
     } else if (contentType.contains(QLatin1String("application/atom+xml")) ||
                contentType.contains(QLatin1String("text/xml"))) {
            return KGAPI2::XML;
    }

    return KGAPI2::UnknownContentType;
}

QString Utils::bool2Str(bool val)
{
    return (val ? QLatin1String("true") : QLatin1String("false"));
}

QString Utils::ts2Str(quint64 ts)
{
    KDateTime dt;
    dt.setTime_t(ts);
    return dt.toString(KDateTime::RFC3339Date);
}

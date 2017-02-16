/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "utils.h"

#include <QDateTime>

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
    return (val ? QStringLiteral("true") : QStringLiteral("false"));
}

QString Utils::ts2Str(quint64 ts)
{
    return QDateTime::fromTime_t(ts).toUTC().toString(Qt::ISODate);
}

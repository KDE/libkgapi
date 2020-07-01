/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    return QDateTime::fromSecsSinceEpoch(ts).toUTC().toString(Qt::ISODate);
}

QDateTime Utils::rfc3339DateFromString(const QString &string)
{
    return QDateTime::fromString(string, Qt::ISODate);
}

QString Utils::rfc3339DateToString(const QDateTime& dt)
{
    return dt.toUTC().toString(Qt::ISODate);
}

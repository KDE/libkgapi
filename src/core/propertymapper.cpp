/*
    SPDX-FileCopyrightText: 2020 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "propertymapper_p.h"
#include "utils.h"

#include <QJsonValue>

using namespace KGAPI2;

template<>
QString KGAPI2::convertTo<QString>(const QJsonValue &val)
{
    return val.toString();
}

template<>
bool KGAPI2::convertTo<bool>(const QJsonValue &val)
{
    return val.toBool();
}

template<>
QDateTime KGAPI2::convertTo<QDateTime>(const QJsonValue &val)
{
    return Utils::rfc3339DateFromString(val.toString());
}


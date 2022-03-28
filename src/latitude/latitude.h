/*
    SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include <QObject>

#include "kgapilatitude_export.h"

namespace KGAPI2
{

/**
 * @brief A Helper QObject
 */
class KGAPILATITUDE_DEPRECATED_EXPORT Latitude : public QObject
{
    Q_OBJECT

public:
    enum Granularity { City, Best };
    Q_ENUM(Granularity)
};

} // namespace KGAPI2

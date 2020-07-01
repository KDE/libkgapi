/*
    SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#ifndef LIBKGAPI2_LATITUDE_H
#define LIBKGAPI2_LATITUDE_H

#include <QObject>

#include "kgapilatitude_export.h"

namespace KGAPI2
{

/**
 * @brief A Helper QObject
 */
class KGAPILATITUDE_DEPRECATED_EXPORT Latitude: public QObject
{
    Q_OBJECT
    Q_ENUMS(Granularity)

  public:
    enum Granularity {
        City, Best
    };
};

} // namespace KGAPI2

#endif

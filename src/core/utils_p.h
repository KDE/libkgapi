/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "debug.h"
#include "utils.h"

#define GAPI_COMPARE(propName)                                                                                                                                 \
    if (d->propName != other.d->propName) {                                                                                                                    \
        qCDebug(KGAPIDebug) << #propName "s don't match";                                                                                                      \
        return false;                                                                                                                                          \
    }

#define GAPI_COMPARE_CONTAINERS(propName)                                                                                                                      \
    if (!Utils::compareSharedPtrContainers(d->propName, other.d->propName)) {                                                                                  \
        qCDebug(KGAPIDebug) << #propName "s don't match";                                                                                                      \
        return false;                                                                                                                                          \
    }

#define GAPI_COMPARE_SHAREDPTRS(propName)                                                                                                                      \
    if (d->propName || other.d->propName) {                                                                                                                    \
        if (((bool)d->propName != (bool)other.d->propName) || (*d->propName != *other.d->propName)) {                                                          \
            qCDebug(KGAPIDebug) << #propName "s don't match";                                                                                                  \
            return false;                                                                                                                                      \
        }                                                                                                                                                      \
    }

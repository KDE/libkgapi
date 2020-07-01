/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TASKSTESTUTILS_H_
#define TASKSTESTUTILS_H_

#include "types.h"

KGAPI2::TaskPtr taskFromFile(const QString &path);
KGAPI2::TaskListPtr taskListFromFile(const QString &path);

#endif


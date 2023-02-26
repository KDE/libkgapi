/*
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "types.h"

namespace KGAPI2 {
namespace People {
namespace TestUtils {

PersonPtr personFromFile(const QString &path);
ContactGroupPtr contactGroupFromFile(const QString &path);
PersonList connectionsFromFile(const QString &path);
ContactGroupList contactGroupsFromFile(const QString &path);
QByteArray rawPhotoDataFromPhotoRequestData(const QString &path);
PersonPtr personFromPhotoUpdateResponseFile(const QString &path);

} // TestUtils
} // People
} // KGAPI2

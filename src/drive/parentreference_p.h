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

#ifndef LIBKGAPI2_DRIVEPARENTREFERENCE_P_H
#define LIBKGAPI2_DRIVEPARENTREFERENCE_P_H

#include "parentreference.h"

#include <QVariantMap>

namespace KGAPI2
{

namespace Drive
{

class Q_DECL_HIDDEN ParentReference::Private
{
  public:
    Private();
    Private(const Private &other);

    QString id;
    QUrl selfLink;
    QUrl parentLink;
    bool isRoot;

    static ParentReferencePtr fromJSON(const QVariantMap &map);
    static QVariantMap toJSON(const ParentReferencePtr &reference);
};

} // namespace Drive

} // namespace KGAPI2

#endif // LIBKGAPI2_DRIVEPARENTREFERENCE_P_H

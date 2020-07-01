/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

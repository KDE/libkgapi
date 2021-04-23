/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "object.h"
#include "kgapiblogger_export.h"

#include <QDateTime>
#include <QVariant>

namespace KGAPI2
{

namespace Blogger
{

class KGAPIBLOGGER_EXPORT Blog : public KGAPI2::Object
{
  public:
    explicit Blog();
    ~Blog();

    QString id() const;
    QString name() const;
    QString description() const;
    QDateTime published() const;
    QDateTime updated() const;
    QUrl url() const;
    uint postsCount() const;
    uint pagesCount() const;
    QString language() const;
    QString country() const;
    QString languageVariant() const;
    QVariant customMetaData() const;

    static BlogPtr fromJSON(const QByteArray &rawData);
    static BlogsList fromJSONFeed(const QByteArray &rawData);

  private:
    Q_DISABLE_COPY(Blog)

    class Private;
    Private * const d;
    friend class Private;
};

} // namespace Blogger
} // namespace KGAPI2


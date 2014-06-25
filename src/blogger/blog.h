/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef KGAPI2_BLOG_H
#define KGAPI2_BLOG_H

#include <libkgapi2/object.h>
#include <libkgapi2/libkgapi2_export.h>

#include <KDateTime>
#include <QVariant>

namespace KGAPI2
{

namespace Blogger
{

class LIBKGAPI2_EXPORT Blog : public KGAPI2::Object
{
  public:
    explicit Blog();
    ~Blog();

    QString id() const;
    QString name() const;
    QString description() const;
    KDateTime published() const;
    KDateTime updated() const;
    QUrl url();
    uint postsCount() const;
    uint pagesCount() const;
    QString langauge() const;
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

#endif // KGAPI2_BLOG_H

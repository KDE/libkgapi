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

#ifndef KGAPI2_BLOGGER_PAGE_H
#define KGAPI2_BLOGGER_PAGE_H

#include <libkgapi2/object.h>
#include <libkgapi2/libkgapi2_export.h>
#include <KDateTime>

namespace KGAPI2
{
namespace Blogger
{

class LIBKGAPI2_EXPORT Page : public KGAPI2::Object
{
  public:
    enum Status {
        UnknownStatus,
        Draft,
        Live,
        Imported
    };
    explicit Page();
    virtual ~Page();

    QString id() const;
    void setId(const QString &id);

    QString blogId() const;
    void setBlogId(const QString &blogId);

    KDateTime published() const;
    void setPublished(const KDateTime &published);

    KDateTime updated() const;
    void setUpdated(KDateTime &updated);

    QUrl url() const;
    void setUrl(const QUrl &url);

    QString title() const;
    void setTitle(const QString &title);

    QString content() const;
    void setContent(const QString &content);

    QString authorId() const;
    QString authorName() const;
    QUrl authorUrl() const;
    QUrl authorImageUrl() const;

    Status status() const;
    void setStatus(Status status);

    static PagePtr fromJSON(const QByteArray &rawData);
    static ObjectsList fromJSONFeed(const QByteArray &rawData);
    static QByteArray toJSON(const PagePtr &page);

  private:
    Q_DISABLE_COPY(Page)

    class Private;
    Private * const d;
    friend class Private;
};
}
}

#endif // KGAPI2_BLOGGER_PAGE_H

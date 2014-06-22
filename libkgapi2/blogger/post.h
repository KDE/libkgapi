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

#ifndef KGAPI2_BLOGGER_POST_H
#define KGAPI2_BLOGGER_POST_H

#include <libkgapi2/object.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QVariant>
#include <QStringList>

#include <KDateTime>

namespace KGAPI2
{
namespace Blogger
{

class LIBKGAPI2_EXPORT Post : public KGAPI2::Object
{
  public:
    Post();
    virtual ~Post();

    QString id();
    void setId(const QString &id);

    QString blogId();
    void setBlogId(const QString &id);

    KDateTime published() const;
    void setPublished(const KDateTime &published);

    KDateTime updated() const;
    void setUpdated(const KDateTime &updated);

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

    uint commentsCount() const;

    QStringList labels() const;
    void setLabels(const QStringList &labels);

    QVariant customMetaData() const;
    void setCustomMetaData(const QVariant &metadata);

    QString location() const;
    void setLocation(const QString &location);

    double latitude() const;
    void setLatitude(double lat);

    double longitude() const;
    void setLongitute(double lng);

    QList<QUrl> images() const;
    void setImages(const QList<QUrl> &images);

    QString status() const;

    static PostPtr fromJSON(const QByteArray &json);
    static ObjectsList fromJSONFeed(const QByteArray &json, FeedData &feedData);
    static QByteArray toJSON(const PostPtr &post);

  private:
    Q_DISABLE_COPY(Post)

    class Private;
    Private * const d;
    friend class Private;
};

} // namespace Blogger
} // namespace KGAPI2

#endif // KGAPI2_BLOGGER_POST_H

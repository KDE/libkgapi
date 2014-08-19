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

#ifndef KGAPI2_BLOGGER_COMMENT_H
#define KGAPI2_BLOGGER_COMMENT_H

#include "object.h"
#include "kgapiblogger_export.h"

#include <QDateTime>

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT Comment : public KGAPI2::Object
{
  public:
    explicit Comment();
    virtual ~Comment();

    QString id() const;
    void setId(const QString &id);

    QString postId() const;
    void setPostId(const QString &postId);

    QString blogId() const;
    void setBlogId(const QString &blogId);

    QDateTime published() const;
    void setPublished(const QDateTime &published);

    QDateTime updated() const;
    void setUpdated(const QDateTime &updated);

    QString content() const;
    void setContent(const QString &content);

    QString authorId() const;
    void setAuthorId(const QString &authorId);

    QString authorName() const;
    void setAuthorName(const QString &authorName);

    QUrl authorUrl() const;
    void setAuthorUrl(const QUrl &url);

    QUrl authorImageUrl() const;
    void setAuthorImageUrl(const QUrl &authorImageUrl);

    QString inReplyTo() const;
    void setInReplyTo(const QString &inReplyTo);

    QString status() const;
    void setStatus(const QString &status);

    static CommentPtr fromJSON(const QByteArray &rawData);
    static ObjectsList fromJSONFeed(const QByteArray &rawData, FeedData &feedData);

  private:
    Q_DISABLE_COPY(Comment)

    class Private;
    Private * const d;
    friend class Private;
};
}
}

#endif // KGAPI2_BLOGGER_COMMENT_H

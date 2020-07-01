/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    ~Comment() override;

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

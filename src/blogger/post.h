/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "object.h"
#include "kgapiblogger_export.h"

#include <QVariant>
#include <QStringList>

#include <QDateTime>

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT Post : public KGAPI2::Object
{
  public:
    Post();
    ~Post() override;

    QString id();
    void setId(const QString &id);

    QString blogId();
    void setBlogId(const QString &id);

    QDateTime published() const;
    void setPublished(const QDateTime &published);

    QDateTime updated() const;
    void setUpdated(const QDateTime &updated);

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


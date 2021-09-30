/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "object.h"
#include "kgapiblogger_export.h"

#include <QDateTime>

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT Page : public KGAPI2::Object
{
  public:
    enum Status {
        UnknownStatus,
        Draft,
        Live,
        Imported
    };
    explicit Page();
    ~Page() override;

    QString id() const;
    void setId(const QString &id);

    QString blogId() const;
    void setBlogId(const QString &blogId);

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


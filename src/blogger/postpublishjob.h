/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_BLOGGER_POSTPUBLISHJOB_H
#define KGAPI2_BLOGGER_POSTPUBLISHJOB_H

#include "job.h"
#include "kgapiblogger_export.h"

#include <QDateTime>

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT PostPublishJob : public KGAPI2::Job
{
    Q_OBJECT
  public:
    enum PublishAction {
      Publish,
      Unpublish
    };

    explicit PostPublishJob(const QString &blogId,
                            const QString &postId,
                            PublishAction action,
                            const AccountPtr &account,
                            QObject *parent = nullptr);
    explicit PostPublishJob(const PostPtr &post,
                            PublishAction action,
                            const AccountPtr &account,
                            QObject *parent = nullptr);
    explicit PostPublishJob(const QString &blogId,
                            const QString &postId,
                            const QDateTime &publishDate,
                            const AccountPtr &account,
                            QObject *parent = nullptr);
    explicit PostPublishJob(const PostPtr &post,
                            const QDateTime &publishDate,
                            const AccountPtr &account,
                            QObject *parent = nullptr);
    ~PostPublishJob() override;

    ObjectPtr item() const;

  protected:
    void start() override;
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;
    void dispatchRequest(QNetworkAccessManager *accessManager,
                         const QNetworkRequest &request,
                         const QByteArray &data,
                         const QString &contentType) override;

  private:
    class Private;
    Private * const d;
    friend class Private;
};
}
}

#endif // KGAPI2_BLOGGER_POSTPUBLISHJOB_H

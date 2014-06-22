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

#ifndef KGAPI2_BLOGGER_POSTPUBLISHJOB_H
#define KGAPI2_BLOGGER_POSTPUBLISHJOB_H

#include <libkgapi2/job.h>
#include <libkgapi2/libkgapi2_export.h>

#include <KDateTime>

namespace KGAPI2
{
namespace Blogger
{

class LIBKGAPI2_EXPORT PostPublishJob : public KGAPI2::Job
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
                            QObject *parent = 0);
    explicit PostPublishJob(const PostPtr &post,
                            PublishAction action,
                            const AccountPtr &account,
                            QObject *parent = 0);
    explicit PostPublishJob(const QString &blogId,
                            const QString &postId,
                            const KDateTime &publishDate,
                            const AccountPtr &account,
                            QObject *parent = 0);
    explicit PostPublishJob(const PostPtr &post,
                            const KDateTime &publishDate,
                            const AccountPtr &account,
                            QObject *parent = 0);
    virtual ~PostPublishJob();

    ObjectPtr item() const;

  protected:
    void start();
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData);
    void dispatchRequest(QNetworkAccessManager *accessManager,
                         const QNetworkRequest &request,
                         const QByteArray &data,
                         const QString &contentType);

  private:
    class Private;
    Private * const d;
    friend class Private;
};
}
}

#endif // KGAPI2_BLOGGER_POSTPUBLISHJOB_H

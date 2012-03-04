/*
    LibKGoogle FetchListJob
    Copyright (C) 2012  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef LIBKGOOGLE_FETCHLISTJOB_H
#define LIBKGOOGLE_FETCHLISTJOB_H

#include <KDE/KJob>

#include <QtCore/QUrl>

#include <libkgoogle/request.h>
#include <libkgoogle/libkgoogle_export.h>

namespace KGoogle
{

class Object;

class FetchListJobPrivate;

/**
 * @short Simple KJob to fetch all items from given service.
 *
 * This job simplyfies fetching of items from remote service. The job will
 * automatically handle multiple replies and will finish when all data chunks
 * are received.
 */
class LIBKGOOGLE_EXPORT FetchListJob: public KJob
{

    Q_OBJECT
  public:
    /**
     * Create a new job
     *
     * @param url Resource URL
     * @param service Name of KGoogle service
     * @param accountName Name of valid account stored in KGoogle::Auth
     */
    FetchListJob(const QUrl &url, const QString &service, const QString &accountName);

    FetchListJob(const FetchListJob &other);

    virtual ~FetchListJob();

    /**
     * Sets resource URL.
     *
     * @param url
     */
    void setUrl(const QUrl &url);

    /**
     * @return Resource URL.
     */
    QUrl url() const;

    /**
     * Sets service name.
     *
     * @param service
     */
    void setService(const QString &service);

    /**
     * @return Name of KGoogle Service.
     */
    QString service() const;

    /**
     * Sets name of account which will authenticate the request.
     *
     * @param accountName
     */
    void setAccountName(const QString &accountName);

    /**
     * @return Name of account.
     */
    QString accountName() const;

    /**
     * @return List of all items received from remote service. The list will
     *     be empty until the job is finished.
     */
    QList< KGoogle::Object* > items() const;

    /**
     * Starts the job.
     *
     * Will throw KGoogle::Exceptions::InvalidAccount or
     * KGoogle::Exceptions::BackendNotReady when the given account name is
     * invalid or the KGoogle::Auth backend is not ready.
     */
    virtual void start();

  private:
    FetchListJobPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(FetchListJob)

};

} /* namespace KGoogle */

#endif // LIBKGOOGLE_FETCHLISTJOB_H

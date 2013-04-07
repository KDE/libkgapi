/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef LIBKGAPI2_JOB_H
#define LIBKGAPI2_JOB_H

#include <QtCore/QObject>

#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

namespace KGAPI2 {

/**
 * @headerfile Job
 * @brief Abstract base class for all jobs in LibKGAPI
 *
 * Usual workflow of Job subclasses is to reimplement Job::start,
 * Job::dispatchRequest and Job::handleReply, then enqueue a QNetworkRequest using
 * Job::enqueueRequest. The request will automatically be scheduled in a queue
 * and dispatched by calling Job::dispatchRequest implementation. When a reply
 * is received, the Job will automatically perform error handling and if there
 * is no error, the reply is passed to implementation of Job::handleReply.
 *
 * Job is automatically when program enters an event loop.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT Job : public QObject
{
    Q_OBJECT

    /**
     * @brief Maximum interval between requests.
     *
     * Some Google APIs have a quota on maximum amount of requests per account
     * per second. When this quota is exceeded, the Job will automatically increase
     * the interval between dispatching requests, wait for a while and then try
     * again. If however the interval is incresed over @p maxTimeout, the job
     * will fail and finish immediatelly. By default @p maxTimeout is @p -1, which
     * allows the interval to be increased indefinitely.
     *
     * @see Job::maxTimeout, Job::setMaxTimeout
     */
    Q_PROPERTY(int maxTimeout READ maxTimeout WRITE setMaxTimeout)

    /**
     * @brief Whether the job is running
     *
     * This property indicates whether the job is running or not. The value is
     * set to @p true when the job is started (see Job::start) and back to 
     * @p false right before Job::finished is emitted.
     *
     * @see Job::isRunning, Job::finished
     */
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY finished)
  public:

    /**
     * @brief Constructor for jobs that don't require authentication
     *
     * @param parent
     */
    explicit Job(QObject* parent = 0);

    /**
     * @brief Constructor for jobs that require authentication
     *
     * @param account Account to use to authenticate the requests send by this job
     * @param parent
     * @see Job::Account, Job::setAccount
     */
    explicit Job(const AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~Job();

    /**
     * @brief Error code
     *
     * This method can only be called after the job has emitted Job::finished
     * signal. Calling this method on a running job will always return
     * KGAPI2::NoError.
     *
     * @return Returns code of ocurred error or KGAPI2::NoError when no error
     *         has ocurred.
     */
    KGAPI2::Error error() const;

    /**
     * @brief Error string
     *
     * This method can only be called after the job has emitted Job::finished
     * signal. Calling this method on a running job will alaways return an empty
     * string.
     *
     * @return Returns localized description of error or an empty string if no
     * error has ocurred.
     */
    QString errorString() const;

    /**
     * @brief Set maximum quota timeout
     *
     * Sets maximum interval for which the job should wait before trying to submit
     * a request that has previously failed due to exceeded quota.
     *
     * Default timeout is 1 seconds, then after every failed request the timeout
     * is increesed exponentially until reaching @p maxTimeout.
     *
     * @param maxTimeout Maximum timeout (in seconds), or @p -1 for no timeout
     */
    void setMaxTimeout(int maxTimeout);

    /**
     * @brief Maximum quota limit.
     *
     * @return Returns maximum timeout in seconds or -1 if there is no timeout set.
     * @see Job::setMaxTimeout
     */
    int maxTimeout() const;

    /**
     * @brief Whether job is running
     *
     * A job is considered running from the moment it's started until
     * until Job::finished is emitted. Some methods should not be
     * called when a job is running.
     *
     * @return Returns whether this job is currently running.
     * @sa start()
     */
    bool isRunning() const;

    /**
     * @brief Set account to be used to authenticate requests
     *
     * By default, no account is set and all request are sent without any
     * authentication.
     *
     * @param account Account to use
     */
    void setAccount(const AccountPtr &account);

    /**
     * @brief Returns account used to authenticate requests
     *
     * For jobs that don't require authentication, this method returns a null
     * pointer.
     *
     * @return Am Account or a null pointer when no account was set.
     */
    AccountPtr account() const;

    /**
     * @brief Restarts this job
     *
     * When a job finishes, it's possible to run it again, without having
     * to create a new job.
     *
     * The job will throw away all results retrieved in previous run and retrieve
     * everything again.
     * 
     * @see Job::aboutToStart
     */
    void restart();

  Q_SIGNALS:

    /**
     * @brief Emitted when @p job has finished
     *
     * The signal is emitted every time, no matter whether the job is successful
     * or an error has ocurred.
     *
     * Sublcasses should never ever emit this signal directly.
     * Use Job::emitFinished instead.
     *
     * @param job The job that has finished
     * @sa emitFinished()
     */
    void finished(KGAPI2::Job *job);

    /**
     * @brief Emitted when a job progress changes.
     *
     * Note that some jobs might not provide progress information, thus this
     * signal will never be emitted.
     *
     * @param job The job that the information relates to
     * @param processed Amount of already processed items
     * @param total Total amount of items to process
     */
    void progress(KGAPI2::Job *job, int processed, int total);

  protected:

    /**
     * @brief Set job error to @p error
     *
     * @param error Error code to set
     * @see Job::error
     */
    void setError(KGAPI2::Error error);

   /**
     * @brief Set job error description to @p errorString
     *
     * @param errorString Error description to set
     * @see Job::errorString
     */
    void setErrorString(const QString &errorString);

    /**
     * @brief Emits Job::finished() signal
     *
     * Subclasses should always use this method instead of directly emitting
     * Job::finished().
     */
    virtual void emitFinished();

    /**
     * @brief This method is invoked right before finished() is emitted
     *
     * Sublcasses can reimplement this method to do final clean up before
     * the Job::finished() signal is emitted.
     *
     * @note Note that after Job::finished() the job is not running anymore and
     *       therefore the job should not modify any data accessible by user.
     */
    virtual void aboutToFinish();

    /**
     * @brief Emit progress() signal
     *
     * Subclasses should always use this method instead of directly emitting
     * Job::progress().
     *
     * @param processed Amount of already processed items
     * @param total Total amount of itms to process
     */
    virtual void emitProgress(int processed, int total);

    /**
     * @brief This method is invoked right before Job::start() is called.
     *
     * Sublcasses should reset their internal state and call parent implementation.
     */
    virtual void aboutToStart();

    /**
     * @brief This method is invoked when job is started.
     *
     * Job is automatically started when application enters event loop.
     */
    virtual void start() = 0;

    /**
     * @brief Dispatches @p request via @p accessManager
     *
     * Because different types of request require different HTTP method to be
     * used, subclasses must reimplement this method and use respective HTTP
     * method to send the @p request via @p accessManager.
     *
     * @param accessManager QNetworkAccessManager used to dispatch the request
     * @param request Request to dispatch
     * @param data Data to send in body of the request
     * @param contentType Content-Type of @p data
     */
    virtual void dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request,
                                 const QByteArray &data, const QString &contentType) = 0;

    /**
     * @brief Called when a reply is received.
     *
     * Sublcasses must reimplement this method to handle reply content.
     *
     * @param reply A reply received from server
     * @param rawData Raw content of the reply. Don't use QNetworkReply::readAll,
     *        because this method has already been called by Job and thus it would
     *        return nothing.
     */
    virtual void handleReply(const QNetworkReply *reply, const QByteArray &rawData) = 0;

    /**
     * @brief Enqueues @p request in dispatcher queue
     *
     * Subclasses should call this method to enqueue the @p request in main job
     * queue. The request is automatically dispatched, and reply is handled.
     *
     * @param request Request to enqueue
     * @param data Data to be send in body of the request
     * @param contentType Content type of @p data
     */
    virtual void enqueueRequest(const QNetworkRequest &request, const QByteArray &data = QByteArray(),
                              const QString &contentType = QString());

  private:
    class Private;
    Private * const d;
    friend class Private;

    friend class AuthJob;

    Q_PRIVATE_SLOT(d, void _k_doStart())
    Q_PRIVATE_SLOT(d, void _k_doEmitFinished())
    Q_PRIVATE_SLOT(d, void _k_replyReceived(QNetworkReply *reply))
    Q_PRIVATE_SLOT(d, void _k_dispatchTimeout())

};

} // namespace KGAPI2

#endif // LIBKGAPI2_JOB_H

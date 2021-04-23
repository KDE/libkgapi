/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "types.h"
#include "kgapicore_export.h"

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

namespace KGAPI2 {

/**
 * @headerfile job.h
 * @brief Abstract base class for all jobs in LibKGAPI
 *
 * Usual workflow of Job subclasses is to reimplement Job::start,
 * Job::dispatchRequest and Job::handleReply, then enqueue a QNetworkRequest using
 * Job::enqueueRequest. Authorization headers and standard query parameters will be
 * set by Job class. The request will automatically be scheduled in a queue and
 * dispatched by calling Job::dispatchRequest implementation. When a reply is received,
 * the Job will automatically perform error handling and if there is no error, the
 * reply is passed to implementation of Job::handleReply.
 *
 * Job is automatically started when program enters an event loop.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICORE_EXPORT Job : public QObject
{
    Q_OBJECT

    /**
     * @brief Maximum interval between requests.
     *
     * Some Google APIs have a quota on maximum amount of requests per account
     * per second. When this quota is exceeded, the Job will automatically increase
     * the interval between dispatching requests, wait for a while and then try
     * again. If however the interval is increased over @p maxTimeout, the job
     * will fail and finish immediately. By default @p maxTimeout is @p -1, which
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
    explicit Job(QObject* parent = nullptr);

    /**
     * @brief Constructor for jobs that require authentication
     *
     * @param account Account to use to authenticate the requests send by this job
     * @param parent
     * @see Job::Account, Job::setAccount
     */
    explicit Job(const AccountPtr &account, QObject* parent = nullptr);

    struct KGAPICORE_EXPORT StandardParams {
        static const QString PrettyPrint;
        static const QString Fields;
    };

    /**
     * @brief Destructor
     */
    ~Job() override;

    /**
     * @brief Error code
     *
     * This method can only be called after the job has emitted Job::finished
     * signal. Calling this method on a running job will always return
     * KGAPI2::NoError.
     *
     * @return Returns code of occurred error or KGAPI2::NoError when no error
     *         has occurred.
     */
    KGAPI2::Error error() const;

    /**
     * @brief Error string
     *
     * This method can only be called after the job has emitted Job::finished
     * signal. Calling this method on a running job will always return an empty
     * string.
     *
     * @return Returns localized description of error or an empty string if no
     * error has occurred.
     */
    QString errorString() const;

    /**
     * @brief Set maximum quota timeout
     *
     * Sets maximum interval for which the job should wait before trying to submit
     * a request that has previously failed due to exceeded quota.
     *
     * Default timeout is 1 seconds, then after every failed request the timeout
     * is increased exponentially until reaching @p maxTimeout.
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
     * @brief Sets whether response will have indentations and line breaks.
     *
     * When this is false, it can reduce the response payload size,
     * which might lead to better performance in some environments.
     * Default is false.
     *
     * @param prettyPrint
     */
    void setPrettyPrint(bool prettyPrint);

    /**
     * @brief Returns prettyPrint query parameter.
     *
     * @return prettyPrint query parameter
     */
    bool prettyPrint() const;

    /**
     * @brief Set subset of fields to include in the response.
     *
     * Use for better performance.
     *
     * @param fields List of fields
     */
    void setFields(const QStringList &fields);

    static QString buildSubfields(const QString &field, const QStringList &fields);

    /**
     * @brief Returns fields selector.
     *
     * @return List of fields
     */
    QStringList fields() const;

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
     * or an error has occurred.
     *
     * Subclasses should never ever emit this signal directly.
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
     * Subclasses can reimplement this method to do a final cleanup before
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
     * @param total Total amount of items to process
     */
    virtual void emitProgress(int processed, int total);

    /**
     * @brief This method is invoked right before Job::start() is called.
     *
     * Subclasses should reset their internal state and call parent implementation.
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
     * @param data Data to sent in the body of the request
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
     * Authorization headers and standards query parameters will be applied.
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
};

} // namespace KGAPI2


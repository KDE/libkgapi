/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2020 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_DRIVEFILEABSTRACTRESUMALEJOB_H
#define KGAPI2_DRIVEFILEABSTRACTRESUMALEJOB_H

#include "file.h"
#include "account.h"
#include "fileabstractdatajob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

/**
 * @headerfile fileabstractresumablejob.h
 *
 * @brief Abstract superclass for KGAPI2::Drive::File create or modify jobs
 * that use chunked uploading of the file's content.
 *
 * The Job starts an upload session and data is written in chunks. When the
 * Job is out of chunks to write it will emit the @p readyWrite() signal and
 * expect new data to be written via @p write().
 *
 * Writing 0 bytes will indicate that the File has been completely transferred
 * and the Job will close the upload session.
 *
 * @see <a href="https://developers.google.com/drive/api/v2/manage-uploads#resumable">Perform a resumable upload</a>
 * @see readyWrite, write
 *
 * @author David Barchiesi <david@barchie.si>
 * @since 20.12
 */
class KGAPIDRIVE_EXPORT FileAbstractResumableJob : public KGAPI2::Drive::FileAbstractDataJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will upload an Untitled file in the
     * users root folder.
     *
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit FileAbstractResumableJob(const AccountPtr &account,
                                   QObject *parent = nullptr);

    /**
     * @brief Constructs a job that will upload a file with its metadata.
     *
     * @param metadata File metadata to upload
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit FileAbstractResumableJob(const FilePtr &metadata,
                                   const AccountPtr &account,
                                   QObject *parent = nullptr);

    /**
     * @brief Constructs a job that will upload an Untitled file in the
     * users root folder with data contained in device.
     *
     * @param device Open device to read from
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit FileAbstractResumableJob(QIODevice *device,
                                   const AccountPtr &account,
                                   QObject *parent = nullptr);

    /**
     * @brief Constructs a job that will upload a file with its metadata
     * with data contained in device.
     *
     * @param device Open device to read from
     * @param metadata File metadata to upload
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit FileAbstractResumableJob(QIODevice *device,
                                   const FilePtr &metadata,
                                   const AccountPtr &account,
                                   QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~FileAbstractResumableJob() override;

    /**
     * @brief Returns metadata supplied at Job creation or retrieved on
     * Job completion.
     */
    FilePtr metadata() const;

    /**
     * @brief Sets the total upload size and is required for progress reporting
     * via the Job::progress() signal.
     */
    void setUploadSize(int size);

    /**
     * @brief This function writes all the bytes in \p data to the upload session.
     *
     * The write operation splits written data in chunks that will be
     * subsequently uploaded in the session.
     *
     * Writing an empty \p data wil signal the Job that it can complete as no
     * more data will be written.
     *
     * When more data is required the readyWrite() signal is emitted.
     *
     * @see readyWrite
     *
     * @param data the data to write
     */
    void write(const QByteArray &data);

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

    /**
     * @brief KGAPI2::Job::handleReply implementation
     *
     * @param reply
     * @param rawData
     */
    void handleReply(const QNetworkReply *reply,
                             const QByteArray &rawData) override;

    /**
     * @brief KGAPI2::Job::dispatchRequest implementation
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    void dispatchRequest(QNetworkAccessManager *accessManager,
                                 const QNetworkRequest &request,
                                 const QByteArray &data,
                                 const QString &contentType) override;

    /**
     * @brief Emit readyWrite() signal
     */
    void emitReadyWrite();

    /**
     * @brief Generates url that will be used during upload session start.
     */
    virtual QUrl createUrl() = 0;

  Q_SIGNALS:

    /**
     * @brief Emitted when @p job requires more data to proceed. Bytes should
     * be written via write(). When connecting to this signal always use a
     * synchronous connection type like Qt::DirectConnection.
     *
     * Subclasses should never ever emit this signal directly.
     *
     * @param job The job that requires data
     * @sa readyWrite()
     *
     * @note Connecting via Qt::QueuedConnection or any other asynchronous connection
     * type will potentially crash the job or end it prematurely. Use Qt::DirectConnection
     *
     * @see write
     */
    void readyWrite(KGAPI2::Drive::FileAbstractResumableJob *job);

  private:
    class Private;
    QScopedPointer<Private> d;
    friend class Private;

    Q_PRIVATE_SLOT(d, void _k_uploadProgress(qint64 uploadedBytes, qint64 totalBytes))
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILEABSTRACTRESUMALEJOB_H

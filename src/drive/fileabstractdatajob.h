/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_DRIVEFILEABSTRACTDATAJOB_H
#define KGAPI2_DRIVEFILEABSTRACTDATAJOB_H

#include "createjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT FileAbstractDataJob : public KGAPI2::Job
{

    Q_OBJECT

    /**
     * Whether to convert this file to the corresponding Google Docs format.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool convert
               READ convert
               WRITE setConvert)

    /**
     * Whether to attempt OCR on .jpg, .png, .gif, or .pdf uploads.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool ocr
               READ ocr
               WRITE setOcr)

    /**
     * If ocr is true, hints at the language to use. Valid values are
     * ISO 639-1 codes.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(QString  ocrLanguage
               READ ocrLanguage
               WRITE setOcrLanguage)

    /**
     * Whether to pin the head revision of the new file.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool pinned
               READ pinned
               WRITE setPinned)

    /**
     * The language of timed text,
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(QString timedTextLanguage
               READ timedTextLanguage
               WRITE setTimedTextLanguage)

    /**
     * The timed text track name.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(QString timedTextTrackName
               READ timedTextTrackName
               WRITE setTimedTextTrackName)

    /**
     *  Sets whether the request supports both My Drives and shared drives.
     *
     * Set to true by default as LibKGAPI supports Team Drives.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool supportsAllDrives
               READ supportsAllDrives
               WRITE setSupportsAllDrives)

  public:
    explicit FileAbstractDataJob(const AccountPtr &account,
                                 QObject *parent = nullptr);
    ~FileAbstractDataJob() override;

    bool convert() const;
    void setConvert(bool convert);

    bool ocr() const;
    void setOcr(bool ocr);

    QString ocrLanguage() const;
    void setOcrLanguage(const QString &ocrLanguage);

    bool pinned() const;
    void setPinned(bool pinned);

    QString timedTextLanguage() const;
    void setTimedTextLanguage(const QString &timedTextLanguage);

    QString timedTextTrackName() const;
    void setTimedTextTrackName(const QString &timedTextTrackName);

    /**
     * @brief Whether the request supports both My Drives and shared drives.
     *
     * Set to true by default as LibKGAPI supports Team Drives.
     *
     * @deprecated This parameter will only be effective until June 1, 2020. Afterwards all applications
     * are assumed to support shared drives.
     */
    KGAPIDRIVE_DEPRECATED bool supportsAllDrives() const;

    /**
     * @brief Sets whether the request supports both My Drives and shared drives.
     *
     * Set to true by default as LibKGAPI supports Team Drives.
     *
     * @deprecated This parameter will only be effective until June 1, 2020. Afterwards all applications
     * are assumed to support shared drives.
     */
    KGAPIDRIVE_DEPRECATED void setSupportsAllDrives(bool supportsAllDrives);

  protected:
    QUrl updateUrl(QUrl &url);

  private:
    class Private;
    Private *const d;
    friend class Private;

};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILEABSTRACTDATAJOB_H

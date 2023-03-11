/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

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
    Q_PROPERTY(bool convert READ convert WRITE setConvert)

    /**
     * Whether to opt in to API behavior that aims for all items to have exactly
     * one parent. This parameter only takes effect if the item is not in a shared
     * drive. Requests that specify more than one parent fail.
     *
     * If the item's owner makes a request to add a single parent, the item is removed
     * from all current folders and placed in the requested folder. Other requests
     * that increase the number of parents fail, except when the canAddMyDriveParent
     * file capability is true and a single parent is being added.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool enforceSingleParent READ enforceSingleParent WRITE setEnforceSingleParent)

    /**
     * Specifies which additional view's permissions to include in the response.
     * Only 'published' is supported.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(QString includePermissionsForView READ includePermissionsForView WRITE setIncludePermissionsForView)

    /**
     * Whether to attempt OCR on .jpg, .png, .gif, or .pdf uploads.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool ocr READ ocr WRITE setOcr)

    /**
     * If ocr is true, hints at the language to use. Valid values are
     * ISO 639-1 codes.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(QString ocrLanguage READ ocrLanguage WRITE setOcrLanguage)

    /**
     * Whether to pin the head revision of the new file.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool pinned READ pinned WRITE setPinned)

    /**
     *  Sets whether the request supports both My Drives and shared drives.
     *
     * Set to true by default as LibKGAPI supports Team Drives.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool supportsAllDrives READ supportsAllDrives WRITE setSupportsAllDrives)

    /**
     * The language of timed text,
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(QString timedTextLanguage READ timedTextLanguage WRITE setTimedTextLanguage)

    /**
     * The timed text track name.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(QString timedTextTrackName READ timedTextTrackName WRITE setTimedTextTrackName)

    /**
     * Whether to use the content as indexable text.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool useContentAsIndexableText READ useContentAsIndexableText WRITE setUseContentAsIndexableText)

public:
    explicit FileAbstractDataJob(const AccountPtr &account, QObject *parent = nullptr);
    ~FileAbstractDataJob() override;

    Q_REQUIRED_RESULT bool convert() const;
    void setConvert(bool convert);

    Q_REQUIRED_RESULT bool enforceSingleParent() const;
    void setEnforceSingleParent(bool enforceSingleParent);

    Q_REQUIRED_RESULT QString includePermissionsForView() const;
    void setIncludePermissionsForView(const QString &includePermissionsForView);

    Q_REQUIRED_RESULT bool ocr() const;
    void setOcr(bool ocr);

    Q_REQUIRED_RESULT QString ocrLanguage() const;
    void setOcrLanguage(const QString &ocrLanguage);

    Q_REQUIRED_RESULT bool pinned() const;
    void setPinned(bool pinned);

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

    Q_REQUIRED_RESULT QString timedTextLanguage() const;
    void setTimedTextLanguage(const QString &timedTextLanguage);

    Q_REQUIRED_RESULT QString timedTextTrackName() const;
    void setTimedTextTrackName(const QString &timedTextTrackName);

    Q_REQUIRED_RESULT bool useContentAsIndexableText() const;
    void setUseContentAsIndexableText(bool useContentAsIndexableText);

protected:
    QUrl updateUrl(QUrl &url);

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

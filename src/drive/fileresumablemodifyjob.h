/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2020 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fileabstractresumablejob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT FileResumableModifyJob : public KGAPI2::Drive::FileAbstractResumableJob
{
    Q_OBJECT

    /**
     * Whether a blob upload should create a new revision. If set to false,
     * the blob data in the current head revision is replaced. If true, a new
     * blob is created as head revision, and previous revisions are preserved
     * (causing increased use of the user's data storage quota).
     *
     * Default value is true.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool createNewRevision READ createNewRevision WRITE setCreateNewRevision)

    /**
     * Whether to set the modified date with the supplied modified date.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool updateModifiedDate READ updateModifiedDate WRITE setUpdateModifiedDate)

    /**
     * Whether to update the view date after successfully updating the file.
     *
     * Default value is true.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool updateViewedDate READ updateViewedDate WRITE setUpdateViewedDate)

public:
    explicit FileResumableModifyJob(const FilePtr &metadata, const AccountPtr &account, QObject *parent = nullptr);

    explicit FileResumableModifyJob(const QString &fileId, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileResumableModifyJob(QIODevice *device, const FilePtr &metadata, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileResumableModifyJob(QIODevice *device, const QString &fileId, const AccountPtr &account, QObject *parent = nullptr);
    ~FileResumableModifyJob() override;

    bool createNewRevision() const;
    void setCreateNewRevision(bool createNewRevision);

    bool updateModifiedDate() const;
    void setUpdateModifiedDate(bool updateModifiedDate);

    bool updateViewedDate() const;
    void setUpdateViewedDate(bool updateViewedDate);

protected:
    QUrl createUrl() override;

private:
    class Private;
    QScopedPointer<Private> d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

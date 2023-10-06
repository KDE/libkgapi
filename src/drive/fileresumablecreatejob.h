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

class KGAPIDRIVE_EXPORT FileResumableCreateJob : public KGAPI2::Drive::FileAbstractResumableJob
{
    Q_OBJECT

public:
    explicit FileResumableCreateJob(const AccountPtr &account, QObject *parent = nullptr);
    explicit FileResumableCreateJob(const FilePtr &metadata, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileResumableCreateJob(QIODevice *device, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileResumableCreateJob(QIODevice *device, const FilePtr &metadata, const AccountPtr &account, QObject *parent = nullptr);
    ~FileResumableCreateJob() override;

protected:
    [[nodiscard]] QUrl createUrl() override;

private:
    class Private;
    QScopedPointer<Private> d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

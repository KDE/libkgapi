/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KGAPI2_DRIVEFILEFETCHJOB_H
#define KGAPI2_DRIVEFILEFETCHJOB_H

#include "fetchjob.h"
#include "kgapidrive_export.h"

#include <QStringList>

namespace KGAPI2
{

namespace Drive
{

class FileSearchQuery;
class KGAPIDRIVE_EXPORT FileFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    /**
     * Whether to update the view date after successfully retrieving files.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool updateViewedDate
               READ updateViewedDate
               WRITE setUpdateViewedDate)

  public:
    enum Fields {
        AllFields                     = 0ULL,
        Id                            = 1ULL << 0,
        Title                         = 1ULL << 1,
        MimeType                      = 1ULL << 2,
        Description                   = 1ULL << 3,
        Labels                        = 1ULL << 4,
        CreatedDate                   = 1ULL << 5,
        ModifiedDate                  = 1ULL << 6,
        ModifiedByMeDate              = 1ULL << 7,
        DownloadUrl                   = 1ULL << 8,
        IndexableText                 = 1ULL << 9,
        UserPermission                = 1ULL << 10,
        FileExtension                 = 1ULL << 11,
        MD5Checksum                   = 1ULL << 12,
        FileSize                      = 1ULL << 13,
        AlternateLink                 = 1ULL << 14,
        EmbedLink                     = 1ULL << 15,
        SharedWithMeDate              = 1ULL << 16,
        Parents                       = 1ULL << 17,
        ExportLinks                   = 1ULL << 18,
        OriginalFilename              = 1ULL << 19,
        OwnerNames                    = 1ULL << 20,
        LastModifiedByMeDate          = 1ULL << 21,
        Editable                      = 1ULL << 22,
        WritersCanShare               = 1ULL << 23,
        ThumbnailLink                 = 1ULL << 24,
        LastViewedByMeDate            = 1ULL << 25,
        WebContentLink                = 1ULL << 26,
        ExplicitlyTrashed             = 1ULL << 27,
        ImageMediaMetadata            = 1ULL << 28,
        Thumbnail                     = 1ULL << 29,
        WebViewLink                   = 1ULL << 30,
        IconLink                      = 1ULL << 31,
        Shared                        = 1ULL << 32,
        Owners                        = 1ULL << 33,
        LastModifyingUser             = 1ULL << 34,
        AppDataContents               = 1ULL << 35,
        OpenWithLinks                 = 1ULL << 36,
        DefaultOpenWithLink           = 1ULL << 37,
        HeadRevisionId                = 1ULL << 38,
        Copyable                      = 1ULL << 39,
        Properties                    = 1ULL << 40,
        MarkedViewedByMeDate          = 1ULL << 41,
        Version                       = 1ULL << 42,
        SharingUser                   = 1ULL << 43,
        Permissions                   = 1ULL << 44,

        BasicFields        = Id | Title | MimeType | CreatedDate | ModifiedDate | FileSize | DownloadUrl | Permissions,
        AccessFields       = CreatedDate | ModifiedDate | ModifiedByMeDate | LastModifiedByMeDate | LastViewedByMeDate | MarkedViewedByMeDate,
        SharingFields      = SharedWithMeDate | WritersCanShare | Shared | Owners | SharingUser | OwnerNames
        // TODO: More?
    };

    explicit FileFetchJob(const QString &fileId,
                          const AccountPtr &account, QObject *parent = nullptr);
    explicit FileFetchJob(const QStringList &filesIds,
                          const AccountPtr &account, QObject *parent = nullptr);
    explicit FileFetchJob(const FileSearchQuery &query,
                          const AccountPtr &account, QObject *parent = nullptr);
    explicit FileFetchJob(const AccountPtr &account, QObject *parent = nullptr);
    ~FileFetchJob() override;

    bool updateViewedDate() const;
    void setUpdateViewedDate(bool updateViewedDate);

    void setFields(qulonglong fields);
    qulonglong fields() const;

  protected:
    void start() override;
    KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
            const QByteArray &rawData) override;

  private:
    class Private;
    Private *const d;
    friend class Private;

};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILEFETCHJOB_H

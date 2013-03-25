/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBKGAPI2_DRIVECHILDREFERENCE_H
#define LIBKGAPI2_DRIVECHILDREFERENCE_H

#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QString>
#include <QtCore/QVariantMap>

#include <KDE/KDateTime>

namespace KGAPI2
{

/**
 * @brief DriveRevision contains a revision of a file.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/revisions">Revisions</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class LIBKGAPI2_EXPORT DriveRevision
{

  public:
    explicit DriveRevision();
    explicit DriveRevision(const QVariantMap &jsonData);
    explicit DriveRevision(const DriveRevision &other);
    virtual ~DriveRevision();

    /**
     * @brief Returns the id of the revision.
     */
    QString id();

    /**
     * @brief Sets the id of the revision.
     *
     * @param id
     */
    void setId(const QString &id);

    /**
     * @brief Returns a link back to this revision.
     */
    QString selfLink();

    /**
     * @brief Sets a link back to this revision.
     *
     * @param selfLink
     */
    void setSelfLink(const QString &selfLink);

    /**
     * @brief Returns the MIME type of the revision.
     */
    QString mimeType();

    /**
     * @brief Sets the MIME type of the revision.
     *
     * @param mimeType
     */
    void setMimeType(const QString &mimeType);

    /**
     * @brief Returns the last time this revision was modified.
     */
    KDateTime modifiedDate();

    /**
     * @brief Sets the last time this revision was modified.
     *
     * @param modified
     */
    void setModifiedDate(const KDateTime &modifiedDate);

    /**
     * @brief Returns whether this revision is pinned to prevent automatic purging.
     *
     * This will only be populated and can only be modified on files with content
     * stored in Drive which are not Google Docs.
     *
     * Revisions can also be pinned when they are created through the
     * drive.files.insert/update/copy by using the pinned query parameter.
     */
    bool pinned();

    /**
     * @brief Sets whether this revision is pinned to prevent automatic purging.
     *
     * This will only be populated and can only be modified on files with content
     * stored in Drive which are not Google Docs.
     *
     * Revisions can also be pinned when they are created through the
     * drive.files.insert/update/copy by using the pinned query parameter.
     *
     * @param pinned
     */
    void setPinned(bool pinned);

    /**
     * @brief Returns whether this revision is published.
     *
     * This is only populated and can only be modified for Google Docs.
     */
    bool published();

    /**
     * @brief Sets whether this revision is published.
     *
     * This is only populated and can only be modified for Google Docs.
     */
    void setPublished(bool published);

    /**
     * @brief Returns a link to the published revision.
     */
    QString publishedLink();

    /**
     * @brief Sets a link to the published revision.
     *
     * @param publishedLink
     */
    void setPublishedLink(const QString &publishedLink);

    /**
     * @brief Returns whether subsequent revisions will be automatically republished.
     *
     * This is only populated and can only be modified for Google Docs.
     */
    bool publishedAuto();

    /**
     * @brief Sets whether subsequent revisions will be automatically republished.
     *
     * This is only populated and can only be modified for Google Docs.
     *
     * @param publishedAuto
     */
    void setPublishedAuto(bool publishedAuto);

    /**
     * @brief Returns whether this revision is published outside the domain.
     *
     * This is only populated and can only be modified for Google Docs.
     */
    bool publishedOutsideDomain();

    /**
     * @brief Sets whether this revision is published outside the domain.
     *
     * This is only populated and can only be modified for Google Docs.
     *
     * @param publishedOutsideDomain
     */
    void setPublishedOutsideDomain(bool publishedOutsideDomain);

    /**
     * @brief Returns a short term download URL for the file.
     *
     * This will only be populated on files with content stored in Drive.
     */
    QString downloadUrl();

    /**
     * @brief Sets a short term download URL for the file.
     *
     * This will only be populated on files with content stored in Drive.
     *
     * @param downloadUrl
     */
    void setDownloadUrl(const QString &downloadUrl);

    /**
     * @brief Returns the links for exporting Google Docs to specific formats.
     *
     * This is a map from the export format to URL.
     */
    QMap<QString /* format */, QString /* url */> exportLinks();

    /**
     * @brief Sets the links for exporting Google Docs to specific formats.
     *
     * This is a map from the export format to URL.
     *
     * @param exportLinks
     */
    void setExportLinks(const QMap<QString /* format */, QString /* url */> &exportLinks);

    /**
     * @brief Returns the name of the last user to modify this revision.
     */
    QString lastModifyingUserName();

    /**
     * @brief Sets the name of the last user to modify this revision.
     *
     * @paraam lastModifyingUserName
     */
    void setLastModifyingUserName(const QString &lastModifyingUserName);

    /**
     * @brief Returns the original filename when this revision was created.
     *
     * This will only be populated on files with content stored in Drive.
     */
    QString originalFilename();

    /**
     * @brief Sets the original filename when this revision was created.
     *
     * This will only be populated on files with content stored in Drive.
     *
     * @param originalFilename
     */
    void setOriginalFilename(const QString &originalFilename);

    /**
     * @brief Returns an MD5 checksum for the content of this revision.
     *
     * This will only be populated on files with content stored in Drive
     */
    QString md5Checksum();

    /**
     * @brief Sets an MD5 checksum for the content of this revision.
     *
     * This will only be populated on files with content stored in Drive
     *
     * @param md5Checksum
     */
    void setMd5Checksum(const QString &md5Checksum);

    /**
     * @brief Returns the size of the revision in bytes.
     *
     * This will only be populated on files with content stored in Drive.
     */
    qlonglong fileSize();

    /**
     * @brief Sets the size of the revision in bytes.
     *
     * This will only be populated on files with content stored in Drive.
     *
     * @param fileSize
     */
    void setFileSize(qlonglong fileSize);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVECHILDREFERENCE_H

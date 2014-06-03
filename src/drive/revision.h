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

#ifndef LIBKGAPI2_DRIVEREVISION_H
#define LIBKGAPI2_DRIVEREVISION_H

#include <libkgapi2/types.h>
#include <libkgapi2/object.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QVariantMap>

#include <KDE/KDateTime>

namespace KGAPI2
{

namespace Drive
{

/**
 * @brief Revision contains a revision of a file.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/revisions">Revisions</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class LIBKGAPI2_EXPORT Revision: public KGAPI2::Object
{

  public:
    explicit Revision();
    explicit Revision(const Revision &other);
    virtual ~Revision();

    /**
     * @brief Returns the id of the revision.
     */
    QString id() const;

    /**
     * @brief Returns a link back to this revision.
     */
    QUrl selfLink() const;

    /**
     * @brief Returns the MIME type of the revision.
     */
    QString mimeType() const;

    /**
     * @brief Returns the last time this revision was modified.
     */
    KDateTime modifiedDate() const;

    /**
     * @brief Returns whether this revision is pinned to prevent automatic purging.
     *
     * This will only be populated and can only be modified on files with content
     * stored in Drive which are not Google Docs.
     *
     * Revisions can also be pinned when they are created through the
     * drive.files.insert/update/copy by using the pinned query parameter.
     */
    bool pinned() const;

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
    bool published() const;

    /**
     * @brief Sets whether this revision is published.
     *
     * @param published
     */
    void setPublished(bool published);

    /**
     * @brief Returns a link to the published revision.
     */
    QUrl publishedLink() const;

    /**
     * @brief Returns whether subsequent revisions will be automatically republished.
     *
     * This is only populated and can only be modified for Google Docs.
     */
    bool publishAuto() const;

    /**
     * @brief Sets whether subsequent revisions will be automatically republished.
     *
     * This is only populated and can only be modified for Google Docs.
     *
     * @param publishedAuto
     */
    void setPublishAuto(bool publishAuto);

    /**
     * @brief Returns whether this revision is published outside the domain.
     *
     * This is only populated and can only be modified for Google Docs.
     */
    bool publishedOutsideDomain() const;

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
    QUrl downloadUrl() const;

    /**
     * @brief Returns the links for exporting Google Docs to specific formats.
     *
     * This is a map from the export format to URL.
     */
    QMap < QString /* format */, QUrl /* url */ > exportLinks() const;

    /**
     * @brief Returns the name of the last user to modify this revision.
     */
    QString lastModifyingUserName() const;

    /**
     * @brief Returns object representing the last user to modify this revision
     */
    UserPtr lastModifyingUser() const;

    /**
     * @brief Returns the original filename when this revision was created.
     *
     * This will only be populated on files with content stored in Drive.
     */
    QString originalFilename() const;

    /**
     * @brief Returns an MD5 checksum for the content of this revision.
     *
     * This will only be populated on files with content stored in Drive
     */
    QString md5Checksum() const;

    /**
     * @brief Returns the size of the revision in bytes.
     *
     * This will only be populated on files with content stored in Drive.
     */
    qlonglong fileSize() const;

    static RevisionPtr fromJSON(const QByteArray &jsonData);
    static RevisionsList fromJSONFeed(const QByteArray &jsonData);
    static QByteArray toJSON(const RevisionPtr &revision);

  private:
    class Private;
    Private *const d;
    friend class Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVEREVISION_H

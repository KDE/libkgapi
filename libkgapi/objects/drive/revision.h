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

#ifndef LIBKGAPI_OBJECTS_DRIVECHILDREFERENCE_H
#define LIBKGAPI_OBJECTS_DRIVECHILDREFERENCE_H

#include <libkgapi/object.h>
#include <libkgapi/libkgapi_export.h>

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

#include <KDateTime>

namespace KGAPI
{

namespace Objects
{

class DriveRevisionPrivate;

/**
 * DriveRevision contains a revision of a file.
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/revisions">Revisions</a>
 */
class LIBKGAPI_EXPORT DriveRevision: public KGAPI::Object
{
public:
    typedef QSharedPointer< DriveRevision > Ptr;
    typedef QList< DriveRevision > List;
    
    DriveRevision();
    DriveRevision(const DriveRevision& other);
    virtual ~DriveRevision();
    
    /**
     * Returns the id of the revision.
     */
    QString id();
    
    /**
     * Sets the id of the revision.
     */
    void setId(const QString& id);
    
    /**
     * Returns a link back to this revision.
     */
    QString selfLink();
    
    /**
     * Sets a link back to this revision.
     */
    void setSelfLink(const QString& selfLink);
    
    /**
     * Returns the MIME type of the revision.
     */
    QString mimeType();
    
    /**
     * Sets the MIME type of the revision.
     */
    void setMimeType(const QString& mimeType);
    
    /**
     * Returns the last time this revision was modified.
     */
    KDateTime modifiedDate();
    
    /**
     * Sets the last time this revision was modified.
     */
    void setModifiedDate(const KDateTime& modifiedDate);
    
    /**
     * Returns whether this revision is pinned to prevent automatic purging.
     * This will only be populated and can only be modified on files with content stored in Drive which are not Google Docs.
     * Revisions can also be pinned when they are created through the drive.files.insert/update/copy by using the pinned query parameter.
     */
    QVariant pinned();
    
    /**
     * Sets whether this revision is pinned to prevent automatic purging.
     * This will only be populated and can only be modified on files with content stored in Drive which are not Google Docs.
     * Revisions can also be pinned when they are created through the drive.files.insert/update/copy by using the pinned query parameter.
     */
    void setPinned(const QVariant& pinned);
    
    /**
     * Returns whether this revision is published. This is only populated and can only be modified for Google Docs.
     */
    QVariant published();
    
    /**
     * Sets whether this revision is published. This is only populated and can only be modified for Google Docs.
     */
    void setPublished(const QVariant& published);
    
    /**
     * Returns a link to the published revision.
     */
    QString publishedLink();
    
    /**
     * Sets a link to the published revision.
     */
    void setPublishedLink(const QString& publishedLink);
    
    /**
     * Returns whether subsequent revisions will be automatically republished.
     * This is only populated and can only be modified for Google Docs.
     */
    QVariant publishedAuto();
    
    /**
     * Sets whether subsequent revisions will be automatically republished.
     * This is only populated and can only be modified for Google Docs.
     */
    void setPublishedAuto(const QVariant& publishedAuto);
    
    /**
     * Returns whether this revision is published outside the domain.
     * This is only populated and can only be modified for Google Docs.
     */
    QVariant publishedOutsideDomain();
    
    /**
     * Sets whether this revision is published outside the domain.
     * This is only populated and can only be modified for Google Docs.
     */
    void setPublishedOutsideDomain(const QVariant& publishedOutsideDomain);
    
    /**
     * Returns a short term download URL for the file.
     * This will only be populated on files with content stored in Drive.
     */
    QString downloadUrl();
    
    /**
     * Sets a short term download URL for the file.
     * This will only be populated on files with content stored in Drive.
     */
    void setDownloadUrl(const QString& downloadUrl);
    
    /**
     * Returns the links for exporting Google Docs to specific formats.
     * This is a map from the export format to URL.
     */
    QMap<QString, QString> exportLinks();
    
    /**
     * Sets the links for exporting Google Docs to specific formats.
     * This is a map from the export format to URL.
     */
    void setExportLinks(const QMap<QString, QString>& exportLinks);
    
    /**
     * Returns the name of the last user to modify this revision.
     */
    QString lastModifyingUserName();
    
    /**
     * Sets the name of the last user to modify this revision.
     */
    void setLastModifyingUserName(const QString& lastModifyingUserName);
    
    /**
     * Returns the original filename when this revision was created.
     * This will only be populated on files with content stored in Drive.
     */
    QString originalFilename();
    
    /**
     * Sets the original filename when this revision was created.
     * This will only be populated on files with content stored in Drive.
     */
    void setOriginalFilename(const QString& originalFilename);
    
    /**
     * Returns an MD5 checksum for the content of this revision.
     * This will only be populated on files with content stored in Drive
     */
    QString md5Checksum();
    
    /**
     * Sets an MD5 checksum for the content of this revision.
     * This will only be populated on files with content stored in Drive
     */
    void setMd5Checksum(const QString& md5Checksum);
    
    /**
     * Returns the size of the revision in bytes.
     * This will only be populated on files with content stored in Drive.
     */
    qlonglong fileSize();
    
    /**
     * Sets the size of the revision in bytes.
     * This will only be populated on files with content stored in Drive.
     */
    void setFileSize(const qlonglong& fileSize);

private:
    DriveRevisionPrivate* d;
};

} /* namespace Objects */

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Objects::DriveRevision::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveRevision*)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveRevision::List)

#endif // LIBKGAPI_OBJECTS_DRIVECHILDREFERENCE_H

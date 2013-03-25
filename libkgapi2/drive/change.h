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

#ifndef LIBKGAPI2_DRIVE_CHANGE_H
#define LIBKGAPI2_DRIVE_CHANGE_H

#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QString>

namespace KGAPI2
{

/**
 * @brief DriveChange contains the representation of a change to a file
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/changes">Changes</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class LIBKGAPI2_EXPORT DriveChange
{
  public:
    explicit DriveChange();
    explicit DriveChange(const DriveChange& other);
    virtual ~DriveChange();

    /**
     * @brief Returns the id of the change.
     */
    qlonglong id() const;

    /**
     * @brief Sets the id of the change.
     *
     * @param id
     */
    void setId(const qlonglong& id);

    /**
     * @brief Returns the id of the file associated with this change.
     */
    QString fileId() const;

    /**
     * @brief Sets the id of the file associated with this change.
     *
     * @param fileId
     */
    void setFileId(const QString& fileId);

    /**
     * @brief Returns a link back to this change.
     */
    QString selfLink() const;

    /**
     * @brief Sets the link back to this change.
     *
     * @param selfLink
     */
    void setSelfLink(const QString& selfLink);

    /**
     * Returns whether this file has been deleted.
     */
    bool deleted() const;

    /**
     * @brief Sets whether this file has been deleted.
     *
     * @param deleted
     */
    void setDeleted(bool deleted);

    /**
     * @brief Returns the updated state of the file.
     *
     * Present if the file has not been deleted.
     */
    DriveFilePtr file() const;

    /**
     * Sets the updated state of the file.
     * Present if the file has not been deleted.
     *
     * @param file
     */
    void setFile(const DriveFilePtr& file);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVE_CHANGE_H

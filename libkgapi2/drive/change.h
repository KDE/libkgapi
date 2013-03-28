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
#include <libkgapi2/object.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QString>
#include <QtCore/QUrl>

namespace KGAPI2
{

namespace Drive
{

/**
 * @brief Change contains the representation of a change to a file
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/changes">Changes</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class LIBKGAPI2_EXPORT Change: public KGAPI2::Object
{
  public:
    explicit Change();
    explicit Change(const Change& other);
    virtual ~Change();

    /**
     * @brief Returns the id of the change.
     */
    qlonglong id() const;

    /**
     * @brief Returns the id of the file associated with this change.
     */
    QString fileId() const;

    /**
     * @brief Returns a link back to this change.
     */
    QUrl selfLink() const;

    /**
     * Returns whether this file has been deleted.
     */
    bool deleted() const;

    /**
     * @brief Returns the updated state of the file.
     *
     * Present if the file has not been deleted.
     */
    FilePtr file() const;

    static ChangePtr fromJSON(const QByteArray &jsonData);
    static ChangesList fromJSONFeed(const QByteArray &jsonData, FeedData &feedData);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVE_CHANGE_H

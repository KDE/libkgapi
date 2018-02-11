/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBKGAPI2_DRIVE_CHANGE_H
#define LIBKGAPI2_DRIVE_CHANGE_H

#include "types.h"
#include "object.h"
#include "kgapidrive_export.h"

#include <QString>
#include <QUrl>

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
class KGAPIDRIVE_EXPORT Change: public KGAPI2::Object
{
  public:
    explicit Change();
    explicit Change(const Change& other);
    ~Change() override;

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
    friend class File;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVE_CHANGE_H

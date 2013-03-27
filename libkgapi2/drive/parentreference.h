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

#include <libkgapi2/types.h>
#include <libkgapi2/object.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QString>
#include <QtCore/QUrl>

namespace KGAPI2
{

/**
 * @brief DriveParentReference contains a reference to a file's parent.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/parents">Parents</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class LIBKGAPI2_EXPORT DriveParentReference: public KGAPI2::Object
{

  public:
    class Private;

    explicit DriveParentReference(const QString &id);
    explicit DriveParentReference(const DriveParentReference &other);
    virtual ~DriveParentReference();

    /**
     * @brief Returns the id of the parent.
     */
    QString id() const;

    /**
     * @brief Returns a link back to this reference.
     */
    QUrl selfLink() const;

    /**
     * @brief Returns a link to the parent.
     */
    QUrl parentLink() const;

    /**
     * @brief Returns whether or not the parent is the root folder.
     */
    bool isRoot() const;

    static DriveParentReferencePtr fromJSON(const QByteArray &jsonData);
    static DriveParentReferencesList fromJSONFeed(const QByteArray &jsonData);
    static QByteArray toJSON(const DriveParentReferencePtr &reference);

  private:
    Private * const d;
    friend class Private;
};

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVECHILDREFERENCE_H

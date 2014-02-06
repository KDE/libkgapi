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

#ifndef LIBKGAPI2_DRIVEPARENTREFERENCE_H
#define LIBKGAPI2_DRIVEPARENTREFERENCE_H

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
 * @brief ParentReference contains a reference to a file's parent.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/parents">Parents</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class LIBKGAPI2_EXPORT ParentReference: public KGAPI2::Object
{

  public:
    class Private;

    explicit ParentReference(const QString &id);
    explicit ParentReference(const ParentReference &other);
    virtual ~ParentReference();

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

    static ParentReferencePtr fromJSON(const QByteArray &jsonData);
    static ParentReferencesList fromJSONFeed(const QByteArray &jsonData);
    static QByteArray toJSON(const ParentReferencePtr &reference);

  private:
    Private *const d;
    friend class Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVEPARENTREFERENCE_H

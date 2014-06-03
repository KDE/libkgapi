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

#ifndef LIBKGAPI2_DRIVE_CHILDREFERENCE_H
#define LIBKGAPI2_DRIVE_CHILDREFERENCE_H

#include "types.h"
#include "object.h"
#include "kgapidrive_export.h"

#include <QtCore/QString>
#include <QtCore/QUrl>

namespace KGAPI2
{

namespace Drive
{

/**
 * @brief ChildReference contains a reference to a folder's child.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/children">Children</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class KGAPIDRIVE_EXPORT ChildReference: public KGAPI2::Object
{

  public:
    explicit ChildReference(const QString &id);
    explicit ChildReference(const ChildReference &other);
    virtual ~ChildReference();

    /**
     * @brief Returns the id of the child.
     */
    QString id() const;

    /**
     * @brief Returns a link back to this reference.
     */
    QUrl selfLink() const;

    /**
     * @brief Returns a link to the child.
     */
    QUrl childLink() const;

    static ChildReferencePtr fromJSON(const QByteArray &jsonData);
    static ChildReferencesList fromJSONFeed(const QByteArray &jsonData,
                                            FeedData &feedData);
    static QByteArray toJSON(const ChildReferencePtr &reference);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVE_CHILDREFERENCE_H

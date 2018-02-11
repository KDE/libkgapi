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

#ifndef LIBKGAPI2_DRIVEPARENTREFERENCE_H
#define LIBKGAPI2_DRIVEPARENTREFERENCE_H

#include "types.h"
#include "object.h"
#include "file.h"
#include "kgapidrive_export.h"

#include <QString>
#include <QUrl>

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
class KGAPIDRIVE_EXPORT ParentReference: public KGAPI2::Object
{

  public:
    explicit ParentReference(const QString &id);
    explicit ParentReference(const ParentReference &other);
    ~ParentReference() override;

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
    class Private;
    Private *const d;
    friend class Private;
    friend class File;
    friend class File::Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVEPARENTREFERENCE_H

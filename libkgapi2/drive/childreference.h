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

#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QString>

namespace KGAPI2
{

/**
 * @brief DriveChildReference contains a reference to a folder's child.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/children">Children</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class LIBKGAPI2_EXPORT DriveChildReference
{

  public:
    explicit DriveChildReference();
    explicit DriveChildReference(const DriveChildReference &other);
    virtual ~DriveChildReference();

    /**
     * @brief Returns the id of the child.
     */
    QString id();

    /**
     * @brief Sets the id of the child.
     *
     * @param id
     */
    void setId(const QString &id);

    /**
     * @brief Returns a link back to this reference.
     */
    QString selfLink();

    /**
     * @brief Sets a link back to this reference.
     *
     * @param selfLink
     */
    void setSelfLink(const QString &selfLink);

    /**
     * @brief Returns a link to the child.
     */
    QString childLink();

    /**
     * @brief Sets a link to the child.
     *
     * @param childLink
     */
    void setChildLink(const QString &childLink);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVE_CHILDREFERENCE_H

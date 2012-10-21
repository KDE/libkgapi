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
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

namespace KGAPI
{

namespace Objects
{

class DriveParentReferencePrivate;

class LIBKGAPI_EXPORT DriveParentReference: public KGAPI::Object
{
public:
    typedef QSharedPointer< DriveParentReference > Ptr;
    typedef QList< DriveParentReference > List;
    
    DriveParentReference();
    DriveParentReference(const DriveParentReference& other);
    virtual ~DriveParentReference();
    
    /**
     * Returns the id of the parent.
     */
    QString id();
    
    /**
     * Sets the id of the parent.
     */
    void setId(const QString& id);
    
    /**
     * Returns a link back to this reference.
     */
    QString selfLink();
    
    /**
     * Sets the link back to this reference.
     */
    void setSelfLink(const QString& selfLink);
    
    /**
     * Returns a link to the parent.
     */
    QString parentLink();
    
    /**
     * Sets the link to the parent.
     */
    void setParentLink(const QString& parentLink);
    
    /**
     * Returns whether or not the parent is the root folder.
     */
    bool isRoot();
    
    /**
     * Sets whether or not the parent is the root folder.
     */
    void setIsRoot(const bool& isRoot);

private:
    DriveParentReferencePrivate* d;
};

} /* namespace Objects */

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Objects::DriveParentReference::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveParentReference*)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveParentReference::List)

#endif // LIBKGAPI_OBJECTS_DRIVECHILDREFERENCE_H

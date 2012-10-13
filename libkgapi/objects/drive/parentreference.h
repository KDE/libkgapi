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
    
    QString id();
    void setId(const QString& id);
    
    QString selfLink();
    void setSelfLink(const QString& selfLink);
    
    QString parentLink();
    void setParentLink(const QString& parentLink);
    
    bool isRoot();
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

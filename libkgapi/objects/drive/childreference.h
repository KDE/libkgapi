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

class DriveChildReferencePrivate;

class LIBKGAPI_EXPORT DriveChildReference: public KGAPI::Object
{
public:
    typedef QSharedPointer< DriveChildReference > Ptr;
    typedef QList< DriveChildReference > List;
    
    DriveChildReference();
    DriveChildReference(const DriveChildReference& other);
    virtual ~DriveChildReference();
    
    QString id();
    void setId(const QString& id);
    
    QString selfLink();
    void setSelfLink(const QString& selfLink);
    
    QString childLink();
    void setChildLink(const QString& childLink);

private:
    DriveChildReferencePrivate* d;
};

} /* namespace Objects */

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Objects::DriveChildReference::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveChildReference*)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveChildReference::List)

#endif // LIBKGAPI_OBJECTS_DRIVECHILDREFERENCE_H

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

#ifndef LIBKGAPI_OBJECTS_DRIVEAPP_H
#define LIBKGAPI_OBJECTS_DRIVEAPP_H

#include <libkgapi/object.h>
#include <libkgapi/libkgapi_export.h>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

namespace KGAPI
{

namespace Objects
{

class DriveAppPrivate;
class DriveAppPrivate_Icon;

class LIBKGAPI_EXPORT DriveApp: public KGAPI::Object
{
public:
    class Icon
    {
    public:
        Icon();
        Icon(const Icon& other);
        virtual ~Icon();
        
        QString category();
        void setCategory(const QString& category);
        
        int size();
        void setSize(const int& size);
        
        QString iconUrl();
        void setIconUrl(const QString& iconUrl);
    private:
        DriveAppPrivate_Icon* d;
    };
    
    typedef QSharedPointer< DriveApp > Ptr;
    typedef QList< DriveApp > List;
    
    DriveApp();
    DriveApp(const DriveApp& other);
    virtual ~DriveApp();
    
    QString id();
    void setId(const QString& id);
    
    QString name();
    void setName(const QString& name);
    
    QString objectType();
    void setObjectType(const QString& objectType);
    
    bool supportsCreate();
    void setSupportsCreate(const bool& supportsCreate);
    
    bool supportsImport();
    void setSupportsImport(const bool& supportsImport);
    
    bool installed();
    void setInstalled(const bool& installed);
    
    bool authorized();
    void setAuthorized(const bool& authorized);
    
    bool useByDefault();
    void setUseByDefault(const bool& useByDefault);
    
    QString productUrl();
    void setProductUrl(const QString& productUrl);
    
    QStringList primaryMimeTypes();
    void setPrimaryMimeTypes(const QStringList& primaryMimeTypes);
    
    QStringList secondaryMimeTypes();
    void setSecondaryMimeTypes(const QStringList& secondaryMimeTypes);
    
    QStringList primaryFileExtensions();
    void setPrimaryFileExtensions(const QStringList& primaryFileExtensions);
    
    QStringList secondaryFileExtensions();
    void setSecondaryFileExtensions(const QStringList& secondaryFileExtensions);
    
    QList<Icon> icons();
    void setIcons(const QList<Icon>& icons);
    
private:
    DriveAppPrivate* d;
};

} /* namespace Objects */

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Objects::DriveApp::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveApp*)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveApp::List)

#endif // LIBKGAPI_OBJECTS_DRIVEAPP_H

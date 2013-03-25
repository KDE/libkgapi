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

#ifndef LIBKGAPI2_DRIVE_APP_H
#define LIBKGAPI2_DRIVE_APP_H

#include <libkgapi2/object.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

namespace KGAPI2
{

class LIBKGAPI2_EXPORT DriveApp: public KGAPI2::Object
{
  public:

    class Icon
    {
      public:
        explicit Icon();
        explicit Icon(const Icon &other);
        virtual ~Icon();

        QString category();
        void setCategory(const QString &category);

        int size();
        void setSize(const int &size);

        QString iconUrl();
        void setIconUrl(const QString &iconUrl);

      private:
        class Private;
        Private *const d;
        friend class Private;
    };

    explicit DriveApp();
    explicit DriveApp(const DriveApp &other);
    virtual ~DriveApp();

    QString id();
    void setId(const QString &id);

    QString name();
    void setName(const QString &name);

    QString objectType();
    void setObjectType(const QString &objectType);

    bool supportsCreate();
    void setSupportsCreate(bool supportsCreate);

    bool supportsImport();
    void setSupportsImport(bool supportsImport);

    bool installed();
    void setInstalled(bool installed);

    bool authorized();
    void setAuthorized(bool authorized);

    bool useByDefault();
    void setUseByDefault(bool useByDefault);

    QString productUrl();
    void setProductUrl(const QString &productUrl);

    QStringList primaryMimeTypes();
    void setPrimaryMimeTypes(const QStringList &primaryMimeTypes);

    QStringList secondaryMimeTypes();
    void setSecondaryMimeTypes(const QStringList &secondaryMimeTypes);

    QStringList primaryFileExtensions();
    void setPrimaryFileExtensions(const QStringList &primaryFileExtensions);

    QStringList secondaryFileExtensions();
    void setSecondaryFileExtensions(const QStringList &secondaryFileExtensions);

    QList<Icon> icons();
    void setIcons(const QList<Icon> &icons);

  private:
    class Private;
    Private *const d;
    friend class Private;
};

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVE_APP_H

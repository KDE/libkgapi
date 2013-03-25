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

#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

namespace KGAPI2
{

class LIBKGAPI2_EXPORT DriveApp
{
  public:

    class Icon
    {
      public:
        explicit Icon();
        explicit Icon(const Icon &other);
        virtual ~Icon();

        QString category() const;
        void setCategory(const QString &category);

        int size() const;
        void setSize(const int &size);

        QString iconUrl() const;
        void setIconUrl(const QString &iconUrl);

      private:
        class Private;
        Private *const d;
        friend class Private;
    };

    typedef QSharedPointer<Icon> IconPtr;
    typedef QList<IconPtr> IconsList;

    explicit DriveApp();
    explicit DriveApp(const DriveApp &other);
    virtual ~DriveApp();

    QString id() const;
    void setId(const QString &id);

    QString name() const;
    void setName(const QString &name);

    QString objectType() const;
    void setObjectType(const QString &objectType);

    bool supportsCreate() const;
    void setSupportsCreate(bool supportsCreate);

    bool supportsImport() const;
    void setSupportsImport(bool supportsImport);

    bool installed() const;
    void setInstalled(bool installed);

    bool authorized() const;
    void setAuthorized(bool authorized);

    bool useByDefault() const;
    void setUseByDefault(bool useByDefault);

    QString productUrl() const;
    void setProductUrl(const QString &productUrl);

    QStringList primaryMimeTypes() const;
    void setPrimaryMimeTypes(const QStringList &primaryMimeTypes);

    QStringList secondaryMimeTypes() const;
    void setSecondaryMimeTypes(const QStringList &secondaryMimeTypes);

    QStringList primaryFileExtensions() const;
    void setPrimaryFileExtensions(const QStringList &primaryFileExtensions);

    QStringList secondaryFileExtensions() const;
    void setSecondaryFileExtensions(const QStringList &secondaryFileExtensions);

    IconsList icons() const;
    void setIcons(const IconsList &icons);

  private:
    class Private;
    Private *const d;
    friend class Private;
};

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVE_APP_H

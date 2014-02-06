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

#include <libkgapi2/types.h>
#include <libkgapi2/object.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

namespace KGAPI2
{

namespace Drive
{

class LIBKGAPI2_EXPORT App: public KGAPI2::Object
{
    class Private;

  public:

    class Icon
    {
      public:
        enum Category {
            UndefinedCategory,
            ApplicationCategory,
            DocumentCategory,
            DocumentSharedCategory
        };

        explicit Icon();
        explicit Icon(const Icon &other);
        virtual ~Icon();

        Category category() const;
        int size() const;
        QUrl iconUrl() const;

      private:
        class Private;
        Private *const d;
        friend class Private;
        friend class App::Private;
    };

    typedef QSharedPointer<Icon> IconPtr;
    typedef QList<IconPtr> IconsList;

    explicit App();
    explicit App(const App &other);
    virtual ~App();

    QString id() const;
    QString name() const;
    QString objectType() const;
    bool supportsCreate() const;
    bool supportsImport() const;
    bool installed() const;
    bool authorized() const;
    bool useByDefault() const;
    QUrl productUrl() const;
    QStringList primaryMimeTypes() const;
    QStringList secondaryMimeTypes() const;
    QStringList primaryFileExtensions() const;
    QStringList secondaryFileExtensions() const;
    IconsList icons() const;

    static AppsList fromJSONFeed(const QByteArray &jsonData);
    static AppPtr fromJSON(const QByteArray &jsonData);

  private:
    Private *const d;
    friend class Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVE_APP_H

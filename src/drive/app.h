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

#ifndef LIBKGAPI2_DRIVE_APP_H
#define LIBKGAPI2_DRIVE_APP_H

#include "types.h"
#include "object.h"
#include "kgapidrive_export.h"

#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QUrl>

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT App: public KGAPI2::Object
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

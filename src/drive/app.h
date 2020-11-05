/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
        bool operator==(const Icon &other) const;
        bool operator!=(const Icon &other) const { return !operator==(other); }

        Category category() const;
        int size() const;
        QUrl iconUrl() const;

      private:
        class Private;
        Private *const d;
        friend class Private;
        friend class App::Private;
    };

    using IconPtr = QSharedPointer<Icon>;
    using IconsList = QList<IconPtr>;

    explicit App();
    explicit App(const App &other);
    virtual ~App();
    bool operator==(const App &other) const;
    bool operator!=(const App &other) const { return !operator==(other); }

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

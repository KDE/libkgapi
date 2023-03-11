/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "kgapidrive_export.h"
#include "object.h"
#include "types.h"

#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QUrl>

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT App : public KGAPI2::Object
{
    class Private;

public:
    class Icon
    {
    public:
        enum Category { UndefinedCategory, ApplicationCategory, DocumentCategory, DocumentSharedCategory };

        explicit Icon();
        explicit Icon(const Icon &other);
        virtual ~Icon();
        bool operator==(const Icon &other) const;
        bool operator!=(const Icon &other) const
        {
            return !operator==(other);
        }

        Q_REQUIRED_RESULT Category category() const;
        Q_REQUIRED_RESULT int size() const;
        Q_REQUIRED_RESULT QUrl iconUrl() const;

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
    ~App() override;
    bool operator==(const App &other) const;
    bool operator!=(const App &other) const
    {
        return !operator==(other);
    }

    Q_REQUIRED_RESULT QString id() const;
    Q_REQUIRED_RESULT QString name() const;
    Q_REQUIRED_RESULT QString objectType() const;
    Q_REQUIRED_RESULT bool supportsCreate() const;
    Q_REQUIRED_RESULT bool supportsImport() const;
    Q_REQUIRED_RESULT bool installed() const;
    Q_REQUIRED_RESULT bool authorized() const;
    Q_REQUIRED_RESULT bool useByDefault() const;
    Q_REQUIRED_RESULT QUrl productUrl() const;
    Q_REQUIRED_RESULT QStringList primaryMimeTypes() const;
    Q_REQUIRED_RESULT QStringList secondaryMimeTypes() const;
    Q_REQUIRED_RESULT QStringList primaryFileExtensions() const;
    Q_REQUIRED_RESULT QStringList secondaryFileExtensions() const;
    Q_REQUIRED_RESULT IconsList icons() const;

    static AppsList fromJSONFeed(const QByteArray &jsonData);
    static AppPtr fromJSON(const QByteArray &jsonData);

private:
    Private *const d;
    friend class Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

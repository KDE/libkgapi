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

        [[nodiscard]] Category category() const;
        [[nodiscard]] int size() const;
        [[nodiscard]] QUrl iconUrl() const;

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

    [[nodiscard]] QString id() const;
    [[nodiscard]] QString name() const;
    [[nodiscard]] QString objectType() const;
    [[nodiscard]] bool supportsCreate() const;
    [[nodiscard]] bool supportsImport() const;
    [[nodiscard]] bool installed() const;
    [[nodiscard]] bool authorized() const;
    [[nodiscard]] bool useByDefault() const;
    [[nodiscard]] QUrl productUrl() const;
    [[nodiscard]] QStringList primaryMimeTypes() const;
    [[nodiscard]] QStringList secondaryMimeTypes() const;
    [[nodiscard]] QStringList primaryFileExtensions() const;
    [[nodiscard]] QStringList secondaryFileExtensions() const;
    [[nodiscard]] IconsList icons() const;

    static AppsList fromJSONFeed(const QByteArray &jsonData);
    static AppPtr fromJSON(const QByteArray &jsonData);

private:
    Private *const d;
    friend class Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

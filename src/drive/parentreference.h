/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "file.h"
#include "kgapidrive_export.h"
#include "object.h"
#include "types.h"

#include <QString>
#include <QUrl>

namespace KGAPI2
{

namespace Drive
{

/**
 * @brief ParentReference contains a reference to a file's parent.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/parents">Parents</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class KGAPIDRIVE_EXPORT ParentReference : public KGAPI2::Object
{
public:
    explicit ParentReference(const QString &id);
    explicit ParentReference(const ParentReference &other);
    ~ParentReference() override;
    bool operator==(const ParentReference &other) const;
    bool operator!=(const ParentReference &other) const
    {
        return !operator==(other);
    }

    /**
     * @brief Returns the id of the parent.
     */
    [[nodiscard]] QString id() const;

    /**
     * @brief Returns a link back to this reference.
     */
    [[nodiscard]] QUrl selfLink() const;

    /**
     * @brief Returns a link to the parent.
     */
    [[nodiscard]] QUrl parentLink() const;

    /**
     * @brief Returns whether or not the parent is the root folder.
     */
    [[nodiscard]] bool isRoot() const;

    static ParentReferencePtr fromJSON(const QByteArray &jsonData);
    static ParentReferencesList fromJSONFeed(const QByteArray &jsonData);
    static QByteArray toJSON(const ParentReferencePtr &reference);

private:
    class Private;
    Private *const d;
    friend class Private;
    friend class File;
    friend class File::Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

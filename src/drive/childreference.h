/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

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
 * @brief ChildReference contains a reference to a folder's child.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/children">Children</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class KGAPIDRIVE_EXPORT ChildReference : public KGAPI2::Object
{
public:
    explicit ChildReference(const QString &id);
    explicit ChildReference(const ChildReference &other);
    ~ChildReference() override;
    bool operator==(const ChildReference &other) const;
    bool operator!=(const ChildReference &other) const
    {
        return !operator==(other);
    }

    /**
     * @brief Returns the id of the child.
     */
    Q_REQUIRED_RESULT QString id() const;

    /**
     * @brief Returns a link back to this reference.
     */
    Q_REQUIRED_RESULT QUrl selfLink() const;

    /**
     * @brief Returns a link to the child.
     */
    Q_REQUIRED_RESULT QUrl childLink() const;

    static ChildReferencePtr fromJSON(const QByteArray &jsonData);
    static ChildReferencesList fromJSONFeed(const QByteArray &jsonData, FeedData &feedData);
    static QByteArray toJSON(const ChildReferencePtr &reference);

private:
    class Private;
    Private *const d;
    friend class Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

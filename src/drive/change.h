/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef LIBKGAPI2_DRIVE_CHANGE_H
#define LIBKGAPI2_DRIVE_CHANGE_H

#include "types.h"
#include "object.h"
#include "kgapidrive_export.h"

#include <QString>
#include <QUrl>

namespace KGAPI2
{

namespace Drive
{

/**
 * @brief Change contains the representation of a change to a file
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/changes">Changes</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class KGAPIDRIVE_EXPORT Change: public KGAPI2::Object
{
  public:
    explicit Change();
    explicit Change(const Change& other);
    ~Change() override;
    bool operator==(const Change &other) const;
    bool operator!=(const Change &other) const { return !operator==(other); }

    /**
     * @brief Returns the id of the change.
     */
    qlonglong id() const;

    /**
     * @brief Returns the id of the file associated with this change.
     */
    QString fileId() const;

    /**
     * @brief Returns a link back to this change.
     */
    QUrl selfLink() const;

    /**
     * Returns whether this file has been deleted.
     */
    bool deleted() const;

    /**
     * @brief Returns the updated state of the file.
     *
     * Present if the file has not been deleted.
     */
    FilePtr file() const;

    static ChangePtr fromJSON(const QByteArray &jsonData);
    static ChangesList fromJSONFeed(const QByteArray &jsonData, FeedData &feedData);

  private:
    class Private;
    Private * const d;
    friend class Private;
    friend class File;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVE_CHANGE_H

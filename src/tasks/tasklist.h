/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef LIBKGAPI2_TASKLIST_H
#define LIBKGAPI2_TASKLIST_H

#include "object.h"
#include "types.h"
#include "kgapitasks_export.h"

#include <QScopedPointer>

namespace KGAPI2
{

/**
 * @brief Represents a tasklist for Google Tasks service.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since: 0.3
 */
class KGAPITASKS_EXPORT TaskList: public KGAPI2::Object
{
  public:

    /**
     * @brief Constructor
     */
    TaskList();

    /**
     * @brief Copy constructor
     */
    TaskList (const TaskList& other);

    /**
     * @brief Destructor
     */
    ~TaskList() override;

    bool operator==(const TaskList &other) const;

    /**
     * @brief Sets tasklist UID
     *
     * @param uid
     */
    void setUid(const QString &uid);

    /**
     * @brief Returns tasklist UID
     */
    QString uid() const;

    /**
     * @brief Sets tasklist name
     *
     * @param title
     */
    void setTitle(const QString &title);

    /**
     * @brief Returns tasklist title
     */
    QString title() const;

    /**
     * @brief Sets tasklist selfLink
     *
     * @param selfLink
     */
    void setSelfLink(const QString &selfLink);

    /**
     * @brief Returns tasklist selfLink
     */
    QString selfLink() const;

    /**
     * @brief Sets tasklist updated
     *
     * @param updated
     */
    void setUpdated(const QString &updated);

    /**
     * @brief Returns tasklist updated
     */
    QString updated() const;

  private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_TASKLIST_H

/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */


#ifndef LIBKGAPI2_TASKLIST_H
#define LIBKGAPI2_TASKLIST_H

#include <libkgapi2/object.h>
#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2
{

/**
 * @brief Represents a tasklist for Google Tasks service.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since: 0.3
 */
class LIBKGAPI2_EXPORT TaskList: public KGAPI2::Object
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
    virtual ~TaskList();

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

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_TASKLIST_H

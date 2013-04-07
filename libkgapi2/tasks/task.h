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

#ifndef LIBKGAPI2_TASK_H
#define LIBKGAPI2_TASK_H

#include <libkgapi2/object.h>
#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

#include <KDE/KCalCore/Todo>

namespace KGAPI2
{

/**
 * @brief Represents a single task (or todo)
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.3
 */
class LIBKGAPI2_EXPORT Task: public KGAPI2::Object,
                            public KCalCore::Todo
{
  public:

    /**
     * @deprecated Provided for backward compatibility. Will be removed in future.
     */
    KGAPI_DEPRECATED typedef TaskPtr Ptr;

    /**
     * @deprecated Provided for backward compatibility. Will be removed in future.
     */
    KGAPI_DEPRECATED typedef QList<Task> List;

    /**
     * @brief Constructor
     */
    explicit Task();

    /**
     * @brief Copy constructor
     */
    Task(const Task& other);

    /**
     * @brief Copy constructor
     */
    Task(const KCalCore::Todo &other);

    /**
     * @brief Destructor
     */
    virtual ~Task();

    /**
     * @brief Sets whether the task has been deleted
     *
     * @param deleted
     */
    void setDeleted(bool deleted);

    /**
     * @brief Returns whether the task has been deleted
     */
    bool deleted() const;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2/

#endif // LIBKGAPI2_OBJECTS_TASK_H

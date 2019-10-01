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

#ifndef LIBKGAPI2_TASK_H
#define LIBKGAPI2_TASK_H

#include "object.h"
#include "types.h"
#include "kgapitasks_export.h"

#include <QScopedPointer>

#include <KCalendarCore/Todo>

namespace KGAPI2
{

/**
 * @brief Represents a single task (or todo)
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.3
 */
class KGAPITASKS_EXPORT Task: public KGAPI2::Object,
                              public KCalendarCore::Todo
{
  public:

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
    Task(const KCalendarCore::Todo &other);

    /**
     * @brief Destructor
     */
    ~Task() override;

    bool operator==(const Task &other) const;

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
    QScopedPointer<Private> const d;
    friend class Private;

};

} // namespace KGAPI2/

#endif // LIBKGAPI2_OBJECTS_TASK_H

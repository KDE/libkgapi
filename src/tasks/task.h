/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

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
    explicit Task(const KCalendarCore::Todo &other);

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


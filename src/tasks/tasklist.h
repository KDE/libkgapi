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


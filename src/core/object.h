/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <QSharedPointer>
#include <QString>

#include "kgapicore_export.h"
#include "types.h"

namespace KGAPI2
{

/**
 * @brief Base class for all objects
 *
 * There are many container classes to represent data fetched from Google servers,
 * like KGAPI2::Contact, KGAPI2::Event, etc. All these container classes must
 * be subclasses of the KGAPI2::Object.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICORE_EXPORT Object
{
public:
    /**
     * @brief Constructor
     */
    Object();

    /**
     * @brief Copy constructor
     */
    Object(const Object &other);

    /**
     * @brief Destructor
     */
    virtual ~Object();

    bool operator==(const Object &other) const;

    /**
     * @brief Set the etag of this object.
     *
     * Etag represents a revision of an object. When the object is changed on the
     * remote side it is given a new etag.
     *
     * @param etag
     */
    void setEtag(const QString &etag);

    /**
     * @brief Returns etag of this object.
     *
     * @return Etag string
     */
    [[nodiscard]] QString etag() const;

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace KGAPI2

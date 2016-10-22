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
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBKGAPI2_OBJECT_H
#define LIBKGAPI2_OBJECT_H

#include <QString>
#include <QSharedPointer>

#include "types.h"
#include "kgapicore_export.h"

namespace KGAPI2
{

/**
 * @brief Base class for all objects
 *
 * There are many container classes to represent data fetched from Google servers,
 * like KGAPI2::Contact, KGAPI2::Event, etc. All these container classes must
 * be subclasses of KGAPI2::Object.
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

    /**
     * @brief Sets etag of this object.
     *
     * Etag represents a revision of an object When object is changed on the 
     * remote side is given a new etag.
     *
     * @param etag
     */
    void setEtag(const QString &etag);

    /**
     * @brief Returns etag of this object.
     *
     * @return Etag string
     */
    QString etag() const;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_OBJECT_H

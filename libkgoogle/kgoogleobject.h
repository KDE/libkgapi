/*
    libKGoogle - KGoogleObject
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef KGOOGLEOBJECT_H
#define KGOOGLEOBJECT_H

#include <qobject.h>

#include <libkgoogle/libkgoogle_export.h>

namespace KGoogle {

  /**
   * Base class for Google objects.
   * 
   * Google object represents data received
   * from a Google service.
   */
  class LIBKGOOGLE_EXPORT KGoogleObject
  {
    public:
      KGoogleObject();
      virtual ~KGoogleObject();

      /**
       * Sets ID of this object.
       */
      void setId (const QString &id);
      
      /**
       * Returns ID of this object.
       */
      QString id();
      
      /**
       * Sets etag of this object.
       * 
       * Etag represents a revision of an object.
       * When object is changed on the remote side
       * is given a new etag.
       */
      void setEtag (const QString &etag);
      
      /**
       * Returns tag of this object.
       */
      QString etag();
      
    private:
      QString m_id;
      QString m_etag;
  };

} // namespace KGoogle

#endif // KGOOGLEOBJECT_H

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

#include "libkgoogle_export.h"

namespace KGoogle {
  
  class LIBKGOOGLE_EXPORT KGoogleObject
  {
    public:
      KGoogleObject();
      virtual ~KGoogleObject();

      void setId (const QString &id);
      QString id();
      
      void setEtag (const QString &etag);
      QString etag();
      
      
      //virtual bool operator== (const KGoogleObject &other);
      //virtual KGoogleObject & operator= (const KGoogleObject &other);
      
    private:
      QString m_id;
      QString m_etag;
  };

} // namespace KGoogle

#endif // KGOOGLEOBJECT_H

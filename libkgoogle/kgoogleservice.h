/*
    libKGoogle - KGoogleService
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


#ifndef KGOOGLESERVICE_H
#define KGOOGLESERVICE_H

#include "libkgoogle_export.h"

#include <qobject.h>
#include <qvariant.h>
#include <qsharedpointer.h>
#include <qdom.h>
#include <qurl.h>

#include <kdatetime.h>

namespace KGoogle {

  class KGoogleObject;
 
  struct FeedData_ {
    int startIndex;
    int itemsPerPage;
    int totalResults;
    QUrl nextLink;
  };
  
  typedef struct FeedData_ FeedData;
  
  class LIBKGOOGLE_EXPORT KGoogleService
  {

    public:
      virtual ~KGoogleService() { }
      
      virtual KGoogleObject* JSONToObject(const QByteArray& jsonData) = 0;
      virtual QByteArray objectToJSON(KGoogleObject* object) = 0;
      virtual QList< KGoogleObject* > parseJSONFeed(const QByteArray& jsonFeed, FeedData* feedData = 0) = 0;
	
      virtual KGoogleObject* XMLToObject(const QByteArray& xmlData) = 0;
      virtual QByteArray objectToXML(KGoogleObject* object) = 0;
      virtual QList< KGoogleObject* > parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData = 0) = 0;
      
      virtual const QString protocolVersion() = 0;

    };
  
} // namespace KGoogle

#endif // KGOOGLESERVICE_H

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

#include <qobject.h>
#include <qvariant.h>
#include <qsharedpointer.h>
#include <qdom.h>
#include <qurl.h>

#include <kdatetime.h>

#include <libkgoogle/libkgoogle_export.h>

namespace KGoogle {

  class KGoogleObject;
 
  /**
   * Struct to store additional informations
   * about a feed.
   */
  typedef struct FeedData_ {
    int startIndex;	/// Index of first item on current feed page.
    int itemsPerPage;	/// Number of items per feed page. This will be same  for all pages (except for the last one which can be shorter).
    int totalResults;	/// Number of all items.
    QUrl nextLink;	/// Link to next page of feed.
  } FeedData;
  
  /**
   * A base class for all services.
   * 
   * Service is a class that provides additional informations
   * about a Google service and implements parsers for objects
   * from service.
   */
  class LIBKGOOGLE_EXPORT KGoogleService
  {

    public:
      virtual ~KGoogleService() { }
      
      /**
       * Parses raw p0 jsonData to a KGoogleObject.
       */
      virtual KGoogleObject* JSONToObject(const QByteArray& jsonData) = 0;
      
      /**
       * Serializes a KGoogleObject to raw JSON data.
       */
      virtual QByteArray objectToJSON(KGoogleObject* object) = 0;
      
      /**
       * Parses raw JSON feed data into list of KGoogleObject objects.
       */ 
      virtual QList< KGoogleObject* > parseJSONFeed(const QByteArray& jsonFeed, FeedData* feedData = 0) = 0;

      /**
       * Parsers raw p0 xmlData into a KGoogleObject.
       */
      virtual KGoogleObject* XMLToObject(const QByteArray& xmlData) = 0;
      
      /**
       * Serializes a KGoogleObject to raw XML data.
       */
      virtual QByteArray objectToXML(KGoogleObject* object) = 0;
      
      /**
       * Parses raw JSON feed data into list of KGoogleObject objects.
       */
      virtual QList< KGoogleObject* > parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData = 0) = 0;
      
      /**
       * Returns version of the services protocol.
       */
      virtual const QString protocolVersion() = 0;

    };
  
} // namespace KGoogle

#endif // KGOOGLESERVICE_H

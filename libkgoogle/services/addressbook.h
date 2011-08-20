/*
    <one line to give the program's name and a brief idea of what it does.>
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


#ifndef SERVICE_ADDRESSBOOK_H
#define SERVICE_ADDRESSBOOK_H

#include "kgoogleservice.h"

#include "libkgoogle_export.h"

#include <qmetatype.h>

namespace KGoogle {
  
  class KGoogleObject;
 
  namespace Service {

    class LIBKGOOGLE_EXPORT Addressbook: public KGoogleService
    {
      public:
	~Addressbook();
	
	KGoogleObject* JSONToObject(const QByteArray& jsonData);
	QByteArray objectToJSON(KGoogleObject* object);
	QList< KGoogleObject* > parseJSONFeed(const QByteArray& jsonFeed, FeedData* feedData = 0);
	
	KGoogleObject* XMLToObject(const QByteArray& xmlData);
	QByteArray objectToXML(KGoogleObject* object);
	QList< KGoogleObject* > parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData = 0);

	const QString protocolVersion();
	
	static QString scopeUrl();
	static QString createUrl();
	static QString fetchAllUrl();
	static QString fetchUrl();
	static QString updateUrl();
	static QString removeUrl();
	
	static bool supportsJSONRead(QString* urlParam);
	static bool supportsJSONWrite(QString* urlParam);
    };
    
  } // namespace Service
  
} // namespace KGoogle

Q_DECLARE_METATYPE(KGoogle::Service::Addressbook)

#endif // SERVICE_ADDRESSBOOK_H

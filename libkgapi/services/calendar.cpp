/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calendar.h"
#include "debug.h"
#include "accessmanager.h"
#include "objects/calendar.h"
#include "objects/event.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QtCore/QVariant>

#include <libkgapi2/calendar/calendarservice.h>
#include <libkgapi2/calendar/calendar.h>
#include <libkgapi2/calendar/event.h>
#include <libkgapi2/account.h>

using namespace KGAPI;

QUrl Services::Calendar::ScopeUrl(KGAPI2::Account::calendarScopeUrl());

static const QString serviceNameStr(QLatin1String("KGAPI::Services::Calendar"));


QString Services::Calendar::serviceName()
{
    if (QMetaType::type(serviceNameStr.toLatin1().constData()) == 0) {
        qRegisterMetaType< KGAPI::Services::Calendar >(serviceNameStr.toLatin1().constData());
    }

    return serviceNameStr;
}

/********** PUBLIC JSON INTERFACE ************/

KGAPI::Object* Services::Calendar::JSONToObject(const QByteArray& jsonData)
{
    QJson::Parser parser;

    QVariantMap object = parser.parse(jsonData).toMap();

    if ((object.value(QLatin1String("kind")) == QLatin1String("calendar#calendarListEntry")) ||
        (object.value(QLatin1String("kind")) == QLatin1String("calendar#calendar"))) {
        KGAPI2::CalendarPtr calendar = KGAPI2::CalendarService::JSONToCalendar(jsonData);
        return new Objects::Calendar(*reinterpret_cast<Objects::Calendar*>(calendar.data()));
    } else if (object.value(QLatin1String("kind")) == QLatin1String("calendar#event")) {
        KGAPI2::EventPtr event = KGAPI2::CalendarService::JSONToEvent(jsonData);
        return new Objects::Event(*reinterpret_cast<Objects::Event*>(event.data()));
    }

    return 0;
}

QByteArray Services::Calendar::objectToJSON(KGAPI::Object* object)
{
    if (dynamic_cast< const Objects::Calendar* >(object)) {
        KGAPI2::CalendarPtr calendar(new KGAPI2::Calendar(*reinterpret_cast<KGAPI2::Calendar*>(object)));
        return KGAPI2::CalendarService::calendarToJSON(calendar);
    } else if (dynamic_cast< const Objects::Event* >(object)) {
        KGAPI2::EventPtr event(new KGAPI2::Event(*reinterpret_cast<KGAPI2::Event*>(object)));
        return KGAPI2::CalendarService::eventToJSON(event);
    }

    return QByteArray();
}

QList< KGAPI::Object* > Services::Calendar::parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QJson::Parser parser;

    QVariantMap data = parser.parse(jsonFeed).toMap();

    bool isCalendar = true;
    KGAPI2::ObjectsList objects;

    if (data.value(QLatin1String("kind")) == QLatin1String("calendar#calendarList")) {
        objects = KGAPI2::CalendarService::parseCalendarJSONFeed(jsonFeed, feedData);
        isCalendar = true;

    } else if (data.value(QLatin1String("kind")) == QLatin1String("calendar#events")) {
        objects = KGAPI2::CalendarService::parseEventJSONFeed(jsonFeed, feedData);
        isCalendar = false;
    } else {
        return QList< KGAPI::Object* >();
    }

    QList< KGAPI::Object* > list;
    Q_FOREACH(const KGAPI2::ObjectPtr &object, objects) {
        if (isCalendar) {
            KGAPI2::CalendarPtr calendar = object.dynamicCast<KGAPI2::Calendar>();
            list << new Objects::Calendar(*reinterpret_cast<Objects::Calendar*>(calendar.data()));
        } else {
            KGAPI2::EventPtr event = object.dynamicCast<KGAPI2::Event>();
            list << new Objects::Event(*reinterpret_cast<Objects::Event*>(event.data()));
        }
    }
    return list;
}


/************* PUBLIC XML INTERFACE ***********/

KGAPI::Object* Services::Calendar::XMLToObject(const QByteArray& xmlData)
{
    Q_UNUSED(xmlData);

    return 0;
}

QByteArray Services::Calendar::objectToXML(KGAPI::Object* object)
{
    Q_UNUSED(object);

    return QByteArray();
}

QList< KGAPI::Object* > Services::Calendar::parseXMLFeed(const QByteArray& xmlFeed, FeedData& feedData)
{
    Q_UNUSED(xmlFeed);
    Q_UNUSED(feedData);

    return QList< KGAPI::Object* >();
}


/************* URLS **************/

QUrl Services::Calendar::scopeUrl() const
{
    return KGAPI2::Account::calendarScopeUrl();
}

QUrl Services::Calendar::fetchCalendarsUrl()
{
    return KGAPI2::CalendarService::fetchCalendarsUrl();
}

QUrl Services::Calendar::fetchCalendarUrl(const QString& calendarID)
{
    return KGAPI2::CalendarService::fetchCalendarUrl(calendarID);
}

QUrl Services::Calendar::updateCalendarUrl(const QString &calendarID)
{
    return KGAPI2::CalendarService::updateCalendarUrl(calendarID);
}

QUrl Services::Calendar::createCalendarUrl()
{
    return KGAPI2::CalendarService::createCalendarUrl();
}

QUrl Services::Calendar::removeCalendarUrl(const QString& calendarID)
{
    return KGAPI2::CalendarService::removeCalendarUrl(calendarID);
}

QUrl Services::Calendar::fetchEventsUrl(const QString& calendarID)
{
    return KGAPI2::CalendarService::fetchEventsUrl(calendarID);
}

QUrl Services::Calendar::fetchEventUrl(const QString& calendarID, const QString& eventID)
{
    return KGAPI2::CalendarService::fetchEventUrl(calendarID, eventID);
}

QUrl Services::Calendar::updateEventUrl(const QString& calendarID, const QString& eventID)
{
    return KGAPI2::CalendarService::updateEventUrl(calendarID, eventID);
}

QUrl Services::Calendar::createEventUrl(const QString& calendarID)
{
    return KGAPI2::CalendarService::createEventUrl(calendarID);
}

QUrl Services::Calendar::removeEventUrl(const QString& calendarID, const QString& eventID)
{
    return KGAPI2::CalendarService::removeEventUrl(calendarID, eventID);
}

QUrl Services::Calendar::moveEventUrl(const QString& sourceCalendar, const QString& destCalendar, const QString& eventID)
{
    return KGAPI2::CalendarService::moveEventUrl(sourceCalendar, destCalendar, eventID);
}

QString Services::Calendar::protocolVersion() const
{
    return KGAPI2::CalendarService::APIVersion();
}

bool Services::Calendar::supportsJSONRead(QString* urlParam)
{
    return true;

    Q_UNUSED(urlParam)
}

bool Services::Calendar::supportsJSONWrite(QString* urlParam)
{
    return true;

    Q_UNUSED(urlParam)
}

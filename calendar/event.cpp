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


#include <QtCore/QString>
#include <QtCore/QVariantMap>
#include <QtCore/QVariantList>
#include <QtCore/QDebug>

#include <KDE/KCalCore/Event>
#include <KDE/KCalCore/Attendee>
#include <KDE/KCalCore/Alarm>
#include <KDE/KCalCore/Recurrence>
#include <KDE/KCalCore/RecurrenceRule>
#include <KDE/KCalCore/ICalFormat>

#include "event.h"
#include "event_p.h"

Event::EventPrivate::EventPrivate(const EventPrivate &other):
    QSharedData(other),
    deleted(other.deleted),
    id(other.id),
    etag(other.etag)
{ }



Event::Event::Event()
{
    d = new EventPrivate;
}

Event::Event::Event(const Event &other):
  KCalCore::Event(other),
  d(other.d)
{ }

Event::Event::Event(const KCalCore::Event *event): 
  KCalCore::Event(*event),
  d(new EventPrivate)
{ }


Event::Event::~Event()
{ }

void Event::Event::setDeleted(const bool deleted)
{
  d->deleted = deleted;
}

bool Event::Event::deleted()
{
  return d->deleted;
}

void Event::Event::setId(const QString& id)
{
  d->id = id;
}

QString Event::Event::id()
{
  return d->id;
}

void Event::Event::setETag(const QString& etag)
{
  d->etag = etag;
}

QString Event::Event::etag()
{
  return d->etag;
}

void Event::Event::fromJSON(QVariantMap jsonData)
{
  /* ID */
  setUid(jsonData["id"].toString());
  
  /* ETAG */
  setETag(jsonData["etag"].toString());
  
  /* Deleted? */
  setDeleted(jsonData["deleted"].toBool());

  /* Can edit? */
  setReadOnly(!jsonData["canEdit"].toBool());
  
  /* Summary */
  setSummary(jsonData["title"].toString());
  
  /* Description */
  setDescription(jsonData["details"].toString());
  
  /* Created */
  setCreated(KDateTime::fromString(jsonData["created"].toString(), "%Y-%m-%dT%H:%M:%S%:SZ"));
  
  /* Last updated */
  setLastModified(KDateTime::fromString(jsonData["updated"].toString(), "%Y-%m-%dT%H:%M:%S%:SZ"));
  
  /* Status */
  if (jsonData["status"].toString() == "confirmed")
    setStatus(KCalCore::Incidence::StatusConfirmed);
  else if (jsonData["status"].toString() == "canceled")
    setStatus(KCalCore::Incidence::StatusCanceled);
  else if (jsonData["status"].toString() == "tentative")
    setStatus(KCalCore::Incidence::StatusTentative);
  else
    setStatus(KCalCore::Incidence::StatusNone);
  
  /* Transparency */
  if (jsonData["transparency"].toString() == "transparent")
    setTransparency(KCalCore::Event::Transparent);
  else /* Assume opaque as default transparency */
    setTransparency(KCalCore::Event::Opaque);
  
  /* Location */
  setLocation(jsonData["location"].toString());
  
  /* Attendees */
  QVariantList attendees = jsonData["attendees"].toList();
  foreach (const QVariant &a, attendees) {
    QVariantMap att = a.toMap();
    KCalCore::Attendee *attendee = new KCalCore::Attendee(att["displayName"].toString(),
							  att["email"].toString());
    if (att["status"].toString() == "accepted")
      attendee->setStatus(KCalCore::Attendee::Accepted);
    else if (att["status"].toString() == "invited")
      attendee->setStatus(KCalCore::Attendee::None);
    else if (att["status"].toString() == "declined")
      attendee->setStatus(KCalCore::Attendee::Declined);
    else if (att["status"].toString() == "tentative")
      attendee->setStatus(KCalCore::Attendee::Tentative);
    else
      attendee->setStatus(KCalCore::Attendee::None);

    if (att["type"] == "required")
      attendee->setRole(KCalCore::Attendee::ReqParticipant);
    else if (att["type"] == "optional")
      attendee->setRole(KCalCore::Attendee::OptParticipant);
    /* No further roles supported by Google */
    
    /* Google sends organizer as one of attendees with rel="organizer" attribute */
    if (att["rel"] == "organizer") {
      KCalCore::Person *person = new KCalCore::Person(att["displayName"].toString(),
						      att["email"].toString());
      setOrganizer((KCalCore::Person::Ptr) person);
    }
    
    addAttendee((KCalCore::Attendee::Ptr)attendee, true);
  }
  
  /* Start, End, all-day event */
  QVariantList whens = jsonData["when"].toList();
  foreach (const QVariant &w, whens) {
    QVariantMap when = w.toMap();
    KDateTime dtStart, dtEnd;
    /* When dtStart and dtEnd are only dates without times, than assume all-day event */
    if ((when["start"].toString().length() == 10) &&
       (when["end"].toString().length() == 10)) {
      setAllDay(true);
      dtStart = KDateTime::fromString(when["start"].toString(), "%Y-%m-%d");
      dtEnd = KDateTime::fromString(when["end"].toString(), "%Y-%m-%d");
      dtEnd = dtEnd.addDays(-1); /* Google says one-day all-day event = 2011-05-20 to 2011-05-21, 
			          * KDE says one-day all-day event = 2011-05-20 to 2011-05-20 */
    } else {
      dtStart = KDateTime::fromString(when["start"].toString(), KDateTime::RFC3339Date);
      dtEnd = KDateTime::fromString(when["end"].toString(), KDateTime::RFC3339Date);
      
      if (!dtStart.isLocalZone())
	dtStart = dtStart.toLocalZone();
      
      if (!dtEnd.isLocalZone())
	dtEnd = dtEnd.toLocalZone();
    }
    
    setDtStart(dtStart);
    setDtEnd(dtEnd);
  }
  
  /* Recurrence */
  QStringList recrs = jsonData["recurrence"].toString().split("\r\n");
  foreach (const QString &rec, recrs) {
    if (rec.left(7) == "DTSTART") {
      /* Sometimes the date is YYYYMMDDTHHMMSS and sometimes there Z at the end of the date */
      int start = rec.lastIndexOf(":")+1;
      setDtStart(KDateTime::fromString(rec.mid(start, 15), "%Y%m%dT%H%M%S"));
    } else  if (rec.left(5) == "DTEND") {
      int start = rec.lastIndexOf(":")+1;
      setDtEnd(KDateTime::fromString(rec.mid(start, 15), "%Y%m%dT%H%M%S"));
      /* Minus one day: Google says that all-day event is from 2011-05-20 to 2011-05-21, but
       * KDE says that all-day event is from 2011-05-20 to 2011-05-20. */
      setDtEnd(dtEnd().addDays(-1));
    } else if (rec.left(5) == "RRULE") {
      KCalCore::ICalFormat format;
      KCalCore::RecurrenceRule *recurrenceRule = recurrence()->defaultRRule(true);
      format.fromString(recurrenceRule, rec.mid(6));
    } else if (rec == "BEGIN:VTIMEZONE") {
      /* TODO: Implement time-zone dependent recurrences. For now take only the "standard"
       * and leave when it comes to any other */
      break;
    }
  }
  
  /* Reminders - the recurrence and/or start/end must be set BEFORE alarms 
   * because alarms must already know dtStart! */
  QVariantList reminders = jsonData["reminders"].toList();
  foreach (const QVariant &r, reminders) {
    QVariantMap reminder = r.toMap();
    KCalCore::Alarm *alarm = new KCalCore::Alarm(this);
    alarm->setTime(dtStart());
    
    if (reminder["method"].toString() == "alert")
      alarm->setType(KCalCore::Alarm::Display);
    else if (reminder["method"].toString() == "email")
      alarm->setType(KCalCore::Alarm::Email);
    else {
      delete alarm;
      continue;
    }
    
    if (!reminder["minutes"].toString().isEmpty()) {
      /* Start offset is in seconds and must be negative, so that the alarm trigger times
       * is BEFORE the event start. When alarm should trigger AFTER the event start, 
       * it is specified as "absoluteTime" */
      alarm->setStartOffset(KCalCore::Duration(reminder["minutes"].toInt()*(-60)));
    } else if (!reminder["absoluteTime"].toString().isEmpty()) {
      alarm->setTime(KDateTime::fromString(reminder["absoluteTime"].toString(), "%Y-%m-%dT%H:%M:%S"));
    } else {
      delete alarm;
      continue;
    }
    alarm->setEnabled(true);
    addAlarm((KCalCore::Alarm::Ptr) alarm);
  }
  
  /* TODO: Implement support for additional features:
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   */
}

QVariantMap Event::Event::toJSON()
{
 QVariantMap output;
  
  /* ID */
  output["id"] = uid();
  
  /* Deleted? */
  output["deleted"] = deleted();
  
  /* Can edit? */
  output["canEdit"] = isReadOnly();
   
  /* Summary */
  output["title"] = summary();
  
  /* Description */
  output["details"] = description();
  
  /* Created */
  output["created"] = created().toString(KDateTime::RFC3339Date);
  
  /* Last updated */
  output["updated"] = lastModified().toString(KDateTime::RFC3339Date);
  
  /* Status */
  if (status() == KCalCore::Incidence::StatusConfirmed)
    output["status"] = "confirmed";
  else if (status() == KCalCore::Incidence::StatusCanceled)
    output["status"] = "canceled";
  else if (status() == KCalCore::Incidence::StatusTentative)
    output["status"] = "tentative";

  /* Transparency */
  if (transparency() == KCalCore::Event::Transparent)
    output["transparency"] = "transparent";
  else
    output["transparency"] = "opaque";
  
  /* Location */
  output["location"] = location();
  
  /* Attendees */
  QVariantList atts;
  foreach (const KCalCore::Attendee::Ptr attee, attendees()) {
    QVariantMap att;
    
    att["displayName"] = attee->name();
    att["email"] = attee->email();
    
    if (attee->status() == KCalCore::Attendee::Accepted)
      att["status"] = "accepted";
    else if (attee->status() == KCalCore::Attendee::None)
      att["status"] = "invited";
    else if (attee->status() == KCalCore::Attendee::Declined)
      att["status"] = "declined";
    else if (attee->status() == KCalCore::Attendee::Tentative)
      att["status"] == "tentative";
    
    if (attee->role() == KCalCore::Attendee::ReqParticipant)
      att["type"] = "required";
    else if (attee->role() == KCalCore::Attendee::OptParticipant)
      att["type"] = "optional";
    /* No further roles supported by Google */

    atts.append(att);
  }
  
  /* Organizer. Google expects organizator as one of attendees. */
  QVariantMap org;
  org["displayName"] = organizer()->name();
  org["email"] = organizer()->email();
  org["rel"] = "organizer";
  atts.append(org);
  
  output["attendees"] = atts;

  /* Start, end */
  QVariantList whens;
  QVariantMap when;
  if (allDay()) {
    when["start"] = dtStart().toString("%Y-%m-%d");
    when["end"] = dtEnd().addDays(1).toString("%Y-%m-%d");
  } else {
    when["start"] = dtStart().toString(KDateTime::RFC3339Date);
    when["end"] = dtEnd().toString(KDateTime::RFC3339Date);
  }

  /* Reminders (subpart of "WHEN") */
  QVariantList als;
  foreach (KCalCore::Alarm::Ptr a, alarms()) {
    QVariantMap alarm;
    if (a->type() == KCalCore::Alarm::Display)
      alarm["method"] = "alert";
    else if (a->type() == KCalCore::Alarm::Email)
      alarm["method"] = "email";
    
    if (a->startOffset().asSeconds() < 0)
      alarm["minutes"] = a->startOffset().asSeconds()/(-60);
    else
      alarm["absoluteTime"] = a->time().toString(KDateTime::RFCDate);
    
    als.append(alarm);
  }
  when["reminders"] = als;
  whens.append(when);
  output["when"] = whens;
  
  /* Recurrence */
  KCalCore::ICalFormat format;
  QString recStr;
  QString start, end;
  if (allDay()) {
    start = "VALUE=DATE:" + dtStart().toUtc().toString("%Y%m%d");
    end = "VALUE=DATE:" + dtEnd().toUtc().toString("%Y%m%d");
  } else {
    start = "TZID=" + dtStart().timeZone().name() + ":" + dtStart().toLocalZone().toString("%Y%m%dT%H%M%S");
    end = "TZID=" + dtEnd().timeZone().name() + ":" + dtEnd().toLocalZone().toString("%Y%m%dT%H%M%S");
  }
  recStr = "DTSTART;" + start + "\r\n" +
	   "DTEND;" + end;
  foreach (KCalCore::RecurrenceRule *rrule, recurrence()->rRules())
    recStr += "\r\n" + format.toString(rrule);
  if (recurrence()->rRules().length() > 0) {
    output["recurrence"] = recStr;
    output["when"].toMap().remove("start"); /* Can't have 'recurrence' and 'when' together */
    output["when"].toMap().remove("end"); /* Can't have 'recurrence' and 'when' together */
  }
  
  
  /* TODO: Implement support for additional features:
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   */
  
  return output;
}










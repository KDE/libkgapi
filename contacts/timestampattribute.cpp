/*
    Akonadi Google - Contacts Resource
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

#include "timestampattribute.h"

void TimeStampAttribute::deserialize(const QByteArray& data)
{
  const QString timestamp = QString::fromUtf8(data);
  m_timeStamp = KDateTime::fromString(timestamp);
}

QByteArray TimeStampAttribute::serialized() const
{
  return m_timeStamp.toString().toUtf8();
}

Akonadi::Attribute* TimeStampAttribute::clone() const
{
  TimeStampAttribute *attr = new TimeStampAttribute;
  attr->setTimeStamp(m_timeStamp);
  return attr;
}

QByteArray TimeStampAttribute::type() const
{
  return "GOOGLE_TIMESTAMP";
}

void TimeStampAttribute::setTimeStamp(const KDateTime& dateTime)
{
  m_timeStamp = dateTime;
}

KDateTime TimeStampAttribute::timeStamp() const
{
  return m_timeStamp;
}


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


#ifndef TIMESTAMPATTRIBUTE_H
#define TIMESTAMPATTRIBUTE_H

#include <QtCore/QByteArray>

#include <KDE/KDateTime>
#include <Akonadi/Attribute>

class TimeStampAttribute : public Akonadi::Attribute
{

  public:
    virtual void deserialize(const QByteArray& data);
    virtual QByteArray serialized() const;
    virtual Akonadi::Attribute* clone() const;
    virtual QByteArray type() const;
    void setTimeStamp (const KDateTime &dateTime);
    KDateTime timeStamp() const;
  private:
    KDateTime m_timeStamp;
};

#endif // TIMESTAMPATTRIBUTE_H

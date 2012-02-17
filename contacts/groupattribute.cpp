/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Dan Vratil <dan@progdan.cz>

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


#include "groupattribute.h"

GroupAttribute::GroupAttribute()
{ }

GroupAttribute::GroupAttribute(const QStringList &groups):
  m_groups(groups)
{ }

GroupAttribute::~GroupAttribute()
{ }

void GroupAttribute::deserialize(const QByteArray &data)
{
  m_groups = QString(data).split(",");
}

QByteArray GroupAttribute::serialized() const
{
  return m_groups.join(",").toLatin1();
}

Akonadi::Attribute *GroupAttribute::clone() const
{
  return new GroupAttribute(m_groups);
}

QByteArray GroupAttribute::type() const
{
  return "group";
}

bool GroupAttribute::containsGroup(const QString &groupId) const
{
  return m_groups.contains(groupId);
}

const QStringList &GroupAttribute::groups() const
{
  return m_groups;
}

void GroupAttribute::setGroups(const QStringList &groups)
{
  m_groups = groups;
}




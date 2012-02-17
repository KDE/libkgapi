/*
    Akonadi Google - Contacts Resource - Group Attribute
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


#ifndef GROUPATTRIBUTE_H
#define GROUPATTRIBUTE_H

#include <akonadi/attribute.h>

#include <qstringlist.h>

class GroupAttribute : public Akonadi::Attribute
{
  public:
    explicit GroupAttribute();
    GroupAttribute(const QStringList &groups);

    virtual ~GroupAttribute();

    void deserialize(const QByteArray &data);
    QByteArray serialized() const;

    Akonadi::Attribute *clone() const;
    QByteArray type() const;

    bool containsGroup(const QString &groupId) const;

    const QStringList& groups() const;

    void setGroups(const QStringList &groups);

  private:
    QStringList m_groups;
};

#endif // GROUPATTRIBUTE_H

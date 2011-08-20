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


#include "kgoogleobject.h"

using namespace KGoogle;

KGoogleObject::KGoogleObject()
{ 

}

KGoogleObject::~KGoogleObject()
{

}

void KGoogleObject::setId(const QString& id)
{
  m_id = id;
}

QString KGoogleObject::id()
{
  return m_id;
}

void KGoogleObject::setEtag(const QString& etag)
{
  m_etag = etag;
}

QString KGoogleObject::etag()
{
  return m_etag;
}
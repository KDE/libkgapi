/*
    libKGoogle - Object
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


#include "object.h"

using namespace KGoogle;

KGoogle::Object::Object()
{

}

KGoogle::Object::~Object()
{

}

void KGoogle::Object::setEtag(const QString& etag)
{
  m_etag = etag;
}

const QString& KGoogle::Object::etag() const
{
  return m_etag;
}

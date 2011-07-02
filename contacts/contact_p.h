/*
    Akonadi Google - Contacts resource
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

#ifndef CONTACT_P_H
#define CONTACT_P_H

#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtCore/QUrl>

#include <KDE/KABC/Address>
#include <KDE/KABC/PhoneNumber>

namespace Contact {
  
class ContactPrivate : public QSharedData
{

  public:
    ContactPrivate() { };
    ContactPrivate(const ContactPrivate &other);
    ~ContactPrivate() { };
   
    bool deleted;
    QUrl photoUrl;
    QString id;
    QString etag;
    
    QString name;
    QString notes;
    QString job;
    QString jobTitle;
    QString birthday;
    
    Email::List emails;
    IM::List ims;
    PhoneNumber::List phoneNumbers;
    Address::List addresses;
};

}

#endif /* CONTACT_P_H */

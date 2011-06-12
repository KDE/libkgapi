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


#ifndef CONTACTCREATEJOB_H
#define CONTACTCREATEJOB_H

#include <KDE/KJob>
#include <KDE/KABC/Addressee>

#include <QtNetwork/QNetworkReply>

#include "contact.h"

class ContactCreateJob : public KJob
{
  Q_OBJECT
  public:
    ContactCreateJob(KABC::Addressee addressee, const QString &accessToken);
    
    void start();
    
    Contact::Contact* newContact() { return m_contact; } 
    
  private Q_SLOTS:
    void requestFinished(QNetworkReply*);
    
  private:
    QString m_accessToken;
    KABC::Addressee m_addressee;
    Contact::Contact *m_contact;
};

#endif // CONTACTCREATEJOB_H

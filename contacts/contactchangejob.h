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


#ifndef CONTACTCHANGEJOB_H
#define CONTACTCHANGEJOB_H

#include <QtNetwork/QNetworkReply>

#include <KDE/KJob>
#include <KDE/KABC/Addressee>

class ContactChangeJob : public KJob
{

  Q_OBJECT
  public:
    ContactChangeJob(KABC::Addressee addressee, const QString &contactId, const QString &accessToken);
    
    void start();
    
    KABC::Addressee newAddressee() { return m_newAddressee; }
    
  private Q_SLOTS:
    void requestFinished(QNetworkReply*);
    
  private:
    KABC::Addressee m_addressee;
    KABC::Addressee m_newAddressee;
    QString m_contactId;
    QString m_accessToken;  
};

#endif // CONTACTCHANGEJOB_H

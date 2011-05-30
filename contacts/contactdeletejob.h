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


#ifndef CONTACTDELETEJOB_H
#define CONTACTDELETEJOB_H

#include <KDE/KJob>

#include <QtNetwork/QNetworkReply>

class ContactDeleteJob: public KJob
{
  Q_OBJECT
  public:
    ContactDeleteJob(const QString &uid, const QString &accessToken);
    ~ContactDeleteJob();
    
    void start();
    
  private Q_SLOTS:
    void requestFinished(QNetworkReply*);

  private:
    QString m_uid;
    QString m_accessToken;

};

#endif // CONTACTDELETEJOB_H

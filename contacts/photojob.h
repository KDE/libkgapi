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


#ifndef PHOTOJOB_H
#define PHOTOJOB_H

#include <KDE/KJob>

#include <QtCore/QUrl>
#include <QtGui/QImage>
#include <QtNetwork/QNetworkReply>

/**
 * @brief A KJob for downloading photos for contacts.
 * 
 * Google does not send photos with contacts, it just provides
 * an URL from which the picture can be fetched.
 */
class PhotoJob: public KJob
{
  
  Q_OBJECT
  public:
    /**
     * @brief Creates a new job for to fetch photo from \photoUrl
     * 
     * @param accessToken Google API access token
     * @param photoUrl ID of contact picture provided by Google
     */
    PhotoJob(const QString &accessToken, const QUrl &photoUrl);
    
    virtual ~PhotoJob() {};
    
    /**
     * @brief Starts the job, fetches the picture
     */
    void start();
    
    /**
     * @brief Returns QImage containing the fetched picture
     * 
     * @return Returns fetched picture.
     */
    QImage photo() { return m_photo; }
    
  private Q_SLOTS:
    void photoRetrieved(QNetworkReply *reply);
    
  private:
    QString m_accessToken;
    QUrl m_photoUrl;
    
    QImage m_photo;
};

#endif // PHOTOJOB_H

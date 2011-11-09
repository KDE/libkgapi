/*
    libKGoogle - KGoogleAccessManager
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


#ifndef KGOOGLEACCESSMANAGER_H
#define KGOOGLEACCESSMANAGER_H

#include <qobject.h>
#include <qurl.h>

#include <kdatetime.h>

#include <libkgoogle/kgooglerequest.h>
#include <libkgoogle/libkgoogle_export.h>

class QNetworkAccessManager;
class QNetworkReply;

namespace KGoogle {
  class KGoogleAuth;
  class KGoogleReply;
}

namespace KGoogle {

  /**
   * KGoogleAccessManager allows application to send
   * requests to Google services.
   */
  class LIBKGOOGLE_EXPORT KGoogleAccessManager: public QObject
  {

    Q_OBJECT

    public:
      /**
       * Constructs a new access manager 
       */
      KGoogleAccessManager(KGoogleAuth *auth);
      
      virtual ~KGoogleAccessManager();
      
      /**
       * Converts RFC3339 date and time to KDateTime object.
       * 
       * KDateTime::fromString() supportes RFC3339 date 
       * since KDE 4.7.0. This function uses native implementation
       * when available and provides custom implementation for
       * backwards compatibilityt with older KDE versions.       
       * 
       * @param datetime String with date and time in RFC3339 format
       * @return \p datetime converted to KDateTime object
       */
      static KDateTime RFC3339StringToDate(const QString &datetime);
      
      /**
       * Converts KDateTime object do RFC3339 string.
       * 
       * KDateTime::toString() supportes RFC3339 date 
       * since KDE 4.7.0. This function uses native implementation
       * when available and provides custom implementation for
       * backwards compatibilityt with older KDE versions.
       * 
       * @param datetime KDateTime object to be converted
       * @return \p datetime converted to RFC3339 string
       */
      static QString dateToRFC3339String(const KDateTime &datetime);      
      
    public Q_SLOTS:
      /**
       * Sends p0 request to a Google service.
       * 
       * @param request A request to be send
       */
      void sendRequest(KGoogleRequest *request);
      
    Q_SIGNALS:
      /**
       * A reply was received.
       * 
       * This signal is emitted whenever a reply from
       * a Google service was received.
       */
      void replyReceived(KGoogleReply *reply);
      
      /**
       * A request was finished.
       * 
       * This signal is emitted when last batch of data is received,
       * for example when all pages of events feed are fetched.
       * 
       * @param request The related request.
       */
      void requestFinished(KGoogleRequest *request);
      
      /**
       * An error occurred.
       * 
       * This signal is emitted whenever an error
       * occurs.
       */
      void error(const QString &msg, const int errorCode);

    private Q_SLOTS:
      void nam_replyReceived(QNetworkReply *reply);
      void nam_sendRequest(KGoogleRequest *request);

      void newTokensReceived();
      void submitCache();
      
    private:
      KGoogleAuth *m_auth;
      QNetworkAccessManager *m_nam;
      
      QList<KGoogleRequest*> m_cache;
      
      bool m_namLocked;
  };

}  // namespace KGoogle

#endif // KGOOGLEACCESSMANAGER_H

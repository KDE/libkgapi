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

#include "kgooglerequest.h"
#include "libkgoogle_export.h"
#include <qmutex.h>

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
       * This signal is emitted when a reply from
       * a Google service was received.
       */
      void replyReceived(KGoogleReply *reply);
      
      /**
       * An error occurred.
       * 
       * This signal is emitted whenever an error
       * occurs.
       */
      void error(QString msg, int errorCode);

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

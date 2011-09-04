/*
    libKGoogle - KGoogleAuth
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


#ifndef KGOOGLEAUTH_H
#define KGOOGLEAUTH_H

#include "libkgoogle_export.h"

#include <qobject.h>
#include <qwindowdefs.h>
#include <qstringlist.h>

namespace KWallet {
  class Wallet;
}

class QNetworkReply;

namespace KGoogle {

  /**
   * KGoogleAuth provides API for authentication against Google services.
   */
  class LIBKGOOGLE_EXPORT KGoogleAuth: public QObject
  {

    Q_OBJECT

    public:
      /**
       * Constructs a new KGoogleObject.
       * 
       * @param clientId ClientID assigned by Google.
       * @param clientSecret Client secret ID assigned by Google
       * @param scope Service scope URL.
       */
      KGoogleAuth(const QString &clientId, const QString &clientSecret, const QString &scope);
      ~KGoogleAuth();
    
      
    public Q_SLOTS:
      /**
       * Request new tokens from Google. 
       * 
       * This will popup AuthDialog. When tokens
       * are received the tokensReceived() signal will be emitted and the dialog is closed.
       */
      void requestToken(const WId windowId);
      
      /**
       * Refresh current tokens.
       */
      void refreshToken();
    
      /**
       * Remove all tokens from wallet
       */
      void revokeTokens();
    
      /**
       * Returns current access token. 
       * 
       * When no access token is available, an empty string is returned. This usually means
       * that user needs to call requestToken() to get the tokens.
       */
      QString accessToken();
        
    signals:
      /**
       * Emitted when an error occurs during authentication.
       */
      void error(QString errorString);
      
      /**
       * New tokens were received.
       */
      void tokensRecevied(QString accessToken, QString refreshToken);
    
    private Q_SLOTS:
      void refreshTokenFinished(QNetworkReply *reply);
    
      void setTokens(const QString &accessToken, const QString &refreshToken);

    private:
      QString m_accessToken;
      QString m_refreshToken;
    
      QString m_clientId;
      QString m_clientSecret;
      
      QString m_scope;
    
      KWallet::Wallet *m_kwallet;

  };
  
} // namespace KGoogle

#endif // KGOOGLEAUTH_H

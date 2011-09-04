/*
    <one line to give the program's name and a brief idea of what it does.>
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


#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QtCore/QUrl>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QProgressBar>
#include <QtGui/QLabel>
#include <QtNetwork/QNetworkReply>

#include <Akonadi/ResourceBase>

#include <KDE/KDialog>
#include <KDE/KWebView>
#include <KDE/KPushButton>
#include <kdemacros.h>

/**
 * Dialog for authentication with Google services.
 * 
 * The dialog opens Google login page in embedded webivew, 
 * asking user to login and confirm access of this application 
 * to some services.
 * After that the webview is hidden and the dialog exchanges some tokens
 * for access and refresh tokens. 
 * The access token is then used for authenticating any request on Google
 * services, the refresh token is used to refresh expired access token.
 */
class KDE_EXPORT AuthDialog: public KDialog
{
  Q_OBJECT
  public:
    /**
     * Creates the dialog
     * 
     * @param windowId ID of window 
     **/
    AuthDialog(WId windowId);
    virtual ~AuthDialog();

    /**
     * Sets scopes which to obtain authentication for.
     * 
     * See http://code.google.com/apis/gdata/faq.html#AuthScopes for
     * list of available scopes.
     * 
     * @param List of scope URLs
     * */
    void setScopes (const QStringList &scopes);
    
    /**
     * Starts the authentication process.
     * 
     * @param m_clientId ID of this application as assigned by Google
     * @param m_clientSecret Secret token for this application as assigned
     * 			     by Google
     */
    void auth (const QString &m_clientId, const QString &m_clientSecret);
    
    /**
     * Returns retrieved access token.
     * 
     * Access token is a persitent token which must be present
     * in every request on Google API. Through this token, the 
     * remote server can grant access to requested data.
     * 
     * There is no point in calling this method before \finished()
     * is emitted or the dialog is \accepted().
     * 
     * @return Authorized access token
     */
    QString accessToken() { return m_accessToken; }

    /**
     * Returns retrieved refresh token.
     * 
     * The access token has limited validity. When the access token
     * expires, every authenticated service will return Error 401 
     * Unauthorized.
     * By sending the refresh token a new pair of access and refresh token
     * will be received.
     *
     * There is no point in calling this method before \finished()
     * is emitted or the dialog is \accepted().
     * 
     * @return Refresh token 
     */
    QString refreshToken() { return m_refreshToken; }
    
  signals:
    /**
     * This signal is emitted when access token and refresh token were successfully retrieved.
     * 
     * When running the dialog synchronously via \KDialog::exec() the dialog
     * also emits \KDialog::accepted()
     * 
     * @param accessToken Received access token
     * @param refreshtoken Received refresh token
     */
    void finished(QString accessToken, QString refreshToken);
    
  private slots:
    void webviewUrlChanged(const QUrl &url);
    void webviewFinished();
    
    void networkRequestFinished(QNetworkReply *reply);
    
  private:
    void setError(const QString &error);
    
    /* GUI */
    QWidget *m_widget;
    QProgressBar *m_progressbar;
    QHBoxLayout *m_hbox;
    QVBoxLayout *m_vbox;
    KWebView *m_webiew;
    QLabel *m_label;
    
    QStringList m_scopes;
    QString m_clientId;
    QString m_clientSecret;    
    QString m_accessToken;
    QString m_refreshToken;
};

#endif // AUTHDIALOG_H

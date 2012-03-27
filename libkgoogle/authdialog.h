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

#include <libkgoogle/account.h>
#include <libkgoogle/common.h>

#include <QtCore/QUrl>
#include <KDE/KDialog>

class QNetworkReply;
class QHBoxLayout;
class QVBoxLayout;
class QProgressBar;
class QLabel;
class KWebView;

namespace KGoogle
{

class Reply;

};


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
 *
 * This dialog is not (for now) part of public API, thus it's not in the KGoogle
 * namespace and neither is this header installed.
 */
class LIBKGOOGLE_EXPORT AuthDialog: public KDialog
{
    Q_OBJECT

  public:
    /**
     * Creates the dialog
     *
     * @param windowId ID of parent window
     */
    AuthDialog(WId windowId);

    virtual ~AuthDialog();

    /**
     * Starts the authentication process of \p account. When finished
     * the authenticated() signal will be emitted with the same instance
     * of \p account and with tokens and name filled.
     *
     * Throws KGoogle::Exception::Invalid account when the list
     * of scopes in \p account is empty.
     *
     * @param account An account to be authenticated.
     */
    void authenticate(KGoogle::Account::Ptr &account);

    /**
     * The passed username will be injected in the OAuth form
     *
     * @param QString The username to use
     */
    void setUsername(const QString &username);

    /**
     * The passed password will be injected in the OAuth form
     *
     * @param QString The password to use
     */
    void setPassword(const QString &password);

    /**
     * Remove any previously set username or password
     */
    void clearCredentials();

  Q_SIGNALS:
    /**
     * This signal is emitted when access token and refresh token were successfully retrieved.
     *
     * When running the dialog synchronously via KDialog::exec() the dialog
     * also emits KDialog::accepted()
     *
     * @param account Successfully authenticated account with name tokens set
     */
    void authenticated(KGoogle::Account::Ptr &account);

    /**
     * Emitted whenever an error occurs during the authentication.
     */
    void error(const KGoogle::Error errCode, const QString &msg);

  private Q_SLOTS:
    /**
     * Emits error() signal with same parameters and displays the error
     * on the dialog.
     */
    void emitError(const KGoogle::Error errCode, const QString &msg);

    void webviewUrlChanged(const QUrl &url);
    void webviewFinished();

    void networkRequestFinished(QNetworkReply *reply);
    void accountInfoReceived(KGoogle::Reply *reply);

  private:
    /* GUI */
    QWidget *m_widget;
    QProgressBar *m_progressbar;
    QHBoxLayout *m_hbox;
    QVBoxLayout *m_vbox;
    KWebView *m_webiew;
    QLabel *m_label;

    QString m_username;
    QString m_password;
    KGoogle::Account::Ptr m_account;
};

#endif // AUTHDIALOG_H

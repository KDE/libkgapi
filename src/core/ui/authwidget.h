/*
    SPDX-FileCopyrightText: 2012-2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef LIBKGAPI2_UI_AUTHWIDGET_H
#define LIBKGAPI2_UI_AUTHWIDGET_H

#include <QWidget>

#include "types.h"
#include "account.h"
#include "kgapicore_export.h"

namespace KGAPI {
    typedef KGAPI2::Account Account;
}

namespace KGAPI2 {

class AuthWidgetPrivate;

/**
 * A widget for authentication that can be easily embedded
 * to any application.
 *
 * @since 0.3.2
 */
class KGAPICORE_EXPORT AuthWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool showProgressBar WRITE setShowProgressBar READ getShowProgressBar)

  public:
    /**
     * Describes progress of the authentication so that external observers can react
     * (for example you can hide the widget after user login)
     */
    enum Progress {
        None,               /**< Initial state, before authenticate() is called */
        UserLogin,          /**< The webview where user has to login to Google is displayed */
        TokensRetrieval,    /**< Tokens are being retrieved (webview no longer visible) */
        Finished,           /**< Tokens were retrieved and authenticated() signal has been emitted */
        Error               /**< An error occurred and error() signal has been emitted */
    };

    explicit AuthWidget(QWidget* parent = nullptr);

    ~AuthWidget() override;

    /**
     * Runs the authentication.
     *
     * Displays the webview and starts the actual process of authentication.
     *
     * The method will throw an KGAPI::InvalidAccount exception if
     * no account was set via setAccount() before invoking this method.
     */
    void authenticate();

    /**
      * Sets the username that will be used when authenticate is called
      *
      * The username will be automatically filled in the Google login
      * form in the authentication widget.
      *
      * Be aware that the username will be set every time \sa authenticate is
      * called so if you want to change or remove it call \sa setUsername again
      * with empty string or \sa clearCredentials.
      *
      * @param username username to use
      */
    void setUsername(const QString &username);

    /**
     * Sets the password that will be used when authenticate is called
     *
     * The password will be automatically filled in the Google login
     * form in the authentication widget.
     *
     * Be aware that the password will be set every time \sa authenticate is
     * called so if you want to change or remove it call \sa setPassword again
     * with empty string or \sa clearCredentials.
     *
     * @param password password to use
     */
    void setPassword(const QString &password);

    /**
     * Sets to empty username and password
     *
     * Sets to empty the username and the password which were set by
     * calling \sa setUsername and \sa setPassword.
     */
    void clearCredentials();

    /**
     * Sets an account for which to obtain authentication.
     */
    void setAccount(const KGAPI2::AccountPtr &account);

    /**
     * Sets whether to show progressbar above the webview when loading
     * and displaying the Google login form.
     *
     * Default is true
     *
     * @param showProgressBar
     */
    void setShowProgressBar(bool showProgressBar);

    /**
     * Returns whether a progressbar above webview will be shown.
     *
     * @see setShowProgressBar
     */
    bool getShowProgressBar() const;

    /**
     * Returns current progress state.
     *
     * @see progress()
     */
    AuthWidget::Progress getProgress() const;

  Q_SIGNALS:
    /**
     * Account was successfully authenticated.
     *
     * This signal is emitted when the authentication was successful
     * and tokens were stored within the \p account
     */
    void authenticated(const KGAPI2::AccountPtr &account);

    /**
     * Emitted when an error occurs
     *
     * Signalizes any authentication error
     */
    void error(const KGAPI2::Error errCode, const QString &msg);

    /**
     * Emitted whenever a state of the authentication process changes.
     *
     * @param progress The new state of authentication
     */
    void progress(KGAPI2::AuthWidget::Progress progress);

  protected:
    AuthWidget(AuthWidgetPrivate *dptr, QWidget *parent = nullptr);

    AuthWidgetPrivate * const d;
    friend class AuthWidgetPrivate;
    friend class AuthJob;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_UI_AUTHWIDGET_H

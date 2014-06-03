/*
 * This file is part of LibKGAPI
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef LIBKGAPI2_AUTHJOB_H
#define LIBKGAPI2_AUTHJOB_H

#include <libkgapi2/job.h>
#include <libkgapi2/libkgapi2_export.h>

class QWidget;

namespace KGAPI2 {

/**
 * @headerfile AuthJob
 * @brief A job to authenticate against Google and fetch tokens
 *
 * This job can be either used to refresh expired tokens (this is usually done
 * automatically by Job implementation), or to request tokens for a new account.
 *
 * In the latter case, the AuthJob will automatically show a dialog where user
 * has to provide Google account credentials and grant access to all requested
 * scopes (@see Account::scopes).
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT AuthJob : public KGAPI2::Job
{
    Q_OBJECT

  public:

    /**
     * @brief Creates a new authentication job that will use @p parent as parent
     *        for the authentication dialog.
     *
     * When constructed with a widget parent, AuthJob will place the
     * authentication widget on the @p parent instead of displaying a dialog.
     * This allows embedding the authentication process into a wizard for
     * instance.
     *
     * @param account Account to authenticate. When only scopes are set, a full
     *                authentication process will run (including showing the auth
     *                widget) and the rest will be filled by the job.
     *                @par
     *                Passing an Account with account name, scopes and both
     *                tokens filled will only refresh the access token. If however
     *                the scopes have been changed a full authentication will be
     *                started.
     *                @par
     *                Any other Account will be considered invalid and the job
     *                will finish immediatelly.
     *
     * @param apiKey Application API key
     * @param secretKey Application secret API key
     * @param parent Parent widget on which auth widget should be constructed if
     *               necessary.
     */
    explicit AuthJob(const AccountPtr &account, const QString &apiKey,
                     const QString &secretKey, QWidget* parent);

    /**
     * @brief Creates a new authentication job
     *
     * When constructed without a parent, or with a non-QWidget parent, the
     * job might pop up the authentication dialog.
     *
     * @param account Account to authenticate.
     *                See AuthJob(AccountPtr,QString,QString,QWidget) for 
     *                detailed description of @p account content.
     * @param apiKey Application API key
     * @param secretKey Application secret API key
     * @param parent
     */
    explicit AuthJob(const AccountPtr &account, const QString &apiKey,
                     const QString &secretKey, QObject* parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~AuthJob();

    /**
     * @brief Returns reauthenticated account.
     *
     * @returns An account pointer passed to the AuthJob() constructor with
     *          all fields filled and validated. When the job fails, the account
     *          is unchanged.
     */
    AccountPtr account() const;

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

  protected:

    /**
     * @brief KGAPI2::Job::handleReply implementation
     *
     * @param reply
     * @param rawData
     */
    virtual void handleReply(const QNetworkReply *reply, const QByteArray &rawData);

    /**
     * @brief KGAPI2::Job::displayRequest implementation
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    virtual void dispatchRequest(QNetworkAccessManager* accessManager,
                                 const QNetworkRequest& request,
                                 const QByteArray& data,
                                 const QString& contentType);

    /**
     * @brief KGAPI2::Job::start implementation
     */
    virtual void start();

  private:
    class Private;
    Private * const d;
    friend class Private;

    Q_PRIVATE_SLOT(d, void _k_fullAuthenticationFinished(const KGAPI2::AccountPtr& account))
    Q_PRIVATE_SLOT(d, void _k_fullAuthenticationFailed(KGAPI2::Error errorCode, const QString &errorMessage))
};

} // namespace KGAPI2

#endif // LIBKGAPI2_AUTHJOB_H

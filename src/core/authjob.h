/*
 * This file is part of LibKGAPI
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "job.h"
#include "kgapicore_export.h"

class QWidget;

namespace KGAPI2 {

/**
 * @headerfile authjob.h
 * @brief A job to authenticate against Google and fetch tokens
 *
 * This job can be either used to refresh expired tokens (this is usually done
 * automatically by Job implementation), or to request tokens for a new account.
 *
 * In the latter case, the AuthJob will automatically open a browser window
 * where user has to provide Google account credentials and grant access to all
 * requested scopes (@see Account::scopes).
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICORE_EXPORT AuthJob : public KGAPI2::Job
{
    Q_OBJECT

  public:
    /**
     * @brief Creates a new authentication job
     *
     * When constructed without a parent, or with a non-QWidget parent, the
     * job might pop up the authentication dialog.
     *
     * @param account Account to authenticate.
     * @param apiKey Application API key
     * @param secretKey Application secret API key
     * @param parent
     */
    explicit AuthJob(const AccountPtr &account, const QString &apiKey,
                     const QString &secretKey, QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~AuthJob() override;

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
      * @param username username to use
      */
    void setUsername(const QString &username);

    /**
     * Sets the password that will be used when authenticate is called
     *
     * @param password password to use
     * @deprecated
     */
    QT_DEPRECATED_X("It's no longer possible to prefill password")
    void setPassword(const QString &password);

  protected:
    /**
     * @brief KGAPI2::Job::handleReply implementation
     *
     * @param reply
     * @param rawData
     */
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;

    /**
     * @brief KGAPI2::Job::displayRequest implementation
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    void dispatchRequest(QNetworkAccessManager* accessManager,
                                 const QNetworkRequest& request,
                                 const QByteArray& data,
                                 const QString& contentType) override;

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

  private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;
};

} // namespace KGAPI2


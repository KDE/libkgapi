/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "authjob.h"
#include "account.h"
#include "../debug.h"
#include "job_p.h"
#include "ui/authwidget.h"
#include "ui/authwidget_p.h"
#include "ui/authwidgetfactory_p.h"
#include "private/newtokensfetchjob_p.h"

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QJsonDocument>
#include <QPointer>
#include <QPushButton>
#include <QNetworkCookieJar>
#include <QUrlQuery>

#include <KWindowSystem>


using namespace KGAPI2;

class Q_DECL_HIDDEN AuthJob::Private
{
  public:
    Private(AuthJob *parent);

    QWidget* fullAuthentication();
    void refreshTokens();

    void _k_fullAuthenticationFinished(const KGAPI2::AccountPtr& account);
    void _k_fullAuthenticationFailed(KGAPI2::Error errorCode, const QString &errorMessage);
    void _k_destructDelayed();

    AccountPtr account;
    QString apiKey;
    QString secretKey;
    QWidget* widget;

    QString username;
    QString password;
    QPointer<QDialog> dialog;

  private:
    AuthJob *q;
};

AuthJob::Private::Private(AuthJob *parent):
    widget(nullptr),
    q(parent)
{
}

QWidget* AuthJob::Private::fullAuthentication()
{
    AuthWidget* authWidget = AuthWidgetFactory::instance()->create(widget);
    // FIXME: Find a better way to pass the keys
    authWidget->d->apiKey = apiKey;
    authWidget->d->secretKey = secretKey;

    authWidget->setUsername(username);
    authWidget->setPassword(password);
    authWidget->setAccount(account);

    return authWidget;
}

void AuthJob::Private::refreshTokens()
{
    static_cast<Job*>(q)->d->accessManager->setCookieJar(new QNetworkCookieJar);

    QNetworkRequest request;

    request.setUrl(QUrl(QStringLiteral("https://accounts.google.com/o/oauth2/token")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));

    QUrlQuery params;
    params.addQueryItem(QStringLiteral("client_id"), apiKey);
    params.addQueryItem(QStringLiteral("client_secret"), secretKey);
    params.addQueryItem(QStringLiteral("refresh_token"), account->refreshToken());
    params.addQueryItem(QStringLiteral("grant_type"), QStringLiteral("refresh_token"));

    qCDebug(KGAPIDebug) << "Requesting token refresh.";

    q->enqueueRequest(request, params.toString(QUrl::FullyEncoded).toLatin1());
}

void AuthJob::Private::_k_fullAuthenticationFailed(Error errorCode, const QString &errorMessage)
{
    q->setError(errorCode);
    q->setErrorString(errorMessage);
    q->emitFinished();
}

void AuthJob::Private::_k_fullAuthenticationFinished( const AccountPtr &account_ )
{
    account = account_;
    q->emitFinished();
}

void AuthJob::Private::_k_destructDelayed()
{
    if (!dialog)
        return;
    if (dialog->isVisible())
        dialog->hide();
    dialog->deleteLater();
    dialog = nullptr;
}


AuthJob::AuthJob(const AccountPtr& account, const QString &apiKey, const QString &secretKey, QWidget* parent):
    Job(parent),
    d(new Private(this))
{
    d->account = account;
    d->apiKey = apiKey;
    d->secretKey = secretKey;
    d->widget = parent;
}

AuthJob::AuthJob(const AccountPtr& account, const QString &apiKey, const QString &secretKey, QObject* parent):
    Job(parent),
    d(new Private(this))
{
    d->account = account;
    d->apiKey = apiKey;
    d->secretKey = secretKey;
}

AuthJob::~AuthJob()
{
    delete d;
}

AccountPtr AuthJob::account() const
{
    return d->account;
}

void AuthJob::setUsername(const QString& username)
{
    d->username = username;
}

void AuthJob::setPassword(const QString& password)
{
    d->password = password;
}

void AuthJob::handleReply(const QNetworkReply *reply, const QByteArray& rawData)
{
    Q_UNUSED(reply);

    QJsonDocument document = QJsonDocument::fromJson(rawData);
    if (document.isNull()) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Failed to parse newly fetched tokens"));
        emitFinished();
        return;
    }

    QVariantMap map = document.toVariant().toMap();

    /* Expected structure:
     * {
     *  "access_token": "the_access_token",
     *  "token_type":"Bearer",
     *  "expires_in":3600
     * }
     */
    const qlonglong expiresIn = map.value(QStringLiteral("expires_in")).toLongLong();
    d->account->setExpireDateTime(QDateTime::currentDateTime().addSecs(expiresIn));
    d->account->setAccessToken(map.value(QStringLiteral("access_token")).toString());
    emitFinished();
}

void AuthJob::dispatchRequest(QNetworkAccessManager* accessManager, const QNetworkRequest& request, const QByteArray& data, const QString& contentType)
{
    Q_UNUSED(contentType);

    accessManager->post(request, data);
}

void AuthJob::start()
{
    AuthWidget *widget = nullptr;

    if (d->account->refreshToken().isEmpty() || (d->account->m_scopesChanged == true)) {

        d->account->addScope(Account::accountInfoEmailScopeUrl());

        /* Pre-fill the username in the dialog so that user knows what account
         * (s)he is re-authenticating for */
        if (!d->account->accountName().isEmpty() && d->username.isEmpty()) {
            d->username = d->account->accountName();
        }

        widget = qobject_cast<AuthWidget*>(d->fullAuthentication());

    } else {

        if (d->account->accountName().isEmpty()) {
            setError(KGAPI2::InvalidAccount);
            setErrorString(tr("Account name is empty"));
            emitFinished();
            return;
        }

        d->refreshTokens();
    }

    if (widget) {
        d->dialog = new QDialog();
        d->dialog->setModal(true);
        d->dialog->resize(840, 760);
        KWindowSystem::setMainWindow(d->dialog, KWindowSystem::activeWindow());

        QVBoxLayout *layout = new QVBoxLayout(d->dialog);
        layout->addWidget(widget, 2);

        QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Cancel, Qt::Horizontal, d->dialog);
        layout->addWidget(buttons, 0);

        connect(buttons, &QDialogButtonBox::rejected,
                this, [this]() {
                    d->_k_destructDelayed();
                    d->_k_fullAuthenticationFailed(AuthCancelled, tr("Authentication canceled"));
                });
        connect(widget, &AuthWidget::authenticated,
                this, [this](const KGAPI2::AccountPtr &account) {
                    d->_k_destructDelayed();
                    d->_k_fullAuthenticationFinished(account);
                });
        connect(widget, &AuthWidget::error,
                this, [this](KGAPI2::Error error, const QString &str) {
                    d->_k_destructDelayed();
                    d->_k_fullAuthenticationFailed(error, str);
                });

        d->dialog->show();
        buttons->button(QDialogButtonBox::Cancel)->setDefault(false); // QTBUG-66109

        widget->authenticate();
    }
}


#include "moc_authjob.cpp"

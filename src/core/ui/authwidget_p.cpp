/*
    Copyright 2012, 2013  Daniel Vrátil <dvratil@redhat.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "authwidget_p.h"
#include "account.h"
#include "accountinfo/accountinfo.h"
#include "accountinfo/accountinfofetchjob.h"
#include "private/newtokensfetchjob_p.h"
#include "../../debug.h"

#include <QWebEngineView>
#include <QNetworkReply>
#include <QContextMenuEvent>

#include <QDateTime>

using namespace KGAPI2;

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
}


WebView::~WebView()
{

}

void WebView::contextMenuEvent(QContextMenuEvent *e)
{
    // No menu
    e->accept();
}

AuthWidget::Private::Private(AuthWidget *parent):
    QObject(),
    showProgressBar(true),
    progress(AuthWidget::None),
    q(parent)
{
    setupUi();
}

AuthWidget::Private::~Private()
{
}

void AuthWidget::Private::setupUi()
{
    vbox = new QVBoxLayout(q);
    q->setLayout(vbox);

    label = new QLabel(q);
    label->setText(QLatin1String("<b>") % tr("Authorizing token. This should take just a moment...") % QLatin1String("</b>"));
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    label->setVisible(false);
    vbox->addWidget(label);

    progressbar = new QProgressBar(q);
    progressbar->setMinimum(0);
    progressbar->setMaximum(100);
    progressbar->setValue(0);
    vbox->addWidget(progressbar);

    webview = new WebView(q);

    vbox->addWidget(webview);
    connect(webview, &QWebEngineView::loadProgress, progressbar, &QProgressBar::setValue);
    connect(webview, &QWebEngineView::urlChanged, this, &Private::webviewUrlChanged);
    connect(webview, &QWebEngineView::loadFinished, this, &Private::webviewFinished);
}

void AuthWidget::Private::setProgress(AuthWidget::Progress progress)
{
    qCDebug(KGAPIDebug) << progress;
    this->progress = progress;
    Q_EMIT q->progress(progress);
}


void AuthWidget::Private::emitError(const enum Error errCode, const QString& msg)
{
    label->setVisible(true);
    webview->setVisible(false);
    progressbar->setVisible(false);

    label->setText(QLatin1String("<b>") % msg % QLatin1String("</b>"));

    Q_EMIT q->error(errCode, msg);
    setProgress(AuthWidget::Error);
}


void AuthWidget::Private::webviewUrlChanged(const QUrl &url)
{
    qCDebug(KGAPIDebug) << url;

    /* Access token here - hide browser and tell user to wait until we
     * finish the authentication process ourselves */
    if (url.host() == QLatin1String("accounts.google.com") && url.path() == QLatin1String("/o/oauth2/approval")) {
        webview->setVisible(false);
        progressbar->setVisible(false);
        label->setVisible(true);

        setProgress(AuthWidget::TokensRetrieval);
    }
}

void AuthWidget::Private::webviewFinished(bool ok)
{
    if (!ok) {
        qCWarning(KGAPIDebug) << "Failed to load" << webview->url();
    }

    QUrl url = webview->url();
    qCDebug(KGAPIDebug) << url;

    if (url.host() == QLatin1String("accounts.google.com") && url.path() == QLatin1String("/ServiceLogin")) {
        if (username.isEmpty() && password.isEmpty()) {
            return;
        }

#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
        const auto js = QStringLiteral("document.getElementById(\"%1\").value = \"%2\";");
        if (!username.isEmpty()) {
            webview->page()->runJavaScript(js.arg(QStringLiteral("Email"), username));
        }

        if (!password.isEmpty()) {
            webview->page()->runJavaScript(js.arg(QStringLiteral("Passwd"), password));
        }
#endif

        return;
    }

    if (url.host() == QLatin1String("accounts.google.com") && url.path() == QLatin1String("/o/oauth2/approval")) {
        QString title = webview->title();
        QString token;

        if (title.startsWith(QLatin1String("success"), Qt::CaseInsensitive)) {
            int pos = title.indexOf(QLatin1String("code="));
            /* Skip the 'code=' string as well */
            token = title.mid (pos + 5);
        } else {
            webview->page()->toHtml([title](const QString &html) {
                qCDebug(KGAPIDebug) << "Parsing token page failed. Title:" << title;
                qCDebug(KGAPIDebug) << html;
            });
            emitError(AuthError, tr("Parsing token page failed."));
            return;
        }

        if (token.isEmpty()) {
            webview->page()->toHtml([](const QString &html) {
                qCDebug(KGAPIDebug) << "Failed to obtain token.";
                qCDebug(KGAPIRaw) << html;
            });
            emitError(AuthError, tr("Failed to obtain token."));
            return;
        }

        KGAPI2::NewTokensFetchJob *fetchJob = new KGAPI2::NewTokensFetchJob(token, apiKey, secretKey);
        connect(fetchJob, &Job::finished,
                this, &Private::tokensReceived);
    }
}

void AuthWidget::Private::tokensReceived(KGAPI2::Job* job)
{
    KGAPI2::NewTokensFetchJob *tokensFetchJob = qobject_cast<KGAPI2::NewTokensFetchJob*>(job);

    account->setAccessToken(tokensFetchJob->accessToken());
    account->setRefreshToken(tokensFetchJob->refreshToken());
    account->setExpireDateTime(QDateTime::currentDateTime().addSecs(tokensFetchJob->expiresIn()));
    tokensFetchJob->deleteLater();

    KGAPI2::AccountInfoFetchJob *fetchJob = new KGAPI2::AccountInfoFetchJob(account, this);

    connect(fetchJob, &Job::finished,
            this, &Private::accountInfoReceived);
    qCDebug(KGAPIDebug) << "Requesting AccountInfo";
}

void AuthWidget::Private::accountInfoReceived(KGAPI2::Job* job)
{
    if (job->error()) {
        qCDebug(KGAPIDebug) << "Error when retrieving AccountInfo:" << job->errorString();
        emitError((enum Error) job->error(), job->errorString());
        return;
    }

    KGAPI2::ObjectsList objects = qobject_cast<KGAPI2::AccountInfoFetchJob*>(job)->items();
    Q_ASSERT(!objects.isEmpty());

    KGAPI2::AccountInfoPtr accountInfo = objects.first().staticCast<KGAPI2::AccountInfo>();
    account->setAccountName(accountInfo->email());

    job->deleteLater();

    Q_EMIT q->authenticated(account);
    setProgress(AuthWidget::Finished);
}


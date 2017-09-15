/*
    Copyright 2012, 2013  Daniel Vrátil <dvratil@redhat.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "authwidget_p.h"
#include "account.h"
#include "accountinfo/accountinfo.h"
#include "accountinfo/accountinfofetchjob.h"
#include "private/newtokensfetchjob_p.h"
#include "../../debug.h"

#include <QWebEngineProfile>
#include <QContextMenuEvent>

#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>

#include <QDateTime>

using namespace KGAPI2;

namespace
{

class WebView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit WebView(QWidget *parent = nullptr)
        : QWebEngineView(parent)
    {
        // Don't store cookies, so that subsequent invocations of AuthJob won't remember
        // the previous accounts.
        QWebEngineProfile::defaultProfile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    }

    void contextMenuEvent(QContextMenuEvent *e) override
    {
        // No menu
        e->accept();
    }
};

class WebPage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit WebPage(QObject *parent = nullptr)
        : QWebEnginePage(parent)
        , mLastError(nullptr)
    {
    }

    QWebEngineCertificateError *lastCertificateError() const
    {
        return mLastError;
    }

    bool certificateError(const QWebEngineCertificateError &err) override
    {
        if (mLastError) {
            delete mLastError;
        }
        mLastError = new QWebEngineCertificateError(err.error(), err.url(), err.isOverridable(), err.errorDescription());
        Q_EMIT sslError();

        return false; // don't let it through
    }

Q_SIGNALS:
    void sslError();

private:
    QWebEngineCertificateError *mLastError;
};

}




AuthWidgetPrivate::AuthWidgetPrivate(AuthWidget *parent):
    QObject(),
    showProgressBar(true),
    progress(AuthWidget::None),
    q(parent)
{
    setupUi();
}

AuthWidgetPrivate::~AuthWidgetPrivate()
{
}

void AuthWidgetPrivate::setSslIcon(const QString &iconName)
{
    // FIXME: workaround for silly Breeze icons: the small 22x22 icons are
    // monochromatic, which is absolutely useless since we are trying to security
    // information here, so instead we force use the bigger 48x48 icons which
    // have colors and downscale them
    sslIndicator->setIcon(QIcon::fromTheme(iconName).pixmap(48));
}

void AuthWidgetPrivate::setupUi()
{
    vbox = new QVBoxLayout(q);
    q->setLayout(vbox);

    label = new QLabel(q);
    label->setText(QLatin1String("<b>") % tr("Authorizing token. This should take just a moment...") % QLatin1String("</b>"));
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    label->setVisible(false);
    vbox->addWidget(label);

    auto hbox = new QHBoxLayout;
    hbox->setSpacing(0);
    sslIndicator = new QToolButton(q);
    connect(sslIndicator, &QToolButton::clicked,
            this, [this]() {
                auto page = qobject_cast<WebPage*>(webview->page());
                if (auto err = page->lastCertificateError()) {
                    QMessageBox msg;
                    msg.setIconPixmap(QIcon::fromTheme(QStringLiteral("security-low")).pixmap(64));
                    msg.setText(err->errorDescription());
                    msg.addButton(QMessageBox::Ok);
                    msg.exec();
                }
            });
    hbox->addWidget(sslIndicator);
    urlEdit = new QLineEdit(q);
    urlEdit->setReadOnly(true);
    hbox->addWidget(urlEdit);
    vbox->addLayout(hbox);

    progressbar = new QProgressBar(q);
    progressbar->setMinimum(0);
    progressbar->setMaximum(100);
    progressbar->setValue(0);
    vbox->addWidget(progressbar);

    webview = new WebView(q);

    auto webpage = new WebPage(webview);
    connect(webpage, &WebPage::sslError,
            this, [this]() {
                setSslIcon(QStringLiteral("security-low"));
            });
    webview->setPage(webpage);

    vbox->addWidget(webview);
    connect(webview, &QWebEngineView::loadProgress, progressbar, &QProgressBar::setValue);
    connect(webview, &QWebEngineView::urlChanged, this, &AuthWidgetPrivate::webviewUrlChanged);
    connect(webview, &QWebEngineView::loadFinished, this, &AuthWidgetPrivate::webviewFinished);
}

void AuthWidgetPrivate::setProgress(AuthWidget::Progress progress)
{
    qCDebug(KGAPIDebug) << progress;
    this->progress = progress;
    Q_EMIT q->progress(progress);
}


void AuthWidgetPrivate::emitError(const enum Error errCode, const QString& msg)
{
    label->setVisible(true);
    sslIndicator->setVisible(false);
    urlEdit->setVisible(false);
    webview->setVisible(false);
    progressbar->setVisible(false);

    label->setText(QLatin1String("<b>") % msg % QLatin1String("</b>"));

    Q_EMIT q->error(errCode, msg);
    setProgress(AuthWidget::Error);
}


void AuthWidgetPrivate::webviewUrlChanged(const QUrl &url)
{
    qCDebug(KGAPIDebug) << "URLChange:" << url;

    // Whoa! That should not happen!
    if (url.scheme() != QLatin1String("https")) {
        QTimer::singleShot(0, this, [this, url]() {
            QUrl sslUrl = url;
            sslUrl.setScheme(QStringLiteral("https"));
            webview->setUrl(sslUrl);
        });
        return;
    }

    if (!isGoogleHost(url)) {
        // We handled SSL above, so we are secure. We are however outside of
        // accounts.google.com, which is a little suspicious in context of this class
        setSslIcon(QStringLiteral("security-medium"));
        return;
    }

    if (qobject_cast<WebPage*>(webview->page())->lastCertificateError()) {
        setSslIcon(QStringLiteral("security-low"));
    } else {
        // We have no way of obtaining current SSL certifiace from QWebEngine, but we
        // handled SSL and accounts.google.com cases above and QWebEngine did not report
        // any SSL error to us, so we can assume we are safe.
        setSslIcon(QStringLiteral("security-high"));
    }


    // Username and password inputs are loaded dynamically, so we only get
    // urlChanged, but not urlFinished.
    if (isUsernameFrame(url)) {
        if (!username.isEmpty()) {
            webview->page()->runJavaScript(QStringLiteral("document.getElementById(\"identifierId\").value = \"%1\";").arg(username));
        }
    } else if (isPasswordFrame(url)) {
        if (!password.isEmpty()) {
            webview->page()->runJavaScript(QStringLiteral("var elems = document.getElementsByTagName(\"input\");"
                                                          "for (var i = 0; i < elems.length; i++) {"
                                                          "  if (elems[i].type == \"password\" && elems[i].name == \"password\") {"
                                                          "      elems[i].value = \"%1\";"
                                                          "      break;"
                                                          "  }"
                                                          "}").arg(password));
        }
    } else if (isTokenPage(url)) {
        /* Access token here - hide browser and tell user to wait until we
         * finish the authentication process ourselves */
        sslIndicator->setVisible(false);
        urlEdit->setVisible(false);
        webview->setVisible(false);
        progressbar->setVisible(false);
        label->setVisible(true);

        setProgress(AuthWidget::TokensRetrieval);
    }
}

void AuthWidgetPrivate::webviewFinished(bool ok)
{
    if (!ok) {
        qCWarning(KGAPIDebug) << "Failed to load" << webview->url();
    }

    const QUrl url = webview->url();
    urlEdit->setText(url.toDisplayString(QUrl::PrettyDecoded));
    urlEdit->setCursorPosition(0);
    qCDebug(KGAPIDebug) << "URLFinished:" << url;

    if (!isGoogleHost(url)) {
        return;
    }

    if (isTokenPage(url)) {
        const auto token = url.queryItemValue(QStringLiteral("approvalCode"));
        if (!token.isEmpty()) {
            qCDebug(KGAPIDebug) << "Got token: " << token;
            auto fetch = new KGAPI2::NewTokensFetchJob(token, apiKey, secretKey);
            connect(fetch, &Job::finished, this, &AuthWidgetPrivate::tokensReceived);
        } else {
            qCWarning(KGAPIDebug) << "Failed to parse token from URL, peaking into HTML...";
            webview->page()->runJavaScript(
                QStringLiteral("document.getElementById(\"code\").value;"),
                [this](const QVariant &result) {
                    const auto token = result.toString();
                    if (token.isEmpty()) {
                        qCWarning(KGAPIDebug) << "Peaked into HTML, but cound not find token :(";
                        webview->page()->toHtml([](const QString &html) {
                            qCDebug(KGAPIDebug) << "Parsing token page failed";
                            qCDebug(KGAPIDebug) << html;
                        });
                        emitError(AuthError, tr("Parsing token page failed."));
                        return;
                    }
                    qCDebug(KGAPIDebug) << "Peaked into HTML and found token: " << token;
                    auto fetch = new KGAPI2::NewTokensFetchJob(token, apiKey, secretKey);
                    connect(fetch, &Job::finished, this, &AuthWidgetPrivate::tokensReceived);
                });
        }
    } else {
        //qCDebug(KGAPIDebug) << "Unhandled page:" << url.host() << ", " << url.path();
    }
}

void AuthWidgetPrivate::tokensReceived(KGAPI2::Job* job)
{
    KGAPI2::NewTokensFetchJob *tokensFetchJob = qobject_cast<KGAPI2::NewTokensFetchJob*>(job);

    account->setAccessToken(tokensFetchJob->accessToken());
    account->setRefreshToken(tokensFetchJob->refreshToken());
    account->setExpireDateTime(QDateTime::currentDateTime().addSecs(tokensFetchJob->expiresIn()));
    tokensFetchJob->deleteLater();

    KGAPI2::AccountInfoFetchJob *fetchJob = new KGAPI2::AccountInfoFetchJob(account, this);

    connect(fetchJob, &Job::finished,
            this, &AuthWidgetPrivate::accountInfoReceived);
    qCDebug(KGAPIDebug) << "Requesting AccountInfo";
}

void AuthWidgetPrivate::accountInfoReceived(KGAPI2::Job* job)
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


#include "authwidget_p.moc"

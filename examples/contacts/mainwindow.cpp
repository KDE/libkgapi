/*
    Copyright (C) 2012  Jan Grulich <grulja@gmail.com>
    Copyright (C) 2012  Dan Vratil <dan@progdan.cz>

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


#include "mainwindow.h"
#include "ui_main.h"

#include <libkgapi/objects/contact.h>
#include <libkgapi/services/contacts.h>
#include <libkgapi/auth.h>
#include <libkgapi/accessmanager.h>
#include <libkgapi/fetchlistjob.h>
#include <libkgapi/request.h>
#include <libkgapi/reply.h>

#include <QListWidgetItem>
#include <QDebug>
#include <KJob>
#include <QStringBuilder>

MainWindow::MainWindow(QWidget * parent):
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    /* Initialice GUI */
    m_ui->setupUi(this);
    m_ui->errorLabel->setVisible(false);
    connect(m_ui->authButton, SIGNAL(clicked(bool)),
            this, SLOT(authenticate()));
    connect(m_ui->contactListButton, SIGNAL(clicked(bool)),
            this, SLOT(fetchContactList()));
    connect(m_ui->contactList, SIGNAL(itemSelectionChanged()),
            this, SLOT(contactSelected()));

    /* Retrieve instance of KGAPI::Auth */
    KGAPI::Auth * auth = KGAPI::Auth::instance();

    /* Initialize the KGAPI::Auth - set KWallet forlder and keys from
     * your Google application */
    auth->init(QLatin1String("KGAPI example"), QLatin1String("554041944266.apps.googleusercontent.com"),
               QLatin1String("mdT1DjzohxN3npUUzkENT0gO"));
    connect(auth, SIGNAL(authenticated(KGAPI::Account::Ptr&)),
            this, SLOT(authenticationFinished(KGAPI::Account::Ptr&)));
    connect(auth, SIGNAL(error(KGAPI::Error,QString)),
            this, SLOT(error(KGAPI::Error,QString)));

    /* Construct KGAPI::AccessManager which we will use to communicate with
     * Google */
    m_accessManager = new KGAPI::AccessManager();
    connect(m_accessManager, SIGNAL(replyReceived(KGAPI::Reply*)),
           this, SLOT(replyReceived(KGAPI::Reply*)));
    connect(m_accessManager, SIGNAL(error(KGAPI::Error,QString)),
           this, SLOT(error(KGAPI::Error,QString)));

}

MainWindow::~MainWindow()
{
    /* Remove the KGAPI::Account from KWallet */
    KGAPI::Auth *auth = KGAPI::Auth::instance();
    auth->revoke(m_account);

    delete m_ui;
}


void MainWindow::authenticate()
{
    KGAPI::Auth *auth = KGAPI::Auth::instance();

    /* Create a new account */
    KGAPI::Account::Ptr account(new KGAPI::Account());
    /* Set scopes we want the account to have access to */
    account->addScope(KGAPI::Services::Contacts::ScopeUrl);

    /* Try to authenticate, or fail */
    try {
        auth->authenticate(account, true);
    } catch (KGAPI::Exception::BaseException &e) {
        m_ui->errorLabel->setText(QLatin1String("<b>") % i18n("Error:") % QLatin1String("</b> ") % QLatin1String(e.what()));
        m_ui->errorLabel->setVisible(true);
    }
}

void MainWindow::authenticationFinished(KGAPI::Account::Ptr& account)
{
    m_account = account;

    m_ui->authStatusLabel->setText(i18n("Authenticated"));
    m_ui->contactListButton->setEnabled(true);
    m_ui->authButton->setEnabled(false);
}

void MainWindow::fetchContactList()
{
    if (m_account.isNull()) {
        m_ui->errorLabel->setText(QLatin1String("<b>") % i18n("Error:") % QLatin1String("</b> ") % i18n("Please authenticate first"));
        m_ui->errorLabel->setVisible(true);
        m_ui->authButton->setVisible(true);
        return;
    }

    /* Retrieve URL to which we will send the request */
    QUrl url = KGAPI::Services::Contacts::fetchAllContactsUrl(m_account->accountName(), true);

    KGAPI::FetchListJob *fetchJob;
    /* FetchList job is a comfortable way to retrieve many items from Google.
     * Set URL, name of service we want to use and name of the account to use */
    fetchJob = new KGAPI::FetchListJob(url, KGAPI::Services::Contacts::serviceName(), m_account->accountName());
    connect(fetchJob, SIGNAL(finished(KJob*)), this, SLOT(fetchJobFinished(KJob*)));
    fetchJob->setAutoDelete(true);
    fetchJob->start();

    m_ui->contactListButton->setEnabled(false);
}

void MainWindow::error(KGAPI::Error err, QString msg)
{
    Q_UNUSED(err);

    m_ui->errorLabel->setText(QLatin1String("<b>") % i18n("Error:") % QLatin1String("</b> ") % msg);
    m_ui->errorLabel->setVisible(true);
    m_ui->contactListButton->setEnabled(true);
}

void MainWindow::fetchJobFinished(KJob * job)
{
    if (job->error()) {
        m_ui->errorLabel->setText(QLatin1String("<b>") % i18n("Error:") % QLatin1String("</b> ") % job->errorString());
        m_ui->errorLabel->setVisible(true);
        m_ui->contactListButton->setEnabled(true);
        return;
    }

    KGAPI::FetchListJob *fetchJob = dynamic_cast<KGAPI::FetchListJob *>(job);

    /* Get all items the job has retrieved */
    QList< KGAPI::Object * > objects = fetchJob->items();

    Q_FOREACH (KGAPI::Object * object, objects) {

        KGAPI::Objects::Contact *contact = static_cast<KGAPI::Objects::Contact *>(object);

        /* Convert the contact to QListWidget item */
        QListWidgetItem *item = new QListWidgetItem(m_ui->contactList);
        item->setText(contact->name());
        item->setData(Qt::UserRole, contact->uid());

        m_ui->contactList->addItem(item);
    }

    m_ui->contactListButton->setEnabled(true);
}

void MainWindow::contactSelected()
{
    if (m_ui->contactList->selectedItems().count() == 0) {
        m_ui->contactInfo->clear();
        return;
    }

    QString id = m_ui->contactList->selectedItems().at(0)->data(Qt::UserRole).toString();
    QUrl url = KGAPI::Services::Contacts::fetchContactUrl(m_account->accountName(), id);

    /* Construct a new request that we will send to retrieve a specific selected contact */
    KGAPI::Request * request;
    request = new KGAPI::Request(url, KGAPI::Request::Fetch, KGAPI::Services::Contacts::serviceName(), m_account);

    m_accessManager->sendRequest(request);
}


void MainWindow::replyReceived(KGAPI::Reply * reply)
{
    m_ui->contactListButton->setEnabled(true);

    if (reply->error() != KGAPI::OK) {
        m_ui->errorLabel->setText(QLatin1String("<b>") % i18n("Error:") % QLatin1String("</b> ") % reply->errorString());
        m_ui->errorLabel->setVisible(true);
        delete reply;
        return;
    }

    /* Get all items we have received from Google (should be just one) */
    QList<KGAPI::Object *> info = reply->replyData();

    if (info.length() != 1) {
        m_ui->errorLabel->setText(QLatin1String("<b>") % i18n("Error:") % QLatin1String("</b> ") % i18n("Server sent unexpected amount of contacts"));
        m_ui->errorLabel->setVisible(true);
        delete reply;
        return;
    }

    KGAPI::Objects::Contact *contact = static_cast<KGAPI::Objects::Contact *>(info.first());

    QString text = QLatin1String("Name: ") % contact->name() % QLatin1Char('\n');

    if (!contact->phoneNumbers().isEmpty()) {
        text += QLatin1String("Phone: ") % contact->phoneNumbers().first().number() % QLatin1Char('\n');
    }

    if (!contact->emails().isEmpty()) {
        text += QLatin1String("Email: ") % contact->emails().first() % QLatin1Char('\n');
    }

    m_ui->contactInfo->setText(text);

    /* Always remember to delete the reply, otherwise your application will
     * have memory leaks */
    delete reply;
}

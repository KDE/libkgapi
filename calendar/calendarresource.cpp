/*
   Akonadi Google - Calendar Resource
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

#include "calendarresource.h"
#include "defaultreminderattribute.h"
#include "settings.h"
#include "settingsdialog.h"

#include <libkgoogle/common.h>
#include <libkgoogle/account.h>
#include <libkgoogle/accessmanager.h>
#include <libkgoogle/auth.h>
#include <libkgoogle/fetchlistjob.h>
#include <libkgoogle/request.h>
#include <libkgoogle/reply.h>
#include <libkgoogle/objects/calendar.h>
#include <libkgoogle/objects/event.h>
#include <libkgoogle/objects/task.h>
#include <libkgoogle/objects/tasklist.h>
#include <libkgoogle/services/calendar.h>
#include <libkgoogle/services/tasks.h>

#include <QtCore/QStringList>
#include <QtCore/QMetaType>

#include <KDE/KLocalizedString>
#include <KDE/KDialog>
#include <KDE/Akonadi/Attribute>
#include <KDE/Akonadi/AttributeFactory>
#include <KDE/Akonadi/CachePolicy>
#include <KDE/Akonadi/ChangeRecorder>
#include <KDE/Akonadi/CollectionFetchScope>
#include <KDE/Akonadi/EntityDisplayAttribute>
#include <KDE/Akonadi/ItemFetchJob>
#include <KDE/Akonadi/ItemFetchScope>

#ifdef WITH_KCAL
#include <KDE/KCal/Calendar>
using namespace KCal;
#else
#include <KDE/KCalCore/Calendar>
#include <libkgoogle/services/tasks.h>
using namespace KCalCore;
#endif

using namespace Akonadi;
using namespace KGoogle;

CalendarResource::CalendarResource(const QString &id):
    ResourceBase(id),
    m_fetchedCalendars(false),
    m_fetchedTaskLists(false)
{
    qRegisterMetaType< KGoogle::Services::Calendar >("Calendar");
    qRegisterMetaType< KGoogle::Services::Tasks >("Tasks");
    AttributeFactory::registerAttribute< DefaultReminderAttribute >();

    Auth *auth = Auth::instance();
    auth->init("Akonadi Google", Settings::self()->clientId(), Settings::self()->clientSecret());

    setNeedsNetwork(true);
    setOnline(true);

    m_gam = new AccessManager();
    connect(m_gam, SIGNAL(error(KGoogle::Error, QString)),
            this, SLOT(error(KGoogle::Error, QString)));
    connect(m_gam, SIGNAL(replyReceived(KGoogle::Reply*)),
            this, SLOT(replyReceived(KGoogle::Reply*)));

    connect(this, SIGNAL(abortRequested()),
            this, SLOT(slotAbortRequested()));

    changeRecorder()->itemFetchScope().fetchFullPayload(true);
    changeRecorder()->itemFetchScope().setAncestorRetrieval(ItemFetchScope::All);
    changeRecorder()->fetchCollection(true);
    changeRecorder()->collectionFetchScope().setAncestorRetrieval(CollectionFetchScope::All);
}

CalendarResource::~CalendarResource()
{
    delete m_gam;
}

void CalendarResource::aboutToQuit()
{
    slotAbortRequested();
}

void CalendarResource::abort()
{
    cancelTask(i18n("Aborted"));
}

void CalendarResource::slotAbortRequested()
{
    abort();
}

void CalendarResource::error(const KGoogle::Error errCode, const QString &msg)
{
    cancelTask(msg);

    if ((errCode == AuthError) || (errCode == BackendNotReady)) {
        status(Broken, msg);
    }
}

void CalendarResource::configure(WId windowId)
{
    SettingsDialog *settingsDialog = new SettingsDialog(windowId);

    if (settingsDialog->exec() == KDialog::Accepted) {
        emit configurationDialogAccepted();
        synchronize();
    } else {
        emit configurationDialogRejected();
    }

    delete settingsDialog;
}

void CalendarResource::retrieveItems(const Akonadi::Collection& collection)
{
    ItemFetchJob *fetchJob = new ItemFetchJob(collection, this);
    connect(fetchJob, SIGNAL(finished(KJob*)),
            this, SLOT(cachedItemsRetrieved(KJob*)));
    connect(fetchJob, SIGNAL(finished(KJob*)),
            fetchJob, SLOT(deleteLater()));

    /* Can't fetch any items for the top-level collection */
    if (collection.contentMimeTypes().contains(Collection::mimeType())) {
        itemsRetrievalDone();
        return;
    }

    fetchJob->fetchScope().fetchFullPayload(false);
    fetchJob->setProperty("collection", qVariantFromValue(collection));
    fetchJob->start();

    emit percent(0);
}

void CalendarResource::cachedItemsRetrieved(KJob* job)
{
    QUrl url;
    QString service;
    QString lastSync;

    Collection collection = job->property("collection").value<Collection>();

    if (collection.contentMimeTypes().contains(Event::eventMimeType())) {

        service = "Calendar";
        url = Services::Calendar::fetchEventsUrl(collection.remoteId());

    } else if (collection.contentMimeTypes().contains(Todo::todoMimeType())) {

        service = "Tasks";
        url = Services::Tasks::fetchAllTasksUrl(collection.remoteId());

    } else {

        emit cancelTask(i18n("Invalid collection"));
        return;

    }

    lastSync = collection.remoteRevision();
    if (!lastSync.isEmpty()) {
        KDateTime dt;
        dt.setTime_t(lastSync.toInt());
        lastSync = AccessManager::dateToRFC3339String(dt);

        url.addQueryItem("updated-min", lastSync);
    }

    url.addQueryItem("showDeleted", "true");

    FetchListJob *fetchJob = new FetchListJob(url, service, Settings::self()->account());
    fetchJob->setProperty("collection", qVariantFromValue(collection));
    connect(fetchJob, SIGNAL(finished(KJob*)),
            this, SLOT(itemsReceived(KJob*)));
    connect(fetchJob, SIGNAL(percent(KJob*, ulong)),
            this, SLOT(emitPercent(KJob*, ulong)));
    fetchJob->start();
}


bool CalendarResource::retrieveItem(const Akonadi::Item& item, const QSet< QByteArray >& parts)
{
    Q_UNUSED(parts);

    QString service;
    QUrl url;

    if (item.parentCollection().contentMimeTypes().contains(Event::eventMimeType())) {

        service = "Calendar";
        url = Services::Calendar::fetchEventUrl(item.parentCollection().remoteId(), item.remoteId());

    } else if (item.parentCollection().contentMimeTypes().contains(Todo::todoMimeType())) {

        service = "Tasks";
        url = Services::Tasks::fetchTaskUrl(item.parentCollection().remoteId(), item.remoteId());

    }

    Account::Ptr account;
    try {
        Auth *auth = Auth::instance();
        account = auth->getAccount(Settings::self()->account());
    } catch (Exception::BackendNotReady &e) {
        emit status(Broken, e.what());
        return false;
    }

    Request *request = new Request(url, KGoogle::Request::Fetch, service, account);
    request->setProperty("Item", QVariant::fromValue(item));
    m_gam->sendRequest(request);

    return true;
}

void CalendarResource::retrieveCollections()
{
    Akonadi::EntityDisplayAttribute *attr = new Akonadi::EntityDisplayAttribute();
    attr->setDisplayName(Settings::self()->account());

    Collection collection;
    collection.setName(identifier());
    collection.setRemoteId(identifier());
    collection.setParentCollection(Akonadi::Collection::root());
    collection.setContentMimeTypes(QStringList() << Collection::mimeType()
                                                 << Event::eventMimeType()
                                                 << Todo::todoMimeType());
    collection.addAttribute(attr);
    collection.setRights(Collection::ReadOnly);

    m_collections.clear();
    m_collections.append(collection);

    FetchListJob *fetchJob;

    fetchJob = new FetchListJob(Services::Calendar::fetchCalendarsUrl(), "Calendar", Settings::self()->account());
    connect(fetchJob, SIGNAL(finished(KJob*)),
            this, SLOT(calendarsReceived(KJob*)));
    fetchJob->start();

    fetchJob = new FetchListJob(Services::Tasks::fetchTaskListsUrl(),
                                "Tasks", Settings::self()->account());
    connect(fetchJob, SIGNAL(finished(KJob*)),
            this, SLOT(taskListReceived(KJob*)));
    fetchJob->start();
}

void CalendarResource::itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection)
{
    QString service;
    QUrl url;
    QByteArray data;

    if (item.mimeType() == Event::eventMimeType()) {

        EventPtr event = item.payload< EventPtr >();
        Objects::Event kevent(*event);

        service = "Calendar";
        url = Services::Calendar::createEventUrl(collection.remoteId());

        Services::Calendar service;
        kevent.setUid("");
        data = service.objectToJSON(static_cast< KGoogle::Object* >(&kevent));

    } else if (item.mimeType() == Todo::todoMimeType()) {

        TodoPtr todo = item.payload< TodoPtr >();
        todo->setUid("");
        Objects::Task ktodo(*todo);

        service = "Tasks";
        url = Services::Tasks::createTaskUrl(collection.remoteId());
        if (!todo->relatedTo(KCalCore::Incidence::RelTypeParent).isEmpty())
            url.addQueryItem("parent", todo->relatedTo(KCalCore::Incidence::RelTypeParent));

        Services::Tasks service;
        data = service.objectToJSON(static_cast< KGoogle::Object* >(&ktodo));

    } else {
        cancelTask(i18n("Unknown payload type '%1'").arg(item.mimeType()));
        return;
    }

    Account::Ptr account;
    try {
        Auth *auth = Auth::instance();
        account = auth->getAccount(Settings::self()->account());
    } catch (Exception::BaseException &e) {
        emit status(Broken, e.what());
        return;
    }

    Request *request = new Request(url, Request::Create, service, account);
    request->setRequestData(data, "application/json");
    request->setProperty("Item", QVariant::fromValue(item));
    request->setProperty("Collection", QVariant::fromValue(collection));

    m_gam->sendRequest(request);
}

void CalendarResource::itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers)
{
    QUrl url;
    QByteArray data;

    Account::Ptr account;
    try {
        Auth *auth = Auth::instance();
        account = auth->getAccount(Settings::self()->account());
    } catch (Exception::BaseException &e) {
        emit status(Broken, e.what());
        return;
    }

    if (item.mimeType() == Event::eventMimeType()) {

        EventPtr event = item.payload< EventPtr >();
        Objects::Event kevent(*event);

        url = Services::Calendar::updateEventUrl(item.parentCollection().remoteId(), item.remoteId());

        Services::Calendar service;
        data = service.objectToJSON(static_cast< KGoogle::Object* >(&kevent));

        Request *request = new Request(url, Request::Update, "Calendar", account);
        request->setRequestData(data, "application/json");
        request->setProperty("Item", QVariant::fromValue(item));

        m_gam->sendRequest(request);

    } else if (item.mimeType() == Todo::todoMimeType()) {

        TodoPtr todo = item.payload< TodoPtr >();
        Objects::Task ktodo(*todo);

        /* No way to tell whether item was moved or just updated, so if the item has
         * a parent the first do "MOVE" and when it's finished then send the actual
         * update request. Otherwise send only the update request.
         */
        if (!todo->relatedTo(Incidence::RelTypeParent).isEmpty()) {

            QUrl moveUrl = Services::Tasks::moveTaskUrl(item.parentCollection().remoteId(),
                           todo->uid(), todo->relatedTo(KCalCore::Incidence::RelTypeParent));
            Request *request = new Request(moveUrl, Request::Move, "Tasks", account);
            request->setProperty("Item", QVariant::fromValue(item));

            m_gam->sendRequest(request);

        } else {

            url = Services::Tasks::updateTaskUrl(item.parentCollection().remoteId(), item.remoteId());

            Services::Tasks service;
            data = service.objectToJSON(static_cast< KGoogle::Object* >(&ktodo));

            Request *request = new Request(url, Request::Update, "Tasks", account);
            request->setRequestData(data, "application/json");
            request->setProperty("Item", QVariant::fromValue(item));

            m_gam->sendRequest(request);

        }

    } else {
        cancelTask(i18n("Unknown payload type '%1'").arg(item.mimeType()));
        return;
    }

    Q_UNUSED(partIdentifiers);
}

void CalendarResource::itemRemoved(const Akonadi::Item& item)
{
    QString service;
    QUrl url;

    if (item.mimeType() == Event::eventMimeType()) {

        service = "Calendar";
        url = Services::Calendar::removeEventUrl(item.parentCollection().remoteId(), item.remoteId());

    } else if (item.mimeType() == Todo::todoMimeType()) {

        service = "Tasks";
        url = Services::Tasks::removeTaskUrl(item.parentCollection().remoteId(), item.remoteId());

    } else {
        cancelTask(i18n("Unknown payload type '%1'").arg(item.mimeType()));
        return;
    }

    Account::Ptr account;
    try {
        Auth *auth = Auth::instance();
        account = auth->getAccount(Settings::self()->account());
    } catch (Exception::BaseException &e) {
        emit status(Broken, e.what());
        return;
    }

    Request *request = new Request(url, Request::Remove, service, account);
    request->setProperty("Item", QVariant::fromValue(item));

    m_gam->sendRequest(request);
}

void CalendarResource::itemMoved(const Item& item, const Collection& collectionSource, const Collection& collectionDestination)
{
    QString service;
    QUrl url;

    /* Moving tasks between task lists is not supported */
    if (item.mimeType() != Event::eventMimeType())
        return;

    Account::Ptr account;
    try {
        Auth *auth = Auth::instance();
        account = auth->getAccount(Settings::self()->account());
    } catch (Exception::BaseException &e) {
        emit status(Broken, e.what());
        return;
    }

    url = Services::Calendar::moveEventUrl(collectionSource.remoteId(), collectionDestination.remoteId(), item.remoteId());
    Request *request = new Request(url, KGoogle::Request::Move, "Calendar", account);
    request->setProperty("Item", qVariantFromValue(item));

    m_gam->sendRequest(request);
}


void CalendarResource::replyReceived(KGoogle::Reply* reply)
{
    switch (reply->requestType()) {
    case Request::FetchAll:
        /* Handled by FetchListJob */
        break;

    case Request::Fetch:
        itemReceived(reply);
        break;

    case Request::Create:
        itemCreated(reply);
        break;

    case Request::Update:
        itemUpdated(reply);
        break;

    case Request::Remove:
        itemRemoved(reply);
        break;

    case Request::Move:
        itemMoved(reply);
        break;
    }
}

void CalendarResource::itemMoved(Reply* reply)
{
    if (reply->serviceName() == "Calendar") {

        eventMoved(reply);

    } else if (reply->serviceName() == "Tasks") {

        taskDoUpdate(reply);

    } else {

        cancelTask(i18n("Received invalir reply"));

    }
}

void CalendarResource::itemsReceived(KJob *job)
{
    FetchListJob *fetchJob = dynamic_cast< FetchListJob* >(job);

    if (fetchJob->service() == "Calendar") {

        eventsReceived(job);

    } else if (fetchJob->service() == "Tasks") {

        tasksReceived(job);

    } else {

        cancelTask(i18n("Received invalid reply"));

    }
}

void CalendarResource::itemReceived(Reply* reply)
{
    if (reply->serviceName() == "Calendar") {

        eventReceived(reply);

    } else if (reply->serviceName() == "Tasks") {

        taskReceived(reply);

    } else {

        cancelTask(i18n("Received an invalid reply"));

    }

    delete reply;

}

void CalendarResource::itemCreated(Reply* reply)
{
    if (reply->serviceName() == "Calendar") {

        eventCreated(reply);

    } else if (reply->serviceName() == "Tasks") {

        taskCreated(reply);

    } else {

        cancelTask(i18n("Received an invalid reply"));

    }

    delete reply;

}

void CalendarResource::itemUpdated(Reply* reply)
{
    if (reply->serviceName() == "Calendar") {

        eventUpdated(reply);

    } else if (reply->serviceName() == "Tasks") {

        taskUpdated(reply);

    } else {

        cancelTask(i18n("Received an invalid reply"));

    }

    delete reply;

}

void CalendarResource::itemRemoved(Reply* reply)
{
    if (reply->serviceName() == "Calendar") {

        eventRemoved(reply);

    } else if (reply->serviceName() == "Tasks") {

        taskRemoved(reply);

    } else {

        cancelTask(i18n("Received an invalid reply"));

    }

    delete reply;
}

void CalendarResource::emitPercent(KJob* job, ulong progress)
{
    Q_UNUSED(job);

    emit percent(progress);
}


AKONADI_RESOURCE_MAIN(CalendarResource);
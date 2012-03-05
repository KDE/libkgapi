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
#include "settings.h"

#include <libkgoogle/accessmanager.h>
#include <libkgoogle/auth.h>
#include <libkgoogle/fetchlistjob.h>
#include <libkgoogle/reply.h>
#include <libkgoogle/objects/task.h>
#include <libkgoogle/objects/tasklist.h>
#include <libkgoogle/services/tasks.h>

#include <KDE/KLocalizedString>
#include <KDE/KDebug>

#include <KDE/Akonadi/ItemModifyJob>
#include <KDE/Akonadi/EntityDisplayAttribute>
#include <KDE/Akonadi/CollectionModifyJob>

using namespace Akonadi;
using namespace KGoogle;

#ifdef WITH_KCAL
using namespace KCal;
#else
using namespace KCalCore;
#endif


void CalendarResource::taskDoUpdate(Reply* reply)
{
    Item item = reply->request()->property("Item").value< Item >();
    TodoPtr todo = item.payload< TodoPtr >();
    Objects::Task ktodo(*todo);

    Account::Ptr account;
    try {
        Auth *auth = Auth::instance();
        account = auth->getAccount(Settings::self()->account());
    } catch (Exception::BaseException &e) {
        emit status(Broken, e.what());
        return;
    }

    QUrl url = Services::Tasks::updateTaskUrl(item.parentCollection().remoteId(), item.remoteId());

    Services::Tasks service;
    QByteArray data = service.objectToJSON(static_cast< KGoogle::Object *>(&ktodo));

    Request *request = new Request(url, Request::Update, "Tasks", account);
    request->setRequestData(data, "application/json");
    request->setProperty("Item", QVariant::fromValue(item));
    m_gam->sendRequest(request);
}


void CalendarResource::taskListReceived(KJob *job)
{
    if (job->error()) {
        cancelTask();
        emit status(Broken, i18n("Failed to fetch task lists"));
        return;
    }

    FetchListJob *fetchJob = dynamic_cast< FetchListJob* >(job);

    QStringList taskLists = Settings::self()->taskLists();

    QList< Object* > data = fetchJob->items();
    foreach(Object * replyData, data) {

        Objects::TaskList *taskList = static_cast< Objects::TaskList* >(replyData);

        if (!taskLists.contains(taskList->uid()))
            continue;

        Collection collection;
        collection.setRemoteId(taskList->uid());
        collection.setParentCollection(m_collections.first());
        collection.setContentMimeTypes(QStringList() << Todo::todoMimeType());
        collection.setName(taskList->title());
        collection.setRights(Collection::AllRights);

        EntityDisplayAttribute *attr = new EntityDisplayAttribute;
        attr->setDisplayName(taskList->title());
        attr->setIconName("text-calendar");
        collection.addAttribute(attr);

        m_collections.append(collection);

    }

    m_fetchedTaskLists = true;

    if (m_fetchedTaskLists && m_fetchedCalendars) {
        collectionsRetrieved(m_collections);
        m_collections.clear();

        m_fetchedCalendars = false;
        m_fetchedTaskLists = false;
    }
}

void CalendarResource::taskReceived(KGoogle::Reply *reply)
{
    if (reply->error() != OK) {
        cancelTask(i18n("Failed to fetch task: %1").arg(reply->errorString()));
        return;
    }

    QList< Object* > data = reply->replyData();
    if (data.length() != 1) {
        kWarning() << "Server send " << data.length() << "items, which is not OK";
        cancelTask(i18n("Expected a single item, server sent %1 items.").arg(data.length()));
        return;
    }

    Objects::Task *task = static_cast< Objects::Task* >(data.first());

    Item item = reply->request()->property("Item").value<Item>();
    item.setRemoteId(task->uid());
    item.setRemoteRevision(task->etag());
    item.setMimeType(Todo::todoMimeType());
    item.setPayload< TodoPtr >(TodoPtr(task));

    if (static_cast< Objects::Task* >(task)->deleted()) {
        itemsRetrievedIncremental(Item::List(), Item::List() << item);
    } else {
        itemRetrieved(item);
    }
}

void CalendarResource::tasksReceived(KJob *job)
{
    if (job->error()) {
        cancelTask(i18n("Failed to fetch tasks: %1").arg(job->errorString()));
        return;
    }

    FetchListJob *fetchJob = dynamic_cast< FetchListJob* >(job);
    Collection collection = fetchJob->property("collection").value< Collection >();

    Item::List removed;
    Item::List changed;

    QList< Object* > data = fetchJob->items();
    foreach(Object * replyData, data) {

        Objects::Task *task = static_cast< Objects::Task* >(replyData);

        Item item;
        item.setRemoteId(task->uid());
        item.setRemoteRevision(task->etag());
        item.setPayload< TodoPtr >(TodoPtr(task));
        item.setMimeType(Todo::todoMimeType());
        item.setParentCollection(collection);

        if (task->deleted()) {
            removed << item;
        } else {
            changed << item;
        }

    }

    itemsRetrievedIncremental(changed, removed);

    collection.setRemoteRevision(QString::number(KDateTime::currentUtcDateTime().toTime_t()));
    CollectionModifyJob *modifyJob = new CollectionModifyJob(collection, this);
    modifyJob->setAutoDelete(true);
    modifyJob->start();
}

void CalendarResource::taskCreated(KGoogle::Reply *reply)
{
    if (reply->error() != OK) {
        cancelTask(i18n("Failed to create a task: %1").arg(reply->errorString()));
        return;
    }

    QList< Object* > data = reply->replyData();
    if (data.length() != 1) {
        kWarning() << "Server send " << data.length() << "items, which is not OK";
        cancelTask(i18n("Expected a single item, server sent %1 items.").arg(data.length()));
        return;
    }

    Objects::Task *task = static_cast< Objects::Task* >(data.first());

    Item item = reply->request()->property("Item").value<Item>();
    item.setRemoteId(task->uid());
    item.setRemoteRevision(task->etag());
    item.setMimeType(Todo::todoMimeType());
    item.setPayload< TodoPtr >(TodoPtr(task));
    item.setParentCollection(reply->request()->property("Collection").value<Collection>());

    changeCommitted(item);

    Akonadi::ItemModifyJob *modifyJob = new Akonadi::ItemModifyJob(item);
    connect(modifyJob, SIGNAL(finished(KJob*)), modifyJob, SLOT(deleteLater()));
}

void CalendarResource::taskUpdated(KGoogle::Reply *reply)
{
    if (reply->error() != OK) {
        cancelTask(i18n("Failed to update task: %1").arg(reply->errorString()));
        return;
    }

    QList< Object* > data = reply->replyData();
    if (data.length() != 1) {
        kWarning() << "Server send " << data.length() << "items, which is not OK";
        cancelTask(i18n("Expected a single item, server sent %1 items.").arg(data.length()));
        return;
    }

    Objects::Task *task = static_cast< Objects::Task* >(data.first());

    Item item = reply->request()->property("Item").value<Item>();
    item.setRemoteRevision(task->etag());
    item.setPayload< TodoPtr >(TodoPtr(task));

    changeCommitted(item);
}

void CalendarResource::taskRemoved(KGoogle::Reply *reply)
{
    if (reply->error() != OK) {
        cancelTask(i18n("Failed to delete task: %1").arg(reply->errorString()));
        return;
    }

    Item item = reply->request()->property("Item").value<Item>();
    changeCommitted(item);
}

/*
    Akonadi Google - Contacts resource - Fetch Virtual Contacts Job
    Copyright (C) 2012  Dan Vratil <dan@progdan.cz>

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


#include "fetchvirtualcontactsjob.h"
#include "groupattribute.h"

#include <akonadi/itemfetchjob.h>
#include <akonadi/itemfetchscope.h>
#include <akonadi/linkjob.h>
#include <akonadi/unlinkjob.h>

#include <boost/bind.hpp>

using namespace Akonadi;

FetchVirtualContactsJob::FetchVirtualContactsJob(const Akonadi::Collection &collection,
						 const Akonadi::Collection &rootCollection):
  KJob(),
  m_collection(collection),
  m_rootCollection(rootCollection)
{ }


FetchVirtualContactsJob::~FetchVirtualContactsJob()
{ }

void FetchVirtualContactsJob::start()
{
  ItemFetchJob *fetchJob = new ItemFetchJob(m_collection);
  fetchJob->fetchScope().fetchFullPayload(true);
  fetchJob->setAutoDelete(true);
  connect(fetchJob, SIGNAL(result(KJob*)),
	  this, SLOT(rootCollectionFetchJobFinished(KJob*)));

  fetchJob->start();
  qDebug() << "FetchVirtualContactsJob started";
}

void FetchVirtualContactsJob::rootCollectionFetchJobFinished(KJob *job)
{
  ItemFetchJob *fetchJob = dynamic_cast< ItemFetchJob* >(job);

  Item::List existingItems = fetchJob->items();

  ItemFetchJob *rootItemsJob = new ItemFetchJob(m_rootCollection);
  rootItemsJob->fetchScope().fetchFullPayload(false);
  rootItemsJob->fetchScope().fetchAttribute("group", true);
  rootItemsJob->exec();

  Item::List rootItems = rootItemsJob->items();
  Item::List memberItems;

  foreach (Item rootItem, rootItems) {

    GroupAttribute *attr = rootItem.attribute< GroupAttribute >();
    if (!attr)
      continue;

    if (attr->containsGroup(m_collection.remoteId()))
      m_toLink << rootItem;

  }

  UnlinkJob *unlinkJob = new UnlinkJob(m_collection, rootItems);
  unlinkJob->setAutoDelete(true);
  connect(unlinkJob, SIGNAL(finished(KJob*)),
          this, SLOT(unlinkJobFinished(KJob*)));
}

void FetchVirtualContactsJob::unlinkJobFinished(KJob* job)
{
  LinkJob *linkJob = new LinkJob(m_collection, m_toLink);
  linkJob->setAutoDelete(true);
  connect(linkJob, SIGNAL(finished(KJob*)),
          this, SLOT(linkJobFinished(KJob*)));
}

void FetchVirtualContactsJob::linkJobFinished(KJob* )
{
  emitResult();
}


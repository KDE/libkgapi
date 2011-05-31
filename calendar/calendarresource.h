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


#ifndef CALENDARRESOURCE_H
#define CALENDARRESOURCE_H

#include <Akonadi/AgentBase>
#include <Akonadi/ResourceBase>

#include <KDE/KJob>

class CalendarResource : public Akonadi::ResourceBase, public Akonadi::AgentBase::Observer
{

  public:
    CalendarResource(const QString &id);
    ~CalendarResource();

    void configure(WId windowId);
    
    void itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection);
    void itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers);
    void itemRemoved(const Akonadi::Item& item);

  protected:
    void retrieveCollections();
    void retrieveItems(const Akonadi::Collection& collection);
    bool retrieveItem(const Akonadi::Item& item, const QSet< QByteArray >& parts);
    
  private Q_SLOTS:
    void slotAbortRequested();
    void resetState();
    void abort();
    
  private:
    QList<KJob*> m_currentJobs;

};

#endif // CALENDARRESOURCE_H

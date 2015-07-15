/*
 * This file is part of LibKGAPI library
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


#ifndef LIBKGAPI2_PRIVATE_QUEUEHELPER_H
#define LIBKGAPI2_PRIVATE_QUEUEHELPER_H

#include <QQueue>

template<typename T>
class QueueHelper
{
  public:
    explicit QueueHelper() {}
    virtual ~QueueHelper() {}

    bool atEnd() const { return (m_iter == m_items.constEnd()); }

    void currentProcessed() { ++m_iter; }

    T current() { return *m_iter; }

    QueueHelper& operator<<(const T &item)
    {
        m_items << item;
        if (m_items.count() == 1) {
            m_iter = m_items.constBegin();
        }
        return *this;
    }

    QueueHelper& operator<<(const QList<T> &list)
    {
        if (m_items.count() == 0) {
            m_items << list;
            m_iter = m_items.constBegin();
        } else {
            m_items << list;
        }

        return *this;
    }

    QueueHelper& operator=(const QList<T> &list )
    {
        m_items.clear();
        m_items << list;
        m_iter = m_items.constBegin();
        return *this;
    }

  private:
    QQueue<T> m_items;

    typename
    QList<T>::ConstIterator m_iter;
};

#endif // QUEUEHELPER_H

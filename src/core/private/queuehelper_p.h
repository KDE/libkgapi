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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef LIBKGAPI2_PRIVATE_QUEUEHELPER_P_H
#define LIBKGAPI2_PRIVATE_QUEUEHELPER_P_H

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

    void reserve(int n) { m_items.reserve(n); }

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

#endif // LIBKGAPI2_PRIVATE_QUEUEHELPER_P_H

/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#ifndef LIBKGAPI2_PRIVATE_QUEUEHELPER_P_H
#define LIBKGAPI2_PRIVATE_QUEUEHELPER_P_H

#include <QQueue>

template<typename T>
class QueueHelper
{
  public:
    bool atEnd() const { return (m_iter == m_items.constEnd()); }

    void currentProcessed() { ++m_iter; }

    T current() { return *m_iter; }

    void enqueue(const T &item)
    {
        m_items.push_back(item);
        if (m_items.size() == 1) {
            m_iter = m_items.constBegin();
        }
    }

    void enqueue(T &&item)
    {
        m_items.push_back(std::move(item));
        if (m_items.size() == 1) {
            m_iter = m_items.constBegin();
        }
    }

    void enqueue(const QList<T> &list)
    {
        if (m_items.empty()) {
            m_items.append(list);
            m_iter = m_items.constBegin();
        } else {
            m_items.append(list);
        }
    }

    void reserve(int n) { m_items.reserve(n); }

    QueueHelper& operator=(const QList<T> &list )
    {
        m_items.clear();
        m_items.append(list);
        m_iter = m_items.constBegin();
        return *this;
    }

  private:
    QQueue<T> m_items;

    typename QList<T>::ConstIterator m_iter;
};

#endif // LIBKGAPI2_PRIVATE_QUEUEHELPER_P_H

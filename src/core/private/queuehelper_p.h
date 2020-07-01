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

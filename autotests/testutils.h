/*
 * Copyright (C) 2018  Daniel Vrátil <dvratil@kde.org>
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
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include "fakenetworkaccessmanager.h"

#include <QTest>

#define VERIFY2_RET(cond, msg, ret) \
    do { \
        if (cond) { \
            if (!QTest::qVerify(true, #cond, (msg), __FILE__, __LINE__)) \
                return ret; \
        } else { \
            if (!QTest::qVerify(false, #cond, (msg), __FILE__, __LINE__)) \
                return ret; \
        } \
    } while (false)

#define VERIFY_RET(cond, ret) VERIFY2_RET(cond, #cond, ret)

#define COMPARE_RET(actual, expected, ret) \
    do { \
        if (!QTest::qCompare(actual, expected, #actual, #expected, __FILE__, __LINE__)) \
            return ret; \
    } while (false)

#define FAIL_RET(msg, ret) \
    do { \
        QTest::qFail(msg, __FILE__, __LINE__); \
        return ret; \
    } while(false)

namespace KGAPI2 {
class Job;
}

bool execJob(KGAPI2::Job *job);

FakeNetworkAccessManager::Scenario scenarioFromFile(const QString &request,
                                                    const QString &response,
                                                    bool needsAuth = true);

QByteArray reformatJSON(const QByteArray &json);
QByteArray reformatXML(const QByteArray &xml);
QByteArray diffData(const QByteArray &actual, const QByteArray &expected);


template<typename T>
QStringList elementsToIds(const QList<T> &elems)
{
    QStringList ids;
    ids.reserve(elems.size());
    for (const auto &elem : elems) {
        ids.push_back(elem->id());
    }
    return ids;
}

template<typename T>
QStringList elementsToUids(const QList<T> &elems)
{
    QStringList uids;
    uids.reserve(elems.size());
    for (const auto &elem : elems) {
        uids.push_back(elem->uid());
    }
    return uids;
}

#endif

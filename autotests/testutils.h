/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fakenetworkaccessmanager.h"

#include <QTest>

#define VERIFY2_RET(cond, msg, ret)                                                                                                                            \
    do {                                                                                                                                                       \
        if (cond) {                                                                                                                                            \
            if (!QTest::qVerify(true, #cond, (msg), __FILE__, __LINE__))                                                                                       \
                return ret;                                                                                                                                    \
        } else {                                                                                                                                               \
            if (!QTest::qVerify(false, #cond, (msg), __FILE__, __LINE__))                                                                                      \
                return ret;                                                                                                                                    \
        }                                                                                                                                                      \
    } while (false)

#define VERIFY_RET(cond, ret) VERIFY2_RET(cond, #cond, ret)

#define COMPARE_RET(actual, expected, ret)                                                                                                                     \
    do {                                                                                                                                                       \
        if (!QTest::qCompare(actual, expected, #actual, #expected, __FILE__, __LINE__))                                                                        \
            return ret;                                                                                                                                        \
    } while (false)

#define FAIL_RET(msg, ret)                                                                                                                                     \
    do {                                                                                                                                                       \
        QTest::qFail(msg, __FILE__, __LINE__);                                                                                                                 \
        return ret;                                                                                                                                            \
    } while (false)

namespace KGAPI2
{
class Job;
}

bool execJob(KGAPI2::Job *job);

FakeNetworkAccessManager::Scenario scenarioFromFile(const QString &request, const QString &response, bool needsAuth = true);

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

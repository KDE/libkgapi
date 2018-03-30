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

#include "testutils.h"
#include "job.h"

#include <QEventLoop>
#include <QFile>
#include <QProcess>
#include <QTemporaryFile>

#include <iostream>

bool execJob(KGAPI2::Job* job)
{
    QEventLoop loop;
    QObject::connect(job, &KGAPI2::Job::finished, &loop, &QEventLoop::quit);
    return loop.exec() == 0;
}

FakeNetworkAccessManager::Scenario scenarioFromFile(const QString &requestFile,
                                                    const QString &responseFile)
{
    QFile request(requestFile);
    VERIFY_RET(request.open(QIODevice::ReadOnly), {});

    FakeNetworkAccessManager::Scenario scenario;
    auto http = request.readLine().trimmed();
    if (http.startsWith("PUT")) {
        scenario.requestMethod = QNetworkAccessManager::PutOperation;
        scenario.requestUrl = QUrl(QString::fromLatin1(http.constData() + 4));
    } else if (http.startsWith("POST")) {
        scenario.requestMethod = QNetworkAccessManager::PostOperation;
        scenario.requestUrl = QUrl(QString::fromLatin1(http.constData() + 5));
    } else if (http.startsWith("GET")) {
        scenario.requestMethod = QNetworkAccessManager::GetOperation;
        scenario.requestUrl = QUrl(QString::fromLatin1(http.constData() + 4));
    } else if (http.startsWith("DELETE")) {
        scenario.requestMethod = QNetworkAccessManager::DeleteOperation;
        scenario.requestUrl = QUrl(QString::fromLatin1(http.constData() + 7));
    } else {
        FAIL_RET("Invalid request method in test data", {});
    }

    auto line = request.readLine().trimmed();
    while (!line.isEmpty()) {
        const int idx = line.indexOf(':');
        scenario.requestHeaders.push_back(qMakePair(line.left(idx), line.mid(idx + 1).trimmed()));

        line = request.readLine().trimmed();
    }
    scenario.requestData = request.readAll();

    QFile response(responseFile);
    VERIFY_RET(response.open(QIODevice::ReadOnly), {});

    http = response.readLine();
    scenario.responseCode = http.mid(9, 3).toInt();

    line = response.readLine().trimmed();
    while (!line.isEmpty()) {
        const int idx = line.indexOf(':');
        scenario.responseHeaders.push_back(qMakePair(line.left(idx), line.mid(idx + 1).trimmed()));

        line = response.readLine().trimmed();
    }
    scenario.responseData = response.readAll();

    return scenario;
}

QByteArray reformatXML(const QByteArray &xml)
{
#ifdef Q_OS_UNIX
    QProcess lint;
    lint.start(QStringLiteral("xmllint"), { QStringLiteral("--format"), QStringLiteral("-") });
    lint.waitForStarted();
    if (lint.state() != QProcess::Running) { // missing xmllint?
        return xml;
    }
    if (!xml.startsWith("<?xml")) {
        lint.write("<?xml version=\"1.0\" ?>");
    }
    lint.write(xml);
    lint.closeWriteChannel();
    lint.waitForFinished();
    const auto err = lint.readAllStandardError();
    if (!err.isEmpty()) {
        std::cerr << err.constData() << std::endl;
        FAIL_RET("Malformed XML!", xml);
    }
    return lint.readAllStandardOutput();
#else
    return xml;
#endif
}

QByteArray reformatJSON(const QByteArray &json)
{
#ifdef Q_OS_UNIX
    QProcess lint;
    lint.start(QStringLiteral("json_reformat"));
    lint.waitForStarted();
    if (lint.state() != QProcess::Running) { // missing json_reformat?
        return json;
    }
    lint.write(json);
    lint.closeWriteChannel();
    lint.waitForFinished();
    const auto err = lint.readAllStandardError();
    if (!err.isEmpty()) {
        std::cerr << err.constData() << std::endl;
        FAIL_RET("Malformed JSON!", json);
    }
    return lint.readAllStandardOutput();
#else
    return json;
#endif
}

QByteArray diffData(const QByteArray &actual, const QByteArray &expected)
{
#ifdef Q_OS_UNIX
    QTemporaryFile tmpActual(QStringLiteral("%1-actual.XXXXXX").arg(QCoreApplication::applicationName()));
    tmpActual.open();
    tmpActual.write(actual);
    tmpActual.close();

    QTemporaryFile tmpExpected(QStringLiteral("%1-expected.XXXXXX").arg(QCoreApplication::applicationName()));
    tmpExpected.open();
    tmpExpected.write(expected);
    tmpExpected.close();

    QProcess diff;
    diff.start(QStringLiteral("diff"), { QStringLiteral("-a"), QStringLiteral("-u"),
                tmpActual.fileName(), tmpExpected.fileName() });
    diff.waitForStarted();
    if (diff.state() != QProcess::Running) {
        return "======== ACTUAL ========\n\n" + actual + "\n\n"
               "======= EXPECTED =======\n\n" + expected + "\n\n"
               "========================";
    }
    diff.waitForFinished();
    return diff.readAll();
#else
    return "======== ACTUAL ========\n\n" + actual + "\n\n"
           "======= EXPECTED =======\n\n" + expected + "\n\n"
           "========================";
#endif
}

/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "testutils.h"
#include "job.h"

#include <QEventLoop>
#include <QFile>
#include <QJsonDocument>
#include <QProcess>
#include <QTemporaryFile>

#include <iostream>

bool execJob(KGAPI2::Job *job)
{
    QEventLoop loop;
    QObject::connect(job, &KGAPI2::Job::finished, &loop, &QEventLoop::quit);
    return loop.exec() == 0;
}

FakeNetworkAccessManager::Scenario scenarioFromFile(const QString &requestFile, const QString &responseFile, bool needsAuth)
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
    scenario.needsAuth = needsAuth;

    return scenario;
}

QByteArray reformatXML(const QByteArray &xml)
{
#ifdef Q_OS_UNIX
    QProcess lint;
    lint.start(QStringLiteral("xmllint"), {QStringLiteral("--format"), QStringLiteral("-")});
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
    return QJsonDocument::fromJson(json).toJson(QJsonDocument::Indented);
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
    diff.start(QStringLiteral("diff"), {QStringLiteral("-a"), QStringLiteral("-u"), tmpActual.fileName(), tmpExpected.fileName()});
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

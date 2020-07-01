/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI_JOB_P_H
#define KGAPI_JOB_P_H

#include "job.h"

#include <QQueue>
#include <QTimer>
#include <QNetworkReply>
#include <QScopedPointer>

class QFile;

namespace KGAPI2 {

struct Request
{
    QNetworkRequest request;
    QByteArray rawData;
    QString contentType;
};

class Q_DECL_HIDDEN FileLogger
{
public:
    ~FileLogger();

    static FileLogger *self();

    void logRequest(const QNetworkRequest &request, const QByteArray &rawData);
    void logReply(const QNetworkReply *reply, const QByteArray &rawData);

private:
    FileLogger();
    QScopedPointer<QFile> mFile;

    static FileLogger *sInstance;
};

class Q_DECL_HIDDEN Job::Private
{
  public:
    Private(Job *parent);
    void init();

    QString parseErrorMessage(const QByteArray &json);

    void _k_doStart();
    void _k_doEmitFinished();
    void _k_replyReceived(QNetworkReply *reply);
    void _k_dispatchTimeout();

    bool isRunning;

    Error error;
    QString errorString;

    AccountPtr account;
    QNetworkAccessManager *accessManager;
    QQueue<Request> requestQueue;
    QTimer *dispatchTimer;
    int maxTimeout;
    bool prettyPrint;
    QStringList fields;

    Request currentRequest;

  private:
    Job * const q;
};

}

#endif // KGAPI_JOB_P_H

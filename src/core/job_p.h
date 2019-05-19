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

    Request currentRequest;

  private:
    Job * const q;
};

}

#endif // KGAPI_JOB_P_H

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

#ifndef KGAPI_JOB_P_H
#define KGAPI_JOB_P_H

#include "job.h"
#include <QtCore/QQueue>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkReply>

#include <KIO/AccessManager>

namespace KGAPI2 {

struct Request
{
    QNetworkRequest request;
    QByteArray rawData;
    QString contentType;
};

class Job::Private
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
    KIO::Integration::AccessManager *accessManager;
    QQueue<Request> requestQueue;
    QTimer *dispatchTimer;
    int maxTimeout;

    Request currentRequest;

  private:
    Job * const q;
};

}

#endif // KGAPI_JOB_P_H

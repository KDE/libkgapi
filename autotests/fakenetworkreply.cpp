/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "fakenetworkreply.h"
#include "types.h"

FakeNetworkReply::FakeNetworkReply(const FakeNetworkAccessManager::Scenario &scenario)
    : QNetworkReply()
{
    setUrl(scenario.requestUrl);
    setOperation(scenario.requestMethod);
    setAttribute(QNetworkRequest::HttpStatusCodeAttribute, scenario.responseCode);
    setHeader(QNetworkRequest::ContentLengthHeader, scenario.responseData.size());
    if (scenario.responseData.startsWith('<')) {
        setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/atom+xml"));
    } else if (scenario.responseData.startsWith('{')) {
        setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    } else {
        setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("text/plain"));
    }
    for (const auto &header : std::as_const(scenario.responseHeaders)) {
        setRawHeader(header.first, header.second);
    }

    if (scenario.responseCode == KGAPI2::TemporarilyMoved) {
        setHeader(QNetworkRequest::LocationHeader, QString::fromUtf8(scenario.responseData));
    } else {
        mBuffer.setData(scenario.responseData);
    }
    mBuffer.open(QIODevice::ReadOnly);

    open(QIODevice::ReadOnly);
    setFinished(true);
    QMetaObject::invokeMethod(this, "readyRead", Qt::QueuedConnection);
    QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
}

FakeNetworkReply::FakeNetworkReply(QNetworkAccessManager::Operation method, const QNetworkRequest &originalRequest)
    : QNetworkReply()
{
    setOperation(method);
    setUrl(originalRequest.url());

    open(QIODevice::ReadOnly);
    setFinished(true);
    QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection, Q_ARG(QNetworkReply::NetworkError, QNetworkReply::UnknownServerError));
    QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
}

void FakeNetworkReply::abort()
{
    // NOOP
}

bool FakeNetworkReply::atEnd() const
{
    return mBuffer.atEnd();
}

qint64 FakeNetworkReply::bytesAvailable() const
{
    return mBuffer.bytesAvailable();
}

bool FakeNetworkReply::canReadLine() const
{
    return mBuffer.canReadLine();
}

void FakeNetworkReply::close()
{
    mBuffer.close();
}

qint64 FakeNetworkReply::pos() const
{
    return mBuffer.pos();
}

qint64 FakeNetworkReply::size() const
{
    return mBuffer.size();
}

qint64 FakeNetworkReply::readData(char *data, qint64 maxLen)
{
    return mBuffer.read(data, maxLen);
}

qint64 FakeNetworkReply::writeData(const char *data, qint64 len)
{
    return mBuffer.write(data, len);
}

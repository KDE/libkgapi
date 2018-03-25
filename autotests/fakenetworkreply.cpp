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
    for (const auto header : qAsConst(scenario.responseHeaders)) {
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
    QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection,
                              Q_ARG(QNetworkReply::NetworkError, QNetworkReply::UnknownServerError));
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

qint64 FakeNetworkReply::readData(char* data, qint64 maxLen)
{
    return mBuffer.read(data, maxLen);
}

qint64 FakeNetworkReply::writeData(const char* data, qint64 len)
{
    return mBuffer.write(data, len);
}

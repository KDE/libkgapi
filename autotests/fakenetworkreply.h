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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef KGAPI_FAKENETWORKREPLY_H_
#define KGAPI_FAKENETWORKREPLY_H_

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QBuffer>

#include "fakenetworkaccessmanager.h"

class FakeNetworkReply : public QNetworkReply
{
    Q_OBJECT
public:
    FakeNetworkReply(const FakeNetworkAccessManager::Scenario &scenario);
    explicit FakeNetworkReply(QNetworkAccessManager::Operation operation,
                              const QNetworkRequest &originalRequest);

    void abort() override;

    bool atEnd() const override;
    qint64 bytesAvailable() const override;
    bool canReadLine() const override;
    void close() override;
    qint64 size() const override;
    qint64 pos() const override;

protected:
    qint64 readData(char *data, qint64 maxLen) override;
    qint64 writeData(const char *data, qint64 len) override;

private:
    QBuffer mBuffer;
};


#endif

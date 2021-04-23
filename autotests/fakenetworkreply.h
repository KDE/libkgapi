/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

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



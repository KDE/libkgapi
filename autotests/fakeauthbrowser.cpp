/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "fakeauthbrowser.h"

#include <QObject>
#include <QDesktopServices>
#include <QTimer>
#include <QTcpSocket>
#include <QHostAddress>

extern Q_DECL_IMPORT uint16_t kgapiTcpAuthServerPort; // defined in authjob.cpp

class FakeAuthBrowser::Private : public QObject
{
    Q_OBJECT
public:
    explicit Private()
    {
        kgapiTcpAuthServerPort = 42413;
    }

public Q_SLOTS:
    void openUrl(const QUrl &url)
    {
        Q_UNUSED(url)

        // don't do anything, don't even try to load Google auth page. Instead
        // pretend the user have already authenticated and we've reached the
        // part where Google sends us the auth code

        QTimer::singleShot(0, this, [=]() {
            QTcpSocket socket;
            socket.connectToHost(QHostAddress::LocalHost, kgapiTcpAuthServerPort);
            if (!socket.waitForConnected()) {
                qWarning() << "Failed to connect to internal TCP server!";
                return;
            }
            socket.write("GET http://127.0.0.1:42413?code=TheCakeIsALie HTTP/1.1");
            socket.waitForBytesWritten();
            socket.close();
        });
    }
};

FakeAuthBrowser::FakeAuthBrowser()
    : d(new FakeAuthBrowser::Private)
{
    QDesktopServices::setUrlHandler(QStringLiteral("https"), d.get(), "openUrl");
}

FakeAuthBrowser::~FakeAuthBrowser()
{
    QDesktopServices::unsetUrlHandler(QStringLiteral("https"));
}

#include "fakeauthbrowser.moc"

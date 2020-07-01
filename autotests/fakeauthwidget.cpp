/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "fakeauthwidget.h"
#include "ui/authwidget_p.h"

#include <QTimer>
#include <QTcpSocket>
#include <QHostAddress>

FakeAuthWidgetFactory::FakeAuthWidgetFactory()
{
    sFactory = this;
}

FakeAuthWidgetFactory::~FakeAuthWidgetFactory()
{
    sFactory = nullptr;
}

KGAPI2::AuthWidget * FakeAuthWidgetFactory::create(QWidget* parent)
{
    return new FakeAuthWidget(parent);
}


class FakeAuthWidgetPrivate : public KGAPI2::AuthWidgetPrivate
{
    Q_OBJECT

public:
    FakeAuthWidgetPrivate(FakeAuthWidget *parent)
        : KGAPI2::AuthWidgetPrivate(parent)
    {
        serverPort = 42413; // random port, but must be stable
    }
    ~FakeAuthWidgetPrivate() override
    {}

    void setUrl(const QUrl &url) override
    {
        Q_UNUSED(url);

        // don't do anything, don't even try to load Google auth page. Instead
        // pretend the user have already authenticated and we've reached the
        // part where Google sends us the auth code

        QTimer::singleShot(0, this, [=]() {
            QTcpSocket socket;
            socket.connectToHost(QHostAddress::LocalHost, serverPort);
            if (!socket.waitForConnected()) {
                qWarning() << "Failed to connect to internal TCP server!";
                return;
            }
            socket.write("GET http://127.0.0.1:42431?code=TheCakeIsALie HTTP/1.1");
            socket.waitForBytesWritten();
            socket.close();
        });
    }

    void setupUi() override
    {
        // do nothing
    }

    void setVisible(bool) override
    {
        // do nothing
    }
};


FakeAuthWidget::FakeAuthWidget(QWidget *parent)
    : KGAPI2::AuthWidget(new FakeAuthWidgetPrivate(this), parent)
{
}

#include "fakeauthwidget.moc"

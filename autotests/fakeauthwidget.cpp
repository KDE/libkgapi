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

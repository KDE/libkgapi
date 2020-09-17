/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI_FAKEAUTHBROWSER_H_
#define KGAPI_FAKEAUTHBROWSER_H_

#include <QScopedPointer>

class FakeAuthBrowser
{
public:
    explicit FakeAuthBrowser();
    ~FakeAuthBrowser();

private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;
};

#endif


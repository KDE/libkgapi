/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <memory>

class FakeAuthBrowserPrivate;

class FakeAuthBrowser
{
public:
    explicit FakeAuthBrowser();
    ~FakeAuthBrowser();

private:
    std::unique_ptr<FakeAuthBrowserPrivate> const d;
};

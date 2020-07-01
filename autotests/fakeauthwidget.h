/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI_FAKEAUTHWIDGET_H_
#define KGAPI_FAKEAUTHWIDGET_H_

#include "ui/authwidgetfactory_p.h"
#include "ui/authwidget.h"

class FakeAuthWidgetFactory : public KGAPI2::AuthWidgetFactory
{
public:
    FakeAuthWidgetFactory();
    ~FakeAuthWidgetFactory() override;

    KGAPI2::AuthWidget *create(QWidget *parent) override;
};

class FakeAuthWidget : public KGAPI2::AuthWidget
{
    Q_OBJECT
public:
    explicit FakeAuthWidget(QWidget *parent);
};

#endif


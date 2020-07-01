/*
    SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "authwidgetfactory_p.h"
#include "authwidget.h"

using namespace KGAPI2;

AuthWidgetFactory *AuthWidgetFactory::sFactory = nullptr;

AuthWidgetFactory::AuthWidgetFactory()
{
}

AuthWidgetFactory::~AuthWidgetFactory()
{
}


AuthWidgetFactory *AuthWidgetFactory::instance()
{
    if (!sFactory) {
        sFactory = new AuthWidgetFactory();
    }
    return sFactory;
}

AuthWidget *AuthWidgetFactory::create(QWidget *parent)
{
    return new AuthWidget(parent);
}

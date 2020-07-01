/*
    SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef LIBKGAPI_AUTHWIDGETFACTORY_P_H_
#define LIBKGAPI_AUTHWIDGETFACTORY_P_H_

#include <QtGlobal>

#include "kgapicore_export.h"

class QWidget;


namespace KGAPI2 {
class AuthWidget;

// Exported for tests, but otherwise internal
class KGAPICORE_EXPORT AuthWidgetFactory
{
public:
    static AuthWidgetFactory *instance();

    virtual ~AuthWidgetFactory();

    virtual AuthWidget *create(QWidget *parent = nullptr);

protected:
    explicit AuthWidgetFactory();

    static AuthWidgetFactory *sFactory;

private:
    Q_DISABLE_COPY(AuthWidgetFactory)
};

}

#endif

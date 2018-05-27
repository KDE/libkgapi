/*
    Copyright (C) 2018  Daniel Vrátil <dvratil@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
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

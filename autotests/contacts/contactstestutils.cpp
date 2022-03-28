/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "contactstestutils.h"
#include "contactsservice.h"
#include "testutils.h"

#include <QFile>

KGAPI2::ContactPtr contactFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    auto contact = KGAPI2::ContactsService::XMLToContact(f.readAll());
    VERIFY_RET(contact, {});
    return contact;
}

KGAPI2::ContactsGroupPtr contactsGroupFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    auto group = KGAPI2::ContactsService::XMLToContactsGroup(f.readAll());
    VERIFY_RET(group, {});
    return group;
}

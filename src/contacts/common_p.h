/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2020 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LIBKGAPI2_CONTACTS_COMMON_P_H
#define LIBKGAPI2_CONTACTS_COMMON_P_H

#include <QString>

namespace KGAPI2
{

static const QByteArray headerGDataVersion QByteArrayLiteral("GData-Version");
static const QByteArray headerIfMatch = QByteArrayLiteral("If-Match");

static const QByteArray xmlContactHeader = QByteArrayLiteral(
        "<atom:entry xmlns:atom=\"http://www.w3.org/2005/Atom\" "
         "xmlns:gd=\"http://schemas.google.com/g/2005\" "
         "xmlns:gContact=\"http://schemas.google.com/contact/2008\">"
        "<atom:category scheme=\"http://schemas.google.com/g/2005#kind\" "
        " term=\"http://schemas.google.com/contact/2008#contact\"/>");
static const QByteArray xmlGroupHeader = QByteArrayLiteral(
        "<atom:entry xmlns:atom=\"http://www.w3.org/2005/Atom\" "
         "xmlns:gd=\"http://schemas.google.com/g/2005\" "
         "xmlns:gContact=\"http://schemas.google.com/contact/2008\">"
        "<atom:category scheme=\"http://schemas.google.com/g/2005#kind\" "
         "term=\"http://schemas.google.com/g/2008#group\"/>");
static const QByteArray xmlFooter = QByteArrayLiteral("</atom:entry>");

static const QString mimeTypeApplicationAtomXml = QStringLiteral("application/atom+xml");
static const QString mimeTypeImageAny = QStringLiteral("image/*");

static const QString propertyGCalendar = QStringLiteral("GCALENDAR");
static const QString propertyGroupMembershipInfo = QStringLiteral("groupMembershipInfo");

} // namespace KGAPI2

#endif

/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <QSharedDataPointer>
#include <kgapipeople_export.h>

#include <QString>

#include <optional>

class QJsonObject;
class QJsonValue;

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * A person's calendar URL.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#calendarurl
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT CalendarUrl
{
public:
    /** Constructs a new CalendarUrl **/
    explicit CalendarUrl();
    CalendarUrl(const CalendarUrl &);
    CalendarUrl(CalendarUrl &&) noexcept;
    CalendarUrl &operator=(const CalendarUrl &);
    CalendarUrl &operator=(CalendarUrl &&) noexcept;
    /** Destructor. **/
    ~CalendarUrl();

    bool operator==(const CalendarUrl &) const;
    bool operator!=(const CalendarUrl &) const;

    static CalendarUrl fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The calendar URL. **/
    QString url() const;
    /** Sets value of the url property. **/
    void setUrl(const QString &value);

    /** Metadata about the calendar URL. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The type of the calendar URL. The type can be custom or one of these predefined values: * `home` * `freeBusy` * `work` **/
    QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Output only. The type of the calendar URL translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    QString formattedType() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // CalendarUrl

} // namespace KGAPI2::People
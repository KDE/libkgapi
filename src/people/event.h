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

#include <QDate>
#include <QString>

#include <optional>

class QJsonObject;
class QJsonValue;

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * An event related to the person.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#event
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT Event
{
public:
    /** Constructs a new Event **/
    explicit Event();
    Event(const Event &);
    Event(Event &&) noexcept;
    Event &operator=(const Event &);
    Event &operator=(Event &&) noexcept;
    /** Destructor. **/
    ~Event();

    bool operator==(const Event &) const;
    bool operator!=(const Event &) const;

    static Event fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Metadata about the event. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The date of the event. **/
    QDate date() const;
    /** Sets value of the date property. **/
    void setDate(const QDate &value);

    /** The type of the event. The type can be custom or one of these predefined values: * `anniversary` * `other` **/
    QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Output only. The type of the event translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    QString formattedType() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Event

} // namespace KGAPI2::People
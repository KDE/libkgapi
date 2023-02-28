/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <QSharedDataPointer>
#include "kgapipeople_export.h"

#include <QString>

#include <optional>

class QJsonObject;
class QJsonValue;
class QJsonArray;

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * A person's location.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#location
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT Location
{
public:
    /** Constructs a new Location **/
    explicit Location();
    Location(const Location &);
    Location(Location &&) noexcept;
    Location &operator=(const Location &);
    Location &operator=(Location &&) noexcept;
    /** Destructor. **/
    ~Location();

    bool operator==(const Location &) const;
    bool operator!=(const Location &) const;

    Q_REQUIRED_RESULT static Location fromJSON(const QJsonObject &);
    Q_REQUIRED_RESULT static QVector<Location> fromJSONArray(const QJsonArray& data);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    /** The individual desk location. **/
    Q_REQUIRED_RESULT QString deskCode() const;
    /** Sets value of the deskCode property. **/
    void setDeskCode(const QString &value);

    /** The floor name or number. **/
    Q_REQUIRED_RESULT QString floor() const;
    /** Sets value of the floor property. **/
    void setFloor(const QString &value);

    /** The building identifier. **/
    Q_REQUIRED_RESULT QString buildingId() const;
    /** Sets value of the buildingId property. **/
    void setBuildingId(const QString &value);

    /** The type of the location. The type can be custom or one of these predefined values: * `desk` * `grewUp` **/
    Q_REQUIRED_RESULT QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Whether the location is the current location. **/
    Q_REQUIRED_RESULT bool current() const;
    /** Sets value of the current property. **/
    void setCurrent(const bool &value);

    /** The free-form value of the location. **/
    Q_REQUIRED_RESULT QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the location. **/
    Q_REQUIRED_RESULT FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The floor section in `floor_name`. **/
    Q_REQUIRED_RESULT QString floorSection() const;
    /** Sets value of the floorSection property. **/
    void setFloorSection(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Location

} // namespace KGAPI2::People

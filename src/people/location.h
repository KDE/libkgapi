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

    [[nodiscard]] static Location fromJSON(const QJsonObject &);
    [[nodiscard]] static QList<Location> fromJSONArray(const QJsonArray &data);
    [[nodiscard]] QJsonValue toJSON() const;

    /** The individual desk location. **/
    [[nodiscard]] QString deskCode() const;
    /** Sets value of the deskCode property. **/
    void setDeskCode(const QString &value);

    /** The floor name or number. **/
    [[nodiscard]] QString floor() const;
    /** Sets value of the floor property. **/
    void setFloor(const QString &value);

    /** The building identifier. **/
    [[nodiscard]] QString buildingId() const;
    /** Sets value of the buildingId property. **/
    void setBuildingId(const QString &value);

    /** The type of the location. The type can be custom or one of these predefined values: * `desk` * `grewUp` **/
    [[nodiscard]] QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Whether the location is the current location. **/
    [[nodiscard]] bool current() const;
    /** Sets value of the current property. **/
    void setCurrent(bool value);

    /** The free-form value of the location. **/
    [[nodiscard]] QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the location. **/
    [[nodiscard]] FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The floor section in `floor_name`. **/
    [[nodiscard]] QString floorSection() const;
    /** Sets value of the floorSection property. **/
    void setFloorSection(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Location

} // namespace KGAPI2::People

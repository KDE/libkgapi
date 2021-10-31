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
 * **DEPRECATED**: Please use `person.locations` instead. A person's past or
 * current residence.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#residence
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT Residence
{
public:
    /** Constructs a new Residence **/
    explicit Residence();
    Residence(const Residence &);
    Residence(Residence &&) noexcept;
    Residence &operator=(const Residence &);
    Residence &operator=(Residence &&) noexcept;
    /** Destructor. **/
    ~Residence();

    bool operator==(const Residence &) const;
    bool operator!=(const Residence &) const;

    static Residence fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The address of the residence. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the residence. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** True if the residence is the person's current residence; false if the residence is a past residence. **/
    bool current() const;
    /** Sets value of the current property. **/
    void setCurrent(const bool &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Residence

} // namespace KGAPI2::People
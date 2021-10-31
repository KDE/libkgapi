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
 * A skill that the person has.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#skill
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT Skill
{
public:
    /** Constructs a new Skill **/
    explicit Skill();
    Skill(const Skill &);
    Skill(Skill &&) noexcept;
    Skill &operator=(const Skill &);
    Skill &operator=(Skill &&) noexcept;
    /** Destructor. **/
    ~Skill();

    bool operator==(const Skill &) const;
    bool operator!=(const Skill &) const;

    static Skill fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Metadata about the skill. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The skill; for example, `underwater basket weaving`. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Skill

} // namespace KGAPI2::People
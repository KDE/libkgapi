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
 * A person's relation to another person.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#relation
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT Relation
{
public:
    /** Constructs a new Relation **/
    explicit Relation();
    Relation(const Relation &);
    Relation(Relation &&) noexcept;
    Relation &operator=(const Relation &);
    Relation &operator=(Relation &&) noexcept;
    /** Destructor. **/
    ~Relation();

    bool operator==(const Relation &) const;
    bool operator!=(const Relation &) const;

    static Relation fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The person's relation to the other person. The type can be custom or one of these predefined values: * `spouse` * `child` * `mother` * `father` *
     * `parent` * `brother` * `sister` * `friend` * `relative` * `domesticPartner` * `manager` * `assistant` * `referredBy` * `partner` **/
    QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** The name of the other person this relation refers to. **/
    QString person() const;
    /** Sets value of the person property. **/
    void setPerson(const QString &value);

    /** Output only. The type of the relation translated and formatted in the viewer's account locale or the locale specified in the Accept-Language HTTP
     * header. **/
    QString formattedType() const;

    /** Metadata about the relation. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Relation

} // namespace KGAPI2::People
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

#include <QDate>
#include <QString>

#include <optional>

class QJsonObject;
class QJsonValue;
class QJsonArray;

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * A person's past or current organization. Overlapping date ranges are permitted.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#organization
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT Organization
{
public:
    /** Constructs a new Organization **/
    explicit Organization();
    Organization(const Organization &);
    Organization(Organization &&) noexcept;
    Organization &operator=(const Organization &);
    Organization &operator=(Organization &&) noexcept;
    /** Destructor. **/
    ~Organization();

    bool operator==(const Organization &) const;
    bool operator!=(const Organization &) const;

    [[nodiscard]] static Organization fromJSON(const QJsonObject &);
    [[nodiscard]] static QList<Organization> fromJSONArray(const QJsonArray &data);
    [[nodiscard]] QJsonValue toJSON() const;

    /** The location of the organization office the person works at. **/
    [[nodiscard]] QString location() const;
    /** Sets value of the location property. **/
    void setLocation(const QString &value);

    /** The person's job title at the organization. **/
    [[nodiscard]] QString title() const;
    /** Sets value of the title property. **/
    void setTitle(const QString &value);

    /** The type of the organization. The type can be custom or one of these predefined values: * `work` * `school` **/
    [[nodiscard]] QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Metadata about the organization. **/
    [[nodiscard]] FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The symbol associated with the organization; for example, a stock ticker symbol, abbreviation, or acronym. **/
    [[nodiscard]] QString symbol() const;
    /** Sets value of the symbol property. **/
    void setSymbol(const QString &value);

    /** Output only. The type of the organization translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    [[nodiscard]] QString formattedType() const;

    /** The name of the organization. **/
    [[nodiscard]] QString name() const;
    /** Sets value of the name property. **/
    void setName(const QString &value);

    /** True if the organization is the person's current organization; false if the organization is a past organization. **/
    [[nodiscard]] bool current() const;
    /** Sets value of the current property. **/
    void setCurrent(bool value);

    [[nodiscard]] bool hasSetCurrent() const;

    /** The person's cost center at the organization. **/
    [[nodiscard]] QString costCenter() const;
    /** Sets value of the costCenter property. **/
    void setCostCenter(const QString &value);

    /** The person's department at the organization. **/
    [[nodiscard]] QString department() const;
    /** Sets value of the department property. **/
    void setDepartment(const QString &value);

    /** The domain name associated with the organization; for example, `google.com`. **/
    [[nodiscard]] QString domain() const;
    /** Sets value of the domain property. **/
    void setDomain(const QString &value);

    /** The start date when the person joined the organization. **/
    [[nodiscard]] QDate startDate() const;
    /** Sets value of the startDate property. **/
    void setStartDate(const QDate &value);

    /** The person's job description at the organization. **/
    [[nodiscard]] QString jobDescription() const;
    /** Sets value of the jobDescription property. **/
    void setJobDescription(const QString &value);

    /** The end date when the person left the organization. **/
    [[nodiscard]] QDate endDate() const;
    /** Sets value of the endDate property. **/
    void setEndDate(const QDate &value);

    /** The phonetic name of the organization. **/
    [[nodiscard]] QString phoneticName() const;
    /** Sets value of the phoneticName property. **/
    void setPhoneticName(const QString &value);

    /** The person's full-time equivalent millipercent within the organization (100000 = 100%). **/
    [[nodiscard]] int fullTimeEquivalentMillipercent() const;
    /** Sets value of the fullTimeEquivalentMillipercent property. **/
    void setFullTimeEquivalentMillipercent(int value);

    [[nodiscard]] bool hasSetFullTimeEquivalentMillipercent() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Organization

} // namespace KGAPI2::People

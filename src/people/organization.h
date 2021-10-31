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
 * A person's past or current organization. Overlapping date ranges are permitted.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#organization
 * @since 5.19.0
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

    static Organization fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The location of the organization office the person works at. **/
    QString location() const;
    /** Sets value of the location property. **/
    void setLocation(const QString &value);

    /** The person's job title at the organization. **/
    QString title() const;
    /** Sets value of the title property. **/
    void setTitle(const QString &value);

    /** The type of the organization. The type can be custom or one of these predefined values: * `work` * `school` **/
    QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Metadata about the organization. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The symbol associated with the organization; for example, a stock ticker symbol, abbreviation, or acronym. **/
    QString symbol() const;
    /** Sets value of the symbol property. **/
    void setSymbol(const QString &value);

    /** Output only. The type of the organization translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    QString formattedType() const;

    /** The name of the organization. **/
    QString name() const;
    /** Sets value of the name property. **/
    void setName(const QString &value);

    /** True if the organization is the person's current organization; false if the organization is a past organization. **/
    bool current() const;
    /** Sets value of the current property. **/
    void setCurrent(const bool &value);

    /** The person's cost center at the organization. **/
    QString costCenter() const;
    /** Sets value of the costCenter property. **/
    void setCostCenter(const QString &value);

    /** The person's department at the organization. **/
    QString department() const;
    /** Sets value of the department property. **/
    void setDepartment(const QString &value);

    /** The domain name associated with the organization; for example, `google.com`. **/
    QString domain() const;
    /** Sets value of the domain property. **/
    void setDomain(const QString &value);

    /** The start date when the person joined the organization. **/
    QDate startDate() const;
    /** Sets value of the startDate property. **/
    void setStartDate(const QDate &value);

    /** The person's job description at the organization. **/
    QString jobDescription() const;
    /** Sets value of the jobDescription property. **/
    void setJobDescription(const QString &value);

    /** The end date when the person left the organization. **/
    QDate endDate() const;
    /** Sets value of the endDate property. **/
    void setEndDate(const QDate &value);

    /** The phonetic name of the organization. **/
    QString phoneticName() const;
    /** Sets value of the phoneticName property. **/
    void setPhoneticName(const QString &value);

    /** The person's full-time equivalent millipercent within the organization (100000 = 100%). **/
    int fullTimeEquivalentMillipercent() const;
    /** Sets value of the fullTimeEquivalentMillipercent property. **/
    void setFullTimeEquivalentMillipercent(const int &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Organization

} // namespace KGAPI2::People
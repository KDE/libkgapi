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
 * A person's miscellaneous keyword.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#misckeyword
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT MiscKeyword
{
public:
    enum class Type {
        TYPE_UNSPECIFIED, ///< Unspecified.
        OUTLOOK_BILLING_INFORMATION, ///< Outlook field for billing information.
        OUTLOOK_DIRECTORY_SERVER, ///< Outlook field for directory server.
        OUTLOOK_KEYWORD, ///< Outlook field for keyword.
        OUTLOOK_MILEAGE, ///< Outlook field for mileage.
        OUTLOOK_PRIORITY, ///< Outlook field for priority.
        OUTLOOK_SENSITIVITY, ///< Outlook field for sensitivity.
        OUTLOOK_SUBJECT, ///< Outlook field for subject.
        OUTLOOK_USER, ///< Outlook field for user.
        HOME, ///< Home.
        WORK, ///< Work.
        OTHER, ///< Other.
    };

    /** Constructs a new MiscKeyword **/
    explicit MiscKeyword();
    MiscKeyword(const MiscKeyword &);
    MiscKeyword(MiscKeyword &&) noexcept;
    MiscKeyword &operator=(const MiscKeyword &);
    MiscKeyword &operator=(MiscKeyword &&) noexcept;
    /** Destructor. **/
    ~MiscKeyword();

    bool operator==(const MiscKeyword &) const;
    bool operator!=(const MiscKeyword &) const;

    static MiscKeyword fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Metadata about the miscellaneous keyword. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The value of the miscellaneous keyword. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** The miscellaneous keyword type. **/
    MiscKeyword::Type type() const;
    /** Sets value of the type property. **/
    void setType(const MiscKeyword::Type &value);

    /** Output only. The type of the miscellaneous keyword translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale.
     * **/
    QString formattedType() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // MiscKeyword

} // namespace KGAPI2::People
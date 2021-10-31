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
 * A person's SIP address. Session Initial Protocol addresses are used for VoIP
 * communications to make voice or video calls over the internet.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#sipaddress
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT SipAddress
{
public:
    /** Constructs a new SipAddress **/
    explicit SipAddress();
    SipAddress(const SipAddress &);
    SipAddress(SipAddress &&) noexcept;
    SipAddress &operator=(const SipAddress &);
    SipAddress &operator=(SipAddress &&) noexcept;
    /** Destructor. **/
    ~SipAddress();

    bool operator==(const SipAddress &) const;
    bool operator!=(const SipAddress &) const;

    static SipAddress fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The SIP address in the [RFC 3261 19.1](https://tools.ietf.org/html/rfc3261#section-19.1) SIP URI format. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the SIP address. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The type of the SIP address. The type can be custom or or one of these predefined values: * `home` * `work` * `mobile` * `other` **/
    QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Output only. The type of the SIP address translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    QString formattedType() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // SipAddress

} // namespace KGAPI2::People
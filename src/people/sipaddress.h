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
 * A person's SIP address. Session Initial Protocol addresses are used for VoIP
 * communications to make voice or video calls over the internet.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#sipaddress
 * @since 5.23.0
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

    Q_REQUIRED_RESULT static SipAddress fromJSON(const QJsonObject &);
    Q_REQUIRED_RESULT static QVector<SipAddress> fromJSONArray(const QJsonArray &data);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    /** The SIP address in the [RFC 3261 19.1](https://tools.ietf.org/html/rfc3261#section-19.1) SIP URI format. **/
    Q_REQUIRED_RESULT QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the SIP address. **/
    Q_REQUIRED_RESULT FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The type of the SIP address. The type can be custom or or one of these predefined values: * `home` * `work` * `mobile` * `other` **/
    Q_REQUIRED_RESULT QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Output only. The type of the SIP address translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    Q_REQUIRED_RESULT QString formattedType() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // SipAddress

} // namespace KGAPI2::People

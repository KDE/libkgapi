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
struct ImClientDefinition;

/**
 * A person's instant messaging client.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#imclient
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT ImClient
{
public:
    /** Constructs a new ImClient **/
    explicit ImClient();
    ImClient(const ImClientDefinition &definition);
    ImClient(const ImClient &);
    ImClient(ImClient &&) noexcept;
    ImClient &operator=(const ImClient &);
    ImClient &operator=(ImClient &&) noexcept;
    /** Destructor. **/
    ~ImClient();

    bool operator==(const ImClient &) const;
    bool operator!=(const ImClient &) const;

    [[nodiscard]] static ImClient fromJSON(const QJsonObject &);
    [[nodiscard]] static QList<ImClient> fromJSONArray(const QJsonArray &data);
    [[nodiscard]] QJsonValue toJSON() const;

    /** The protocol of the IM client. The protocol can be custom or one of these predefined values: * `aim` * `msn` * `yahoo` * `skype` * `qq` * `googleTalk` *
     * `icq` * `jabber` * `netMeeting` **/
    [[nodiscard]] QString protocol() const;
    /** Sets value of the protocol property. **/
    void setProtocol(const QString &value);

    /** The user name used in the IM client. **/
    [[nodiscard]] QString username() const;
    /** Sets value of the username property. **/
    void setUsername(const QString &value);

    /** The type of the IM client. The type can be custom or one of these predefined values: * `home` * `work` * `other` **/
    [[nodiscard]] QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Metadata about the IM client. **/
    [[nodiscard]] FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** Output only. The type of the IM client translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    [[nodiscard]] QString formattedType() const;

    /** Output only. The protocol of the IM client formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    [[nodiscard]] QString formattedProtocol() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // ImClient

} // namespace KGAPI2::People

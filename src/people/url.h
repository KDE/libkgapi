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
 * A person's associated URLs.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#url
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT Url
{
public:
    /** Constructs a new Url **/
    explicit Url();
    Url(const Url &);
    Url(Url &&) noexcept;
    Url &operator=(const Url &);
    Url &operator=(Url &&) noexcept;
    /** Destructor. **/
    ~Url();

    bool operator==(const Url &) const;
    bool operator!=(const Url &) const;

    Q_REQUIRED_RESULT static Url fromJSON(const QJsonObject &);
    Q_REQUIRED_RESULT static QVector<Url> fromJSONArray(const QJsonArray& data);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    /** The URL. **/
    Q_REQUIRED_RESULT QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** The type of the URL. The type can be custom or one of these predefined values: * `home` * `work` * `blog` * `profile` * `homePage` * `ftp` *
     * `reservations` * `appInstallPage`: website for a Currents application. * `other` **/
    Q_REQUIRED_RESULT QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Metadata about the URL. **/
    Q_REQUIRED_RESULT FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** Output only. The type of the URL translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    Q_REQUIRED_RESULT QString formattedType() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Url

} // namespace KGAPI2::People

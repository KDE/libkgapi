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
 * The name that should be used to sort the person in a list.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#fileas
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT FileAs
{
public:
    /** Constructs a new FileAs **/
    explicit FileAs();
    FileAs(const FileAs &);
    FileAs(FileAs &&) noexcept;
    FileAs &operator=(const FileAs &);
    FileAs &operator=(FileAs &&) noexcept;
    /** Destructor. **/
    ~FileAs();

    bool operator==(const FileAs &) const;
    bool operator!=(const FileAs &) const;

    [[nodiscard]] static FileAs fromJSON(const QJsonObject &obj);
    [[nodiscard]] static QList<FileAs> fromJSONArray(const QJsonArray &data);
    [[nodiscard]] QJsonValue toJSON() const;

    /** Metadata about the file-as. **/
    [[nodiscard]] FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The file-as value **/
    [[nodiscard]] QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // FileAs

} // namespace KGAPI2::People

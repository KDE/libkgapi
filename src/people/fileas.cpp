/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "fileas.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class FileAs::Private : public QSharedData
{
public:
    explicit Private() = default;
    Private(const Private &) = default;
    Private(Private &&) noexcept = delete;
    Private &operator=(const Private &) = delete;
    Private &operator=(Private &&) noexcept = delete;
    ~Private() = default;

    bool operator==(const Private &other) const
    {
        return metadata == other.metadata && value == other.value;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    FieldMetadata metadata{};
    QString value{};
};

FileAs::FileAs()
    : d(new Private)
{
}

FileAs::FileAs(const FileAs &) = default;
FileAs::FileAs(FileAs &&) noexcept = default;
FileAs &FileAs::operator=(const FileAs &) = default;
FileAs &FileAs::operator=(FileAs &&) noexcept = default;
FileAs::~FileAs() = default;

bool FileAs::operator==(const FileAs &other) const
{
    return *d == *other.d;
}

bool FileAs::operator!=(const FileAs &other) const
{
    return !(*this == other);
}

FieldMetadata FileAs::metadata() const
{
    return d->metadata;
}

void FileAs::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString FileAs::value() const
{
    return d->value;
}

void FileAs::setValue(const QString &value)
{
    d->value = value;
}

FileAs FileAs::fromJSON(const QJsonObject &obj)
{
    FileAs fileAs;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        fileAs.setMetadata(FieldMetadata::fromJSON(metadata));
        fileAs.setValue(obj.value(QStringLiteral("value")).toString());
    }

    return fileAs;
}

QVector<FileAs> FileAs::fromJSONArray(const QJsonArray& data)
{
    QVector<FileAs> fileAses;

    for(const auto &fileAs : data) {
        if(fileAs.isObject()) {
            const auto objectifiedFileAs = fileAs.toObject();
            fileAses.append(fromJSON(objectifiedFileAs));
        }
    }

    return fileAses;
}

QJsonValue FileAs::toJSON() const
{
    QJsonObject obj;

    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata", d->metadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "value", d->value);
    return obj;
}

} // namespace KGAPI2::People

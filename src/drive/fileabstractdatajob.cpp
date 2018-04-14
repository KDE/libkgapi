/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fileabstractdatajob.h"
#include "../debug.h"
#include "utils.h"

#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileAbstractDataJob::Private
{
  public:
    Private();

    bool convert;
    bool ocr;
    QString ocrLanguage;
    bool pinned;
    QString timedTextLanguage;
    QString timedTextTrackName;
};

FileAbstractDataJob::Private::Private():
    convert(false),
    ocr(false),
    pinned(false)
{
}

FileAbstractDataJob::FileAbstractDataJob(const AccountPtr &account,
                                         QObject *parent):
    Job(account, parent),
    d(new Private)
{
}

FileAbstractDataJob::~FileAbstractDataJob()
{
    delete d;
}

bool FileAbstractDataJob::convert() const
{
    return d->convert;
}

void FileAbstractDataJob::setConvert(bool convert)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify setConvert property when job is running";
        return;
    }

    d->convert = convert;
}

bool FileAbstractDataJob::ocr() const
{
    return d->ocr;
}

void FileAbstractDataJob::setOcr(bool ocr)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify ocr property when job is running";
        return;
    }

    d->ocr = ocr;
}

QString FileAbstractDataJob::ocrLanguage() const
{
    return d->ocrLanguage;
}

void FileAbstractDataJob::setOcrLanguage(const QString &ocrLanguage)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify ocrLanguage property when job is running";
        return;
    }

    d->ocrLanguage = ocrLanguage;
}

bool FileAbstractDataJob::pinned() const
{
    return d->pinned;
}

void FileAbstractDataJob::setPinned(bool pinned)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify pinned property when job is running";
        return;
    }

    d->pinned = pinned;
}

QString FileAbstractDataJob::timedTextLanguage() const
{
    return d->timedTextLanguage;
}


void FileAbstractDataJob::setTimedTextLanguage(const QString &timedTextLanguage)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify timedTextLanguage property when job is running";
        return;
    }

    d->timedTextLanguage = timedTextLanguage;
}

QString FileAbstractDataJob::timedTextTrackName() const
{
    return d->timedTextLanguage;
}

void FileAbstractDataJob::setTimedTextTrackName(const QString &timedTextTrackName)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify timedTextTrackName property when job is running";
        return;
    }

    d->timedTextTrackName = timedTextTrackName;
}

QUrl FileAbstractDataJob::updateUrl(QUrl &url)
{
    QUrlQuery query(url);
    query.removeQueryItem(QStringLiteral("convert"));
    query.addQueryItem(QStringLiteral("convert"), Utils::bool2Str(d->convert));

    query.removeQueryItem(QStringLiteral("ocr"));
    query.removeQueryItem(QStringLiteral("ocrLanguage"));
    query.addQueryItem(QStringLiteral("ocr"), Utils::bool2Str(d->ocr));
    if (d->ocr && !d->ocrLanguage.isEmpty()) {
        query.addQueryItem(QStringLiteral("ocrLanguage"), d->ocrLanguage);
    }

    query.removeQueryItem(QStringLiteral("pinned"));
    query.addQueryItem(QStringLiteral("pinned"), Utils::bool2Str(d->pinned));

    query.removeQueryItem(QStringLiteral("timedTextLanguage"));
    if (!d->timedTextLanguage.isEmpty()) {
        query.addQueryItem(QStringLiteral("timedTextLanguage"), d->timedTextLanguage);
    }

    query.removeQueryItem(QStringLiteral("timedTextTrackName"));
    if (!d->timedTextTrackName.isEmpty()) {
        query.addQueryItem(QStringLiteral("timedTextTrackName"), d->timedTextTrackName);
    }
    url.setQuery(query);

    return url;
}


#include "moc_fileabstractdatajob.cpp"

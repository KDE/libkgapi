/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "fileabstractdatajob.h"
#include "../debug.h"
#include "utils.h"

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class FileAbstractDataJob::Private
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
    url.removeQueryItem(QLatin1String("convert"));
    url.addQueryItem(QLatin1String("convert"), Utils::bool2Str(d->convert));

    url.removeQueryItem(QLatin1String("ocr"));
    url.removeQueryItem(QLatin1String("ocrLanguage"));
    url.addQueryItem(QLatin1String("ocr"), Utils::bool2Str(d->ocr));
    if (d->ocr && !d->ocrLanguage.isEmpty()) {
        url.addQueryItem(QLatin1String("ocrLanguage"), d->ocrLanguage);
    }

    url.removeQueryItem(QLatin1String("pinned"));
    url.addQueryItem(QLatin1String("pinned"), Utils::bool2Str(d->pinned));

    url.removeQueryItem(QLatin1String("timedTextLanguage"));
    if (!d->timedTextLanguage.isEmpty()) {
        url.addQueryItem(QLatin1String("timedTextLanguage"), d->timedTextLanguage);
    }

    url.removeQueryItem(QLatin1String("timedTextTrackName"));
    if (!d->timedTextTrackName.isEmpty()) {
        url.addQueryItem(QLatin1String("timedTextTrackName"), d->timedTextTrackName);
    }

    return url;
}


#include "moc_fileabstractdatajob.cpp"

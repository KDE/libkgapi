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
#include "debug.h"
#include "utils.h"

using namespace KGAPI2;

class DriveFileAbstractDataJob::Private
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

DriveFileAbstractDataJob::Private::Private():
    convert(false),
    ocr(false),
    pinned(false)
{
}

DriveFileAbstractDataJob::DriveFileAbstractDataJob(const AccountPtr &account,
                                                   QObject *parent):
    Job(account, parent),
    d(new Private)
{
}

DriveFileAbstractDataJob::~DriveFileAbstractDataJob()
{
    delete d;
}

bool DriveFileAbstractDataJob::convert() const
{
    return d->convert;
}

void DriveFileAbstractDataJob::setConvert(bool convert)
{
    if (isRunning()) {
        kWarning() << "Can't modify setConvert property when job is running";
        return;
    }

    d->convert = convert;
}

bool DriveFileAbstractDataJob::ocr() const
{
    return d->ocr;
}

void DriveFileAbstractDataJob::setOcr(bool ocr)
{
    if (isRunning()) {
        kWarning() << "Can't modify ocr property when job is running";
        return;
    }

    d->ocr = ocr;
}

QString DriveFileAbstractDataJob::ocrLanguage() const
{
    return d->ocrLanguage;
}

void DriveFileAbstractDataJob::setOcrLanguage(const QString &ocrLanguage)
{
    if (isRunning()) {
        kWarning() << "Can't modify ocrLanguage property when job is running";
        return;
    }

    d->ocrLanguage = ocrLanguage;
}

bool DriveFileAbstractDataJob::pinned() const
{
    return d->pinned;
}

void DriveFileAbstractDataJob::setPinned(bool pinned)
{
    if (isRunning()) {
        kWarning() << "Can't modify pinned property when job is running";
        return;
    }

    d->pinned = pinned;
}

QString DriveFileAbstractDataJob::timedTextLanguage() const
{
    return d->timedTextLanguage;
}


void DriveFileAbstractDataJob::setTimedTextLanguage(const QString &timedTextLanguage)
{
    if (isRunning()) {
        kWarning() << "Can't modify timedTextLanguage property when job is running";
        return;
    }

    d->timedTextLanguage = timedTextLanguage;
}

QString DriveFileAbstractDataJob::timedTextTrackName() const
{
    return d->timedTextLanguage;
}

void DriveFileAbstractDataJob::setTimedTextTrackName(const QString &timedTextTrackName)
{
    if (isRunning()) {
        kWarning() << "Can't modify timedTextTrackName property when job is running";
        return;
    }

    d->timedTextTrackName = timedTextTrackName;
}

QUrl DriveFileAbstractDataJob::updateUrl(QUrl &url)
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


#include "fileabstractdatajob.moc"

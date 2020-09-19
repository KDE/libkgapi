/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

    bool convert = false;
    bool enforceSingleParent = false;
    QString includePermissionsForView;
    bool ocr = false;
    QString ocrLanguage;
    bool pinned = false;
    bool supportsAllDrives = true;
    QString timedTextLanguage;
    QString timedTextTrackName;
    bool useContentAsIndexableText = false;
};

FileAbstractDataJob::Private::Private()
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


bool FileAbstractDataJob::enforceSingleParent() const
{
    return d->enforceSingleParent;
}

void FileAbstractDataJob::setEnforceSingleParent(bool enforceSingleParent)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify enforceSingleParent property when job is running";
        return;
    }

    d->enforceSingleParent = enforceSingleParent;
}

QString FileAbstractDataJob::includePermissionsForView() const
{
    return d->includePermissionsForView;
}

void FileAbstractDataJob::setIncludePermissionsForView(const QString &includePermissionsForView)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify includePermissionsForView property when job is running";
        return;
    }

    d->includePermissionsForView = includePermissionsForView;
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

bool FileAbstractDataJob::supportsAllDrives() const
{
    return d->supportsAllDrives;
}

void FileAbstractDataJob::setSupportsAllDrives(bool supportsAllDrives)
{
    d->supportsAllDrives = supportsAllDrives;
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

void FileAbstractDataJob::setUseContentAsIndexableText(bool useContentAsIndexableText)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify useContentAsIndexableText property when job is running";
        return;
    }

    d->useContentAsIndexableText = useContentAsIndexableText;
}

bool FileAbstractDataJob::useContentAsIndexableText() const
{
    return d->useContentAsIndexableText;
}

QUrl FileAbstractDataJob::updateUrl(QUrl &url)
{
    QUrlQuery query(url);
    query.removeQueryItem(QStringLiteral("convert"));
    query.addQueryItem(QStringLiteral("convert"), Utils::bool2Str(d->convert));

    query.removeQueryItem(QStringLiteral("enforceSingleParent"));
    query.removeQueryItem(QStringLiteral("includePermissionsForView"));
    query.addQueryItem(QStringLiteral("enforceSingleParent"), Utils::bool2Str(d->enforceSingleParent));
    if (!d->includePermissionsForView.isEmpty()) {
        query.addQueryItem(QStringLiteral("includePermissionsForView"), d->includePermissionsForView);
    }

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
    
    query.removeQueryItem(QStringLiteral("useContentAsIndexableText"));
    query.addQueryItem(QStringLiteral("useContentAsIndexableText"), Utils::bool2Str(d->useContentAsIndexableText));

    query.removeQueryItem(QStringLiteral("supportsAllDrives"));
    query.addQueryItem(QStringLiteral("supportsAllDrives"), Utils::bool2Str(d->supportsAllDrives));

    url.setQuery(query);
    return url;
}


#include "moc_fileabstractdatajob.cpp"

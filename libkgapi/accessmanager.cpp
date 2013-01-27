/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "accessmanager.h"
#include "accessmanager_p.h"
#include "auth.h"

#include "request.h"

#include <QtCore/QtGlobal>
#include <QtCore/QRegExp>
#include <QtCore/QDateTime>

#include <KSystemTimeZones>
#include <kdeversion.h>
#include <kglobal.h>

static const KCatalogLoader loader(QLatin1String("libkgapi"));

using namespace KGAPI;

AccessManager::AccessManager():
    d_ptr(new AccessManagerPrivate(this))
{
    connect(Auth::instance(), SIGNAL(error(KGAPI::Error,QString)),
            this, SIGNAL(error(KGAPI::Error,QString)));
}

AccessManager::~AccessManager()
{
    delete d_ptr;
}

void AccessManager::queueRequest (Request* request)
{
    Q_D(AccessManager);

    if (request)
        d->cache.enqueue(request);
}


void KGAPI::AccessManager::sendRequest(KGAPI::Request *request)
{
    Q_D(AccessManager);

    /* Queue to cache */
    queueRequest(request);

    if (d->cacheSemaphore->available()) {
        d->submitCache();
    }
}

KDateTime KGAPI::AccessManager::RFC3339StringToDate(const QString& datetime)
{
#if KDE_IS_VERSION(4,6,90)
    return KDateTime::fromString(datetime, KDateTime::RFC3339Date);
#else
    /* This is the original implementation of KDateTime::fromString() for RFC3339Date
     * from kdelibs/kdecore/date/kdatetime.cpp by David Jarvie.  */
    QRegExp rx(QString::fromLatin1("^(\\d{4})-(\\d\\d)-(\\d\\d)[Tt](\\d\\d):(\\d\\d):(\\d\\d)(?:\\.(\\d+))?([Zz]|([+-])(\\d\\d):(\\d\\d))$"));
    if (datetime.indexOf(rx))
        return KDateTime();
    const QStringList parts = rx.capturedTexts();
    bool ok, ok1, ok2;
    int msecs  = 0;
    bool leapSecond = false;
    int year = parts[1].toInt(&ok);
    int month = parts[2].toInt(&ok1);
    int day = parts[3].toInt(&ok2);
    if (!ok || !ok1 || !ok2)
        return KDateTime();
    QDate d(year, month, day);
    if (!d.isValid())
        return KDateTime();
    int hour = parts[4].toInt(&ok);
    int minute = parts[5].toInt(&ok1);
    int second = parts[6].toInt(&ok2);
    if (!ok || !ok1 || !ok2)
        return KDateTime();
    leapSecond = (second == 60);
    if (leapSecond)
        second = 59;   // apparently a leap second - validate below, once time zone is known
    if (!parts[7].isEmpty()) {
        QString ms = parts[7] + QLatin1String("00");
        ms.truncate(3);
        msecs = ms.toInt(&ok);
        if (!ok)
            return KDateTime();
        if (msecs && leapSecond)
            return KDateTime();   // leap second only valid if 23:59:60.000
    }
    QTime t(hour, minute, second, msecs);
    if (!t.isValid())
        return KDateTime();
    int offset = 0;
    KDateTime::SpecType spec = (parts[8].toUpper() == QLatin1String("Z")) ? KDateTime::UTC : KDateTime::OffsetFromUTC;
    if (spec == KDateTime::OffsetFromUTC) {
        offset = parts[10].toInt(&ok) * 3600;
        offset += parts[11].toInt(&ok1) * 60;
        if (!ok || !ok1)
            return KDateTime();
        if (parts[9] == QLatin1String("-")) {
            if (!offset && leapSecond)
                return KDateTime();      // leap second only valid if known time zone
            offset = -offset;
        }
    }
    if (leapSecond) {
        // Validate a leap second time. Leap seconds are inserted after 23:59:59 UTC.
        // Convert the time to UTC and check that it is 00:00:00.
        if ((hour * 3600 + minute * 60 + 60 - offset + 86400 * 5) % 86400) // (max abs(offset) is 100 hours)
            return KDateTime();    // the time isn't the last second of the day
    }

    return KDateTime(d, t, KDateTime::Spec(spec, offset));
#endif
}

QString KGAPI::AccessManager::dateToRFC3339String(const KDateTime& datetime)
{
#if KDE_IS_VERSION(4,6,90)
    return datetime.toString(KDateTime::RFC3339Date);
#else
    /* This is the original implementation of KDateTime::toString() for RFC3339Date
     * from kdelibs/kdecore/date/kdatetime.cpp by David Jarvie.  */
    QString s;
    QString result;
    const char *tzcolon = "";
    KTimeZone tz;
    char tzsign = '+';
    int offset = 0;

    result += s.sprintf("%04d-%02d-%02dT%02d:%02d:%02d",
                        datetime.date().year(), datetime.date().month(), datetime.date().day(),
                        datetime.time().hour(), datetime.time().minute(), datetime.time().second());
    int msec = datetime.time().msec();
    if (msec) {
        int digits = 3;
        if (!(msec % 10))
            msec /= 10, --digits;
        if (!(msec % 10))
            msec /= 10, --digits;
        result += s.sprintf(".%0*d", digits, datetime.time().msec());
    }
    if (datetime.timeType() == KDateTime::UTC)
        return result + QLatin1Char('Z');
    if (datetime.timeType() == KDateTime::ClockTime)
        tz = KSystemTimeZones::local();
    tzcolon = ':';

    // Return the string with UTC offset ±hhmm appended
    if (datetime.timeType() == KDateTime::OffsetFromUTC  ||  datetime.timeType() == KDateTime::TimeZone  ||  tz.isValid()) {
        if (datetime.timeType() == KDateTime::TimeZone)
            offset = datetime.timeZone().offsetAtZoneTime(datetime.dateTime());
        else
            offset = tz.isValid() ? tz.offsetAtZoneTime(datetime.dateTime()) : datetime.utcOffset();
        if (offset < 0) {
            offset = -offset;
            tzsign = '-';
        }
    }
    offset /= 60;
    return result + s.sprintf("%c%02d%s%02d", tzsign, offset / 60, tzcolon, offset % 60);
#endif
}

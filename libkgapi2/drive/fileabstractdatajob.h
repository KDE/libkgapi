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

#ifndef KGAPI2_DRIVEFILEABSTRACTDATAJOB_H
#define KGAPI2_DRIVEFILEABSTRACTDATAJOB_H

#include <libkgapi2/createjob.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2
{

namespace Drive
{

class LIBKGAPI2_EXPORT FileAbstractDataJob : public KGAPI2::Job
{

    Q_OBJECT

    /**
     * Whether to convert this file to the corresponding Google Docs format.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool convert
               READ convert
               WRITE setConvert)

    /**
     * Whether to attempt OCR on .jpg, .png, .gif, or .pdf uploads.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool ocr
               READ ocr
               WRITE setOcr)

    /**
     * If ocr is true, hints at the language to use. Valid values are
     * ISO 639-1 codes.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(QString  ocrLanguage
               READ ocrLanguage
               WRITE setOcrLanguage)

    /**
     * Whether to pin the head revision of the new file.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool pinned
               READ pinned
               WRITE setPinned)

    /**
     * The language of timed text,
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(QString timedTextLanguage
               READ timedTextLanguage
               WRITE setTimedTextLanguage)

    /**
     * The timed text track name.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(QString timedTextTrackName
               READ timedTextTrackName
               WRITE setTimedTextTrackName)

  public:
    explicit FileAbstractDataJob(const AccountPtr &account,
                                 QObject *parent = 0);
    virtual ~FileAbstractDataJob();

    bool convert() const;
    void setConvert(bool convert);

    bool ocr() const;
    void setOcr(bool ocr);

    QString ocrLanguage() const;
    void setOcrLanguage(const QString &ocrLanguage);

    bool pinned() const;
    void setPinned(bool pinned);

    QString timedTextLanguage() const;
    void setTimedTextLanguage(const QString &timedTextLanguage);

    QString timedTextTrackName() const;
    void setTimedTextTrackName(const QString &timedTextTrackName);

  protected:
    QUrl updateUrl(QUrl &url);

  private:
    class Private;
    Private *const d;
    friend class Private;

};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILEABSTRACTDATAJOB_H

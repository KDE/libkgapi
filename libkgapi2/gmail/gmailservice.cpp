/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "gmailservice.h"

#include <QStringBuilder>

inline QUrl operator%(const QUrl &url, const QString &path)
{
    return QUrl(url.toString() % QLatin1Char('/') % path);
}

namespace KGAPI2
{

namespace GmailService
{

namespace Private
{
    static const QUrl GoogleApisUrl(QLatin1String("https://www.googleapis.com"));

    QString draftBaseUrl(const QString &userId,
                         const QString &draftId = QString())
    {
        const QString draft = !draftId.isEmpty() ? (QLatin1String("/") % draftId) : QString();
        const QString path = QLatin1String("/gmail/v1/users/") % userId % QLatin1String("/drafts") % draft;
        return path;
    }

    QString labelBaseUrl(const QString &userId,
                         const QString &labelId = QString())
    {
        const QString label = !labelId.isEmpty() ? (QLatin1String("/") % labelId) : QString();
        const QString path = QLatin1String("/gmail/v1/users/") % userId % QLatin1String("/labels") % label;
        return path;
    }

    QString messageBaseUrl(const QString &userId,
                           const QString messageId = QString())
    {
        const QString message = !messageId.isEmpty() ? (QLatin1String("/") % messageId) : QString();
        const QString path = QLatin1String("/gmail/v1/users/") % userId % QLatin1String("/messages") % message;
        return path;
    }
}

}

}

using namespace KGAPI2;
using namespace KGAPI2::GmailService::Private;

QUrl GmailService::createDraftUrl(const QString &userId)
{
    return GoogleApisUrl % draftBaseUrl(userId);
}

QUrl GmailService::uploadDraftUrl(const QString &userId,
                                  const QString &draftId)
{
    return GoogleApisUrl % QLatin1String("/upload") % draftBaseUrl(userId, draftId);
}

QUrl GmailService::deleteDraftUrl(const QString &userId,
                                  const QString &draftId)
{
    return GoogleApisUrl % draftBaseUrl(userId, draftId);
}

QUrl GmailService::fetchDraftUtl(const QString &userId,
                                 const QString &draftId)
{
    return GoogleApisUrl % draftBaseUrl(userId, draftId);
}

QUrl GmailService::listDraftsUrl(const QString &userId)
{
    return GoogleApisUrl % draftBaseUrl(userId);
}

QUrl GmailService::updateDraftUrl(const QString &userId,
                                  const QString &draftId)
{
    return GoogleApisUrl % draftBaseUrl(userId, draftId);
}

QUrl GmailService::sendDraftUrl(const QString &userId)
{
    return GoogleApisUrl % draftBaseUrl(userId) % QLatin1String("/send");
}

QUrl GmailService::listHistoryUrl(const QString &userId)
{
    return GoogleApisUrl % QLatin1String("/gmail/v1/users/") % userId % QLatin1String("/history");
}

QUrl GmailService::createLabelUrl(const QString &userId)
{
    return GoogleApisUrl % labelBaseUrl(userId);
}

QUrl GmailService::deleteLabelUrl(const QString &userId,
                                  const QString &labelId)
{
    return GoogleApisUrl % labelBaseUrl(userId, labelId);
}

QUrl GmailService::fetchLabelUrl(const QString &userId,
                                 const QString &labelId)
{
    return GoogleApisUrl % labelBaseUrl(userId, labelId);
}

QUrl GmailService::listLabelsUrl(const QString &userId)
{
    return GoogleApisUrl % labelBaseUrl(userId);
}

QUrl GmailService::updateLabelUrl(const QString &userId,
                                  const QString &labelId)
{
    return GoogleApisUrl % labelBaseUrl(userId, labelId);
}

QUrl GmailService::deleteMessageUrl(const QString &userId,
                                    const QString &messageId)
{
    return GoogleApisUrl % messageBaseUrl(userId, messageId);
}

QUrl GmailService::fetchMessageUrl(const QString &userId,
                                   const QString &messageId)
{
    return GoogleApisUrl % messageBaseUrl(userId, messageId);
}

QUrl GmailService::insertMessageUrl(const QString &userId)
{
    return GoogleApisUrl % messageBaseUrl(userId);
}

QUrl GmailService::insertMessageUploadUrl(const QString &userId)
{
    return GoogleApisUrl % QLatin1String("/upload") % messageBaseUrl(userId);
}

QUrl GmailService::listMessagesUrl(const QString &userId)
{
    return GoogleApisUrl % messageBaseUrl(userId);
}

QUrl GmailService::modifyMessageUrl(const QString &userId,
                                    const QString &messageId)
{
    return GoogleApisUrl % messageBaseUrl(userId, messageId) % QLatin1String("/modify");
}

QUrl GmailService::sendMessageUrl(const QString &userId)
{
    return GoogleApisUrl % messageBaseUrl(userId) % QLatin1String("/send");
}

QUrl GmailService::trashMessageUrl(const QString &userId,
                                   const QString &messageId)
{
    return GoogleApisUrl % messageBaseUrl(userId, messageId) % QLatin1String("/trash");
}

QUrl GmailService::untrashMessageUrl(const QString &userId,
                                     const QString &messageId)
{
    return GoogleApisUrl % messageBaseUrl(userId, messageId) % QLatin1String("/untrash");
}

QUrl GmailService::importMessageUrl(const QString &userId)
{
    return GoogleApisUrl % messageBaseUrl(userId) % QLatin1String("/import");
}

QUrl GmailService::importMessageUploadUrl(const QString &userId)
{
    return GoogleApisUrl % QLatin1String("/upload") % messageBaseUrl(userId) % QLatin1String("/import");
}

QUrl GmailService::fetchAttachmentUrl(const QString &userId,
                                      const QString &messageId,
                                      const QString &attachmentId)
{
    return GoogleApisUrl % messageBaseUrl(userId, messageId) % QLatin1String("/attachments/") % attachmentId;
}

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

#ifndef KGAPI2_GMAIL_GMAILSERVICE_H
#define KGAPI2_GMAIL_GMAILSERVICE_H

#include <QUrl>

#include <libkgapi2_export.h>
#include <libkgapi2/types.h>

namespace KGAPI2
{

namespace GmailService
{

    LIBKGAPI2_EXPORT QUrl createDraftUrl(const QString &userId);
    LIBKGAPI2_EXPORT QUrl deleteDraftUrl(const QString &userId,
                                         const QString &draftId);
    LIBKGAPI2_EXPORT QUrl fetchDraftUtl(const QString &userId,
                                        const QString &draftId);
    LIBKGAPI2_EXPORT QUrl listDraftsUrl(const QString &userId);
    LIBKGAPI2_EXPORT QUrl updateDraftUrl(const QString &userId,
                                         const QString &draftId);
    LIBKGAPI2_EXPORT QUrl sendDraftUrl(const QString &userId);
    LIBKGAPI2_EXPORT QUrl uploadDraftUrl(const QString &userId,
                                         const QString &draftId = QString());

    LIBKGAPI2_EXPORT QUrl listHistoryUrl(const QString &userId);

    LIBKGAPI2_EXPORT QUrl createLabelUrl(const QString &userId);
    LIBKGAPI2_EXPORT QUrl deleteLabelUrl(const QString &userId,
                                         const QString &labelId);
    LIBKGAPI2_EXPORT QUrl listLabelsUrl(const QString &userId);
    LIBKGAPI2_EXPORT QUrl updateLabelUrl(const QString &userId,
                                         const QString &labelId);
    LIBKGAPI2_EXPORT QUrl fetchLabelUrl(const QString &userId,
                                        const QString &labelId);

    LIBKGAPI2_EXPORT QUrl deleteMessageUrl(const QString &userId,
                                           const QString &messageId);
    LIBKGAPI2_EXPORT QUrl fetchMessageUrl(const QString &userId,
                                          const QString &messageId);
    LIBKGAPI2_EXPORT QUrl insertMessageUrl(const QString &userId);
    LIBKGAPI2_EXPORT QUrl insertMessageUploadUrl(const QString &userId);
    LIBKGAPI2_EXPORT QUrl listMessagesUrl(const QString &userId);
    LIBKGAPI2_EXPORT QUrl modifyMessageUrl(const QString &userId,
                                           const QString &messageId);
    LIBKGAPI2_EXPORT QUrl sendMessageUrl(const QString &userId);
    LIBKGAPI2_EXPORT QUrl trashMessageUrl(const QString &userId,
                                          const QString &messageId);
    LIBKGAPI2_EXPORT QUrl untrashMessageUrl(const QString &userId,
                                            const QString &messageId);
    LIBKGAPI2_EXPORT QUrl importMessageUrl(const QString &userId);
    LIBKGAPI2_EXPORT QUrl importMessageUploadUrl(const QString &userId);

    LIBKGAPI2_EXPORT QUrl fetchAttachmentUrl(const QString &userId,
                                             const QString &messageId,
                                             const QString &attachmentId);
}

}

#endif
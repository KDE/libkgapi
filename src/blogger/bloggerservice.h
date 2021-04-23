/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "types.h"
#include "kgapiblogger_export.h"

#include <QUrl>

namespace KGAPI2
{

namespace BloggerService
{

    KGAPIBLOGGER_EXPORT QUrl fetchBlogByBlogIdUrl(const QString &blogId);
    KGAPIBLOGGER_EXPORT QUrl fetchBlogByBlogUrlUrl(const QString &blogUrl);
    KGAPIBLOGGER_EXPORT QUrl fetchBlogsByUserIdUrl(const QString &userId);

    KGAPIBLOGGER_EXPORT QUrl fetchCommentsUrl(const QString &blogId,
                                           const QString &postId = QString(),
                                           const QString &commentId = QString());

    KGAPIBLOGGER_EXPORT QUrl approveCommentUrl(const QString &blogId,
                                            const QString &postId,
                                            const QString &commentId);
    KGAPIBLOGGER_EXPORT QUrl markCommentAsSpamUrl(const QString &blogId,
                                               const QString &postId,
                                               const QString &commentId);
    KGAPIBLOGGER_EXPORT QUrl deleteCommentUrl(const QString &blogId,
                                           const QString &postId,
                                           const QString &commentId);
    KGAPIBLOGGER_EXPORT QUrl deleteCommentContentUrl(const QString &blogId,
                                                  const QString &postId,
                                                  const QString &commentId);

    KGAPIBLOGGER_EXPORT QUrl fetchPageUrl(const QString &blogId,
                                       const QString &pageId);
    KGAPIBLOGGER_EXPORT QUrl deletePageUrl(const QString &blogId,
                                        const QString &pageId);
    KGAPIBLOGGER_EXPORT QUrl modifyPageUrl(const QString &blogId,
                                        const QString &pageId);
    KGAPIBLOGGER_EXPORT QUrl createPageUrl(const QString &blogId);

    KGAPIBLOGGER_EXPORT QUrl fetchPostUrl(const QString &blogId,
                                       const QString &postId);
    KGAPIBLOGGER_EXPORT QUrl searchPostUrl(const QString &blogId);
    KGAPIBLOGGER_EXPORT QUrl createPostUrl(const QString &blogId);
    KGAPIBLOGGER_EXPORT QUrl deletePostUrl(const QString &blogId,
                                        const QString &postId);
    KGAPIBLOGGER_EXPORT QUrl modifyPostUrl(const QString &blogId,
                                        const QString &postId);
    KGAPIBLOGGER_EXPORT QUrl publishPostUrl(const QString &blogId,
                                         const QString &postId);
    KGAPIBLOGGER_EXPORT QUrl revertPostUrl(const QString &blogId,
                                        const QString &postId);

} // namespace BloggerService
} // namespace KGAPI2


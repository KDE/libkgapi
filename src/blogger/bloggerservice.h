/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "types.h"

#include <QUrl>

namespace KGAPI2
{

namespace BloggerService
{

QUrl fetchBlogByBlogIdUrl(const QString &blogId);
QUrl fetchBlogByBlogUrlUrl(const QString &blogUrl);
QUrl fetchBlogsByUserIdUrl(const QString &userId);

QUrl fetchCommentsUrl(const QString &blogId, const QString &postId = QString(), const QString &commentId = QString());

QUrl approveCommentUrl(const QString &blogId, const QString &postId, const QString &commentId);
QUrl markCommentAsSpamUrl(const QString &blogId, const QString &postId, const QString &commentId);
QUrl deleteCommentUrl(const QString &blogId, const QString &postId, const QString &commentId);
QUrl deleteCommentContentUrl(const QString &blogId, const QString &postId, const QString &commentId);

QUrl fetchPageUrl(const QString &blogId, const QString &pageId);
QUrl deletePageUrl(const QString &blogId, const QString &pageId);
QUrl modifyPageUrl(const QString &blogId, const QString &pageId);
QUrl createPageUrl(const QString &blogId);

QUrl fetchPostUrl(const QString &blogId, const QString &postId);
QUrl searchPostUrl(const QString &blogId);
QUrl createPostUrl(const QString &blogId);
QUrl deletePostUrl(const QString &blogId, const QString &postId);
QUrl modifyPostUrl(const QString &blogId, const QString &postId);
QUrl publishPostUrl(const QString &blogId, const QString &postId);
QUrl revertPostUrl(const QString &blogId, const QString &postId);

} // namespace BloggerService
} // namespace KGAPI2

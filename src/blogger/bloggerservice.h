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

#ifndef KGAPI2_BLOGGERSERVICE_H
#define KGAPI2_BLOGGERSERVICE_H

#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QUrl>

namespace KGAPI2
{

namespace BloggerService
{

    LIBKGAPI2_EXPORT QUrl fetchBlogByBlogIdUrl(const QString &blogId);
    LIBKGAPI2_EXPORT QUrl fetchBlogByBlogUrlUrl(const QString &blogUrl);
    LIBKGAPI2_EXPORT QUrl fetchBlogsByUserIdUrl(const QString &userId);

    LIBKGAPI2_EXPORT QUrl fetchCommentsUrl(const QString &blogId,
                                           const QString &postId = QString(),
                                           const QString &commentId = QString());

    LIBKGAPI2_EXPORT QUrl approveCommentUrl(const QString &blogId,
                                            const QString &postId,
                                            const QString &commentId);
    LIBKGAPI2_EXPORT QUrl markCommentAsSpamUrl(const QString &blogId,
                                               const QString &postId,
                                               const QString &commentId);
    LIBKGAPI2_EXPORT QUrl deleteCommentUrl(const QString &blogId,
                                           const QString &postId,
                                           const QString &commentId);
    LIBKGAPI2_EXPORT QUrl deleteCommentContentUrl(const QString &blogId,
                                                  const QString &postId,
                                                  const QString &commentId);

    LIBKGAPI2_EXPORT QUrl fetchPageUrl(const QString &blogId,
                                       const QString &pageId);
    LIBKGAPI2_EXPORT QUrl deletePageUrl(const QString &blogId,
                                        const QString &pageId);
    LIBKGAPI2_EXPORT QUrl modifyPageUrl(const QString &blogId,
                                        const QString &pageId);
    LIBKGAPI2_EXPORT QUrl createPageUrl(const QString &blogId);

    LIBKGAPI2_EXPORT QUrl fetchPostUrl(const QString &blogId,
                                       const QString &postId);
    LIBKGAPI2_EXPORT QUrl searchPostUrl(const QString &blogId);
    LIBKGAPI2_EXPORT QUrl createPostUrl(const QString &blogId);
    LIBKGAPI2_EXPORT QUrl deletePostUrl(const QString &blogId,
                                        const QString &postId);
    LIBKGAPI2_EXPORT QUrl modifyPostUrl(const QString &blogId,
                                        const QString &postId);
    LIBKGAPI2_EXPORT QUrl publishPostUrl(const QString &blogId,
                                         const QString &postId);
    LIBKGAPI2_EXPORT QUrl revertPostUrl(const QString &blogId,
                                        const QString &postId);

} // namespace BloggerService
} // namespace KGAPI2

#endif // KGAPI2_BLOGGERSERVICE_H

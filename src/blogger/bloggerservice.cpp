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

#include "bloggerservice.h"

inline QUrl operator%(const QUrl &url, const QString &path)
{
    return QUrl(url.toString() % QLatin1Char('/') % path);
}

namespace KGAPI2
{
namespace BloggerService
{
namespace Private
{
    static const QUrl GoogleApisUrl(QLatin1String("https://www.googleapis.com"));

    auto commentBasePath(const QString &blogId,
                         const QString &postId = QString(),
                         const QString &commentId = QString()) -> QString
    {
        const auto post = !postId.isEmpty() ? (QLatin1String("/posts/") % postId) : QString();
        const auto comment = !commentId.isEmpty() ? (QLatin1Char('/') % commentId) : QString();
        auto path = QLatin1String("blogger/v3/blogs/") % blogId 
                        % post % QLatin1String("/comments") % comment;
        return path;
    }

    auto pageBasePath(const QString &blogId,
                      const QString &pageId = QString()) -> QString
    {
        const auto page = !pageId.isEmpty() ? (QLatin1Char('/') % pageId) : QString();
        const auto path = QLatin1String("blogger/v3/blogs/") % blogId
                            % QLatin1String("/pages") % page;
        return path;
    }

    auto postBasePath(const QString &blogId,
                      const QString &postId = QString()) -> QString
    {
        const auto post = !postId.isEmpty() ? (QLatin1Char('/') % postId) : QString();
        const auto path = QLatin1String("blogger/v3/blogs/") % blogId % QLatin1String("/posts");
        return path;
    }

} // namespace Private
} // namespace BloggerService
} // namespace KGAPI2


using namespace KGAPI2;
using namespace KGAPI2::BloggerService::Private;

QUrl BloggerService::fetchBlogByBlogIdUrl(const QString &blogId)
{
    return GoogleApisUrl % QLatin1String("/blogger/v3/blogs/") % blogId;
}

QUrl BloggerService::fetchBlogByBlogUrlUrl(const QString &blogUrl)
{
    QUrl url = GoogleApisUrl % QLatin1String("/blogger/v3/blogs/byurl");
    url.addQueryItem(QLatin1String("url"), blogUrl);
    return url;
}

QUrl BloggerService::fetchBlogsByUserIdUrl(const QString &userId)
{
    return GoogleApisUrl % QLatin1String("/blogger/v3/users/") % userId % QLatin1String("/blogs");
}


QUrl BloggerService::fetchCommentsUrl(const QString &blogId,
                                      const QString &postId,
                                      const QString &commentId)
{
    return GoogleApisUrl % commentBasePath(blogId, postId, commentId);
}

QUrl BloggerService::approveCommentUrl(const QString &blogId,
                                       const QString &postId,
                                       const QString &commentId)
{
    return GoogleApisUrl % commentBasePath(blogId, postId, commentId) % QLatin1String("/approve");
}

QUrl BloggerService::markCommentAsSpamUrl(const QString &blogId,
                                          const QString &postId,
                                          const QString &commentId)
{
    return GoogleApisUrl % commentBasePath(blogId, postId, commentId) % QLatin1String("/spam");
}

QUrl BloggerService::deleteCommentUrl(const QString &blogId,
                                      const QString &postId,
                                      const QString &commentId)
{
    return GoogleApisUrl % commentBasePath(blogId, postId, commentId);
}

QUrl BloggerService::deleteCommentContentUrl(const QString &blogId,
                                             const QString &postId,
                                             const QString &commentId)
{
    return GoogleApisUrl % commentBasePath(blogId, postId, commentId) % QLatin1String("/removecontent");
}

QUrl BloggerService::fetchPageUrl(const QString &blogId,
                                  const QString &pageId)
{
    return GoogleApisUrl % pageBasePath(blogId, pageId);
}

QUrl BloggerService::deletePageUrl(const QString &blogId, const QString &pageId)
{
    return GoogleApisUrl % pageBasePath(blogId, pageId);
}

QUrl BloggerService::modifyPageUrl(const QString &blogId, const QString &pageId)
{
    return GoogleApisUrl % pageBasePath(blogId, pageId);
}

QUrl BloggerService::createPageUrl(const QString &blogId)
{
    return GoogleApisUrl % pageBasePath(blogId);
}

QUrl BloggerService::fetchPostUrl(const QString &blogId, const QString &postId)
{
    return GoogleApisUrl % postBasePath(blogId, postId);
}

QUrl BloggerService::searchPostUrl(const QString &blogId)
{
    return GoogleApisUrl % postBasePath(blogId) % QLatin1String("/search");
}

QUrl BloggerService::createPostUrl(const QString &blogId)
{
    return GoogleApisUrl % postBasePath(blogId);
}

QUrl BloggerService::deletePostUrl(const QString &blogId, const QString &postId)
{
    return GoogleApisUrl % postBasePath(blogId, postId);
}

QUrl BloggerService::modifyPostUrl(const QString &blogId, const QString &postId)
{
    return GoogleApisUrl % postBasePath(blogId, postId);
}

QUrl BloggerService::publishPostUrl(const QString &blogId, const QString &postId)
{
    return GoogleApisUrl % postBasePath(blogId, postId) % QLatin1String("/publish");
}

QUrl BloggerService::revertPostUrl(const QString &blogId, const QString &postId)
{
    return GoogleApisUrl % postBasePath(blogId, postId) % QLatin1String("/revert");
}

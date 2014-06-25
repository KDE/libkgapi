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

#include <KUrl>

namespace KGAPI2
{
namespace BloggerService
{
namespace Private
{
    KUrl commentBaseUrl(const QString &blogId,
                        const QString &postId = QString(),
                        const QString &commentId = QString())
    {
        KUrl url(QLatin1String("https://www.googleapis.com/blogger/v3/blogs/"));
        url.addPath(blogId);
        if (!postId.isEmpty()) {
            url.addPath(QLatin1String("posts"));
            url.addPath(postId);
        }
        url.addPath(QLatin1String("comments"));
        if (!commentId.isEmpty()) {
            url.addPath(commentId);
        }
        return url;
    }

    KUrl pageBaseUrl(const QString &blogId,
                     const QString &pageId = QString())
    {
        KUrl url(QLatin1String("https://www.googleapis.com/blogger/v3/blogs/"));
        url.addPath(blogId);
        url.addPath(QLatin1String("pages"));
        if (!pageId.isEmpty()) {
            url.addPath(pageId);
        }
        return url;
    }

    KUrl postBaseUrl(const QString &blogId,
                     const QString &postId = QString())
    {
        KUrl url(QLatin1String("https://www.googleapis.com/blogger/v3/blogs/"));
        url.addPath(blogId);
        url.addPath(QLatin1String("posts"));
        if (!postId.isEmpty()) {
            url.addPath(postId);
        }
        return url;
    }

} // namespace Private
} // namespace BloggerService
} // namespace KGAPI2


using namespace KGAPI2;

QUrl BloggerService::fetchBlogByBlogIdUrl(const QString &blogId)
{
    KUrl url(QLatin1String("https://www.googleapis.com/blogger/v3/blogs/"));
    url.addPath(blogId);
    return url;
}

QUrl BloggerService::fetchBlogByBlogUrlUrl(const QString &blogUrl)
{
    KUrl url(QLatin1String("https://www.googleapis.com/blogger/v3/blogs/byurl"));
    url.addQueryItem(QLatin1String("url"), blogUrl);
    return url;
}

QUrl BloggerService::fetchBlogsByUserIdUrl(const QString &userId)
{
    KUrl url(QLatin1String("https://www.googleapis.com/blogger/v3/users/"));
    url.addPath(userId);
    url.addPath(QLatin1String("blogs"));
    return url;
}


QUrl BloggerService::fetchCommentsUrl(const QString &blogId,
                                      const QString &postId,
                                      const QString &commentId)
{
    return Private::commentBaseUrl(blogId, postId, commentId);
}

QUrl BloggerService::approveCommentUrl(const QString &blogId,
                                       const QString &postId,
                                       const QString &commentId)
{
    KUrl url = Private::commentBaseUrl(blogId, postId, commentId);
    url.addPath(QLatin1String("approve"));
    return url;
}

QUrl BloggerService::markCommentAsSpamUrl(const QString &blogId,
                                          const QString &postId,
                                          const QString &commentId)
{
    KUrl url = Private::commentBaseUrl(blogId, postId, commentId);
    url.addPath(QLatin1String("spam"));
    return url;
}

QUrl BloggerService::deleteCommentUrl(const QString &blogId,
                                      const QString &postId,
                                      const QString &commentId)
{
    return Private::commentBaseUrl(blogId, postId, commentId);
}

QUrl BloggerService::deleteCommentContentUrl(const QString &blogId,
                                             const QString &postId,
                                             const QString &commentId)
{
    KUrl url = Private::commentBaseUrl(blogId, postId, commentId);
    url.addPath(QLatin1String("removecontent"));
    return url;
}

QUrl BloggerService::fetchPageUrl(const QString &blogId,
                                  const QString &pageId)
{
    return Private::pageBaseUrl(blogId, pageId);
}

QUrl BloggerService::deletePageUrl(const QString &blogId, const QString &pageId)
{
    return Private::pageBaseUrl(blogId, pageId);
}

QUrl BloggerService::modifyPageUrl(const QString &blogId, const QString &pageId)
{
    return Private::pageBaseUrl(blogId, pageId);
}

QUrl BloggerService::createPageUrl(const QString &blogId)
{
    return Private::pageBaseUrl(blogId);
}

QUrl BloggerService::fetchPostUrl(const QString &blogId, const QString &postId)
{
    return Private::postBaseUrl(blogId, postId);
}

QUrl BloggerService::searchPostUrl(const QString &blogId)
{
    KUrl url = Private::postBaseUrl(blogId);
    url.addPath(QLatin1String("search"));
    return url;
}

QUrl BloggerService::createPostUrl(const QString &blogId)
{
    return Private::postBaseUrl(blogId);
}

QUrl BloggerService::deletePostUrl(const QString &blogId, const QString &postId)
{
    return Private::postBaseUrl(blogId, postId);
}

QUrl BloggerService::modifyPostUrl(const QString &blogId, const QString &postId)
{
    return Private::postBaseUrl(blogId, postId);
}

QUrl BloggerService::publishPostUrl(const QString &blogId, const QString &postId)
{
    KUrl url = Private::postBaseUrl(blogId, postId);
    url.addPath(QLatin1String("publish"));
    return url;
}

QUrl BloggerService::revertPostUrl(const QString &blogId, const QString &postId)
{
    KUrl url = Private::postBaseUrl(blogId, postId);
    url.addPath(QLatin1String("revert"));
    return url;
}

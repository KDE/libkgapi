/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "bloggerservice.h"

#include <QUrlQuery>

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
static const QUrl GoogleApisUrl(QStringLiteral("https://www.googleapis.com"));

auto commentBasePath(const QString &blogId, const QString &postId = QString(), const QString &commentId = QString()) -> QString
{
    const auto post = !postId.isEmpty() ? (QLatin1StringView("/posts/") % postId) : QString();
    const auto comment = !commentId.isEmpty() ? (QLatin1Char('/') % commentId) : QString();
    const QString path = QLatin1StringView("blogger/v3/blogs/") % blogId % post % QLatin1String("/comments") % comment;
    return path;
}

auto pageBasePath(const QString &blogId, const QString &pageId = QString()) -> QString
{
    const auto page = !pageId.isEmpty() ? (QLatin1Char('/') % pageId) : QString();
    const QString path = QLatin1StringView("blogger/v3/blogs/") % blogId % QLatin1String("/pages") % page;
    return path;
}

auto postBasePath(const QString &blogId, const QString &postId = QString()) -> QString
{
    const auto post = !postId.isEmpty() ? (QLatin1Char('/') % postId) : QString();
    const QString path = QLatin1StringView("blogger/v3/blogs/") % blogId % QLatin1String("/posts") % post;
    return path;
}

} // namespace Private
} // namespace BloggerService
} // namespace KGAPI2

using namespace KGAPI2;
using namespace KGAPI2::BloggerService::Private;

QUrl BloggerService::fetchBlogByBlogIdUrl(const QString &blogId)
{
    return GoogleApisUrl % QStringLiteral("/blogger/v3/blogs/") % blogId;
}

QUrl BloggerService::fetchBlogByBlogUrlUrl(const QString &blogUrl)
{
    QUrl url = GoogleApisUrl % QStringLiteral("/blogger/v3/blogs/byurl");
    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("url"), blogUrl);
    url.setQuery(query);
    return url;
}

QUrl BloggerService::fetchBlogsByUserIdUrl(const QString &userId)
{
    return GoogleApisUrl % QStringLiteral("/blogger/v3/users/") % userId % QStringLiteral("/blogs");
}

QUrl BloggerService::fetchCommentsUrl(const QString &blogId, const QString &postId, const QString &commentId)
{
    return GoogleApisUrl % commentBasePath(blogId, postId, commentId);
}

QUrl BloggerService::approveCommentUrl(const QString &blogId, const QString &postId, const QString &commentId)
{
    return GoogleApisUrl % commentBasePath(blogId, postId, commentId) % QStringLiteral("/approve");
}

QUrl BloggerService::markCommentAsSpamUrl(const QString &blogId, const QString &postId, const QString &commentId)
{
    return GoogleApisUrl % commentBasePath(blogId, postId, commentId) % QStringLiteral("/spam");
}

QUrl BloggerService::deleteCommentUrl(const QString &blogId, const QString &postId, const QString &commentId)
{
    return GoogleApisUrl % commentBasePath(blogId, postId, commentId);
}

QUrl BloggerService::deleteCommentContentUrl(const QString &blogId, const QString &postId, const QString &commentId)
{
    return GoogleApisUrl % commentBasePath(blogId, postId, commentId) % QStringLiteral("/removecontent");
}

QUrl BloggerService::fetchPageUrl(const QString &blogId, const QString &pageId)
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
    return GoogleApisUrl % postBasePath(blogId) % QStringLiteral("/search");
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
    return GoogleApisUrl % postBasePath(blogId, postId) % QStringLiteral("/publish");
}

QUrl BloggerService::revertPostUrl(const QString &blogId, const QString &postId)
{
    return GoogleApisUrl % postBasePath(blogId, postId) % QStringLiteral("/revert");
}

/*
    Copyright (C) 2012  Jan Grulich <grulja@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef LIBKGAPI2_LATITUDESERVICE_H
#define LIBKGAPI2_LATITUDESERVICE_H

#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QVariantMap>

namespace KGAPI2
{

/**
 * @brief A Helper QObject
 */
class Latitude: public QObject
{
    Q_OBJECT
    Q_ENUMS(Granularity)

  public:
    enum Granularity {
        City, Best
    };
};


/**
 * @brief Additional methods for implementing support for Google Latitude service
 *
 * You should never need to use these methods, unless implementing your own Job
 */
namespace LatitudeService
{

    /**
     * @brief Parses JSON data into a Location object
     *
     * @param jsonData
     */
    LIBKGAPI2_EXPORT LocationPtr JSONToLocation(const QByteArray& jsonData);

    /**
     * @brief Serializes a Location object to JSON
     *
     * @param location
     */
    LIBKGAPI2_EXPORT QByteArray locationToJSON(const LocationPtr &location);


    /**
     * @brief Parses a JSON feed into list of Locations
     *
     * @param jsonFeed
     * @param feedData The structure will be filled with additional data about
     *                 the feed
     */
    LIBKGAPI2_EXPORT ObjectsList parseLocationJSONFeed(const QByteArray& jsonFeed, FeedData& feedData);

    /**
     * @brief Supported version of API
     */
    LIBKGAPI2_EXPORT QString APIVersion();

    /**
     * @brief Returns URL for fetch current user's location
     *
     * @param granularity
     */
    LIBKGAPI2_EXPORT QUrl retrieveCurrentLocationUrl(const Latitude::Granularity granularity = Latitude::City);

    /**
     *  @brief Returns URL for deleting user's current location
     */
    LIBKGAPI2_EXPORT QUrl deleteCurrentLocationUrl();

    /**
     *  @brief Returns URL for uploading user's current location
     */
    LIBKGAPI2_EXPORT QUrl insertCurrentLocationUrl();

    /**
     * @brief Returns URL for fetching history of user's locations
     *
     * @param granularity Precision of the coordinates
     * @param maxResults Maximum amount of results to return
     * @param maxTime Maximum timestamp since epoch (in ms)
     * @param minTime Minimum timestamp since epoch (in ms)
     */
    LIBKGAPI2_EXPORT QUrl locationHistoryUrl(const Latitude::Granularity granularity,
                                            const int maxResults = 0,
                                            const qlonglong maxTime = 0,
                                            const qlonglong minTime = 0);

    /**
     * @brief URL for fetching a specific location
     *
     * @param id
     * @param granularity;
     */
    LIBKGAPI2_EXPORT QUrl retrieveLocationUrl(const qlonglong id,
                                             const Latitude::Granularity granularity = Latitude::City);

    /**
     * @brief Returns URL for uploading a location
     */
    LIBKGAPI2_EXPORT QUrl insertLocationUrl();

    /**
     * @brief Returns URL to delete a specific location
     *
     * @param id
     */
    LIBKGAPI2_EXPORT QUrl deleteLocationUrl(const qlonglong id);

} // namespace LatitudeService

} // namespace KGAPI2

#endif // LIBKGAPI2_LATITUDESERVICE_H

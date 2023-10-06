/*
    SPDX-FileCopyrightText: 2012 Jan Grulich <grulja@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "kgapimaps_export.h"

#include <QColor>

#include <KContacts/Address>
#include <KContacts/Geo>

namespace KGAPI2
{

/**
 * @brief Represents marker with defined label, color, size and markers
 *
 * @author: Jan Grulich <grulja@gmail.com>
 * @since: 0.4
 */
class KGAPIMAPS_EXPORT StaticMapMarker
{
public:
    enum MarkerSize { Tiny, Small, Middle, Normal };

    enum LocationType { Undefined = -1, String, KABCAddress, KABCGeo };

    /**
     * @brief Constructs an empty marker
     */
    StaticMapMarker();

    /**
     * @brief Constructs a new marker
     *
     * @param address Location in QString where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     * @param size Specifies the size of marker
     * @param color Color of marker
     */
    explicit StaticMapMarker(const QString &address, const QChar &label = QChar(), const MarkerSize size = Normal, const QColor &color = Qt::red);

    /**
     * @brief Constructs a new marker
     *
     * @param address Location in KContacts::Address where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     * @param size Specifies the size of marker
     * @param color Color of marker
     */
    explicit StaticMapMarker(const KContacts::Address &address, QChar label = QChar(), const MarkerSize size = Normal, const QColor &color = Qt::red);

    /**
     * @brief Constructs a new marker
     *
     * @param address Location in KContacts::Geo where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     * @param size Specifies the size of marker
     * @param color Color of marker
     */
    explicit StaticMapMarker(const KContacts::Geo &address, QChar label = QChar(), const MarkerSize size = Normal, const QColor &color = Qt::red);

    /**
     * @brief Constructs a new marker
     *
     * @param locations Locations as a QStringList where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     * @param size Specifies the size of marker
     * @param color Color of marker
     */
    explicit StaticMapMarker(const QStringList &locations, QChar label = QChar(), const MarkerSize size = Normal, const QColor &color = Qt::red);

    /**
     * @brief Constructs a new marker
     *
     * @param locations Locations in KContacts::Address where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     * @param size Specifies the size of marker
     * @param color Color of marker
     */
    explicit StaticMapMarker(const KContacts::Address::List &locations, QChar label = QChar(), const MarkerSize size = Normal, const QColor &color = Qt::red);

    /**
     * @brief Constructs a new marker
     *
     * @param locations Locations in KContacts::Geo where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     * @param size Specifies the size of marker
     * @param color Color of marker
     */
    explicit StaticMapMarker(const QList<KContacts::Geo> &locations, QChar label = QChar(), const MarkerSize size = Normal, const QColor &color = Qt::red);

    /**
     * @brief Copy constructor
     */
    StaticMapMarker(const StaticMapMarker &other);

    /**
     * @brief Destructor
     */
    ~StaticMapMarker();

    /**
     * @brief Returns in which format is location saved.
     */
    [[nodiscard]] LocationType locationType() const;

    /**
     * @brief Returns color of marker
     */
    [[nodiscard]] QColor color() const;

    /**
     * @brief Sets color of marker
     *
     * @param color Color for marker
     */
    void setColor(const QColor &color);

    /**
     * @brief Returns if marker is valid. It means that marker needs defined location
     */
    [[nodiscard]] bool isValid() const;

    /**
     * @brief Returns label of marker
     */
    [[nodiscard]] QChar label() const;

    /**
     * @brief Sets label of marker
     *
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     */
    void setLabel(QChar label);

    /**
     * @brief Returns locations in QString
     */
    [[nodiscard]] QStringList locationsString() const;

    /**
     * @brief Sets one location for marker
     *
     * @param location Location for marker in QString
     */
    void setLocation(const QString &location);

    /**
     * @brief Sets locations for marker
     *
     * @param locations Locations for marker in QString
     */
    void setLocations(const QStringList &locations);

    /**
     * @brief Returns locations in KContacts::Address
     */
    [[nodiscard]] KContacts::Address::List locationsAddress() const;

    /**
     * @brief Sets one location for marker
     *
     * @param location Location for marker in KContacts::Address
     */
    void setLocation(const KContacts::Address &location);

    /**
     * @brief Sets locations for marker
     *
     * @param locations Locations for marker in KContacts::Address
     */
    void setLocations(const KContacts::Address::List &locations);

    /**
     * @brief Returns locations in KContacts::Geo
     */
    [[nodiscard]] QList<KContacts::Geo> locationsGeo() const;

    /**
     * @brief Sets one location for marker
     *
     * @param location Location for marker in KContacts::Geo
     */
    void setLocation(const KContacts::Geo &location);

    /**
     * @brief Sets locations for marker
     *
     * @param locations Locations for marker in KContacts::Geo
     */
    void setLocations(const QList<KContacts::Geo> &locations);

    /**
     * @brief Returns all locations and markers preferences in format to URL query.
     */
    [[nodiscard]] QString toString() const;

    /**
     * @brief Returns size of marker
     */
    [[nodiscard]] MarkerSize size() const;

    /**
     * @brief Sets size of marker
     *
     * @param size Specifies the size of marker
     */
    void setSize(const MarkerSize size);

    /**
     * @brief Assignment operator
     */
    StaticMapMarker &operator=(const StaticMapMarker &other);

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace KGAPI2

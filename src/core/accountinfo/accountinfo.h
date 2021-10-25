/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "object.h"
#include "kgapicore_export.h"

namespace KGAPI2
{

/**
 * @brief AccountInfo contains information about user's Google account.
 *
 * It is possible to obtain only information about account to which
 * we have access token.
 *
 * Some information might be empty, but id and email are guaranteed to
 * be always filled.
 *
 * The accountInfo service provides read-only access.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.2
 */
class KGAPICORE_EXPORT AccountInfo : public KGAPI2::Object
{
  public:
    /**
     * @brief Constructor
     */
    AccountInfo();

    /**
     * @brief Copy constructor
     */
    AccountInfo(const AccountInfo &other);

    /**
     * @brief destructor
     */
    ~AccountInfo() override;

    bool operator==(const AccountInfo &other) const;
    bool operator!=(const AccountInfo &other) const { return !operator==(other); }

    /**
     * @brief Sets an account ID.
     *
     * @param id
     */
    void setId(const QString &id);

    /**
     * @brief Returns account ID.
     */
    QString id() const;

    /**
     * @brief Sets account email.
     *
     * @param email
     */
    void setEmail(const QString &email);

    /**
     * @brief Returns account email address.
     *
     * Note that address does not have to be \@gmail.com.
     */
    QString email() const;

    /**
     * @brief Sets user's real full name.
     *
     * @param name
     */
    void setName(const QString &name);

    /**
     * @brief Returns user's real full name.
     */
    QString name() const;

    /**
     * @brief Sets user's given name.
     *
     * @param givenName
     */
    void setGivenName(const QString &givenName);

    /**
     * @brief Returns user's given name.
     */
    QString givenName() const;

    /**
     * @brief Sets user's family name (surname).
     *
     * @param familyName
     */
    void setFamilyName(const QString &familyName);

    /**
     * @brief Returns user's surname.
     */
    QString familyName() const;

    /**
     * @brief Sets user's birthday
     *
     * @param birthday
     */
    void setBirthday(const QString &birthday);

    /**
     * @brief Returns user's birthday.
     */
    QString birthday() const;

    /**
     * @brief Sets user's gender.
     *
     * @param gender
     */
    void setGender(const QString &gender);

    /**
     * @brief Returns user's gender.
     */
    QString gender() const;

    /**
     * @brief Sets link to user's profile.
     *
     * @param link
     */
    void setLink(const QString &link);

    /**
     * @brief Returns link to user's profile.
     */
    QString link() const;

    /**
     * @brief Sets users locale settings.
     *
     * @param locale
     */
    void setLocale(const QString &locale);

    /**
     * @brief Returns user's preferred locales.
     */
    QString locale() const;

    /**
     * @brief Sets user's timezone name.
     *
     * @param timezone
     */
    void setTimezone(const QString &timezone);

    /**
     * @brief Returns name of user's timezone.
     */
    QString timezone() const;

    /**
     * @brief Sets whether the email address is verified.
     *
     * @param verified
     */
    void setVerifiedEmail(bool verified);

    /**
     * @brief Returns whether the email is verified.
     */
    bool verifiedEmail() const;

    /**
     * @brief Sets URL of user's photo.
     *
     * @param url
     */
    void setPhotoUrl(const QString &url);

    /**
     * @brief Returns URL of user's photo.
     */
    QString photoUrl() const;

    /**
     * @brief Parses raw JSON data into AccountInfo object.
     *
     * @param jsonData JSON data to parse
     */
    static AccountInfoPtr fromJSON(const QByteArray &jsonData);
  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2


/*
    libKGoogle - Objects - AccountInfo
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef LIKBGOOGLE_OBJECTS_ACCOUNTINFO_H
#define LIBKGOOGLE_OBJECTS_ACCOUNTINFO_H

#include <libkgoogle/object.h>

#include <qshareddata.h>
#include <qmetatype.h>

namespace KGoogle {

  namespace Objects {

    class AccountInfoData;

    /**
     * AccountInfo contains informations about user's Google account.
     *
     * It is possible to obtain only informations about account to which
     * we have access token.
     *
     * Some informations might be empty, but id and email are guaranteed to 
     * be always filled.
     *
     * The accountInfo service provides read-only acces.
     *
     * Access to each property must be explicitly requested via scope URL
     * https://Fwww.googleapis.com/auth/userinfo.%propertyName%.
     * To access all properties, use 'profile' scope.
     */
    class AccountInfo : public KGoogle::Object
    {
      public:
        AccountInfo();

        AccountInfo(const AccountInfo &other);

        virtual ~AccountInfo();

        /**
         * Sets an account ID.
         *
         * @see id()
         */
        void setId(const QString &id);

        /**
         * Returns account ID.
         */
        QString id() const;

        /**
         * Sets account email.
         */
        void setEmail(const QString &email);

        /**
         * Returns account email address.
         *
         * The address does not have to be @gmail.com.
         */
        QString email() const;

        /**
         * Sets user's real full name.
         */
        void setName(const QString &name);

        /**
         * Returns user's real full name.
         */
        QString name() const;

        /**
         * Sets user's given name.
         */
        void setGivenName(const QString &givenName);

        /**
         * Returns user's given name.
         */
        QString givenName() const;

        /**
         * Sets user's family name (surname).
         */
        void setFamilyName(const QString &familyName);

        /**
         * Returns user's surname.
         */
        QString familyName() const;

        /**
         * Sets user's birthday
         */
         void setBirthday(const QString &birthday);

         /**
          * Returns user's birthday.
          */
        QString birthday() const;

        /**
         * Sets user's gender.
         */
        void setGender(const QString &gender);

        /**
         * Returns user's gender.
         */
        QString gender() const;

        /**
         * Sets link to user's profile.
         */
        void setLink(const QString &link);

        /**
         * Returns link to user's profile.
         */
        QString link() const;

        /**
         * Sets users locale settings.
         */
        void setLocale(const QString &locale);

        /**
         * Returns user's preffered locales.
         */
        QString locale() const;

        /**
         * Sets user's timezone name.
         */
        void setTimezone(const QString &timezone);

        /**
         * Returns name of user's timezone.
         */
        QString timezone() const;

        /**
         * Sets whether the email address is verified.
         */
        void setVerifiedEmail(const bool verifiedEmail);

        /**
         * Returns whether the email is verified.
         */
        bool verifiedEmail() const;

        /**
         * Sets URL of user's photo.
         */
        void setPhotoUrl(const QString &url);

        /**
         * Returns URL of user's photo.
         */
        const QString& photoUrl() const;

      private:
        QExplicitlySharedDataPointer<AccountInfoData> d;

    };

  } /* namespace Objects */

} /* namespace KGoogle */

Q_DECLARE_METATYPE(KGoogle::Objects::AccountInfo);
Q_DECLARE_METATYPE(KGoogle::Objects::AccountInfo*);

#endif // LIBKGOOGLE_OBJECTS_ACCOUNTINFO_H

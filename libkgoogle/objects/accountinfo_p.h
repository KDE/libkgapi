/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIKBGOOGLE_OBJECTS_ACCOUNTINFO_P_H
#define LIBKGOOGLE_OBJECTS_ACCOUNTINFO_P_H

#include <QtCore/QSharedData>
#include <QtCore/QString>

namespace KGoogle
{

namespace Objects
{

class AccountInfoData: public QSharedData
{
public:
    AccountInfoData() { };
    AccountInfoData(const AccountInfoData &other);
    ~AccountInfoData() { };

    QString id;
    QString email;
    QString name;
    QString givenName;
    QString familyName;

    QString birthday;
    QString gender;

    QString link;
    QString locale;
    QString timezone;

    bool verifiedEmail;

    QString pictureUrl;
};

} /* namespace Objects */

} /* namespace KGoogle */

#endif // LIBKGOOGLE_OBJECTS_ACCOUNTINFO_P_H

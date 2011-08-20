/*
    libKGoogle - KGoogleObject - Contact
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

#ifndef CONTACT_P_H
#define CONTACT_P_H

#include "qshareddata.h"
#include "qstring.h"
#include "qurl.h"

#include "kabc/address.h"
#include "kabc/phonenumber.h"
#include "kdatetime.h"

namespace KGoogle {
  
  namespace Object {
  
    class ContactData : public QSharedData
    {

      public:
	ContactData() { };
	ContactData(const ContactData &other);
	~ContactData() { };
      
	bool deleted;
	QUrl photoUrl;
	
	QString name;
	QString notes;
	QString job;
	QString jobTitle;
	QString birthday;

	KDateTime created;
	KDateTime updated;

	Email::List emails;
	IM::List ims;
	PhoneNumber::List phoneNumbers;
	Address::List addresses;
    };
    
  } // namespace Service

} // namespace KGoogle

#endif /* CONTACT_P_H */

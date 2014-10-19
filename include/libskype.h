/*********************************************************************************
 * libskype - C++ library for interacting with a running skype instance.
 * Copyright (C) 2014 Søren Holm <sgh@sgh.dk>
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
 ********************************************************************************/

#pragma once
#include <iostream>
#include <vector>
#include <map>


class LibSkype_internals;
class LibSkypeContact;

class LibSkype {
public:
	LibSkype();
	virtual ~LibSkype();

	/**
	 * @brief Get a pointer to a skype contact
	 * @param name part of a handle, full name or displayname
	 * @return The first contact that matches on displayname, fullname or handle. NULL if no match
	 */
	LibSkypeContact* get_contact(const std::string& name);


private:
	LibSkype_internals* _d;
};




class LibSkypeContact {
public:
	LibSkypeContact() {}
	LibSkypeContact(LibSkype_internals* connection, const std::string& handle);

	void call();
//	void send_message(const std::string& message);
	void transfer_file(const std::string& filename);

	std::string displayname();
	std::string fullname();
	std::string handle();

	void message_handler(const std::string& message);

private:
	LibSkype_internals* _connection;
	std::string      _handle;
	std::string      _fullname;
	std::string      _displayname;
	int _buddies;
};



class LibSkypeMessage {
public:
	LibSkypeMessage(LibSkype_internals* connection, unsigned int id);

	void set_body(const std::string& body);
	void message_handler(const std::string& message);

private:
	std::string _body;
	std::string _from;
	std::string _chatid;
	LibSkype_internals* _connection;
	unsigned int _id;
	bool _initialized;
};



class LibSkypeChat {
public:
	LibSkypeChat(LibSkype_internals* connection, std::string id);

	void message_handler(const std::string& message);

private:
	std::string _id;
	LibSkype_internals* _connection;
	unsigned long _activity_timestamp;
	bool _initialized;
};


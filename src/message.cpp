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

#include "libskype_internals.h"
#include "tokenizer.h"

#include <stdlib.h>

#include <string>
#include <sstream>

using namespace std;

LibSkypeMessage::LibSkypeMessage(LibSkype_internals* connection, unsigned int id)
	: _connection(connection)
	, _id(id)
{
	_connection->update_max_message_id(id);
	_initialized = false;
}


string LibSkypeMessage::body() const {
	return _body;
}


void LibSkypeMessage::set_body(const string& body) {
	stringstream ss;
	ss << "SET CHATMESSAGE " << _id << " BODY " << body;
	_connection->api_message(ss.str());
}


void LibSkypeMessage::message_handler(const string& message) {
	Tokenizer token(message);
	string cmd = token.next();

	if (cmd == "STATUS") {
		if (!_initialized) {
			stringstream ss;
			ss << "GET CHATMESSAGE " <<_id << " FROM_HANDLE";
			_connection->api_message(ss.str());
		}
	} else
	if (cmd == "FROM_HANDLE") {
		_from = token.remaining();
		if (!_initialized) {
			stringstream ss;
			ss << "GET CHATMESSAGE " << _id << " CHATNAME";
			_connection->api_message(ss.str());
		}
	} else
	if (cmd == "CHATNAME") {
		_chatid = token.remaining();

		if (!_initialized) {
			stringstream ss;
			ss << "GET CHATMESSAGE " << _id << " BODY";
			_connection->api_message(ss.str());
		}
	} else
	if (cmd == "BODY") {
		_body = token.remaining();
		_initialized = true;
	} else
		cout << "SkypeMessage handler - unhandled message:  " << message << endl;

}


bool LibSkypeMessage::valid() const {
	return _initialized;
}

bool LibSkypeMessage::editable() {
	return (_from == _connection->current_user_handle);
}

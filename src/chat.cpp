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

LibSkypeChat::LibSkypeChat(LibSkype_internals* connection, string id) {
	stringstream ss;
	_id = id;
	_activity_timestamp = 0;
	_connection = connection;
	_initialized = false;
}

void LibSkypeChat::message_handler(const string& message) {
	Tokenizer token(message);
	string cmd = token.next();

	if (cmd == "ACTIVITY_TIMESTAMP") {
		stringstream ss;
		_activity_timestamp = atoi(token.next().c_str());
		cout << "Chat " << _id << " timestamp is now " << _activity_timestamp << endl;
		if (_initialized) {
			ss << "GET CHAT " << _id << " RECENTCHATMESSAGES";
			_connection->api_message(ss.str());
		}
		_initialized = true;
	} else
	if (cmd == "RECENTCHATMESSAGES") {
		stringstream ss;
		vector<string> messages;
		token.remaining(messages);

		unsigned int max = 0;
		for (unsigned idx=0; idx<messages.size(); idx++) {
			unsigned int messageid = atoi(messages[idx].c_str());
			if (messageid <= _connection->get_max_message_id())
				continue;
			cout << "Max id is " << _connection->get_max_message_id() <<  ": Fetching message ID " << messageid << endl;
			if (messageid > max)
				max = messageid;
			ss.str("");
			ss << "GET CHATMESSAGE " << messages[idx] << " STATUS";
			_connection->api_message(ss.str());
		}
		_connection->update_max_message_id(max);

	} else
		cout << "Unhandled: " << message << endl;
}


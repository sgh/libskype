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

#include "dbus_connection.h"
#include "libskype.h"

#include <vector>
#include <map>
#include <queue>
#include <string>

using namespace std;

typedef std::vector<std::string> stringvector;

template<class KEY,class VALUE>
class SkypeListMap : public std::map<KEY, VALUE> {
public:
	virtual ~SkypeListMap() {
		typename SkypeListMap<KEY, VALUE>::iterator it;
		it = SkypeListMap<KEY, VALUE>::begin();
		while (it != SkypeListMap<KEY, VALUE>::end()) {
			delete it->second;
			it++;
		}
	}
};


class SkypeChatMap : public SkypeListMap<std::string, LibSkypeChat*> {
};

class SkypeContactMap : public SkypeListMap<std::string, LibSkypeContact*> {
};


class SkypeMessageMap : public SkypeListMap<unsigned int, LibSkypeMessage*> {
};


class LibSkype_internals {
public:

	LibSkype_internals() {
		handler = NULL;
	}

	void api_message(const std::string& message) {
		dbus->send_string(message);
	}

	void update_max_message_id(unsigned long id) {
		if (_maxmessageid < id) {
			_maxmessageid = id;
			cout << "Max message id is " << get_max_message_id() << endl;
		}
	}

	void message_handler(const std::string& message);

	unsigned long get_max_message_id() {
		return _maxmessageid;
	}

	LibSkypeMessage* get_message(unsigned int messageid) {
		LibSkypeMessage* msg;
		SkypeMessageMap::iterator it = messages.find(messageid);
		if (it == messages.end()) {
			messages[messageid] = msg = new  LibSkypeMessage(this, messageid);
			message_dispatch_q.push(msg);
		} else
			msg = it->second;

		return msg;
	}

	void dispatch_events();

	SkypeConnectionDBusConnection* dbus;
	SkypeContactMap contacts;
	SkypeMessageMap messages;
	SkypeChatMap    chats;
	std::string current_user_handle;
	bool init_done;
	int skype_version;
	pthread_cond_t cond;
	pthread_mutex_t lock;
	LibSkypeHandler* handler;

	queue<LibSkypeMessage*> message_dispatch_q;

private:
	unsigned long _maxmessageid;
};

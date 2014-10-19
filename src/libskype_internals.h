#pragma once

#include "dbus_connection.h"
#include "libskype.h"

#include <vector>
#include <map>
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

	SkypeConnectionDBusConnection* dbus;
	SkypeContactMap contacts;
	SkypeMessageMap messages;
	SkypeChatMap    chats;
	std::string current_user_handle;
	bool init_done;
	int skype_version;
	pthread_cond_t cond;
	pthread_mutex_t lock;

private:
	unsigned long _maxmessageid;
};

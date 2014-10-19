#include "libskype_internals.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include <sstream>
#include <vector>
#include <istream>
#include <iterator>
#include <algorithm>
#include <string>

#include "tokenizer.h"
#include "dbus_connection.h"

using namespace std;



LibSkype::LibSkype() {
	_d = new LibSkype_internals;
	pthread_cond_init(&_d->cond, NULL);
	pthread_mutex_init(&_d->lock, NULL);
	_d->init_done = false;
	_d->dbus = new SkypeConnectionDBusConnection(_d);

	_d->dbus->send_string("NAME libskypedbus");
	_d->dbus->send_string("PROTOCOL 5");

	pthread_mutex_lock(&_d->lock);
	while (!_d->init_done) {
		pthread_cond_wait(&_d->cond, &_d->lock);
	}
	pthread_mutex_unlock(&_d->lock);
}


LibSkype::~LibSkype() {
	delete _d->dbus;
}


LibSkypeContact* LibSkype::get_contact(const string& name) {
	std::map<std::string, LibSkypeContact*>::iterator it = _d->contacts.begin();

	string name_upper = name;
	transform(name_upper.begin(), name_upper.end(), name_upper.begin(), ::toupper);
	while (it != _d->contacts.end()) {
		string tmp;
		LibSkypeContact* contact = it->second;

		// Match in display name?
		tmp = contact->displayname();
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
		if (tmp.find(name_upper) != string::npos)
			return contact;

		// Match in full name?
		tmp = contact->fullname();
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
		if (tmp.find(name_upper) != string::npos)
			return contact;

		// Match in handle?
		tmp = contact->handle();
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
		if (tmp.find(name_upper) != string::npos)
			return contact;
		it++;
	}

	// If nothin matched just return NULL
	return NULL;
}


void LibSkype_internals::message_handler(const string& message) {
	Tokenizer token(message);
	string cmd = token.next();

	pthread_mutex_lock(&lock);

//	cout << "Global handler - MESSAGE:  " << message << endl;

	if (cmd == "USER") {
		string username = token.next();
		LibSkypeContact* c = contacts[username];
		c->message_handler(token.remaining());
	} else
	if (cmd == "SKYPEVERSION") {
		skype_version = atoi(token.next().c_str());
		init_done = true;
	} else
	if (cmd == "USERS") {
		stringvector v;
		token.remaining(v);

		contacts.clear();
		for (unsigned int idx=0; idx<v.size(); idx++) {
			contacts[v[idx]] = new LibSkypeContact(this, v[idx]);
		}
		dbus->send_string("GET SKYPEVERSION");
	} else
	if (cmd == "CURRENTUSERHANDLE") {
		current_user_handle = token.next();
//		cout << "Current userhandle: " << current_user_handle << endl;
		dbus->send_string("SEARCH FRIENDS");
		api_message("SEARCH CHATMESSAGES");
		api_message("SEARCH CHATS");
	} else
	if (cmd == "CHAT") {
		string id = token.next();
		if (chats[id] == NULL)
			chats[id] = new LibSkypeChat(this,id);
		chats[id]->message_handler(token.remaining());
	} else
	if (cmd == "CHATS") {
		vector<string> ids;
		token.remaining(ids);
		for (unsigned int i=0 ; i<ids.size(); i++) {
			const string& id = ids[i];
			if (chats[id] == NULL)
				chats[id] = new LibSkypeChat(this,id);
		}
		cout << chats.size() << " chats received" << endl;
	} else
	if (cmd == "CHATMESSAGE") {
		unsigned int messageid = atoi(token.next().c_str());
		if (messages.find(messageid) == messages.end())
			if (messages[messageid] == NULL)
			messages[messageid] = new  LibSkypeMessage(this, messageid);
		messages[messageid]->message_handler(token.remaining());
	} else
	if (cmd == "CHATMESSAGES") {
		update_max_message_id( atoi(token.next().c_str()) );
	} else
	if (cmd == "USERSTATUS") {
	} else
	if (cmd == "CONNSTATUS") {
	} else
	if (cmd == "PROTOCOL") {
	} else
		cout << "Global handler - unhandled message:  " << message << endl;

	pthread_mutex_unlock(&lock);
	pthread_cond_broadcast(&cond);
}

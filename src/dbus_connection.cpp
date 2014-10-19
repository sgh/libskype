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
#include "dbus_connection.h"

#include "stdio.h"
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <iostream>
#include <sstream>


using namespace std;


DBusHandlerResult SkypeConnectionDBusConnection::signal_filter (DBusConnection *connection, DBusMessage *message, void *user_data) {
	DBusError error;
	SkypeConnectionDBusConnection* priv = (SkypeConnectionDBusConnection*)user_data;
	(void)connection;
	char *s;
	dbus_error_init (&error);
	if (dbus_message_get_args(message, &error, DBUS_TYPE_STRING, &s, DBUS_TYPE_INVALID)) {
		stringstream ss;
		ss << "#"  << priv->_sequence << " ";
		string prefix = ss.str();

//		printf("Sequence is %d\n", priv->_sequence);
//		printf("Prefix is \"%s\"\n", prefix.c_str());
//		printf("Signal: Response : %s\n", s);

		// Skip sequence number for global message handler;
		const char* ptr = s;
		if (s[0] == '#')
			ptr = strchr(s, ' ') + 1;
		//priv->message_handler(ptr);

		priv->_response = ptr;
		priv->_sequence++;

//		if (strstr(s, prefix.c_str()) == s) {
//			pthread_mutex_lock(&priv->_lock);
//			priv->_response = s + prefix.size();
//			pthread_cond_broadcast(&priv->_cond);
//			pthread_mutex_unlock(&priv->_lock);
//		}

		return DBUS_HANDLER_RESULT_HANDLED;
	}
	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

}


SkypeConnectionDBusConnection::SkypeConnectionDBusConnection(LibSkype_internals* connection) {
	DBusError error;
	dbus_error_init (&error);
	_bus = dbus_bus_get(DBUS_BUS_SESSION, &error);
	pthread_mutex_init(&_lock, NULL);
	_connection = connection;
	if (!_bus)
		printf("Err: %s: %s\n", error.name, error.message);

	printf("unique name: %s\n", dbus_bus_get_unique_name(_bus));
	dbus_bus_add_match (_bus, "path='/com/Skype/Client'", &error);
	dbus_connection_add_filter (_bus, signal_filter, this, NULL);
	pthread_create(&tid, NULL, eventloop_jump, this);
}


SkypeConnectionDBusConnection::~SkypeConnectionDBusConnection() {
	void* retval;
	pthread_cancel(tid);
	pthread_join(tid, &retval);
}


void SkypeConnectionDBusConnection::send_string(const string& str) {
	DBusMessage *message;
	const char* command = str.c_str();
//	cout << "Command: " << ss.str() << endl;
	message = dbus_message_new_method_call("com.Skype.API", "/com/Skype", "com.Skype.API", "Invoke");
	dbus_message_append_args (message, DBUS_TYPE_STRING, &command, DBUS_TYPE_INVALID);
	pthread_mutex_lock(&_lock);
	dbus_connection_send(_bus, message, NULL);
	pthread_mutex_unlock(&_lock);
	dbus_message_unref (message);
}


void* SkypeConnectionDBusConnection::eventloop_jump(void* arg) {
	SkypeConnectionDBusConnection* p = reinterpret_cast<SkypeConnectionDBusConnection*>(arg);
	return p->eventloop(arg);
}


void SkypeConnectionDBusConnection::message_handler(const string& message) {
	_connection->message_handler(message);
}


void* SkypeConnectionDBusConnection::eventloop(void* arg) {
	(void)arg;
	while (1) {
		unsigned int seq = _sequence;
		pthread_setcancelstate(0, NULL);

		pthread_mutex_lock(&_lock);
		dbus_connection_read_write_dispatch(_bus, 100);
		pthread_mutex_unlock(&_lock);
		if (seq != _sequence) {
			if (_sequence - seq > 1) {
				cout << "Missed messages !!! " << endl;
				exit(42);
			}
			message_handler(_response);
			seq = _sequence;
		}
		pthread_setcancelstate(1, NULL);
	}
}




void pretty_print_message(DBusMessage* m) {
	DBusMessageIter it;
	printf("Message serial %d\n",  dbus_message_get_serial(m));
	printf("  Destination : %s\n", dbus_message_get_destination(m));
	printf("  Path        : %s\n", dbus_message_get_path(m));
	printf("  Member      : %s\n", dbus_message_get_member(m));
	printf("  Interface   : %s\n", dbus_message_get_interface(m));
	printf("  Error name  : %s\n", dbus_message_get_error_name(m));
	printf("  Signature   : %s\n", dbus_message_get_signature(m));
	printf("  Type        : %d\n", dbus_message_get_type(m));

	dbus_message_iter_init(m, &it);
	do {
	char type = dbus_message_iter_get_arg_type(&it);
		printf(" type %c\n", type);
		if (type == 's') {
			char *ptr;
			dbus_message_iter_get_basic(&it, (void*)&ptr);
			printf("S: %s\n", ptr);
		}
		dbus_message_iter_next(&it);
	} while (dbus_message_iter_has_next(&it));
}


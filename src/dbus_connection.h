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

#include <dbus/dbus.h>
#include <iostream>

class LibSkype_internals;

class SkypeConnectionDBusConnection {
public:
	pthread_mutex_t _lock;
	std::string _response;

	DBusConnection* _bus;

	SkypeConnectionDBusConnection(LibSkype_internals* connection);
	virtual ~SkypeConnectionDBusConnection();
	void send_string(const std::string& str);

	static void* eventloop_jump(void* arg);

private:
	LibSkype_internals* _connection;
	unsigned int _sequence;
	pthread_t tid;

	static DBusHandlerResult signal_filter (DBusConnection *connection, DBusMessage *message, void *user_data);

	void* eventloop(void* arg);
};

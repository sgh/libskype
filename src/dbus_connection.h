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

	void message_handler(const std::string& message);

private:
	LibSkype_internals* _connection;
	unsigned int _sequence;
	pthread_t tid;

	static DBusHandlerResult signal_filter (DBusConnection *connection, DBusMessage *message, void *user_data);

	void* eventloop(void* arg);
};

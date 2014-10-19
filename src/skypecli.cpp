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

#include <unistd.h>

#include <iostream>

#include "libskype.h"

using namespace std;

class MyCallbacks : public LibSkypeHandler {

public:
	void message_received(LibSkypeMessage* msg) {
		cout << "New message received" << endl;
		if (msg->editable() && msg->body() == "ping")
			msg->set_body("pong");
	}
};




int main() {
	MyCallbacks callbacks;
	LibSkype skype;
	skype.set_handler(&callbacks);

//	LibSkypeContact* contact =  skype.get_contact("testuser");
//	cout << contact->handle() << endl;
//	contact->transfer_file("uhadada.mp3");
//	contact->send_message("uhadada.mp3");
//	contact->call();
//	usleep(100000);
	while (1)
		usleep(1000000);
}

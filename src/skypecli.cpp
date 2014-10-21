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
#include "libskype.h"

#include <unistd.h>
#include <stdlib.h>

#include <iostream>

using namespace std;

class MyCallbacks : public LibSkypeHandler {

public:
	void message_received(LibSkypeMessage* msg) {
		cout << "New message received" << endl;
		if (msg->editable() && msg->body() == "ping")
			msg->set_body("pong");
	}
};


void print_usage() {
	cout << "Usage: skypecli <options>               " << endl;
	cout << "  -c <contact>  - Call contact          " << endl;
	cout << "  -f <file>     - Send file to a contact" << endl;
	exit(EXIT_FAILURE);
}


int main(int argc, char* const argv[]) {
	int opt;
	string call_handle;
	string send_file;

	while ((opt = getopt(argc, argv, "c:f:h")) != -1) {
		switch (opt) {
			case 'c':
				call_handle = optarg;
				break;
			case 'f':
				send_file = optarg;
				break;
			default:
				print_usage();
		}
	}

	if (argc < 2)
		print_usage();

	MyCallbacks callbacks;
	LibSkype skype;
	skype.set_handler(&callbacks);

	if (!call_handle.empty()) {
		LibSkypeContact* contact = skype.get_contact(call_handle);
		if (!contact) {
			cout << "Error, no such contact" << endl;
			exit(EXIT_FAILURE);
		}
		cout << "Calling " << contact->handle() << endl;
		contact->call();
	} else
	if (!send_file.empty()) {
		string handle;
		cout << "Send file to: " << flush;
		getline(cin, handle);
		LibSkypeContact* contact = skype.get_contact(handle);
		if (!contact) {
			cout << "Error, no such contact" << endl;
			exit(EXIT_FAILURE);
		}
		cout << "Sending file to " << contact->handle() << endl;
		contact->transfer_file(send_file);

	}

	return EXIT_SUCCESS;
}

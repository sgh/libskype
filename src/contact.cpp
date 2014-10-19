#include "libskype_internals.h"
#include "tokenizer.h"

#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <libgen.h>

#include <iostream>
#include <sstream>


using namespace std;

LibSkypeContact::LibSkypeContact(LibSkype_internals* connection, const string& handle)
	: _connection(connection)
	, _handle(handle)
{
//	cout << "SkypeContact: created " << _handle << endl;

	_connection->api_message("GET USER "+ _handle + " FULLNAME");
	_connection->api_message("GET USER "+ _handle + " DISPLAYNAME");
}

void LibSkypeContact::call() {
	_connection->api_message("call " + _handle);
}


//void SkypeContact::send_message(const string& message) {
//	stringstream ss;
//}


void LibSkypeContact::transfer_file(const string& filename) {
	stringstream ss;
	char tmpstring[PATH_MAX];
	realpath(filename.c_str(), tmpstring);
	ss << "OPEN FILETRANSFER " << _handle << " IN " << dirname(tmpstring);
	strcpy(tmpstring, filename.c_str());
//	cout << "Basename " << basename(tmpstring) << endl;
	_connection->api_message(ss.str());
}


string LibSkypeContact::displayname() {
	return _displayname;
}


string LibSkypeContact::fullname() {
	return _fullname;
}


string LibSkypeContact::handle() {
	return _handle;
}

void LibSkypeContact::message_handler(const string& message) {
	Tokenizer token(message);
	string cmd = token.next();

	if (cmd == "FULLNAME") {
		string fullname = token.remaining();
		if (_fullname != fullname) {
			_fullname = fullname;
//			cout << "User " << _handle << ": Fullname changed to: " << _fullname << endl;
		}
	}

	if (cmd == "DISPLAYNAME") {
		string displayname = token.next();
		if (_displayname != displayname) {
			_displayname = displayname;
//			cout << "User " << _handle << ": Displayname changed to: " << _displayname << endl;
		}
	}

	if (cmd == "NROF_AUTHED_BUDDIES") {
		_buddies = atoi(token.next().c_str());
//		cout << "User " << _handle << ": Number of buddies: " << buddies << endl;
	}
}





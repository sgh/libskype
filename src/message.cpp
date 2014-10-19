#include "libskype_internals.h"
#include "tokenizer.h"

#include <stdlib.h>

#include <string>
#include <sstream>

using namespace std;

LibSkypeMessage::LibSkypeMessage(LibSkype_internals* connection, unsigned int id)
	: _connection(connection)
	, _id(id)
{
	_connection->update_max_message_id(id);
	_initialized = false;
}


void LibSkypeMessage::set_body(const string& body) {
	stringstream ss;
	ss << "SET CHATMESSAGE " << _id << " BODY " << body;
	_connection->api_message(ss.str());
}


void LibSkypeMessage::message_handler(const string& message) {
	Tokenizer token(message);
	string cmd = token.next();

	if (cmd == "STATUS") {
		if (!_initialized) {
			stringstream ss;
			ss << "GET CHATMESSAGE " <<_id << " FROM_HANDLE";
			_connection->api_message(ss.str());
		}
	} else
	if (cmd == "BODY") {
		_body = token.remaining();
		cout << "Chatname : " << _chatid << endl;
		cout << "From   :   " << _from << endl;
		cout << "Message: " << _body << endl;
		_initialized = true;
	} else
	if (cmd == "FROM_HANDLE") {
		_from = token.remaining();
		if (!_initialized) {
			stringstream ss;
			ss << "GET CHATMESSAGE " << _id << " CHATNAME";
			_connection->api_message(ss.str());
		}
	} else
	if (cmd == "CHATNAME") {
		_chatid = token.remaining();

//		if (_body == "ping") {
//			stringstream ss;
//			ss << "CHATMESSAGE " << chatid << " pong";
//			_connection->sendmessage(ss.str());
//		}
		if (!_initialized) {
			stringstream ss;
			ss << "GET CHATMESSAGE " << _id << " BODY";
			_connection->api_message(ss.str());
		}
	} else
		cout << "SkypeMessage handler - unhandled message:  " << message << endl;

}





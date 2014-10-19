#pragma once
#include <iostream>
#include <vector>
#include <map>


class LibSkype_internals;
class LibSkypeContact;

class LibSkype {
public:
	LibSkype();
	virtual ~LibSkype();

	/**
	 * @brief Get a pointer to a skype contact
	 * @param name part of a handle, full name or displayname
	 * @return The first contact that matches on displayname, fullname or handle. NULL if no match
	 */
	LibSkypeContact* get_contact(const std::string& name);


private:
	LibSkype_internals* _d;
};




class LibSkypeContact {
public:
	LibSkypeContact() {}
	LibSkypeContact(LibSkype_internals* connection, const std::string& handle);

	void call();
//	void send_message(const std::string& message);
	void transfer_file(const std::string& filename);

	std::string displayname();
	std::string fullname();
	std::string handle();

	void message_handler(const std::string& message);

private:
	LibSkype_internals* _connection;
	std::string      _handle;
	std::string      _fullname;
	std::string      _displayname;
	int _buddies;
};



class LibSkypeMessage {
public:
	LibSkypeMessage(LibSkype_internals* connection, unsigned int id);

	void set_body(const std::string& body);
	void message_handler(const std::string& message);

private:
	std::string _body;
	std::string _from;
	std::string _chatid;
	LibSkype_internals* _connection;
	unsigned int _id;
	bool _initialized;
};



class LibSkypeChat {
public:
	LibSkypeChat(LibSkype_internals* connection, std::string id);

	void message_handler(const std::string& message);

private:
	std::string _id;
	LibSkype_internals* _connection;
	unsigned long _activity_timestamp;
	bool _initialized;
};


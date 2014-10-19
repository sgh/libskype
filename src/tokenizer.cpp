#include <iostream>

#include "tokenizer.h"

using namespace std;

Tokenizer::Tokenizer(const string& message)
	: _message(message)
	, _pos(0)
{
}

string Tokenizer::next() {
	size_t pos = _pos;
	size_t endpos;

	pos = _message.find_first_not_of(" ,", pos);
	endpos = _message.find_first_of(" ,",pos);

	if (pos == string::npos)
		return "";

	_pos = endpos;

	return _message.substr(pos,endpos-pos);
}


string Tokenizer::remaining() {
	size_t pos = _pos;
	_pos = string::npos;
	pos = _message.find_first_not_of(" ,", pos);

	if (pos == string::npos)
		pos = 0;

	return _message.substr(pos);
}


void Tokenizer::remaining(std::vector<string>& v) {
	string str;

	str = next();
	while ( !str.empty() ) {
		v.push_back(str);
		str = next();
	}
}

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

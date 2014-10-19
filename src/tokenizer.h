#pragma once
#include <stddef.h>

#include <vector>
#include <iostream>

class Tokenizer {
public:
	Tokenizer(const std::string& message);

	std::string next();
	std::string remaining();
	void remaining(std::vector<std::string>&);

private:
	const std::string& _message;
	size_t _pos;
};

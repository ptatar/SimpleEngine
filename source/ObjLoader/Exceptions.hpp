#pragma once

#include <exception>
#include <string>

class FileNotFound: public std::exception {
public:
	FileNotFound(const std::string& msg): m_msg(msg) {}

	virtual ~FileNotFound() {};

	virtual const char* what() const override {
		return m_msg.c_str();
	}

private:
	const std::string m_msg;
};
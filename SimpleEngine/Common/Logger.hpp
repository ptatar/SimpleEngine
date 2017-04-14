#pragma once

#include <iostream>

#define LOGD(...) logger::Write(__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGI(...) logger::Write(__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGW(...) logger::Write(__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGE(...) logger::Write(__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGF(...) logger::Write(__FUNCTION__, __LINE__, __VA_ARGS__)

//TODO
namespace logger {

template<typename...Args>
void Write(const char* funcName, int line, const char* str, Args...args) {
	static const int maxMsgSize = 2048;
	static char buffer[maxMsgSize];
	snprintf(buffer, maxMsgSize, str, args...);

	std::cout << funcName << " : " << line << "  " << buffer << "\n";
}

} // namespace logger
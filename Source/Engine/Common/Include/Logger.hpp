#pragma once

#include <iostream>

#define LOGD(...) logger::Write(__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGI(...) logger::Write(__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGW(...) logger::Write(__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGE(...) logger::Write(__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGF(...) logger::Write(__FUNCTION__, __LINE__, __VA_ARGS__)

//TODO
namespace logger {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"

template<typename...Args>
void Write(const char* funcName, int line, const char* str, Args...args) {
    int argNum = sizeof...(args);
    if (argNum)
    {
        static const int bufferSize = 2048;
        static char buffer[bufferSize];

        snprintf(buffer, bufferSize, str, args...);
        std::cout << funcName << " : " << line << "  " << buffer << "\n";
    }
    else
    {
         std::cout << funcName << " : " << line << "  " << str << "\n";
    }
}
#pragma GCC diagnostic push
} // namespace logger

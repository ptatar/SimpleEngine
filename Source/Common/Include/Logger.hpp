#pragma once

#include <iostream>

#define LOGD(...) common::Logger::GetInstance().Write(common::LogLevel::Debug,   __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGI(...) common::Logger::GetInstance().Write(common::LogLevel::Info,    __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGW(...) common::Logger::GetInstance().Write(common::LogLevel::Warning, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGE(...) common::Logger::GetInstance().Write(common::LogLevel::Error,   __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGF(...) common::Logger::GetInstance().Write(common::LogLevel::Fatal,   __FUNCTION__, __LINE__, __VA_ARGS__)

#define LOG_SET_THRESHOLD(Threshold) common::Logger::GetInstance().SetGlobalLogThreshold(Threshold)

#define LOG_THRESHOLD_DEBUG common::LogLevel::Debug
#define LOG_THRESHOLD_INFO common::LogLevel::Info
#define LOG_THRESHOLD_WARNING common::LogLevel::Warning
#define LOG_THRESHOLD_ERROR common::LogLevel::Error
#define LOG_THRESHOLD_FATAL common::LogLevel::Fatal

#define LOCAL_LOG_THRESHOLD m_logThreshold

//TODO
namespace common {

    enum LogLevel
    {
        Debug = 0,
        Info = 1,
        Warning = 2,
        Error = 3,
        Fatal = 4,
    };

    class Logger
    {
        public:
            static Logger& GetInstance()
            {
                static Logger logger;
                return logger;
            }

            void SetGlobalLogThreshold(LogLevel logLevel)
            {
                m_logThreshold = logLevel;
            }

            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wformat-security"

            template<typename...Args>
            void Write(LogLevel level, const char* funcName, int line, const char* str, Args...args)
            {
                if (level < LOCAL_LOG_THRESHOLD)
                {
                    return;
                }

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
        private:
            Logger(): m_logThreshold(LogLevel::Info) {};
        private:
            LogLevel m_logThreshold;

    };
} // namespace common

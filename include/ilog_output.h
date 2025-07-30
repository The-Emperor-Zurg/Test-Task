#pragma once

#include <string>

#ifdef WIN32
    #ifdef MYLOGGER_SHARED
        #ifdef MYLOGGER_EXPORTS
            #define MYLOGGER_API __declspec(dllexport)
        #else
            #define MYLOGGER_API __declspec(dllimport)
        #endif
    #else
        #define MYLOGGER_API
    #endif
#else
    #define MYLOGGER_API
#endif


namespace MyLogger {
    enum class LogResult {
        SUCCESS = 0,
        FILE_OPEN_ERROR = 1,
        WRITE_ERROR = 2,
        INVALID_LEVEL = 3,
        SOCKET_CONNECTION_ERROR = 4,
        SOCKET_SEND_ERROR = 5,
    };

    class MYLOGGER_API ILogOutput {
    public:
        virtual ~ILogOutput() = default;
        virtual LogResult init() = 0;
        virtual LogResult write(const std::string& message) = 0;
        virtual void close() = 0;
    };
}
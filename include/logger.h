#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <mutex>

#include "ilog_output.h"

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
    enum class LogLevel {
        INFO = 0,
        SECRET_INFO = 1,
        TOP_SECRET_INFO = 2,
    };

    class MYLOGGER_API Logger {
    public:
        Logger();
        ~Logger();

        LogResult init(const std::string& fileName, LogLevel defaultMessageLevel);
        LogResult initSocket(const std::string& host, int port, LogLevel defaultMessageLevel);
        LogResult log(const std::string& message, LogLevel level = LogLevel::INFO);
        LogResult setLogLevel(LogLevel level);

        LogLevel getLogLevel() const;
        bool isInitialized() const;

    private:
        LogLevel defaultMessageLevel_;
        std::unique_ptr<ILogOutput> output_;
        std::mutex mutex_;
        bool initialized_;

        static std::string getCurrentTime() ;
        static std::string logLevelToString(LogLevel level) ;
        bool shouldLog(LogLevel level) const;
        static bool isValidLogLevel(LogLevel level) ;
    };
}
#pragma once

#include <string>
#include <fstream>
#include <mutex>

#ifdef WIN32
    #ifdef MYLOGGER_EXPORTS
        #define MYLOGGER_API __declspec(dllexport)
    #else
        #define LOGGER_API __declspec(dllimport)
    #endif
#else
    #define LOGGER_API
#endif

namespace MyLogger {
    enum class LogLevel {
        INFO = 0,
        SECRET_INFO = 1,
        TOP_SECRET_INFO = 2,
    };

    enum class LogResult {
        SUCCESS = 0,
        FILE_OPEN_ERROR = 1,
        WRITE_ERROR = 2,
        INVALID_LEVEL = 3
    };

    class MYLOGGER_API Logger {
    public:
        Logger();
        ~Logger();

        LogResult init(const std::string& fileName, LogLevel defaultMessageLevel);
        LogResult log(const std::string& message, LogLevel level = LogLevel::INFO);
        void setLogLevel(LogLevel level);

        LogLevel getLogLevel() const;
        bool isInitialized() const;

    private:
        std::string fileName_;
        LogLevel defaultMessageLevel_;
        std::ofstream logFile_;
        std::mutex mutex_;
        bool initialized_;

        static std::string getCurrentTime() ;
        static std::string logLevelToString(LogLevel level) ;
        bool shouldLog(LogLevel level) const;
        static bool isValidLogLevel(LogLevel level) ;
    };
}
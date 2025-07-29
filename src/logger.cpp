#include "logger.h"

#include <iomanip>

namespace MuLogger {

    Logger::Logger()
        : defaultMessageLevel_(LogLevel::INFO), initialized_(false)
    {}

    Logger::~Logger() {
        if (logFile_.is_open()) {
            logFile_.close();
        }
    }

    LogResult Logger::init(const std::string& fileName, LogLevel defaultMessageLevel) {
        if (!isValidLogLevel(defaultMessageLevel)) {
            return LogResult::INVALID_LEVEL;
        }

        std::lock_guard lock(mutex_);

        fileName_ = fileName;
        defaultMessageLevel_ = defaultMessageLevel;

        logFile_.open(fileName,std::ios::app);
        if (!logFile_.is_open()) {
            initialized_ = false;
            return LogResult::FILE_OPEN_ERROR;
        }

        initialized_ = true;
        return LogResult::SUCCESS;
    }

    LogResult Logger::log(const std::string& message, LogLevel level) {
        if (!initialized_) {
            return LogResult::FILE_OPEN_ERROR;
        }

        if (!isValidLogLevel(level)) {
            return LogResult::INVALID_LEVEL;
        }

        if (!shouldLog(level)) {
            return LogResult::SUCCESS;
        }

        if (!logFile_.is_open()) {
            return LogResult::FILE_OPEN_ERROR;
        }

        logFile_ << getCurrentTime() << " [" << logLevelToString(level) << "] " << message << '\n';

        if (logFile_.fail()) {
            return LogResult::WRITE_ERROR;
        }

        logFile_.flush();
        return LogResult::SUCCESS;
    }

    void Logger::setLogLevel(LogLevel level) {
        if (!isValidLogLevel(level)) {
            return;
        }

        std::lock_guard lock(mutex_);
        defaultMessageLevel_ = level;
    }

    LogLevel Logger::getLogLevel() const {
        return defaultMessageLevel_;
    }

    bool Logger::isInitialized() const {
        return initialized_;
    }

    std::string Logger::getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

        return ss.str();
    }

    std::string Logger::logLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::INFO:
                return "INFO";
            case LogLevel::SECRET_INFO:
                return "SECRET_INFO";
            case LogLevel::TOP_SECRET_INFO:
                return "TOP_SECRET_INFO";
            default: return "UNKNOWN";
        }
    }

    bool Logger::shouldLog(LogLevel level) const {
        return static_cast<int>(level) >= static_cast<int>(defaultMessageLevel_);
    }

    bool Logger::isValidLogLevel(LogLevel level) {
        return
            level == LogLevel::INFO ||
            level == LogLevel::SECRET_INFO ||
            level == LogLevel::TOP_SECRET_INFO;
    }
}

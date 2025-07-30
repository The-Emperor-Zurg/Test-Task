#include "file_log_output.h"


namespace MyLogger {
    FileLogOutput::FileLogOutput(const std::string& fileName)
        : fileName_(fileName)
    {}

    LogResult FileLogOutput::init() {
        logFile_.open(fileName_.c_str(), std::ios::app);
        if (!logFile_.is_open()) {
            return LogResult::FILE_OPEN_ERROR;
        }

        return LogResult::SUCCESS;
    }

    LogResult FileLogOutput::write(const std::string &message) {
        if (!logFile_.is_open()) {
            return LogResult::FILE_OPEN_ERROR;
        }

        logFile_ << message << '\n';

        if (logFile_.fail()) {
            return LogResult::WRITE_ERROR;
        }

        logFile_.flush();

        return LogResult::SUCCESS;
    }

    void FileLogOutput::close() {
        if (logFile_.is_open()) {
            logFile_.close();
        }
    }

}
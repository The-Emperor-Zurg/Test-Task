#pragma once

#include <fstream>

#include "ilog_output.h"


namespace MyLogger {
    class MYLOGGER_API FileLogOutput : public ILogOutput {
    public:
        explicit FileLogOutput(const std::string& fileName);
        LogResult init() override;
        LogResult write(const std::string& message) override;
        void close() override;

    private:
        std::string fileName_;
        std::ofstream logFile_;
    };
}
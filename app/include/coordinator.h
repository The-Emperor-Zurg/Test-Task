#pragma once
#include <memory>

#include "logger.h"
#include "interfaces/Iinput_handler.h"
#include "interfaces/Ilog_worker.h"


namespace LoggerApp {
    class Coordinator {
    public:
        Coordinator(const std::string& fileName, MyLogger::LogLevel defaultLevel);
        Coordinator(const std::string& fileName, MyLogger::LogLevel defaultLevel,
            std::unique_ptr<ILogWorker> worker, std::unique_ptr<IInputHandler> inputHandler);
        ~Coordinator();

        bool init();
        bool run();
        bool shutdown();

    private:
        void printUsage() const;

        std::string logFileName_;
        MyLogger::LogLevel defaultLevel_;
        std::unique_ptr<ILogWorker> logWorker_;
        std::unique_ptr<IInputHandler> inputHandler_;
        std::shared_ptr<MyLogger::Logger> logger_;
        bool isInitialized_;
    };
}

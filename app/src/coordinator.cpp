#include <iostream>

#include "coordinator.h"
#include "input_handler.h"
#include "log_worker.h"


namespace LoggerApp {
    Coordinator::Coordinator(const std::string& fileName, MyLogger::LogLevel defaultLevel)
        : logFileName_(fileName), defaultLevel_(defaultLevel),
          logger_(std::make_shared<MyLogger::Logger>())
    {}

    Coordinator::Coordinator(const std::string& fileName, MyLogger::LogLevel defaultLevel,
                             std::unique_ptr<ILogWorker> worker, std::unique_ptr<IInputHandler> inputHandler)
        : logFileName_(fileName), defaultLevel_(defaultLevel),
          logger_(std::make_shared<MyLogger::Logger>()), logWorker_(std::move(worker)),
          inputHandler_(std::move(inputHandler))
    {}

    Coordinator::~Coordinator() {
        shutdown();
    }

    bool Coordinator::init() {
        auto result = logger_->init(logFileName_, defaultLevel_);
        if (result != MyLogger::LogResult::SUCCESS) {
            std::cerr << "Failed to init logger: ";

            switch (result) {
                case MyLogger::LogResult::FILE_OPEN_ERROR:
                    std::cerr << "File open error";
                    break;
                case MyLogger::LogResult::INVALID_LEVEL:
                    std::cerr << "Invalid level";
                    break;
                default:
                    std::cerr << "Unknown error";
            }

            std::cerr << '\n';
            return false;
        }

        if (!logWorker_) {
            logWorker_ = std::make_unique<LogWorker>(logger_);
        }

        if (!inputHandler_) {
            inputHandler_ = std::make_unique<InputHandler>();
        }

        logWorker_->start();

        isInitialized_ = true;
        return true;
    }

    void Coordinator::run() {
        if (!isInitialized_) {
            std::cerr << "Coordinator is not initialized!\n";
            return;
        }

        printUsage();

        std::string input;
        while (true) {
            std::cout << "Enter message [level]: ";

            if (!std::getline(std::cin, input)) {
                break;
            }

            auto messageOpt = inputHandler_->parseInput(input, defaultLevel_);
            if (!messageOpt.has_value()) {
                continue;
            }

            const Message& message = messageOpt.value();

            if (message.isExitMessage) {
                logWorker_->pushMessage(message);
                break;
            }

            logWorker_->pushMessage(message);
        }
    }

    void Coordinator::shutdown() {
        if (logWorker_) {
            logWorker_->stop();
            logWorker_.reset();
        }

        isInitialized_ = false;
    }

    void Coordinator::printUsage() const {
        std::cout << "This is multi-threaded logger app\n";
        std::cout << "Usage: Enter a message followed by optional log level\n";
        std::cout << "Available levels: INFO, SECRET_INFO, TOP_SECRET_INFO\n";
        std::cout << "The default level is: ";

        switch (defaultLevel_) {
            case MyLogger::LogLevel::INFO:
                std::cout << "INFO";
                break;
            case MyLogger::LogLevel::SECRET_INFO:
                std::cout << "SECRET_INFO";
                break;
            case MyLogger::LogLevel::TOP_SECRET_INFO:
                std::cout << "TOP_SECRET_INFO";
                break;
        }

        std::cout << '\n';
        std::cout << "Examples:\n";
        std::cout << "Hello, world!\n";
        std::cout << "I'm Achilles, son of Peleus SECRET_INFO\n";
        std::cout << "I am Maximus Decius Meridius TOP_SECRET_INFO\n";
        std::cout << "Type 'exit', 'quit' or 'q' to terminate\n";
        std::cout << "---------------\n";
    }
}

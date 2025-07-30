#include <iostream>

#include "coordinator.h"


void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <log_file.log> [default_level]\n";
    std::cout << "--- log_file.log: Path to the log file\n";
    std::cout << "--- default_level: Default log level (INFO, SECRET_INFO, TOP_SECRET_INFO)\n";
    std::cout << "--- INFO < SECRET_INFO < TOP_SECRET_INFO\n";
    std::cout << "--- Default level is: INFO\n\n";
    std::cout << "Examples: \n";
    std::cout << programName << "logger.log << \n";
    std::cout << programName << "logger.log SECRET_INFO \n";
}

MyLogger::LogLevel parseLogLevel(const std::string& level) {
    if (level == "INFO") {
        return MyLogger::LogLevel::INFO;
    }

    if (level == "SECRET_INFO") {
        return MyLogger::LogLevel::SECRET_INFO;
    }

    if (level == "TOP_SECRET_INFO") {
        return MyLogger::LogLevel::TOP_SECRET_INFO;
    }

    std::cerr << "Unknown log level: " << level << "\n";
    return MyLogger::LogLevel::INFO;
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        printUsage(argv[0]);
        return 1;
    }

    std::string logFileName = argv[1];
    MyLogger::LogLevel defaultLevel = MyLogger::LogLevel::INFO;

    if (argc == 3) {
        defaultLevel = parseLogLevel(argv[2]);
    }

    LoggerApp::Coordinator coordinator(logFileName, defaultLevel);

    if (!coordinator.init()) {
        std::cerr << "Failed to initialize app\n";
        return 1;
    }

    try {
        coordinator.run();
    } catch (const std::exception& e) {
        std::cerr << "Application error: " << e.what() << "\n";
    }

    std::cout << "Application exiting\n";
    return 0;
}
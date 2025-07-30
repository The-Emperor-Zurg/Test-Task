#include <iostream>
#include <fstream>

#include "unit_tests.h"
#include <filesystem>


#include "logger.h"


void runTest(const std::string& testName, bool (*testFunction)(), int& totalTests, int& passedTests) {
    totalTests++;

    std::cout << "Running test: " << testName << " - ";

    if (testFunction()) {
        std::cout << "PASSED\n";
        passedTests++;
    } else {
        std::cout << "FAILED\n";
    }
}

int runAllTests() {
    std::cout << "MyLogger test application\n";

    int totalTests = 0;
    int passedTests = 0;

    // Запускаем тесты
    runTest("Logger Initialization", testLoggerInitialization, totalTests, passedTests);
    runTest("Set Log Level", testSetLogLevel, totalTests, passedTests);
    runTest("Log Message", testLogMessage, totalTests, passedTests);
    runTest("Log Level Filtering", testLogLevelFilter, totalTests, passedTests);
    runTest("Error Handling", testErrorHandling, totalTests, passedTests);
    runTest("Set Level NOT_INITIALIZED Test", testSetLogLevelNotInitialized, totalTests, passedTests);
    runTest("Socket Logger Connection error", testSocketLoggerConnectionError, totalTests, passedTests);
    runTest("Socket Logger Initialization", testSocketLoggerInitialization, totalTests, passedTests);
    runTest("Socket Logger Invalid Host", testSocketLoggerInvalidHost, totalTests, passedTests);
    runTest("Socket Logger Invalid Port", testSocketLoggerInvalidPort, totalTests, passedTests);
    runTest("Socket Logger Set Level (NOT_INITIALIZED)", testSocketLoggerSetLevel, totalTests, passedTests);

    // Выводим результат
    std::cout << "\n ---Test Results ---\n";
    std::cout << "Total tests: " << totalTests << "\n";
    std::cout << "Passed: " << passedTests << "\n";
    std::cout << "Failed: " << (totalTests - passedTests) << "\n";

    if (passedTests == totalTests) {
        std::cout << "All tests passed! Congratulations! \n";
        return 0;
    }

    std::cout << "Some tests failed! \n";
    return 1;
}


bool testLoggerInitialization() {
    {
        MyLogger::Logger logger;

        if (logger.isInitialized()) {
            return false; // Не должен быть инициализирован
        }

        // Тест на успешную инициализацию
        MyLogger::LogResult result = logger.init("test_init.log", MyLogger::LogLevel::INFO);
        if (result != MyLogger::LogResult::SUCCESS) {
            return false;
        }

        if (!logger.isInitialized()) {
            return false;
        }

        if (logger.getLogLevel() != MyLogger::LogLevel::INFO) {
            return false;
        }
    } // Делаем в скопе, чтобы логгер уничтожался и файл закрывался

    // Удаляем созданный файл
    std::filesystem::remove("test_init.log");

    return true;
}

bool testSetLogLevel() {
    {
        MyLogger::Logger logger;
        logger.init("test_level.log", MyLogger::LogLevel::INFO);

        // Тест смены дефолтного уровня
        MyLogger::LogResult result = logger.setLogLevel(MyLogger::LogLevel::TOP_SECRET_INFO);
        if (result != MyLogger::LogResult::SUCCESS || logger.getLogLevel() != MyLogger::LogLevel::TOP_SECRET_INFO) {
            return false;
        }

        result = logger.setLogLevel(MyLogger::LogLevel::SECRET_INFO);
        if (result != MyLogger::LogResult::SUCCESS || logger.getLogLevel() != MyLogger::LogLevel::SECRET_INFO) {
            return false;
        }
    } // Делаем в скопе, чтобы логгер уничтожался и файл закрывался

    // Удаляем созданный файл
    std::filesystem::remove("test_level.log");

    return true;
}

bool testLogMessage() {
    {
        MyLogger::Logger logger;
        logger.init("test_message.log", MyLogger::LogLevel::INFO);

        // Запишем тестовое сообщение
        MyLogger::LogResult result = logger.log("Test message", MyLogger::LogLevel::INFO);
        if (result != MyLogger::LogResult::SUCCESS) {
            return false;
        }
    } // Делаем в скопе, чтобы логгер уничтожался и файл закрывался

    // Проверим содержимое файла
    std::ifstream file("test_message.log");
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    std::getline(file, line);
    file.close();

    bool containsMessage = line.find("Test message") != std::string::npos;
    bool containsLevel = line.find("[INFO]") != std::string::npos;

    // Удаляем созданный файл
    std::filesystem::remove("test_message.log");

    return containsMessage && containsLevel;
}

bool testLogLevelFilter() {
    {
        MyLogger::Logger logger;
        logger.init("test_filtering.log", MyLogger::LogLevel::SECRET_INFO);

        logger.log("Info message", MyLogger::LogLevel::INFO);
        logger.log("Secret message", MyLogger::LogLevel::SECRET_INFO);
        logger.log("Top secret message", MyLogger::LogLevel::TOP_SECRET_INFO);
    } // Делаем в скопе, чтобы логгер уничтожался и файл закрывался

    std::ifstream file("test_filtering.log");
    if (!file.is_open()) {
        return false;
    }

    std::string content((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
    file.close();

    bool infoFiltered = content.find("Info message") == std::string::npos;
    bool secretFiltered = content.find("Secret message") != std::string::npos;
    bool topSecretFiltered = content.find("Top secret message") != std::string::npos;

    // Удаляем созданный файл
    std::filesystem::remove("test_filtering.log");

    return infoFiltered && secretFiltered && topSecretFiltered;
}

bool testErrorHandling() {
    {
        MyLogger::Logger logger;

        // Запись сообщения без инициализированного логгера
        MyLogger::LogResult result = logger.log("Test message", MyLogger::LogLevel::INFO);
        if (result != MyLogger::LogResult::FILE_OPEN_ERROR) {
            return false;
        }

        // Невалидный уровень сообщения
        result = logger.init("test_error.log", static_cast<MyLogger::LogLevel>(777));
        if (result != MyLogger::LogResult::INVALID_LEVEL) {
            return false;
        }
    } // Делаем в скопе, чтобы логгер уничтожался и файл закрывался

    // Удаляем созданный файл
    std::filesystem::remove("test_error.log");
    
    return true;
}

bool testSetLogLevelNotInitialized() {
    MyLogger::Logger logger;

    // Тестируем setLogLevel на неинициализированном логгере
    MyLogger::LogResult result = logger.setLogLevel(MyLogger::LogLevel::INFO);
    if (result != MyLogger::LogResult::NOT_INITIALIZED) {
        return false;
    }

    // Тестируем невалидный уровень
    result = logger.setLogLevel(static_cast<MyLogger::LogLevel>(999));
    if (result != MyLogger::LogResult::INVALID_LEVEL) {
        return false;
    }

    return true;
}

bool testSocketLoggerConnectionError() {
    MyLogger::Logger logger;

    MyLogger::LogResult result = logger.initSocket("0.0.0.0", 9999, MyLogger::LogLevel::INFO);

    if (result != MyLogger::LogResult::SOCKET_CONNECTION_ERROR) {
        return false;
    }

    if (logger.isInitialized()) {
        return false;
    }

    return true;
}

bool testSocketLoggerInitialization() {
    MyLogger::Logger logger;

    if (logger.isInitialized()) {
        return false;
    }

    // некорректный logger
    MyLogger::LogResult result = logger.initSocket("127.0.0.1", 9999, static_cast<MyLogger::LogLevel>(777));
    if (result != MyLogger::LogResult::INVALID_LEVEL) {
        return false;
    }

    if (logger.isInitialized()) {
        return false;
    }

    return true;
}


bool testSocketLoggerInvalidHost() {
    MyLogger::Logger logger;

    // некорректный хост
    MyLogger::LogResult result = logger.initSocket("", 9999, MyLogger::LogLevel::INFO);
    return result == MyLogger::LogResult::SOCKET_CONNECTION_ERROR && !logger.isInitialized();
}

bool testSocketLoggerInvalidPort() {
    MyLogger::Logger logger;

    MyLogger::LogResult result = logger.initSocket("127.0.0.1", -1, MyLogger::LogLevel::INFO);
    return result == MyLogger::LogResult::SOCKET_CONNECTION_ERROR && !logger.isInitialized();
}

bool testSocketLoggerUninitialized() {
    MyLogger::Logger logger;

    MyLogger::LogResult result = logger.log("Test message", MyLogger::LogLevel::INFO);
    return result == MyLogger::LogResult::FILE_OPEN_ERROR;
}

bool testSocketLoggerSetLevel() {
    MyLogger::Logger logger;

    // Тестируем setLogLevel на неинициализированном логгере
    MyLogger::LogResult result = logger.setLogLevel(MyLogger::LogLevel::TOP_SECRET_INFO);
    if (result != MyLogger::LogResult::NOT_INITIALIZED) {
        return false;
    }

    // Тестируем невалидный уровень на неинициализированном логгере
    result = logger.setLogLevel(static_cast<MyLogger::LogLevel>(777));
    if (result != MyLogger::LogResult::INVALID_LEVEL) {
        return false;
    }

    return true;
}

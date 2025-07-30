#include <sstream>

#include "input_handler.h"

#include <algorithm>
#include <iostream>
#include <vector>


namespace LoggerApp {
    std::optional<Message> InputHandler::parseInput(const std::string &input, MyLogger::LogLevel defaultLevel) const {
        std::string trimmedInput = trim(input);

        if (trimmedInput.empty()) {
            return std::nullopt;
        }

        // Проверяем на выход из приложения
        if (trimmedInput == "exit" || trimmedInput == "quit" || trimmedInput == "q") {
            return Message::createExitMessage();
        }

        // Проверяем на изменение дефолтного уровня безопасности
        if (trimmedInput.length() >= 3 && trimmedInput.substr(0, 3) == "-l ") {
            std::string levelStr = trimmedInput.substr(3);
            auto newLevel = parseLogLevel(levelStr);
            if (newLevel.has_value()) {
                return Message::createLevelChangeMessage(newLevel.value());
            }

            std::cout << "Invalid level: " << levelStr << ". Available: INFO, SECRET_INFO, TOP_SECRET_INFO\n";
            return std::nullopt;
        }

        std::istringstream stream(trimmedInput);
        std::string word;
        std::vector<std::string> words;

        while (stream >> word) {
            words.emplace_back(word);
        }

        if (words.empty()) {
            return std::nullopt;
        }

        std::string messageText;
        MyLogger::LogLevel logLevel = defaultLevel;

        auto lastWordLevel = parseLogLevel(words.back());
        if (lastWordLevel.has_value()) {
            logLevel = lastWordLevel.value();
            for (size_t i = 0; i < words.size() - 1; i++) {
                if (i > 0) {
                    messageText += " ";
                }

                messageText += words[i];
            }
        } else {
            for (size_t i = 0; i < words.size(); ++i) {
                if (i > 0) {
                    messageText += " ";
                }

                messageText += words[i];
            }
        }

        if (messageText.empty()) {
            return std::nullopt;
        }

        return Message(messageText, logLevel);
    }

    std::optional<MyLogger::LogLevel> InputHandler::parseLogLevel(const std::string &level) {
        std::string upperLevel = level;
        std::transform(upperLevel.begin(), upperLevel.end(), upperLevel.begin(), ::toupper);

        if (upperLevel == "INFO") {
            return MyLogger::LogLevel::INFO;
        }

        if (upperLevel == "SECRET_INFO") {
            return MyLogger::LogLevel::SECRET_INFO;
        }

        if (upperLevel == "TOP_SECRET_INFO") {
            return MyLogger::LogLevel::TOP_SECRET_INFO;
        }

        return std::nullopt;
    }

    std::string InputHandler::trim(const std::string& input) {
        auto start = input.find_first_not_of(" \n\r\t");
        if (start == std::string::npos) {
            return "";
        }

        auto end = input.find_last_not_of(" \n\r\t");
        return input.substr(start, end - start + 1);
    }
}

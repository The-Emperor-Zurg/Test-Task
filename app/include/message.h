#pragma once
#include "logger.h"


namespace LoggerApp {
    struct Message {
        std::string text;
        MyLogger::LogLevel level;
        bool isExitMessage;

        Message(const std::string& text, MyLogger::LogLevel level, bool isExitMessage = false)
            : text(text), level(level), isExitMessage(isExitMessage)
        {}

        static Message createExitMessage() {
            return Message("", MyLogger::LogLevel::INFO, true);
        }
    };
}

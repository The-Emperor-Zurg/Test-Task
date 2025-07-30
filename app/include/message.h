#pragma once

#include "logger.h"


namespace LoggerApp {
    struct Message {
        std::string text;
        MyLogger::LogLevel level;
        bool isExitMessage;
        bool isLevelChangeMessage;

        Message(const std::string& text, MyLogger::LogLevel level, bool isExitMessage = false, bool isLevelChangeMessage = false)
            : text(text), level(level), isExitMessage(isExitMessage), isLevelChangeMessage(isLevelChangeMessage)
        {}

        static Message createExitMessage() {
            return Message("", MyLogger::LogLevel::INFO, true, false);
        }

        static Message createLevelChangeMessage(MyLogger::LogLevel newLevel) {
            return Message("", newLevel, false, true);
        }
    };
}

#pragma once

#include "interfaces/Iinput_handler.h"


namespace LoggerApp {
    class InputHandler : public IInputHandler {
    public:
        [[nodiscard]] std::optional<Message> parseInput(const std::string& input, MyLogger::LogLevel defaultLevel) const override;

    private:
        [[nodiscard]] static std::optional<MyLogger::LogLevel> parseLogLevel(const std::string& level);
        [[nodiscard]] static std::string trim(const std::string& input);
    };
}
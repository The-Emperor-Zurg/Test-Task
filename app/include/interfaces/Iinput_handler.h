#pragma once

#include <optional>

#include "../message.h"


namespace LoggerApp {
    class IInputHandler {
    public:
        virtual ~IInputHandler() = default;

        [[nodiscard]] virtual std::optional<Message> parseInput(const std::string& input, MyLogger::LogLevel defaultLevel) const = 0;
    };
}

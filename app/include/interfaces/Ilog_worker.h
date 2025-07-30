#pragma once

#include "../message.h"

namespace LoggerApp {
    class ILogWorker {
    public:
        virtual ~ILogWorker() = default;

        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void pushMessage(const Message& message) = 0;
    };
}
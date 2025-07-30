#pragma once

#include <atomic>
#include <memory>
#include <queue>
#include <condition_variable>
#include <thread>

#include "interfaces/Ilog_worker.h"


namespace LoggerApp {
    class LogWorker : public ILogWorker {
    public:
        explicit LogWorker(std::shared_ptr<MyLogger::Logger> logger);
        ~LogWorker() override;

        void start() override;
        void stop() override;
        void pushMessage(const Message& message) override;

    private:
        void workerThread();

        std::shared_ptr<MyLogger::Logger> logger_;
        std::queue<Message> message_queue_;
        std::mutex queueMutex_;
        std::condition_variable queueCondition_;
        std::atomic<bool> shouldStop_;
        std::thread workerThread_;
    };
}

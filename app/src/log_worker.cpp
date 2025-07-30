#include "log_worker.h"

#include <utility>


namespace LoggerApp {
    LogWorker::LogWorker(std::shared_ptr<MyLogger::Logger> logger)
        : logger_(std::move(logger))
    {}

    LogWorker::~LogWorker() {
        stop();
    }

    void LogWorker::start() {
        shouldStop_ = false;
        workerThread_ = std::thread(&LogWorker::workerThread, this);
    }

    void LogWorker::stop() {
        shouldStop_ = true;
        queueCondition_.notify_all();

        if (workerThread_.joinable()) {
            workerThread_.join();
        }
    }

    void LogWorker::pushMessage(const Message &message) {
        std::lock_guard lock(queueMutex_);
        message_queue_.push(message);
        queueCondition_.notify_one();
    }

    void LogWorker::workerThread() {
        while (!shouldStop_) {
            std::unique_lock lock(queueMutex_);

            queueCondition_.wait(lock, [this] {
                return !message_queue_.empty() || shouldStop_;
            });

            while (!message_queue_.empty()) {
                Message message = message_queue_.front();
                message_queue_.pop();
                lock.unlock();

                if (message.isExitMessage) {
                    shouldStop_ = true;
                    return;
                }

                if (logger_ && logger_->isInitialized()) {
                    auto result = logger_->log(message.text, message.level);
                    if (result != MyLogger::LogResult::SUCCESS) {
                        break;
                    }
                }

                lock.lock();
            }
        }
    }



}